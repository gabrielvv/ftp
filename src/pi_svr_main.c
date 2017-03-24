#include "pi_server.c"

int main(int argc, char *argv[]){

  printf("\n");
  printf("\n");
  printf("___  _____  ___     ___  ___  ___ __   __ ___  ___\n");
  printf("| __||_   _|| _ \\  \\ / __|| __|| _ \\\\ \\ / /| __|| _ \\\n");
  printf("| _|   | |  |  _/   \\__ \\| _| |   / \\ V / | _| |   /\n");
  printf("|_|    |_|  |_|     |___/|___||_|_\\  \\_/  |___||_|_\\\n");
  printf("\n");
  printf("\n");
  printf("--help To see the different available commands\n");

  //printf("Init server\n");
  init(); //init socket for window;
  Client clients[MAX_CLIENT];
  int cpt_client = 0;
  char buffer[BUF_SIZE];
  SOCKET sockServer = init_server_connection(PORT,MAX_CLIENT);
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
      int new_sock = accept(sockServer, NULL,NULL);
      if(new_sock == SOCKET_ERROR)
      {
          perror("accept()");
          continue;
      }
      /* after connecting */
      //TODO need authentification
      if(read_client(new_sock, buffer) == -1)
      {
          /* disconnected */
          continue;
      }
      /* wnew maximum fd ? */
      max = new_sock > max ? new_sock : max;

      FD_SET(new_sock, &rdfs);
      Client c = { new_sock };
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
              int c = read_client(clients[i].sock, buffer);
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

}
