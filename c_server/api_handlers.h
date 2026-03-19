#ifndef API_HANDLERS_H
#define API_HANDLERS_H

void lorem_ipsum(int client_socket);
void not_found(int client_socket, const char *path);
void static_html(int client_socket, const char *file_buffer, long file_size);

#endif