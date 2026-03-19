#ifndef ROUTER_H
#define ROUTER_H

void route_request(int client_socket, const char *request_buffer, const char *file_buffer, long file_size);

#endif