#include "pi_server.h"
#include "pi_commons.h"
#include "ftp_commons.h"
#include "dtp_commons.h"

#ifdef WIN32 // if windows

#include <winsock2.h>

#else // if not supported

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h> /* close, getopt */
#include <netdb.h> /* gethostbyname */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pthread.h>

#endif /* WIN32 */

#define MAX_CLIENT 10
#define PI_DEFAULT_PORT 20
#define DTP_DEFAULT_PORT 21


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

void cmd_ls(SOCKET sock, char *arg){
  char cwd[1024];
  char result[1024] = "";
  DIR *dir;
  struct dirent *ent;
  int cpt=0;
  if ((dir = opendir (getcwd(cwd, sizeof(cwd)))) != NULL) {
  /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      cpt++;
      if(cpt>3){
        strcat(result, ent->d_name);
        strcat(result, "\t");

        if(((cpt-3)%5)==0){
          strcat(result, "\n");
        }
      }
    }
    strcat(result, "\n");
    write_socket(sock, result);
    closedir (dir);
  } else {
    write_socket(sock, "250 : Failed to print directory \n");
  }
}

/**
*
* To execute in a thread
*
*/
void* cmd_put(void *arg){

  struct dtp_info *dtpinfo = arg;

  char buffer[BUF_SIZE];
  memset(buffer, 0, BUF_SIZE);

  if(strcmp(dtpinfo->arg, "") == 0){
    write_socket(dtpinfo->sock, "Missing argument\n");
    return;
  }
  write_socket(dtpinfo->sock, "PUT Ready\n");
  //SOCKET sockServer = init_server_cclient_socketonnection(DTP_DEFAULT_PORT, MAX_CLIENT);
  fdownload(dtpinfo->sock, dtpinfo->arg);
}// cmd_put

/**
*
* To execute in a thread
*
*/
void* cmd_get(void *arg){

  struct dtp_info *dtpinfo = arg;

  char buffer[BUF_SIZE];
  memset(buffer, 0, BUF_SIZE);

  if(strcmp(dtpinfo->arg, "") == 0){
    write_socket(dtpinfo->sock, "Missing argument\n");
    pthread_exit("data transfer failure");
  }
  FILE* fp = fopen(dtpinfo->arg, "r");
  if(fp == NULL){
    sprintf(buffer, "%s\n", strerror(errno));
    write_socket(dtpinfo->sock, buffer);
    pthread_exit("data transfer failure");
  }

  SOCKET dtp_sock = init_server_connection(DTP_DEFAULT_PORT, 1);
  int max = dtp_sock;
  int client_sock, ret = 0;
  
  fd_set rdfs;
  fd_set wdfs;
  fd_set edfs;

  FD_ZERO(&rdfs);
  FD_ZERO(&edfs);
  FD_ZERO(&wdfs);

  FD_SET(dtp_sock, &rdfs);
  FD_SET(dtp_sock, &wdfs);
  FD_SET(dtp_sock, &edfs);

  if((ret = select(max + 1, &rdfs, &wdfs, &edfs, /* timeout*/NULL)) < 0)
    {
     perror("select()");
     exit(errno);
    }

    while(!FD_ISSET(dtp_sock, &rdfs)){
      // infinite loop !
    }

    if(FD_ISSET(dtp_sock, &rdfs))
    {
      SOCKADDR_IN csin = { 0 };
      client_sock = accept(dtp_sock, NULL,NULL);
      if(client_sock == SOCKET_ERROR)
      {
          perror("accept()");
          pthread_exit("data transfer failure");
      }

      /* after connecting */

      if(read_socket(client_sock, buffer) == -1)
      {
          /* disconnected */
          
      }
      /* wnew maximum fd ? */
      max = client_sock > max ? client_sock : max;
      FD_SET(client_sock, &rdfs);

    //after connection treat actions
    }
    else
    {

     /* a client sends data */
     if(FD_ISSET(client_sock, &rdfs))
     {
        int c = read_socket(client_sock, buffer);
        if(c == 0)
         {
            printf("Client disconnected !\n");
            closesocket(client_sock);
         }
     }
   }

  write_socket(client_sock, "GET Ready\n");
  fsupload(client_sock, fp);
  pthread_exit("data transfer success");
} // cmd_get

int pi_svr_main(int argc, char *argv[]){
  
  printf("\n");
  printf("\n");
  printf(" ___  _____  ___     ___  ___  ___ __   __ ___  ___\n");
  printf("| __||_   _|| _ \\  \\ / __|| __|| _ \\\\ \\ / /| __|| _ \\\n");
  printf("| _|   | |  |  _/   \\__ \\| _| |   / \\ V / | _| |   /\n");
  printf("|_|    |_|  |_|     |___/|___||_|_\\  \\_/  |___||_|_\\\n");
  printf("\n");
  printf("\n");

  int opt;
  while ((opt = getopt(argc, argv, "h")) != -1) {
       switch (opt) {
       case 'h':
           fprintf(stderr, "Available commands: pwd, rmd, mkd, put, get, ls, dele\n");
       default:
           fprintf(stderr, "Usage: %s [-h]\n",
                   argv[0]);
           exit(EXIT_FAILURE);
       }
  }

  //printf("Init server\n");
  init(); //init socket for windows;
  Client clients[MAX_CLIENT];
  int cpt_client = 0;
  char buffer[BUF_SIZE];
  SOCKET sockServer = init_server_connection(PI_DEFAULT_PORT, MAX_CLIENT);
  int max = sockServer;

  pthread_t threadid;
  
  fd_set rdfs;
  fd_set wdfs;
  fd_set edfs;

  fflush(stdout);

  FD_ZERO(&rdfs);
  FD_ZERO(&edfs);
  FD_ZERO(&wdfs);

  FD_SET(sockServer, &rdfs);
  FD_SET(sockServer, &wdfs);
  FD_SET(sockServer, &edfs);

  while(1){

    int i =0;
    int ret = 0;
 

    /* add socket of each client */
     // for(i = 0; i < cpt_client; i++)
     // {
     //    FD_SET(clients[i].sock, &rdfs);
     // }

    if((ret = select(max + 1, &rdfs, &wdfs, &edfs, /* timeout*/NULL)) < 0)
    {
     perror("select()");
     exit(errno);
    }

    // new client
    if(FD_ISSET(sockServer, &rdfs))
    {

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

      
      Client c = { client_sock };
      strncpy(c.name, buffer, BUF_SIZE - 1);
      clients[cpt_client] = c;
      FD_SET(clients[cpt_client].sock, &rdfs);
      
      cpt_client++;
      //printf("on ajout le 1er client\n");
      fflush(stdout);
      continue;

    //after connection treat actions
    }
    else
    {
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
              if(c == 0)
               {
                  printf("Client disconnected !\n");
                  closesocket(clients[i].sock);
                  continue;
               }
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
                case LS :
                        printf("LS Print Directory %s\n",arg);
                        cmd_ls(clients[i].sock, arg);
                        break;
                case PUT :   
                        printf("PUT %s\n",arg);
                        struct dtp_info dtpinfo_put = {clients[i].sock, arg};
                        pthread_create(&threadid, NULL, &cmd_put, &dtpinfo_put);
                case GET :   
                        printf("GET %s\n",arg);
                        struct dtp_info dtpinfo_get = {clients[i].sock, arg};
                        pthread_create(&threadid, NULL, &cmd_get, &dtpinfo_get);
                        // cmd_get(clients[i].sock, arg);
                case INVALID :
                        printf("Invalid cmd %s\n",arg);
                        break;
              }
           }
        }
    }

  }//while

  return 0;
}// svr_main
