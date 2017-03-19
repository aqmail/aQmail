#include "strerr.h"
#include "stralloc.h"
#include "substdio.h"
#include "getln.h"
#include "exit.h"
#include "readwrite.h"
#include "open.h"
#include "auto_qmail.h"
#include "cdbmss.h"

#define FATAL "qmail-badloadertypes: fatal: "
#define LOADER_LEN 5

void die_read()
{
  strerr_die2sys(111,FATAL,"unable to read control/badloadertypes: ");
}
void die_write()
{
  strerr_die2sys(111,FATAL,"unable to write to control/badloadertypes.tmp: ");
}

char inbuf[1024];
substdio ssin;

int fd;
int fdtemp;

struct cdbmss cdbmss;
stralloc line = {0};
int match;

int main()
{
  umask(033);
  if (chdir(auto_qmail) == -1)
    strerr_die4sys(111,FATAL,"unable to chdir to ",auto_qmail,": ");

  fd = open_read("control/badloadertypes");
  if (fd == -1) die_read();

  substdio_fdbuf(&ssin,read,fd,inbuf,sizeof(inbuf));

  fdtemp = open_trunc("control/badloadertypes.tmp");
  if (fdtemp == -1) die_write();

  if (cdbmss_start(&cdbmss,fdtemp) == -1) die_write();

  for (;;) {
    if (getln(&ssin,&line,&match,'\n') != 0) die_read();
    if (line.s[0] != '#' && line.len > LOADER_LEN)
      if (cdbmss_add(&cdbmss,line.s,LOADER_LEN,"",0) == -1)
	die_write();
    if (!match) break;
  }

  if (cdbmss_finish(&cdbmss) == -1) die_write();
  if (fsync(fdtemp) == -1) die_write();
  if (close(fdtemp) == -1) die_write(); /* NFS stupidity */
  if (rename("control/badloadertypes.tmp","control/badloadertypes.cdb") == -1)
    strerr_die2sys(111,FATAL,"unable to move control/badloadertypes.tmp to control/badloadertypes.cdb");

  _exit(0);
}
