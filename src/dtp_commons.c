#include "dtp_commons.h"
#include "ftp_commons.h" /* SOCKET */
#include <stdio.h> /* FILE */
#include <string.h> /* memset, strerror */
#include <errno.h>

void fsupload(SOCKET to_sock, FILE* fp){
  char buffer[BUF_SIZE];
  memset(buffer, 0, BUF_SIZE);

  while(feof(fp) == 0){
    fread(buffer, sizeof(char), BUF_SIZE, fp);
    write_socket(to_sock, buffer);
  }

  fclose(fp);
  printf("end of server upload\n");
  fflush(stdout);
}

void fupload(SOCKET to_sock, const char* path){
  printf("server upload start\n");
  fflush(stdout);
  char buffer[BUF_SIZE];
  memset(buffer, 0, BUF_SIZE);

  FILE* fp = fopen(path, "r");
  if(fp == NULL){
    sprintf(buffer, "%s\n", strerror(errno));
    write_socket(to_sock, buffer);
    return;
  }
  

  while(feof(fp) == 0){
    fread(buffer, sizeof(char), BUF_SIZE, fp);
    write_socket(to_sock, buffer);
  }
  fclose(fp);
  printf("end of server upload\n");
  fflush(stdout);
}

void fdownload(SOCKET from_sock, const char* path){
   
   printf("client download start\n");
   fflush(stdout);
   char buffer[BUF_SIZE];

   FILE* fp = fopen(path, "w");
   if(fp == NULL){
    sprintf(buffer, "%s\n", strerror(errno));
    write_socket(from_sock, buffer);
    return;
   } 
   

   while(1){
      int n = read_socket(from_sock, buffer);

      int eof = 0;

      // printf("fdownload buffer:%s\n", buffer);
      // printf("fdownload n:%d\n", n);
      
      // ecriture non formatée binaire
      // cf. remarques et limitations dans cours
      fwrite(buffer, sizeof(char), strlen(buffer), fp);

      break;
      int i = 0;
      for(; i < strlen(buffer); i++){
        if(buffer[i] == '\0'){
          eof = 1;
          break;
        }
      }

      if(eof || n == 0)
        break;
   }

   fclose(fp);
   printf("end of client download\n");
   fflush(stdout);
}