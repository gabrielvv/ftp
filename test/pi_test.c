#include <criterion/criterion.h>

#include "pi_server.h"
#include "pi_client.h"
#include "pi_commons.h" /* COMMAND TYPE IDENTIFIERS */
#include "ftp_commons.h" /* SOCKET, BUF_SIZE */

#include <string.h>
#include <unistd.h> /* close */

#define DTP_PORT 1025
#define PI_PORT  1026

/**
* @see http://criterion.readthedocs.io/en/master/starter.html
* @see http://criterion.readthedocs.io/en/master/assert.html
*
*/
Test(/* suite name*/ftp_pi, /* test name */parse_cmd){
	
	char buffer[BUF_SIZE] = "smnt";
	char arg[BUF_SIZE];
	cr_assert_eq(parse_cmd(buffer, arg), SMNT);

	bzero(buffer, sizeof(buffer));
	strcat(buffer,"pwd");
	cr_assert_eq(parse_cmd(buffer, arg), PWD);

	bzero(buffer, sizeof(buffer));
	strcat(buffer,"cdup");
	cr_assert_eq(parse_cmd(buffer, arg), CDUP);

	bzero(buffer, sizeof(buffer));
	strcat(buffer,"stou");
	cr_assert_eq(parse_cmd(buffer, arg), STOU);

    bzero(buffer, sizeof(buffer));
	strcat(buffer,"mkd");
	cr_assert_eq(parse_cmd(buffer, arg), MKD);
}

Test(ftp_pi, init_svr_connection){
	SOCKET sock = init_server_connection(PI_PORT, 1);
	cr_assert(sock >= 0);
	close(sock);
}

Test(ftp_pi, init_cli_connection){
	SOCKET svr_sock = init_server_connection(PI_PORT, 1);
	SOCKET cli_sock = init_client_connection("127.0.0.1", PI_PORT);
	cr_assert(cli_sock >= 0);
	close(cli_sock);
	close(svr_sock);
}

Test(ftp_pi, ftp_read_client){

	// char buffer[BUF_SIZE];
	// SOCKET svr_sock = init_server_connection(PI_PORT, 1);

	// SOCKET cli_sock = accept(svr_sock, NULL, NULL);
	// if(cli_sock == SOCKET_ERROR)
	// {
	//   perror("accept()");
	// }
	// read_client(cli_sock, buffer);
	// close(svr_sock);
	// close(cli_sock);
}

Test(ftp_pi, ftp_write_client){
	//todo
}
