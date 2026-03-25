#include <stdio.h>
#include <string.h>
#include "router.h"
#include "api_handlers.h"

void route_request(int client_socket, const char *request_buffer, const char *file_buffer, long file_size){
    char method[16] = {0};
    char path[256] = {0};

    sscanf(request_buffer, "%s %s", method, path);
    printf("Parsed request -> Method: %s | Path: %s\n", method, path);

    if (strcmp(method, "GET") == 0){
        if (strcmp(path, "/") == 0 || strcmp(path, "/index.html") == 0){
            static_html(client_socket, file_buffer, file_size);
        }else if(strcmp(path, "/lorem_ipsum") == 0){
            lorem_ipsum(client_socket);
        }else if(strcmp(path, "/api/health") == 0){
            api_health(client_socket);
        }else{
            not_found(client_socket, path);
        }
    }
}