#include "alloc.h"
#include "stralloc.h"
#include "open.h"
#include "cdb.h"
#include "case.h"
#include "mfrules.h"
#include "str.h"
#include "byte.h"

/* return -9: problems reading cdb */
/* return -1: key matches; data not */
/* return  0: no key */
/* return  1: key matches without data */
/* return  2: key and data match */

stralloc key = {0};

static struct cdb c;

static int mffind(char *mf)
{
  char *x;
  char *data;
  unsigned int datalen;
  int plus = 0;
  int dlen;
  int len;
  int mflen;
  int delta;

  switch(cdb_find(&c,key.s,key.len)) {
    case -1: return -9;
    case  0: return 0;
  }

  datalen = cdb_datalen(&c);
  data = alloc(datalen);
  if (!data) return -9;
  if (!datalen) return 1;
  mflen = str_len(mf);

  if (cdb_read(&c,data,datalen,cdb_datapos(&c)) == -1) {
    alloc_free(data);
    return -9;
  }

  x = data; dlen = datalen - 1;	/* trailing separator */
 
  while (dlen > 0) {
    plus = byte_rchr(data,dlen,'+');
    x = data + plus + 1;
    len = dlen - plus;
    delta = (mflen > len) ? mflen - len : 0;
    if (!byte_diff(x,len,mf + delta)) { alloc_free(data); return 2; }
    dlen = plus - 1;
  }

  alloc_free(data);
  return -1;
} 

int mfsearch(char *ip,char *host,char *info,char *mf)
{
  int r;

  if (info) {
    if (!stralloc_copys(&key,info)) return -9;
    r = mffind(mf);
    if (r < -1 || r > 0) return r;

    if (!stralloc_cats(&key,"@")) return -9;
    if (!stralloc_cats(&key,ip)) return -9;
    r = mffind(mf);
    if (r < -1 || r > 0) return r;

    if (host) {
      if (!stralloc_copys(&key,info)) return -9;
      if (!stralloc_cats(&key,"@=")) return -9;
      if (!stralloc_cats(&key,host)) return -9;
      r = mffind(mf);
      if (r < -1 || r > 0) return r;
    }
  }

  if (!stralloc_copys(&key,ip)) return -9;
  r = mffind(mf);
  if (r < -1 || r > 0) return r;

  if (host) {
    if (!stralloc_copys(&key,"=")) return -9;
    if (!stralloc_cats(&key,host)) return -9;
    r = mffind(mf);
    if (r < -1 || r > 0) return r;
  }

  if (!stralloc_copys(&key,ip)) return -9;	/* IPv6 */
  while (key.len > 0) {
    if (ip[key.len - 1] == ':') {
      r = mffind(mf);
      if (r < -1 || r > 0) return r;
    }
    --key.len;
  }

  if (!stralloc_copys(&key,ip)) return -9;	/* IPv4 */
  while (key.len > 0) {
    if (ip[key.len - 1] == '.') {
      r = mffind(mf);
      if (r < -1 || r > 0) return r;
    }
    --key.len;
  }

  if (host) {
    while (*host) {
      if (*host == '.') {
        if (!stralloc_copys(&key,"=")) return -9;
        if (!stralloc_cats(&key,host)) return -9;
        r = mffind(mf);
        if (r < -1 || r > 0) return r;
      }
      ++host;
    }
    if (!stralloc_copys(&key,"=")) return -9;
    r = mffind(mf);
    if (r < -1 || r > 0) return r;
  }

  key.len = 0;
/*  return mffind(mf); */
  return -1;
}

int mfrules(int fd,char *ip,char *host,char *info,char *mf)
{
  int r;
  cdb_init(&c,fd);
  case_lowers(mf);
  r = mfsearch(ip,host,info,mf);
  cdb_free(&c);
  return r;
}
