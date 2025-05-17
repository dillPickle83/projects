package main

import (
	"fmt"
	"net/http"

	// Chi is a common package to manage API routes
	"github.com/go-chi/chi"
	// The below is an internal package that we're importing
	"goapi/internal/handlers"
	// The below is used for logging errors. logrus and chi can be installed using "go mod tidy"
	log "github.com/sirupsen/logrus"
)

func main(){

	// When we're logging the below line will print the message with the time and line number
	log.SetReportCaller(true)
	// The chi pointer variable below is essentially just a struct for an API that's being defined by the NewRouter function and then passed to the handler function
	var r *chi.Mux = chi.NewRouter()
	handlers.Handler(r)

	fmt.Println("Starting Go API servce..")

	err := http.ListenAndServe("localhost:8000", r)
	// Log any errors that might show up
	if err != nil {
		log.Error(err)
	}

