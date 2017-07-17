#ifndef PI_COMMONS_H
#define PI_COMMONS_H

//COMMAND TYPE IDENTIFIERS:
#define INVALID -1
#define PWD 0
#define CDUP 1
#define SMNT 2
#define STOU 3
#define MKD 4
#define RMD 5
#define SYST 6
#define DELE 7
#define CWD 8
#define PUT 9
#define GET 10
#define LS 11

/**
 * parse cmd send by client
 * @param[in] buffer 
 * @param[in] arg    
 *
 * @return cmd identifier
 */
int parse_cmd(char* buffer, char * arg);

#endif /* PI_COMMONS_H */