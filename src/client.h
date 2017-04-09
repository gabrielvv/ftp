#ifndef CLIENT_H
#define CLIENT_H

#ifdef WIN32

#include <winsock2.h>

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#endif

#define PORT	 7777
#define BUF_SIZE 1024

void init(void);
int init_client_connection(const char *address);
int read_server(SOCKET sock, char *buffer);
void write_server(SOCKET sock, const char *buffer);

#endif
