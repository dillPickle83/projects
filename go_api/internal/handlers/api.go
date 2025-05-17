package handlers

import (
	"github.com/go-chi/chi"
	chimiddle "github.com/go-chi/chi/middleware"
	"goapi/internal/middleware"
)

// We are defining the handler function that we're using the main.go file. The capital H in Handler means that this function can be imported in other functions (public function).
// Middleware is basically a function that gets called before the main function handles the endpoint.
func Handler(r *chi.Mux) {
	// Global middleware
	// The StripSlashes function removes any trailing "/" in the API address
	r.Use(chimiddle.StripSlashes)

	r.Route("/account", func(router chi.Router) {

		// Middleware for /account route; Requires authentication. If the auth fails, returns an err code.
		router.Use(middleware.Authorization)

		router.Get("/coins", GetCoinBalance)
	})
}
