#ifndef DTP_SVR_H
#define DTP_SVR_H

#include "pi_commons.h"
// variable/fonction statique => invisible à l'extérieur du fichier où elle est déclarée

int init_server_connection(int port, int nb_client);
int read_client(SOCKET sock, char *buffer);
void write_client(int socket_fd, char * message);
int parse_cmd(char* buffer, char * arg);
void cmd_pwd(SOCKET sock,char *arg);
void cmd_mkdir(SOCKET sock, char *arg);
void cmd_rmd(SOCKET sock, char *arg);
void cmd_dele(SOCKET sock, char *arg);
void cmd_cwd(SOCKET sock, char *arg);

void fupload(SOCKET, const char*);
void fsupload(SOCKET, FILE*);
void fdownload(SOCKET, const char*);

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
}Client;

#endif /* DTP_SVR_H */