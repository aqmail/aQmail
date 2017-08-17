#include "alloc.h"
#include "stralloc.h"
#include "open.h"
//#include "cdb.h"
#include "cdbread.h"
#include "str.h"
#include "rules.h"
#include "strerr.h"
#include "byte.h"
#include "fmt.h"
#include "getln.h"
#include "ip4_bit.h"    // !!!!
//#include "ip6.h"
#include "ip.h"

stralloc rules_name = {0};
stralloc ipstring = {0};

static struct cdb c;

static int dorule(void (*callback)(char *, unsigned int)) {
  char *data;
  unsigned int datalen;

  switch (cdb_find(&c, rules_name.s, rules_name.len)) {
    case -1: return -1;
    case  0: return 0;
  }

  datalen = cdb_datalen(&c);
  data = alloc(datalen);
  if (!data) return -1;
  if (cdb_read(&c, data, datalen, cdb_datapos(&c)) == -1) {
    alloc_free(data);
    return -1;
  }

  callback(data, datalen);
  alloc_free(data);
  return 1;
}

static int doit(void (*callback)(char *, unsigned int), char *ip, char *host, char *info) {
  int p;
  int r;
  int ipv6 = str_len(ip) - byte_chr(ip,str_len(ip),':');

  if (info) {                           /* 1. info@ip */
    if (!stralloc_copys(&rules_name,info)) return -1;
    if (!stralloc_cats(&rules_name,"@")) return -1;
    if (ipv6) {
      if (ip6_expandaddr(ip,&ipstring) == 1)
        if (!stralloc_catb(&rules_name,ipstring.s,ipstring.len)) return -1;
    }
    else
      if (!stralloc_cats(&rules_name,ip)) return -1;
    r = dorule(callback);
    if (r) return r;

    if (host) {                         /* 2. info@=host */
      if (!stralloc_copys(&rules_name,info)) return -1;
      if (!stralloc_cats(&rules_name,"@=")) return -1;
      if (!stralloc_cats(&rules_name,host)) return -1;
      r = dorule(callback);
      if (r) return r;
    }
  }

  if (ipv6) {                           /* 3. IPv6/IPv4 */
    if (ip6_expandaddr(ip,&ipstring) == 1) {
        if (!stralloc_copyb(&rules_name,ipstring.s,ipstring.len)) return -1;
        r = dorule(callback);
        if (r) return r;
    }
  } else {
    if (!stralloc_copys(&rules_name,ip)) return -1;
    r = dorule(callback);
    if (r) return r;
  }

  if (host) {                           /* 4. =host */
    if (!stralloc_copys(&rules_name,"=")) return -1;
    if (!stralloc_cats(&rules_name,host)) return -1;
    r = dorule(callback);
    if (r) return r;
  }

  if (!ipv6) {                          /* 5. IPv4 class-based */
    if (!stralloc_copys(&rules_name,ip)) return -1;
    while (rules_name.len > 0) {
      if (ip[rules_name.len - 1] == '.') {
        r = dorule(callback);
        if (r) return r;
      }
      --rules_name.len;
    } 
  }

  if (ipv6) {                           /* 6. IPv6/IPv4 CIDR */
    if (ip6tobitstring(rules_name.s,&ipstring,128) == 1) {
      for (p=129; p>1; p--) {
        if (!stralloc_copys(&rules_name,"^")) return -1;
        if (!stralloc_catb(&rules_name,ipstring.s,p)) return -1;
        r = dorule(callback);
        if (r) return r;
      }
    }
  } else {
    if (!stralloc_copys(&rules_name,ip)) return -1;
    if (getaddressasbit(ip,32,&ipstring) != -1) {
      for (p=33; p>1; p--) {
        if (!stralloc_copys(&rules_name,"_")) return -1;
        if (!stralloc_catb(&rules_name,ipstring.s,p)) return -1;
        r = dorule(callback);
        if (r) return r;
      }
    }
  } 

  if (host) {                           /* 7. =host. */
    while (*host) {
      if (*host == '.') {
        if (!stralloc_copys(&rules_name,"=")) return -1;
        if (!stralloc_cats(&rules_name,host)) return -1;
        r = dorule(callback);
        if (r) return r;
      }
      ++host;
    }
    if (!stralloc_copys(&rules_name,"=")) return -1;   /* 8. = rule */
    r = dorule(callback);
    if (r) return r;
  } 

  rules_name.len = 0;
  return dorule(callback);
}

int rules(void (*callback)(char *, unsigned int), int fd, char *ip, char *host, char *info) {
  int r;
  cdb_init(&c, fd);
  r = doit(callback, ip, host, info);
  cdb_free(&c);
  return r;
}
