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
#define SYST 5

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#endif

static int PORT =7777;
static int MAX_CLIENT = 10;

static void init(void);
static void end(void);
int init_server_connection(int port, int nb_client);
static int read_client(SOCKET sock, char *buffer);
void write_client(int socket_fd, char * message);
int parse_cmd(char* buffer, char * arg);
void cmd_pwd(SOCKET sock,char *arg);

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
}Client;
