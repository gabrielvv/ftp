#include "server.c"

int main(int argc, char *argv[]){
  printf("Init server\n");
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
    printf("on boucle" );

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
      printf("new Client\n");
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
      printf("on recoit le 1er msg\n");
      /* wnew maximum fd ? */
      max = new_sock > max ? new_sock : max;

      FD_SET(new_sock, &rdfs);
      Client c = { new_sock };
      strncpy(c.name, buffer, BUF_SIZE - 1);
      clients[cpt_client] = c;
      cpt_client++;
      printf("on ajout le 1er client");
      fflush(stdout);
      continue;
    //after connection treat actions
    }else{
      printf("traitement ...\n");
      fflush(stdout);
      int i = 0;
        for(i = 0; i < cpt_client; i++)
        {
           /* a client send a cmd */
           if(FD_ISSET(clients[i].sock, &rdfs))
           {
              Client client = clients[i];
              int c = read_client(clients[i].sock, buffer);
           }
        }
    }
    printf("traitement ...\n");
    fflush(stdout);
  };
  return 0;

}
