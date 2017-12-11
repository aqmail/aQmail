/*
 *  Revision 20170926, Kai Peter
 *  - moved directory 'users' to 'var/users'
 *  Revision 20160713, Kai Peter
 *  - switched to 'buffer'
 *  Revision 20170306, Kai Peter
 *  - switched to new(er) cdb lib from ucspi-tcp-0.88
 *  Revision 20160509, Kai Peter
 *  - changed return type of main to int
 *  - added '<sys/stat.h>, 'byte.h', 'rename.h'
 */
#include <unistd.h>
#include <sys/stat.h>
#include "stralloc.h"
#include "getln.h"
#include "buffer.h"
#include "qlibs/include/cdbmake.h"
#include "open.h"
#include "error.h"
#include "case.h"
#include "auto_qmail.h"
#include "byte.h"
#include "rename.h"

void die_temp() { _exit(111); }

void die_chdir()
{
  buffer_putsflush(buffer_2,"qmail-newu: fatal: unable to chdir\n");
  die_temp();
}
void die_nomem()
{
  buffer_putsflush(buffer_2,"qmail-newu: fatal: out of memory\n");
  die_temp();
}
void die_opena()
{
  buffer_putsflush(buffer_2,"qmail-newu: fatal: unable to open var/users/assign\n");
  die_temp();
}
void die_reada()
{
  buffer_putsflush(buffer_2,"qmail-newu: fatal: unable to read var/users/assign\n");
  die_temp();
}
void die_format()
{
  buffer_putsflush(buffer_2,"qmail-newu: fatal: bad format in var/users/assign\n");
  die_temp();
}
void die_opent()
{
  buffer_putsflush(buffer_2,"qmail-newu: fatal: unable to open var/users/cdb.tmp\n");
  die_temp();
}
void die_writet()
{
  buffer_putsflush(buffer_2,"qmail-newu: fatal: unable to write var/users/cdb.tmp\n");
  die_temp();
}
void die_rename()
{
  buffer_putsflush(buffer_2,"qmail-newu: fatal: unable to move var/users/cdb.tmp to var/users/cdb\n");
  die_temp();
}

struct cdb_make c;
stralloc key = {0};
stralloc data = {0};

char inbuf[1024];
buffer ssin;

int fd;
int fdtemp;

stralloc line = {0};
int match;

stralloc wildchars = {0};

int main()
{
  int i;
  int numcolons;

  umask(033);
  if (chdir(auto_qmail) == -1) die_chdir();

  fd = open_read("var/users/assign");
  if (fd == -1) die_opena();

  buffer_init(&ssin,read,fd,inbuf,sizeof(inbuf));

  fdtemp = open_trunc("/var/users/cdb.tmp");
  if (fdtemp == -1) die_opent();

  if (cdb_make_start(&c,fdtemp) == -1) die_writet();

  if (!stralloc_copys(&wildchars,"")) die_nomem();

  for (;;) {
    if (getln(&ssin,&line,&match,'\n') != 0) die_reada();
    if (line.len && (line.s[0] == '.')) break;
    if (!match) die_format();

    if (byte_chr(line.s,line.len,'\0') < line.len) die_format();
    i = byte_chr(line.s,line.len,':');
    if (i == line.len) die_format();
    if (i == 0) die_format();
    if (!stralloc_copys(&key,"!")) die_nomem();
    if (line.s[0] == '+') {
      if (!stralloc_catb(&key,line.s + 1,i - 1)) die_nomem();
      case_lowerb(key.s,key.len);
      if (i >= 2)
    if (byte_chr(wildchars.s,wildchars.len,line.s[i - 1]) == wildchars.len)
      if (!stralloc_append(&wildchars,line.s + i - 1)) die_nomem();
    }
    else {
      if (!stralloc_catb(&key,line.s + 1,i - 1)) die_nomem();
      if (!stralloc_0(&key)) die_nomem();
      case_lowerb(key.s,key.len);
    }

    if (!stralloc_copyb(&data,line.s + i + 1,line.len - i - 1)) die_nomem();

    numcolons = 0;
    for (i = 0;i < data.len;++i)
      if (data.s[i] == ':') {
    data.s[i] = 0;
    if (++numcolons == 6)
      break;
      }
    if (numcolons < 6) die_format();
    data.len = i;

    if (cdb_make_add(&c,key.s,key.len,data.s,data.len) == -1) die_writet();
  }

    if (cdb_make_add(&c,"",0,wildchars.s,wildchars.len) == -1) die_writet();

  if (cdb_make_finish(&c) == -1) die_writet();
  if (fsync(fdtemp) == -1) die_writet();
  if (close(fdtemp) == -1) die_writet(); /* NFS stupidity */
  if (rename("var/users.tmp","var/users.cdb") == -1) die_rename();

  _exit(0);
}
