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

/** get current directory path
* @param sock socket client to write
* @param arg  argument
*/
void cmd_pwd(SOCKET sock,char *arg){
    char cwd[1024];
    char msg_return[1024] = "Current working dir : ";
    if(getcwd(cwd, sizeof(cwd)) != NULL){
      strcat(msg_return, cwd);
      printf("%s\n",msg_return);
      write_client(sock, msg_return);
    }
    else{
       perror("getcwd() error");
       write_client(sock, "getcwd() error");
    }

}

/**
 * create a new directory
 * @param sock socket client to write
 * @param arg  argument file path
 */
void cmd_mkdir(SOCKET sock, char *arg){
  if(arg == NULL){
    perror("mkdir() error");
  }else{
      mkdir(arg, 0777);
      write_client(sock, "created");
  }
}

/**
 * remove a directory
 * @param sock socket client to write
 * @param arg  argument directory path to delete
 */
void cmd_rmd(SOCKET sock, char *arg){
  if(rmdir(arg)==0){
     write_client(sock, "250 : OK, directory deleted \n");
   }else{
     write_client(sock, "550 Cannot delete directory.\n");
   }
}

/**
 * remove a file
 * @param sock socket client to write
 * @param arg  argument file path to delete
 */
void cmd_dele(SOCKET sock, char *arg){
  if(remove(arg)==0){
      write_client(sock, "250 : OK, file deleted \n");
   }else{
      write_client(sock, "550 Cannot delete file.\n");
   }
}

/**
 * change current diretory
 * @param sock socket client to write
 * @param arg    argument Directory path
 */
void cmd_cwd(SOCKET sock, char *arg){
  if(chdir(arg)==0){
      write_client(sock, "250 : OK, Directory changed \n");
    }else{
      write_client(sock, "250 : Failed to change directory \n");
    }
}
