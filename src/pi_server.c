#include "pi_server.h"
#include "pi_commons.h"
#include "ftp_commons.h"

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

#endif /* WIN32 */

#define MAX_CLIENT 10
#define PI_DEFAULT_PORT 20

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
  if(strncmp(buffer, "rmd", 3) ==0){
    buffer = buffer + 3;
    cmd = RMD;
  }

  if(strncmp(buffer, "syst", 4) == 0){
    buffer = buffer + 4;
    cmd = SYST;
  }

  if(strncmp(buffer, "dele",4)==0){
    buffer = buffer + 4;
    cmd = DELE;
  }

  if(strncmp(buffer, "cwd", 3)==0){
    buffer = buffer + 3;
    cmd = CWD;
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
    //Null-terminate:
    *arg = '\0';
  }
  return cmd;
}


/*
   ____ ___  __  __ __  __    _    _   _ ____
  / ___/ _ \|  \/  |  \/  |  / \  | \ | |  _ \
 | |  | | | | |\/| | |\/| | / _ \ |  \| | | | |
 | |__| |_| | |  | | |  | |/ ___ \| |\  | |_| |
  \____\___/|_|  |_|_|  |_/_/   \_\_| \_|____/
*/


void cmd_pwd(SOCKET sock,char *arg){
    char cwd[1024];
    char msg_return[1024] = "Current working dir : ";
    if(getcwd(cwd, sizeof(cwd)) != NULL){
      strcat(msg_return, cwd);
      printf("%s\n",msg_return);
      write_socket(sock, msg_return);
    }
    else{
       perror("getcwd() error");
       write_socket(sock, "getcwd() error");
    }
}


void cmd_mkdir(SOCKET sock, char *arg){
  if(arg == NULL){
    perror("mkdir() error");
  }else{
      mkdir(arg, 0777);
      write_socket(sock, "created");
  }
}


void cmd_rmd(SOCKET sock, char *arg){
  if(rmdir(arg)==0){
     write_socket(sock, "250 : OK, directory deleted \n");
   }else{
     write_socket(sock, "550 Cannot delete directory.\n");
   }
}


void cmd_dele(SOCKET sock, char *arg){
  if(remove(arg)==0){
      write_socket(sock, "250 : OK, file deleted \n");
   }else{
      write_socket(sock, "550 Cannot delete file.\n");
   }
}


void cmd_cwd(SOCKET sock, char *arg){
  if(chdir(arg)==0){
      write_socket(sock, "250 : OK, Directory changed \n");
    }else{
      write_socket(sock, "250 : Failed to change directory \n");
    }
}


int pi_svr_main(int argc, char *argv[]){
  
  printf("\n");
  printf("\n");
  printf(" ___  _____  ___     ___  ___  ___ __   __ ___  ___\n");
  printf("| __||_   _|| _ \\  \\ / __|| __|| _ \\\\ \\ / /| __|| _ \\\n");
  printf("| _|   | |  |  _/   \\__ \\| _| |   / \\ V / | _| |   /\n");
  printf("|_|    |_|  |_|     |___/|___||_|_\\  \\_/  |___||_|_\\\n");
  printf("\n");
  printf("\n");
  printf("--help To see the different available commands\n");

  //printf("Init server\n");
  init(); //init socket for windows;
  Client clients[MAX_CLIENT];
  int cpt_client = 0;
  char buffer[BUF_SIZE];
  SOCKET sockServer = init_server_connection(PI_DEFAULT_PORT, MAX_CLIENT);
  int max = sockServer;
  fd_set rdfs;
  fflush(stdout);

  while(1){
    int i =0;
    int ret = 0;
    FD_ZERO(&rdfs);
    FD_SET(sockServer, &rdfs);
    //printf("on boucle\n" );

    FD_SET(sockServer, &rdfs);
    /* add socket of each client */
     for(i = 0; i < cpt_client; i++)
     {
        FD_SET(clients[i].sock, &rdfs);
     }

    if((ret = select(max + 1, &rdfs, NULL, NULL, NULL)) < 0)
    {
     perror("select()");
     exit(errno);
    }

    // new client
    if(FD_ISSET(sockServer, &rdfs)){
      printf("Welcome new user is online\n");
      fflush(stdout);
      SOCKADDR_IN csin = { 0 };
      int client_sock = accept(sockServer, NULL,NULL);
      if(client_sock == SOCKET_ERROR)
      {
          perror("accept()");
          continue;
      }
      /* after connecting */
      //TODO need authentification
      if(read_socket(client_sock, buffer) == -1)
      {
          /* disconnected */
          continue;
      }
      /* wnew maximum fd ? */
      max = client_sock > max ? client_sock : max;

      FD_SET(client_sock, &rdfs);
      Client c = { client_sock };
      strncpy(c.name, buffer, BUF_SIZE - 1);
      clients[cpt_client] = c;
      cpt_client++;
      //printf("on ajout le 1er client\n");
      fflush(stdout);
      continue;

    //after connection treat actions
    }else{
      //printf("traitement en cours...\n");
      fflush(stdout);
      int i = 0;
        for(i = 0; i < cpt_client; i++)
        {
           /* a client send a cmd */
           if(FD_ISSET(clients[i].sock, &rdfs))
           {
              Client client = clients[i];
              int c = read_socket(clients[i].sock, buffer);
              //printf("n : %s\n",buffer);
              char arg[BUF_SIZE];
              switch(parse_cmd(buffer,arg)){
                case PWD :
                          printf("PWD print directory %s\n", arg);
                          cmd_pwd(clients[i].sock, arg);
                          break;
                case CDUP :
                          printf("CDUP Change to Parent Directory %s \n",arg);
                          break;
                case SMNT :
                          printf("SMNT Structure Mount %s\n",arg);
                          break;
                case STOU :
                          printf("STOU Store Unique %s\n", arg);
                          break;
                case MKD :
                          printf("MKD Make Directory %s\n",arg);
                          cmd_mkdir(clients[i].sock, arg);
                          break;
                case RMD :
                          printf("RMD Delete Directory %s\n", arg);
                          cmd_rmd(clients[i].sock, arg);
                          break;
                case SYST :
                          printf("SYST Make Directory %s\n", arg);
                          break;
                case DELE :
                          printf("DELE DELETE FILE %s\n",arg);
                          cmd_dele(clients[i].sock, arg);
                          break;
                case CWD :
                          printf("CWD change directory %s\n",arg);
                          cmd_cwd(clients[i].sock, arg);
                          break;
                case INVALID :
                          printf("Invalid cmd %s\n",arg);
                          break;
              }
           }
        }
    }
  };
  return 0;
}// svr_main
