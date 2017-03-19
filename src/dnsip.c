#include <sys/types.h>
#include <sys/socket.h>
#include "substdio.h"
#include "subfd.h"
#include "stralloc.h"
#include "dns.h"
#include "dnsdoe.h"
#include "ip.h"
#include "ipalloc.h"
#include "exit.h"

char ipaddr[IPFMT];

stralloc sa = {0};
ipalloc ia = {0};

int main(int argc,char **argv)
{
 int j;

 if (!argv[1]) _exit(100);

 if (!stralloc_copys(&sa,argv[1]))
  { substdio_putsflush(subfderr,"out of memory\n"); _exit(111); }

 dns_init(0);
 dnsdoe(dns_ip(&ia,&sa));
 for (j = 0;j < ia.len;++j)
  {
   switch(ia.ix[j].af) {
   case AF_INET:
      substdio_put(subfdout,ipaddr,ip4_fmt(ipaddr,&ia.ix[j].addr.ip));
      break;
   case AF_INET6:
      substdio_put(subfdout,ipaddr,ip6_fmt(ipaddr,&ia.ix[j].addr.ip6));
      break;
   default:
      substdio_puts(subfdout,"Unknown address family = ");
      substdio_put(subfdout,ipaddr,fmt_ulong(ipaddr,ia.ix[j].af));
   }
   substdio_putsflush(subfdout,"\n");
  }
 _exit(0);
}
