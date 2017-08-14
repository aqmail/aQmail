#ifndef IPALLOC_H
#define IPALLOC_H

#include "ip.h"

struct ip_mx {
  unsigned short af;
  union {
    struct ip_address ip;
    struct ip6_address ip6;
    } addr;
  int pref;
};

#include "gen_alloc.h"

GEN_ALLOC_typedef(ipalloc,struct ip_mx,ix,len,a)
int ipalloc_readyplus();
int ipalloc_append();

#endif
