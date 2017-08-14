#ifndef SOCKET6_IF_H
#define SOCKET6_IF_H

#include "uint32.h"

const char* socket_getifname(uint32 interface);
uint32 socket_getifidx(const char *ifname);

#endif
