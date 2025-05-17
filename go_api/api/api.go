package api

import (
	"encoding/json"
	"net/http"
)

// Coin balance params
type CoinBalanceParams struct {
	Username string
}

// Coin Balance Response
type CoinBalanceResponse struct {
	// Account balance
	Balance int

	// Success code, usually 200
	Code int
}

// Error response
type Error struct {
	// Error code
	Code int

	// Error message
	Message string
}

// Funciton to return an error
func writeError(w http.ResponseWriter, message string, code int) {
	resp := Error{
		Code: code,
		Message: message,
	}

	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(code)

	json.NewEncoder(w).Encode(resp)
}

var (
	// Below handler is for when there are issues with the request itself
	RequestErrorHandler = func(w http.ResponseWriter, err error) {
		writeError(w, err.Error(), http.StatusBadRequest)
	}
	// Below handler is for generic server issues to return a generic err response
	InternalErrorHandler = func(w http.ResponseWriter) {
		writeError(w,"An Unexpected error occured.", http.StatusInternalServerError)
	}
)
