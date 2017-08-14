#include "cdb.h"
#include "byte.h"
#include "open.h"
#include "error.h"
#include "exit.h"
#include "control.h"
#include "constmap.h"
#include "stralloc.h"
#include "rcpthosts.h"

static int flagrh = 0;
static int flagmrh = 0;
static stralloc rh = {0};
static struct constmap maprh;
static int fdmrh;

int rcpthosts_init()
{
  flagrh = control_readfile(&rh,"control/rcpthosts",0);
  if (flagrh != 1) return flagrh;
  if (!constmap_init(&maprh,rh.s,rh.len,0)) return flagrh = -1;
  fdmrh = open_read("control/morercpthosts.cdb");
  if (fdmrh == -1) if (errno != error_noent) return flagmrh = -1;
  if (fdmrh > 0) flagmrh = 1;
  return 0;
}

static stralloc host = {0};

int rcpthosts(char *buf, int len)
{
  int j;

  if (flagrh != 1) return 1;

  j = byte_rchr(buf,len,'@');
  if (j >= len) return 1; /* presumably envnoathost is acceptable */

  ++j; buf += j; len -= j;

  if (!stralloc_copyb(&host,buf,len)) return -1;
  buf = host.s;
  case_lowerb(buf,len);

  for (j = 0;j < len;++j)
    if (!j || (buf[j] == '.'))
      if (constmap(&maprh,buf + j,len - j)) return 1;

  if (flagmrh == 1) {
    fdmrh = open_read("control/morercpthosts.cdb");
    if (fdmrh == -1) if (errno == error_noent) return 0;
    uint32 dlen;
    int r;

    for (j = 0;j < len;++j)
      if (!j || (buf[j] == '.')) {
	r = cdb_seek(fdmrh,buf + j,len - j,&dlen);
	if (r) return r;
      }
    close(fdmrh);
  }

  return 0;
}
