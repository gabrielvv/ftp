#include "dtp_client.h"

int main(int argc, char **argv)
{
   if(argc < 2)
   {
      printf("Usage : %s [address] [pseudo]\n", argv[0]);
      return -1;
   }

   printf("ftp> ");

   SOCKET sock = init_client_connection(argv[1]);
   char buffer[BUF_SIZE];
   fd_set rdfs;

   //TODO
   printf("Authentification\n");
   fflush(stdout);
   write_server(sock, argv[2]);
   
   while(1)
   {
      FD_ZERO(&rdfs);

      FD_SET(STDIN_FILENO, &rdfs);

      /* add the socket */
      FD_SET(sock, &rdfs);

      if(select(sock + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      // get from input
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         
      }
      else if(FD_ISSET(sock, &rdfs))
      {
         // Bloquant !!
         fdownload(sock, "./download.txt");
      }
   }
   closesocket(sock);
   return 0;
}
