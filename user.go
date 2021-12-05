package model

type User struct {
	Name     string `json:"name"`
	Password []byte `json:"password"`
}
