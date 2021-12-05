package main

import (
	"log"
	"os"
	"server/server"
	"server/storage"
)

func main() {
	logger := log.New(os.Stdout, "", 0)

	s := server.New(server.Options{
		Address: ":8000",
		Logger:  logger,
		Storer:  storage.New(),
	})

	if err := s.Start(); err != nil {
		logger.Fatalln("Could not start:", err)
	}
}
