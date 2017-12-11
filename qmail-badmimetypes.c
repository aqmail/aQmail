/*
 *  Revision 20170926, Kai Peter
 *  - changed 'control' directory name to 'etc'
*/
#include <sys/stat.h>
#include "strerr.h"
#include "stralloc.h"
#include "getln.h"
#include "exit.h"
#include "readwrite.h"
#include "open.h"
#include "auto_qmail.h"
#include "qlibs/include/cdbmake.h"
#include "rename.h"

#define FATAL "qmail-badmimetypes: fatal: "
#define MIMETYPE_LEN 9

void die_read()
{
  strerr_die2sys(111,FATAL,"unable to read etc/badmimetypes: ");
}
void die_write()
{
  strerr_die2sys(111,FATAL,"unable to write to etc/badmimetypes.tmp: ");
}

char inbuf[1024];
buffer bin;

int fd;
int fdtemp;

struct cdb_make c;
stralloc line = {0};
int match;

int main()
{
  umask(033);
  if (chdir(auto_qmail) == -1)
    strerr_die4sys(111,FATAL,"unable to chdir to ",auto_qmail,": ");

  fd = open_read("etc/badmimetypes");
  if (fd == -1) die_read();

//  substdio_fdbuf(&ssin,read,fd,inbuf,sizeof(inbuf));
  buffer_init(&bin,read,fd,inbuf,sizeof(inbuf));

  fdtemp = open_trunc("etc/badmimetypes.tmp");
  if (fdtemp == -1) die_write();

//  if (cdbmss_start(&cdbmss,fdtemp) == -1) die_write();
  if (cdb_make_start(&c,fdtemp) == -1) die_write();

  for (;;) {
    if (getln(&bin,&line,&match,'\n') != 0) die_read();
    if (line.s[0] != '#' && line.len > MIMETYPE_LEN)
//      if (cdbmss_add(&cdbmss,line.s,MIMETYPE_LEN,"",0) == -1)
      if (cdb_make_add(&c,line.s,MIMETYPE_LEN,"",0) == -1)
    die_write();
    if (!match) break;
  }

//  if (cdbmss_finish(&cdbmss) == -1) die_write();
  if (cdb_make_finish(&c) == -1) die_write();
  if (fsync(fdtemp) == -1) die_write();
  if (close(fdtemp) == -1) die_write(); /* NFS stupidity */
  if (rename("etc/badmimetypes.tmp","etc/badmimetypes.cdb") == -1)
    strerr_die2sys(111,FATAL,"unable to move etc/badmimetypes.tmp to etc/badmimetypes.cdb");

  _exit(0);
}
