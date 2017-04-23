#include "pi_server.h"
#include "pi_commons.h"

#ifdef WIN32 // if windows

#include <winsock2.h>

#else // if not supported

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

#endif

#define MAX_CLIENT 10

/**
 * init the server
 * @param  port  {int}     port
 * @param  nb_client {int} Number of clients
 * @return {int} return new socket (file descriptor)
 */
int init_server_connection(int port, int nb_client){
  // create a new socket
  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
  SOCKADDR_IN sin = { 0 };

  if(sock == INVALID_SOCKET)
  {
    perror("socket()");
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(port);
   sin.sin_family = AF_INET;

   // bind struct sin on socket IP/PORT
   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   // server listen for client
   if(listen(sock, nb_client) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }
   return sock;
}

/*
*
* read msg from client
* @param  sock  {SOCKET}  socket  to listen
* @param  buffer  {char*}  buffer for msg
* @return {int} return Buffer size
*/
 int read_client(SOCKET sock, char *buffer)
{
   //printf("on passe dans le red client\n");
   int n = 0;
   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      n = 0;
   }
   buffer[n] = 0;
   //printf("n : %s\n",buffer);
   return n;
}

/**
* allow to send msg on client
* @param  sock  {SOCKET}  socket  to write
* @param  message  {char*}  Message to send
*/
void write_client(int socket_fd, char * message) {
    int length = strlen(message);

    if(write(socket_fd, message, length) != length) {
        perror("Error writing message");
        close(socket_fd);
      }
}