#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include "socket6_if.h"

static char ifname[IFNAMSIZ];

const char* socket_getifname(uint32 interface) {
  char *tmp=if_indextoname(interface,ifname);
  if (tmp)
    return tmp;
  else
    return "[unknown]";
}

uint32 socket_getifidx(const char *ifname) {
  return if_nametoindex(ifname);
}

/*
void *get_addr_ptr(struct sockaddr *sockaddr_ptr)
{
  void *addr_ptr = 0;
  if (sockaddr_ptr->sa_family == AF_INET) {
     addr_ptr = &((struct sockaddr_in *)  sockaddr_ptr)->sin_addr;
  }
  else if (sockaddr_ptr->sa_family == AF_INET6) {
     addr_ptr = &((struct sockaddr_in6 *) sockaddr_ptr)->sin6_addr;
  }
  return addr_ptr;
}

uint32_t sockaddr_scope_id(const struct sockaddr* sa)
{
  uint32_t scope_id;
  if (AF_INET6 == sa->sa_family) {
    struct sockaddr_in6 s6;
    memcpy (&s6, sa, sizeof(s6));
    scope_id = s6.sin6_scope_id;
  } else
     scope_id = 0;
  return scope_id;
} 

*/
