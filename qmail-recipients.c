#include <unistd.h>
#include <sys/stat.h>
#include "strerr.h"
#include "stralloc.h"
#include "substdio.h"
#include "getln.h"
//#include "exit.h"
//#include "readwrite.h"
#include "open.h"
#include "auto_qmail.h"
//#include "cdbmss.h"
#include "qlibs/include/cdbmake.h"
//#include "qlibs/include/cdbread.h"
#include "case.h"
#include "rename.h"

#define FATAL "qmail-recipients: fatal: "

void die_read()
{
  strerr_die2sys(111,FATAL,"unable to read users/recipients: ");
}
void die_write()
{
  strerr_die2sys(111,FATAL,"unable to write to users/recipients.tmp: ");
}

char inbuf[1024];
//substdio ssin;
buffer bin;

int fd;
int fdtemp;

//struct cdbmss cdbmss;
static struct cdb_make c;
stralloc line = {0};
stralloc key  = {0};
int match;

int main()
{
  umask(033);
  if (chdir(auto_qmail) == -1)
    strerr_die4sys(111,FATAL,"unable to chdir to ",auto_qmail,": ");

  fd = open_read("users/recipients");
  if (fd == -1) die_read();

//  substdio_fdbuf(&ssin,read,fd,inbuf,sizeof(inbuf));
  buffer_init(&bin,read,fd,inbuf,sizeof(inbuf));

  fdtemp = open_trunc("users/recipients.tmp");
  if (fdtemp == -1) die_write();

//  if (cdbmss_start(&cdbmss,fdtemp) == -1) die_write();
  if (cdb_make_start(&c,fdtemp) == -1) die_write();

  for (;;) {
    stralloc_copys(&key,":");
//    if (getln(&ssin,&line,&match,'\n') != 0) die_read();
    if (getln(&bin,&line,&match,'\n') != 0) die_read();
    while (line.len) {
      if (line.s[line.len - 1] == ' ') { --line.len; continue; }
      if (line.s[line.len - 1] == '\n') { --line.len; continue; }
      if (line.s[line.len - 1] == '\t') { --line.len; continue; }
      if (line.s[0] != '#' && stralloc_cat(&key,&line)) {
        case_lowerb(key.s,key.len);
//	if (cdbmss_add(&cdbmss,key.s,key.len,"",0) == -1)
    if (cdb_make_add(&c,key.s,key.len,"",0) == -1)
	  die_write();
      }
      break;
    }
    if (!match) break;
  }

//  if (cdbmss_finish(&cdbmss) == -1) die_write();
  if (cdb_make_finish(&c) == -1) die_write();
  if (fsync(fdtemp) == -1) die_write();
  if (close(fdtemp) == -1) die_write(); /* NFS stupidity */
  if (rename("users/recipients.tmp","users/recipients.cdb") == -1)
    strerr_die2sys(111,FATAL,"unable to move users/recipients.tmp to users/recipients.cdb");

  _exit(0);
}
