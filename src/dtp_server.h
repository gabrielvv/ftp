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
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define BUF_SIZE 1024
#define closesocket(s) close(s)

//COMMAND TYPE IDENTIFIERS:
#define INVALID -1
#define PWD 0
#define CDUP 1
#define SMNT 2
#define STOU 3
#define MKD 4
#define RMD 5
#define SYST 6
#define DELE 7
#define CWD 8

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#endif

// variable/fonction statique => invisible à l'extérieur du fichier où elle est déclarée

#define MAX_CLIENT 10

void init(void);
void end(void);
int init_server_connection(int port, int nb_client);
int read_client(SOCKET sock, char *buffer);
void write_client(int socket_fd, char * message);
int parse_cmd(char* buffer, char * arg);
void cmd_pwd(SOCKET sock,char *arg);
void cmd_mkdir(SOCKET sock, char *arg);
void cmd_rmd(SOCKET sock, char *arg);
void cmd_dele(SOCKET sock, char *arg);
void cmd_cwd(SOCKET sock, char *arg);

void fupload(SOCKET, const char*);
void fsupload(SOCKET, FILE*);
void fdownload(SOCKET, const char*);

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
}Client;
