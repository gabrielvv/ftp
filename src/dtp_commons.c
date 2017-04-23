#include "dtp_commons.h"
#include "pi_commons.h" /* SOCKET */

void fsupload(SOCKET to_sock, FILE* file){
   //fread
}

void fupload(SOCKET to_sock, const char* path){
  printf("server upload start\n");
  fflush(stdout);

  FILE* fp = fopen(path, "r");
  char buffer[BUF_SIZE];
  memset(buffer, 0, BUF_SIZE);

  while(feof(fp) == 0){
    fread(buffer, sizeof(char), BUF_SIZE, fp);
    write_client(to_sock, buffer);
  }
  printf("end of server upload\n");
  fflush(stdout);
}  

void fdownload(SOCKET to_sock, const char* path){
   
   printf("client download start\n");
   fflush(stdout);

   FILE* fp = fopen(path, "w");
   char buffer[BUF_SIZE];

   while(1){
      int n = read_server(to_sock, buffer);

      // ecriture non formatée binaire
      // cf. remarques et limitations dans cours
      fwrite(buffer, sizeof(char), n, fp);

      // feof returns non-zero if the end-of-file indicator is set
      if(feof(fp) != 0)
         break;
   }

   printf("end of client download\n");
   fflush(stdout);
}