#ifndef DTP_COMMONS_H
#define DTP_COMMONS_H

#include "pi_commons.h" /* SOCKET */

void fupload(SOCKET, const char*);
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