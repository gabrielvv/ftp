#include "pi_commons.h"

#ifdef WIN32
#include <winsock2.h>
#endif

void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      return -1;
   }
#endif
}

void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}