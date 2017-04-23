#ifndef DTP_CLI_H
#define DTP_CLI_H

#include "pi_commons.h" /* SOCKET */

/**
 * init the sclient
 * @param  address {int}     address
 * @return {int} return new socket (file descriptor)
 */
int init_client_connection(const char *address, int port);

/*
* read msg from server
* @param  sock  {SOCKET}  socket  to listen
* @param  buffer  {char*}  buffer for msg
* @return {int} return Buffer size
*/
int read_server(SOCKET sock, void* buffer);

/**
* allow to send msg on server
* @param  sock  {SOCKET}  socket  to write
* @param  message  {char*}  Message to send
*/
void write_server(SOCKET sock, const void* buffer);

#endif /* DTP_CLI_H */
