#ifndef PI_SVR_H
#define PI_SVR_H

#include "pi_commons.h"

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
 * parse cmd send by client
 * @param[in] buffer [description]
 * @param[in] arg    [description]
 *
 * @return cmd identifier
 */
int parse_cmd(char* buffer, char * arg);

/** get current directory path
* @param[in, out] sock socket client to write
* @param[in]      arg  argument
*/
void cmd_pwd(SOCKET sock,char *arg);

/**
 * create a new directory
 * @param[in, out] sock socket client to write
 * @param[in]      arg  argument file path
 */
void cmd_mkdir(SOCKET sock, char *arg);

/**
 * remove a directory
 * @param sock socket client to write
 * @param arg  argument directory path to delete
 */
void cmd_rmd(SOCKET sock, char *arg);

/**
 * remove a file
 * @param[in, out] sock socket client to write
 * @param[in] arg  argument file path to delete
 */
void cmd_dele(SOCKET sock, char *arg);

/**
 * change current diretory
 * @param[in, out] sock socket client to write
 * @param[in] arg  argument Directory path
 */
void cmd_cwd(SOCKET sock, char *arg);

/**
*
* @param[in] argc
* @param[in] argv
* @param[in] PORT 
*/
int svr_main(int argc, char *argv[], int PORT);

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
} Client;

#endif /* PI_SVR_H */