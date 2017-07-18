#ifndef PI_CLI_H
#define PI_CLI_H

#include "ftp_commons.h"
#include "pi_commons.h"

int pi_cli_main(int argc, char **argv);
void* data_transfer(void* arg);

#endif /* PI_CLI_H */
