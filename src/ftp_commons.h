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

/*
* \brief read msg from client
* @param[in]  client_sock   socket  to listen
* @param[out] buffer buffer for msg
*
* @return Buffer size
*/
int read_client(SOCKET client_sock, char *buffer);

/**
* \brief allow to send msg on client
* @param[in, out] client_sock socket to write
* @param[in]      message     Message to send
*/
void write_client(SOCKET client_sock, char * message);

/**
 * init the sclient
 * @param  address
 * @return new socket (file descriptor)
 */
SOCKET init_client_connection(const char *address, const int port);

/*
* read msg from server
* @param  sock  {SOCKET}  socket  to listen
* @param  buffer  {char*}  buffer for msg
* @return {int} return Buffer size
*/
int read_server(SOCKET sock, char *buffer);

/**
* allow to send msg on server
* @param  sock  {SOCKET}  socket  to write
* @param  message  {char*}  Message to send
*/
void write_server(SOCKET sock, const char *buffer);

#endif /* FTP_COMMONS_H */