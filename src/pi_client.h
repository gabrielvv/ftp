#ifndef PI_CLI_H
#define PI_CLI_H

#include "pi_commons.h"

SOCKET init_client_connection(const char *address, const int port);
int read_server(SOCKET sock, char *buffer);
void write_server(SOCKET sock, const char *buffer);

#endif /* PI_CLI_H */
