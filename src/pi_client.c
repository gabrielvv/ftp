#include "pi_client.h"
#include "pi_commons.h"
#include "ftp_commons.h"

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

#endif /* WIN32 */

#define PI_DEFAULT_PORT 20

int pi_cli_main(int argc, char **argv){
   if(argc < 2)
   {
      printf("Usage : %s [address] [pseudo]\n", argv[0]);
      return -1;
   }

   printf("\n\n");
   printf("___  _____  ___    ___  _     ___  ___  _  _  _____ \n");
   printf("| __||_   _|| _ \\  / __|| |   |_ _|| __|| \\| ||_   _|\n");
   printf("| _|   | |  |  _/ | (__ | |__  | | | _| | .` |  | | \n");
   printf("|_|    |_|  |_|    \\___||____||___||___||_|\\_|  |_|  \n");
   printf("\n\n");

   printf("Welcome to Ftp client\n");
   printf("Enter --help  To see the different available commands\n");


   SOCKET sock = init_client_connection(argv[1], PI_DEFAULT_PORT);
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
         fgets(buffer, BUF_SIZE - 1, stdin);
         {
            char *p = NULL;
            p = strstr(buffer, "\n");
            if(p != NULL)
            {
               *p = 0;
            }
            else
            {
               buffer[BUF_SIZE - 1] = 0;
            }
         }
         fflush(stdout);
         printf("write : %s\n", buffer);
         write_server(sock, buffer);
      }
      else if(FD_ISSET(sock, &rdfs))
      {
         printf("Receive : ");
         int n = read_server(sock, buffer);
         if(n == 0)
         {
            printf("Server disconnected !\n");
            break;
         }
         puts(buffer);
      }
   }
   closesocket(sock);
   return 0;
}