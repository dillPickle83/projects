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

    printf("Server is listening on port %d\n", PORT);

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
        read(new_socket, buffer, BUFFER_SIZE);
        printf("---- New request received ----\n%s\n", buffer);

        // send response to the client
        char *html_body = "<html><body><h1>Firmware Dev API Online</h1><p>Ready for embedded systems requests!</p></body></html>";
        char response[2048];
        snprintf(response, sizeof(response),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Content-Length: %lu\r\n"
                 "\r\n"
                 "%s",
                 strlen(html_body), html_body);

        // Send the complete HTTP response back to the client
        write(new_socket, response, strlen(response));
        printf("Response sent to client:\n%s\n", response);

        // close the socket
        close(new_socket);
    }

    // Close the server when the while loop is exited (^C)
    close(server_fd);

    return 0;
}