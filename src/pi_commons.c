#include "pi_commons.h"
#include <string.h>

int parse_cmd(char* buffer, char * arg)
{

memset(arg, 0, strlen(arg));
  while(*buffer == ' ' || *buffer == '\t') {
      buffer++;
  }

  int cmd = INVALID;
  if(strncmp(buffer, "pwd", 3) == 0){
    buffer = buffer + 3;
    cmd = PWD;
  }
  if(strncmp(buffer, "cdup", 4) == 0){
    buffer = buffer + 4;
    cmd = CDUP;
  }
  if(strncmp(buffer, "smnt", 4) == 0){
    buffer = buffer + 4;
    cmd = SMNT;
  }
  if(strncmp(buffer, "stou", 4) == 0){
    buffer = buffer + 4;
    cmd = STOU;
  }

  if(strncmp(buffer, "mkd", 3) == 0){
    buffer = buffer + 3;
    cmd = MKD;
  }
  if(strncmp(buffer, "rmd", 3) ==0){
    buffer = buffer + 3;
    cmd = RMD;
  }

  if(strncmp(buffer, "syst", 4) == 0){
    buffer = buffer + 4;
    cmd = SYST;
  }

  if(strncmp(buffer, "dele",4)==0){
    buffer = buffer + 4;
    cmd = DELE;
  }

  if(strncmp(buffer, "cwd", 3)==0){
    buffer = buffer + 3;
    cmd = CWD;
  }

  if(strncmp(buffer,"ls",3)==0){
    buffer = buffer +2;
    cmd = LS;
  }

  if(strncmp(buffer,"put",3)==0){
    buffer = buffer +3;
    cmd = PUT;
  }

  if(strncmp(buffer,"get",3)==0){
    buffer = buffer +3;
    cmd = GET;
  }


  if(arg != NULL){

    while(*buffer == ' ' || *buffer == '\t') {
        buffer++;
    }

    //Copy the argument over:
    while((*buffer != ' ') && (*buffer != '\n') && (*buffer != '\r') && (*buffer != '\0')) {
        *arg = *buffer;
        arg++;
        buffer++;
    }
    //Null-terminate:
    *arg = '\0';
  }
  return cmd;
}