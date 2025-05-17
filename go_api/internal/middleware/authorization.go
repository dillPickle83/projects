package middleware

import (
	"errors"
	"net/http"

	"goapi/api"
	"goapi/internal/tools"
	log "github.com/sirupsen/logrus"
)

// Can define custom error messages
var UnAuthorizedError = errors.New("Invalid username or token.")

// Writing the autorization function used in the api file
func Authorization(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {

		var username string = r.URL.Query().Get("username")
		var token = r.Header.Get("Authorization")
		var err error

		if username == "" || token == "" {
			log.Error(UnAuthorizedError)
			api.RequestErrorHandler(w, UnAuthorizedError)
			return
		}

		var database *tools.DatabaseInterface
		database, err = tools.NewDatabase()
		if err != nil {
			api.InternalErrorHandler(w)
			return
		}

		var loginDetails *tools.LoginDetails
		loginDetails = (*database).GetUserLoginDetails(username)

		if (loginDetails == nil || (token!= (*loginDetails).AuthToken)) {
			log.Error(UnAuthorizedError)
			api.RequestErrorHandler(w, UnAuthorizedError)
			return
		}

		// The ServeHTTP function calls the next middleware in line or calls the handler for the endpoint if there is no middleware left
		next.ServeHTTP(w, r)
	})
}
