#ifndef PI_SVR_H
#define PI_SVR_H

#include "pi_commons.h"
#include "ftp_commons.h"

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

void cmd_ls(SOCKET sock, char *arg);
void cmd_put(SOCKET sock, char *arg);
void cmd_get(SOCKET sock, char *arg);

/**
* @todo use getsockopt(), select() with efds and wdfds, shutdown()
*
* @param[in] argc
* @param[in] argv
* @param[in] PORT
*/
int pi_svr_main(int argc, char *argv[]);

#endif /* PI_SVR_H */