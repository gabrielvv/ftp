#include "ftp_commons.h"

#ifdef WIN32

#include <winsock2.h>

#else

#include <sys/stat.h> //fstat

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close, write, read */
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#endif /* WIN32 */

void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      return -1;
   }
#endif
}

void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}

SOCKET man init_server_connection(const int port, int nb_client){
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

SOCKET init_client_connection(const char *address, const int port)
{
   // create a new socket
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };
   struct hostent *hostinfo;

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   hostinfo = gethostbyname(address);
   if (hostinfo == NULL)
   {
      fprintf (stderr, "Unknown host %s.\n", address);
      exit(EXIT_FAILURE);
   }

   sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   sin.sin_port = htons(port);
   sin.sin_family = AF_INET;

   // try connect client to server with struct sin
   if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect()");
      exit(errno);
   }
   return sock;
}

int read_socket(SOCKET sock, char *buffer)
{
   int n = 0;

   if(is_socket(sock) > 0){

     // SOCKET
     if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
     {
        perror("recv()");
        close(sock);
        exit(errno);
     }

     buffer[n] = '\0';

   }else{
      // printf("read_socket FILE\n");
      // printf("fd %d\n", sock);
      // FILE
      if((n = read(sock, buffer, BUF_SIZE)) < 0)
      {
        perror("read()");
        close(sock);
        exit(errno);
      }

      // printf("read buffer %s\n", buffer);

      // buffer[n] = '\0';
   }

   return n;
}

void write_socket(SOCKET sock, const char *buffer)
{
  int length = strlen(buffer);

  if(is_socket(sock) > 0){
    // SOCKET
    if( send(sock, buffer, length, /* FLAG */0) != length )
    {
      perror("send()");
      close(sock);
      exit(errno);
    }
  }else{
    // FILE
    if( write(sock, buffer, length) != length )
    {
      perror("write()");
      close(sock);
      exit(errno);
    }
  }
}

int is_socket(int fd){
  struct stat statbuf;
  fstat(fd, &statbuf);
  return S_ISSOCK(statbuf.st_mode);
}