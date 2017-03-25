#include <stdio.h>
#include "dtp_client.h"

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

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}



/**
 * init the sclient
 * @param  address {int}     address
 * @return {int} return new socket (file descriptor)
 */
int init_client_connection(const char *address)
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
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   // try connect client to server with struct sin
   if(connect(sock,(SOCKADDR *) &sin, sizeof(SOCKADDR)) == SOCKET_ERROR)
   {
      perror("connect()");
      exit(errno);
   }
   return sock;
}

/*
* read msg from server
* @param  sock  {SOCKET}  socket  to listen
* @param  buffer  {char*}  buffer for msg
* @return {int} return Buffer size
*/
int read_server(SOCKET sock, void *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      exit(errno);
   }

   // buffer[n] = 0;

   return n;
}


/**
* allow to send msg on server
* @param  sock  {SOCKET}  socket  to write
* @param  message  {char*}  Message to send
*/
void write_server(SOCKET sock, const void *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

//FILE *fopen(const char *path, const char *mode);
void fsupload(SOCKET to_sock, FILE* file){
   //fread
}

void fupload(SOCKET to_sock, const char* path){
   //fread
}  

/**
* w flag
* Truncate file to zero length or create text  file  for  writing.
* The stream is positioned at the beginning of the file.
*
* @desc download data as file and store to path
*
* @param to_sock {SOCKET}
* @param path    {char*}
*
* @see readv
* @see fopen
* @see recvmmsg
* @see recv
* @see select/pselect/poll/ppoll
* @see memset
*/
void fdownload(SOCKET to_sock, const char* path){
   
   printf("client download start\n");
   fflush(stdout);

   FILE* fp = fopen(path, "w");
   char buffer[BUF_SIZE];

   while(1){
      int n = read_server(to_sock, buffer);

      // ecriture non formatée binaire
      // cf. remarques et limitations dans cours
      fwrite(buffer, sizeof(char), n, fp);

      // feof returns non-zero if the end-of-file indicator is set
      if(feof(fp) != 0)
         break;
   }

   printf("end of client download\n");
   fflush(stdout);
}
