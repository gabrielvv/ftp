#include <criterion/criterion.h>

#include <fcntl.h> // open
#include <unistd.h> // read, write, close
#include <errno.h>
#include <string.h>
#include <stdio.h> //fwrite, fclose, fread, remove

#include <sys/types.h>
#include <sys/socket.h> // socket, AF_INET...

#include "dtp_server.h"
#include "dtp_client.h"
#include "dtp_commons.h" /* COMMAND TYPE IDENTIFIERS */
#include "ftp_commons.h" /* SOCKET, BUF_SIZE */

void setup1(void){
	char* pathw = "local_upload.txt";
	FILE* fpw = fopen(pathw, "w");

	char bufferw[BUF_SIZE] = "foo\nbar";
	fwrite(bufferw, sizeof(char), strlen(bufferw), fpw);
	fclose(fpw);
}

void teardown1(void){
	remove("local_upload.txt");
	remove("socket.txt");
}

Test(/* suite name*/ftp_dtp, /* test name */fupload, .init = setup1, .fini = teardown1){
	
	/** socket simulation **/
	int fd = open("socket.txt", O_RDWR | O_CREAT, 0666);
	if(fd < 0){
		perror("open('socket.txt')");
      	exit(errno);
	}

	/********* EXECUTE ************/

	char* pathr = "local_upload.txt";
	char bufferw[BUF_SIZE] = "foo\nbar";
	
	// /!\ call write_socket
	fupload(fd, pathr);
	close(fd);

	/********* TEST ************/

	char bufferr[BUF_SIZE];
	FILE* fpr = fopen(pathr, "r");
	fread(bufferr, sizeof(char), BUF_SIZE, fpr);
	
	cr_assert_eq(strcmp(bufferr, bufferw), 0);
	fclose(fpr);

	// cr_assert_file_contents_eq_str(fpr, bufferw);

}

void setup2(void){
	printf("setup2\n");
	char* pathw = "socket.txt";
	FILE* fpw = fopen(pathw, "w");

	char bufferw[BUF_SIZE] = "foo\nbar";
	fwrite(bufferw, sizeof(char), strlen(bufferw), fpw);
	fclose(fpw);
}

void teardown2(void){
	remove("local_download.txt");
	remove("socket.txt");
}

Test(ftp_dtp, fdownload, .init = setup2, .fini = teardown2){

	char bufferr[BUF_SIZE];
	bzero(bufferr, sizeof(bufferr));
	char bufferw[BUF_SIZE] = "foo\nbar";

	/** socket simulation **/
	int fd = open("socket.txt", O_RDWR | O_CREAT, 0666);
	if(fd < 0){
		perror("open('socket.txt')");
      	exit(errno);
	}

	write(fd, bufferw, strlen(bufferw));
	if(lseek(fd, 0 , SEEK_SET) == -1){
		perror("lseek()");
		exit(errno);
	}
	// printf("SEEK_CUR %d\n", SEEK_CUR);

	/********* EXECUTE ************/

	char* pathw = "local_download.txt";
	
	// /!\ call read_socket
	fdownload(fd, pathw);

	// int n = read(fd, bufferr, BUF_SIZE);
	// printf("bufferw: %s\n", bufferr);
	close(fd);

	// ******** TEST ***********

	FILE* fpr = fopen(pathw, "r");
	bzero(bufferr, sizeof(bufferr));
	fread(bufferr, sizeof(char), BUF_SIZE, fpr);
	
	cr_assert_eq(strcmp(bufferr, bufferw), 0);
	fclose(fpr);

}