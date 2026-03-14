#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    char response[2048];
    char headers[1024];
    char *file_buffer = NULL;
    long file_size = 0;

    FILE *file = fopen("index.html", "r");

    //create socket
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

    // listen for connections
    if (listen(server_fd, 3) < 0){
        perror("LISTEN FAILED");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\r\n", PORT);

    // Get the index HTML file
    if (file != NULL){
        // Find the size of the file
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        rewind(file);
    
        // Allocate memory for the filesize + 1 for the \0 terminator
        file_buffer = malloc(file_size + 1);

        // Read the file into file_buffer
        fread(file_buffer, 1, file_size, file);
        file_buffer[file_size] = '\0';      // Terminate the buffer with the terminator
        fclose(file);
        printf("Cached index.html into %ld bytes of RAM\r\n", file_size);

        // Construct the headers with the file size
        snprintf(headers, sizeof(headers),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %ld\r\n"
            "\r\n", 
            file_size);
    }else{
        printf("WARNING: index.html not found during startup. Server will return 404.\n");
    }
    while(1){
        // accept the connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if(new_socket < 0){
            perror("ACCEPT FAILED");
            exit(EXIT_FAILURE);
        }
    
        // Clear the buffer before reading new data
        memset(buffer, 0, BUFFER_SIZE);

        // read data from the client
        // Reducing the -1 from buffer_size to ensure the last character read is \0
        read(new_socket, buffer, BUFFER_SIZE-1);
        printf("---- New request received ----\n%s\n", buffer);


        // Depending on the method and the destination, serve the right output
        char method[16];
        char path[256];

        sscanf(buffer, "%s %s", method, path);
        printf("Parsed request -> Method: %s | Path: %s\n", method, path);

        if (strcmp(method, "GET") == 0){
            if (strcmp(path, "/") == 0 || strcmp(path, "/index.html") == 0){
                if (file_buffer != NULL) {
                    // Writing the header and the file separately so that it's 
                    // stitched by the browser
                    write(new_socket, headers, strlen(headers));
                    write(new_socket, file_buffer, file_size);
                    printf("Served index.html file.\n");
                } else {
                    // File was never loaded, send 404
                    char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
                    write(new_socket, not_found, strlen(not_found));
                    printf("Sent 404 Not Found.\n");
                }
            }else if(strcmp(path, "/lorem_ipsum") == 0){
                char *lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
                char api_response[2048];

                snprintf(api_response, sizeof(api_response),
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: %zu\r\n"
                    "\r\n"
                    "%s", 
                    strlen(lorem), lorem);

                write(new_socket, api_response, sizeof(api_response));
                printf("Served Lorem Ipsum excerpt.\n");
            }else{
                // Route 3: Catch-all 404 for unknown paths
                char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 23\r\n\r\n404 Endpoint Not Found";
                write(new_socket, not_found, strlen(not_found));
                printf("Sent 404 Not Found for path: %s\n", path);
            }
            // close the socket
            close(new_socket);
        }
    }
    // Close the server and free memory when the while loop is exited (^C)
    if (file_buffer != NULL) {
        free(file_buffer);
    }
    close(server_fd);

    return 0;
}