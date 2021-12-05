package handlers

import (
	"context"
	"encoding/gob"
	"encoding/json"
	"errors"
	"net/http"
	"server/model"
)

type User_new struct {
	Name     string `json:"name"`
	Password string `json:"password"`
}

const (
	sessionUserKey = "user"
)

type sessionGetter interface {
	Exists(ctx context.Context, key string) bool
	Get(ctx context.Context, key string) interface{}
}

type sessionPutter interface {
	RenewToken(ctx context.Context) error
	Put(ctx context.Context, key string, value interface{})
}

type sessionDestroyer interface {
	Destroy(ctx context.Context) error
}

func getUserFromContext(r *http.Request) model.User {
	return r.Context().Value(sessionUserKey).(model.User)
}

type signupper interface {
	Signup(name, password string) error
}

func errorResponse(w http.ResponseWriter, message string, httpStatusCode int) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(httpStatusCode)
	resp := make(map[string]string)
	resp["message"] = message
	jsonResp, _ := json.Marshal(resp)
	w.Write(jsonResp)
}

func SignupHandler(repo signupper) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		var e User_new
		var unmarshalErr *json.UnmarshalTypeError
		decoder := json.NewDecoder(r.Body)
		decoder.DisallowUnknownFields()
		err := decoder.Decode(&e)
		if err != nil {
			if errors.As(err, &unmarshalErr) {
				errorResponse(w, "Bad Request. Wrong Type provided for field "+unmarshalErr.Field, http.StatusBadRequest)
			} else {
				errorResponse(w, "Bad Request "+err.Error(), http.StatusBadRequest)
			}
			return
		}

		if err := repo.Signup(e.Name, e.Password); err != nil {
			http.Error(w, err.Error(), http.StatusBadGateway)
			return
		}
		errorResponse(w, "Success", http.StatusOK)
	}
}

type loginner interface {
	Login(name, password string) (*model.User, error)
}

func LoginHandler(repo loginner, s sessionPutter) http.HandlerFunc {
	// Register our user type with the gob encoding used by the session handler
	gob.Register(model.User{})

	return func(w http.ResponseWriter, r *http.Request) {
		var e User_new
		var unmarshalErr *json.UnmarshalTypeError
		decoder := json.NewDecoder(r.Body)
		decoder.DisallowUnknownFields()
		err := decoder.Decode(&e)
		if err != nil {
			if errors.As(err, &unmarshalErr) {
				errorResponse(w, "Bad Request. Wrong Type provided for field "+unmarshalErr.Field, http.StatusBadRequest)
			} else {
				errorResponse(w, "Bad Request "+err.Error(), http.StatusBadRequest)
			}
			return
		}

		user, err := repo.Login(e.Name, e.Password)
		if err != nil {
			http.Error(w, err.Error(), http.StatusBadGateway)
			return
		}
		if user == nil {
			http.Error(w, "email and/or password incorrect", http.StatusForbidden)
			return
		}

		// Renew the token to avoid session fixation attacks
		if err := s.RenewToken(r.Context()); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}

		// Put the whole user info into the session
		s.Put(r.Context(), sessionUserKey, user)
		errorResponse(w, "Success", http.StatusOK)
	}
}

func LogoutHandler(s sessionDestroyer) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		if err := s.Destroy(r.Context()); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
		}
	}
}

// CheckSessionHandler sits behind the auth middleware and just returns the current user info.
func CheckSessionHandler() http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		user := getUserFromContext(r)
		userAsJSON, err := json.Marshal(user)
		if err != nil {
			panic(err)
		}
		_, _ = w.Write(userAsJSON)
	}
}
