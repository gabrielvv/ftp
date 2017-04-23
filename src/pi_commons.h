#ifndef PI_COMMONS_H
#define PI_COMMONS_H

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
typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
}Client;

/**
* windows support
*/
void init(void);

/**
* windows support
*/
void end(void);

#endif /* PI_COMMONS_H */