#ifndef DTP_SVR_H
#define DTP_SVR_H

#include "pi_commons.h" /* SOCKET */

// variable/fonction statique => invisible à l'extérieur du fichier où elle est déclarée

int init_server_connection(int port, int nb_client);
int read_client(SOCKET sock, char *buffer);
void write_client(int socket_fd, char * message);

#endif /* DTP_SVR_H */