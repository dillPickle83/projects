#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "router.h"

#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct{
    int socket_fd;
    const char* file_buffer;
    long file_size;
}ThreadArgs;

void *handle_client_thread(void *arg){
    ThreadArgs *args = (ThreadArgs *)arg;
    int client_socket = args->socket_fd;
    char buffer[BUFFER_SIZE];


    memset(buffer, 0, BUFFER_SIZE);
    read(client_socket, buffer, BUFFER_SIZE - 1);
    printf("-----Thread %lu handling the request-----\n", pthread_self());

    route_request(client_socket, buffer, args->file_buffer, args->file_size);

    close(client_socket);
    free(args);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    char *file_buffer = NULL;
    long file_size = 0;

    FILE *file = fopen("index.html", "r");

    // Create socket (Changed the '1' back to '0' here!)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == 0){
        perror("SOCKET FAILED");
        exit(EXIT_FAILURE);
    } 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&address, addrlen) < 0){
        perror("BIND FAILED");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0){
        perror("LISTEN FAILED");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\r\n", PORT);

    // Cache the index HTML file into memory
    if (file != NULL){
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        rewind(file);
    
        file_buffer = malloc(file_size + 1);
        fread(file_buffer, 1, file_size, file);
        file_buffer[file_size] = '\0';      
        fclose(file);
        printf("Cached index.html into %ld bytes of RAM\r\n", file_size);
    } else {
        printf("WARNING: index.html not found during startup. Server will return 404.\n");
    }

    // Accept connections till the program is stopped
    while(1){
        // Accept the connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if(new_socket < 0){
            perror("ACCEPT FAILED");
            continue;
        }

        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->socket_fd = new_socket;
        args->file_buffer = file_buffer;
        args->file_size = file_size;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client_thread, args) != 0){
            perror("FAILED TO CREATE THE THREAD\n");
            free(args);
            close(new_socket);
        }else{
            pthread_detach(thread_id);
        }
    }

    if (file_buffer != NULL) {
        free(file_buffer);
    }
    close(server_fd);

    return 0;
}