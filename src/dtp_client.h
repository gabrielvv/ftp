#ifndef DTP_CLI_H
#define DTP_CLI_H

#include "pi_commons.h" /* SOCKET */

int init_client_connection(const char *address, int port);

int read_server(SOCKET sock, void* buffer);
void write_server(SOCKET sock, const void* buffer);

void fupload(SOCKET, const char*);
void fsupload(SOCKET, FILE*);
void fdownload(SOCKET, const char*);

#endif /* DTP_CLI_H */
