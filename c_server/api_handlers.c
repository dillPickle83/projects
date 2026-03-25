#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "api_handlers.h"

void lorem_ipsum(int client_socket){
    char *lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.";
    char api_response[2048];

    snprintf(api_response, sizeof(api_response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s", 
        strlen(lorem), lorem);

    write(client_socket, api_response, sizeof(api_response));
    printf("Served Lorem Ipsum excerpt.\n");
}

void not_found(int client_socket, const char *path){
    char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 23\r\n\r\n404 Endpoint Not Found";
    write(client_socket, not_found, strlen(not_found));
    printf("Sent 404 Not Found for path: %s\n", path);
}

void static_html(int client_socket, const char *file_buffer, long file_size){
    if (file_buffer != NULL) {
        // Writing the header and the file separately so that it's 
        // stitched by the browser
        char headers[1024];
        snprintf(headers, sizeof(headers),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %ld\r\n"
            "\r\n", file_size);

        write(client_socket, headers, strlen(headers));
        write(client_socket, file_buffer, file_size);
        printf("Served index.html file.\n");
    } else {
        not_found(client_socket, "/index.html");
    }
}

void api_health(int client_socket){
    // Open the process loadavg and get the load in the 
    // last 1, 5 and 15 minutes respectively
    FILE *fp = fopen("/proc/loadavg", "r");
    double load1 = 0.0, load5 = 0.0, load15 = 0.0;

    if (file != NULL){
        fscanf(fp, "%lf %lf %lf", &load1, &load5, &load15);
        fclose(fp);
    }else{
        pritnf("Error reading the process 'loadavg'");
    }

    char json_payload[256];
    snprintf(json_payload, sizeof(json_payload),
            "{\n"
            "  \"status\": \"online\",\n"
            "  \"cpu_load_1m\": %.2f,\n"
            "  \"cpu_load_5m\": %.2f,\n"
            "  \"cpu_load_15m\": %.2f\n"
            "}",
            load1, load5, load15);

    char api_response[256];
    snprintf(api_response, sizeof(api_response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %zu\r\n"
            "\r\n"
            "%s",
            strlen(json_payload), json_payload);

    write(client_socket, api_reponse, sizeof(api_response));
    printf("Served the /api/health endpoint");
}