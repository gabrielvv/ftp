#include <criterion/criterion.h>
#include "pi_server.h"
#include <string.h>

Test(server, test){
	
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
