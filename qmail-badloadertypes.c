/*
 *  Revision 20170926, Kai Peter
 *  - changed 'control' directory name to 'etc'
*/
#include <sys/stat.h>
#include "strerr.h"
#include "stralloc.h"
#include "substdio.h"
#include "getln.h"
#include "exit.h"
#include "readwrite.h"
#include "open.h"
#include "auto_qmail.h"
#include "cdbmake.h"
#include "rename.h"

#define FATAL "qmail-badloadertypes: fatal: "
#define LOADER_LEN 5

void die_read()
{
  strerr_die2sys(111,FATAL,"unable to read etc/badloadertypes: ");
}
void die_write()
{
  strerr_die2sys(111,FATAL,"unable to write to etc/badloadertypes.tmp: ");
}

char inbuf[1024];
//substdio ssin;
buffer bin;

int fd;
int fdtemp;

//struct cdbmss cdbmss;
struct cdb_make c;
stralloc line = {0};
int match;

int main()
{
  umask(033);
  if (chdir(auto_qmail) == -1)
    strerr_die4sys(111,FATAL,"unable to chdir to ",auto_qmail,": ");

  fd = open_read("etc/badloadertypes");
  if (fd == -1) die_read();

//  substdio_fdbuf(&ssin,read,fd,inbuf,sizeof(inbuf));
  buffer_init(&bin,read,fd,inbuf,sizeof(inbuf));

  fdtemp = open_trunc("etc/badloadertypes.tmp");
  if (fdtemp == -1) die_write();

//  if (cdbmss_start(&cdbmss,fdtemp) == -1) die_write();
  if (cdb_make_start(&c,fdtemp) == -1) die_write();

  for (;;) {
    if (getln(&bin,&line,&match,'\n') != 0) die_read();
    if (line.s[0] != '#' && line.len > LOADER_LEN)
//      if (cdbmss_add(&cdbmss,line.s,LOADER_LEN,"",0) == -1)
      if (cdb_make_add(&c,line.s,LOADER_LEN,"",0) == -1)
    die_write();
    if (!match) break;
  }

//  if (cdbmss_finish(&cdbmss) == -1) die_write();
  if (cdb_make_finish(&c) == -1) die_write();
  if (fsync(fdtemp) == -1) die_write();
  if (close(fdtemp) == -1) die_write(); /* NFS stupidity */
  if (rename("etc/badloadertypes.tmp","etc/badloadertypes.cdb") == -1)
    strerr_die2sys(111,FATAL,"unable to move etc/badloadertypes.tmp to etc/badloadertypes.cdb");

  _exit(0);
}
