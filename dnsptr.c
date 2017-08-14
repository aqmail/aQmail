#include "substdio.h"
#include "subfd.h"
#include "stralloc.h"
#include "strsalloc.h"
#include "str.h"
#include "scan.h"
#include "dns.h"
#include "dnsdoe.h"
#include "ipalloc.h"
#include "ip.h"
#include "exit.h"

strsalloc ssa = {0};
ipalloc ia = {0};
 
struct ip_address ip;
struct ip6_address ip6;

int main(int argc,char **argv)
{
  int j;
  int k;
 
  if (!argv[1]) _exit(100);

  dns_init(0);

  k = str_chr(argv[1],':');

  if (k > 0 && k > str_len(argv[1])) {
    if (!ip6_scan(&ip6,argv[1])) _exit(1);
    dnsdoe(dns_ptr6(&ssa,&ia.ix[0].addr.ip6));
  }
  else {
    if (!ip4_scan(&ip,argv[1])) _exit(1);
    dnsdoe(dns_ptr(&ssa,&ip));
  }
 
  for (j = 0;j < ssa.len;++j) { 
    substdio_putflush(subfdout,ssa.sa[j].s,ssa.sa[j].len);
    substdio_putsflush(subfdout,"\n");
  }

 _exit(0);
}
