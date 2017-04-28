#ifndef DTP_COMMONS_H
#define DTP_COMMONS_H

#include "ftp_commons.h" /* SOCKET */
#include <stdio.h> /* FILE */

/**
*
* upload of file pointed by path
*
*/
void fupload(SOCKET, const char* path);
void fsupload(SOCKET, FILE*);

/**
* w flag
* Truncate file to zero length or create text  file  for  writing.
* The stream is positioned at the beginning of the file.
*
* @brief download data as file and store to path
*
* @param to_sock {SOCKET}
* @param path    {char*}
*
* @see readv
* @see fopen
* @see recvmmsg
* @see recv
* @see select/pselect/poll/ppoll
* @see memset
*/
void fdownload(SOCKET, const char*);

#endif /* DTP_COMMONS_H */