#ifndef FTP_COMMONS_H
#define FTP_COMMONS_H

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define BUF_SIZE 1024
#define closesocket(s) close(s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
}Client;

struct dtp_info
{
   SOCKET sock;
   char *arg;
};

/**
* windows support
*/
void init(void);

/**
* windows support
*/
void end(void);

/**
 * \brief init the server
 * @param[in]  port      port
 * @param[in]  nb_client Number of clients
 *
 * @return new socket (file descriptor)
 */
SOCKET init_server_connection(int port, int nb_client);

/**
 * init the sclient
 * @param  address
 * @return new socket (file descriptor)
 */
SOCKET init_client_connection(const char *address, const int port);

/*
* \brief read msg from client
* @param[in]  client_sock   socket  to listen
* @param[out] buffer buffer for msg
*
* @return Buffer size
*/
int read_socket(SOCKET sock, char*);

/**
* \brief allow to send msg on client
* @param[in, out] client_sock socket to write
* @param[in]      message     Message to send
*/
void write_socket(SOCKET sock, const char*);

int is_socket(int fd);

#endif /* FTP_COMMONS_H */