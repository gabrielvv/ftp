#include "server.c"




int main(int argc, char *argv[]){
  printf("Init server");
  int listen_fd;
  init(); //init socket for window;

  // create a new socket
  listen_fd= socket(AF_INET, SOCK_STREAM, 0);
  if(listen_fd== INVALID_SOCKET)
  {
    printf("Error socket()");
    return -1;
 }

 // create interface
 SOCKADDR_IN sin = { 0 };
 sin.sin_addr.s_addr = htonl(INADDR_ANY);
 sin.sin_family = AF_INET;
 sin.sin_port = htons(7777);
 if(bind (listen_fd, (SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
 {
     printf("Error bind()");
     return -1;
 }

 //listen client
 if(listen(listen_fd,5) == SOCKET_ERROR){
   printf("Error listen()");
   return -1;
 }



}
