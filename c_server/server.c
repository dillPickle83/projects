#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "router.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    char *file_buffer = NULL;
    long file_size = 0;

    FILE *file = fopen("index.html", "r");

    // 1. Create socket (Changed the '1' back to '0' here!)
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

    // 2. Listen for connections
    if (listen(server_fd, 3) < 0){
        perror("LISTEN FAILED");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\r\n", PORT);

    // 3. Cache the index HTML file into memory
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

    // 4. The Infinite Server Loop
    while(1){
        // Accept the connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if(new_socket < 0){
            perror("ACCEPT FAILED");
            continue; // Changed from exit() to continue; so the server stays alive
        }
    
        // Clear the buffer before reading new data
        memset(buffer, 0, BUFFER_SIZE);

        // Read data from the client
        read(new_socket, buffer, BUFFER_SIZE-1);
        printf("---- New request received ----\n%s\n", buffer);

        // Pass the raw text off to your modular router
        route_request(new_socket, buffer, file_buffer, file_size);

        // Close the socket for this specific client
        close(new_socket);
    } // <-- Removed the rogue extra brace that was here

    // Close the server and free memory when the program is killed (e.g., Ctrl+C)
    if (file_buffer != NULL) {
        free(file_buffer);
    }
    close(server_fd);

    return 0;
}