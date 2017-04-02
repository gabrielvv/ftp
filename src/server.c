#include "server.h"

static void init(void)
{
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

static void end(void)
{
#ifdef WIN32
    WSACleanup();
#endif
}

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
* read msg from client
* @param  sock  {SOCKET}  socket  to listen
* @param  buffer  {char*}  buffer for msg
* @return {int} return Buffer size
*/
static int read_client(SOCKET sock, char *buffer)
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

/**
 * parse cmd send by client
 * @param buffer [description]
 * @param arg    [description]
 */
int parse_cmd(char* buffer, char * arg)
{

  while(*buffer == ' ' || *buffer == '\t') {
      buffer++;
  }

  int cmd = INVALID;
  if(strncmp(buffer, "pwd", 3) == 0){
    buffer = buffer + 3;
    cmd = PWD;
  }
  if(strncmp(buffer, "cdup", 4) == 0){
    buffer = buffer + 4;
    cmd = CDUP;
  }
  if(strncmp(buffer, "smnt", 4) == 0){
    buffer = buffer + 4;
    cmd = SMNT;
  }
  if(strncmp(buffer, "stou", 4) == 0){
    buffer = buffer + 4;
    cmd = STOU;
  }
  if(strncmp(buffer, "mkd", 3) == 0){
    buffer = buffer + 3;
    cmd = MKD;
  }
  if(strncmp(buffer, "syst", 4) == 0){
    buffer = buffer + 4;
    cmd = SYST;
  }

  if(arg != NULL){

    while(*buffer == ' ' || *buffer == '\t') {
        buffer++;
    }

    //Copy the argument over:
    while((*buffer != ' ') && (*buffer != '\n') && (*buffer != '\r') && (*buffer != '\0')) {
        *arg = *buffer;
        arg++;
        buffer++;
    }
    printf("argument : %s\n",arg);

    //Null-terminate:
    *arg = '\0';
  }
  return cmd;
}