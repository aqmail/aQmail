#include "substdio.h"
#include "strerr.h"
#include "stralloc.h"
#include "getln.h"
#include "open.h"
#include "readwrite.h"
#include "token822.h"
#include "control.h"
#include "auto_qmail.h"
#include "env.h"

#define FATAL "newinclude: fatal: "

void nomem()
{
  strerr_die2x(111,FATAL,"out of memory");
}
void usage()
{
  strerr_die1x(100,"newinclude: usage: newinclude list");
}


char *fnlist;
substdio sslist;
char listbuf[1024];

stralloc bin = {0};
#define fnbin bin.s
stralloc tmp = {0};
#define fntmp tmp.s
substdio sstmp;
char tmpbuf[1024];

void readerr()
{
  strerr_die4sys(111,FATAL,"unable to read ",fnlist,": ");
}
void writeerr()
{
  strerr_die4sys(111,FATAL,"unable to write to ",fntmp,": ");
}

void out(s,len)
char *s;
int len;
{
  if (substdio_put(&sstmp,s,len) == -1) writeerr();
}

void doincl(buf,len)
char *buf;
int len;
{
  if (!len)
    strerr_die2x(111,FATAL,"empty :include: filenames not permitted");
  if (byte_chr(buf,len,'\n') != len)
    strerr_die2x(111,FATAL,"newlines not permitted in :include: filenames");
  if (byte_chr(buf,len,'\0') != len)
    strerr_die2x(111,FATAL,"NUL not permitted in :include: filenames");
  if ((buf[0] != '.') && (buf[0] != '/'))
    out("./",2);
  out(buf,len);
  out("",1);
}

void dorecip(buf,len)
char *buf;
int len;
{
  if (!len)
    strerr_die2x(111,FATAL,"empty recipient addresses not permitted");
  if (byte_chr(buf,len,'\n') != len)
    strerr_die2x(111,FATAL,"newlines not permitted in recipient addresses");
  if (byte_chr(buf,len,'\0') != len)
    strerr_die2x(111,FATAL,"NUL not permitted in recipient addresses");
  if (len > 800)
    strerr_die2x(111,FATAL,"addresses must be under 800 bytes");
  if ((buf[len - 1] == ' ') || (buf[len - 1] == '\t'))
    strerr_die2x(111,FATAL,"spaces and tabs not permitted at ends of addresses");
  out("&",1);
  out(buf,len);
  out("",1);
}


void die_control()
{
  strerr_die2sys(111,FATAL,"unable to read controls: ");
}

stralloc me = {0};
stralloc defaulthost = {0};
stralloc defaultdomain = {0};
stralloc plusdomain = {0};

void readcontrols()
{
  int r;
  int fddir;
  char *x;

  fddir = open_read(".");
  if (fddir == -1)
    strerr_die2sys(111,FATAL,"unable to open current directory: ");

  if (chdir(auto_qmail) == -1)
    strerr_die4sys(111,FATAL,"unable to chdir to ",auto_qmail,": ");

  r = control_readline(&me,"control/me");
  if (r == -1) die_control();
  if (!r) if (!stralloc_copys(&me,"me")) nomem();

  r = control_readline(&defaultdomain,"control/defaultdomain");
  if (r == -1) die_control();
  if (!r) if (!stralloc_copy(&defaultdomain,&me)) nomem();
  x = env_get("QMAILDEFAULTDOMAIN");
  if (x) if (!stralloc_copys(&defaultdomain,x)) nomem();

  r = control_readline(&defaulthost,"control/defaulthost");
  if (r == -1) die_control();
  if (!r) if (!stralloc_copy(&defaulthost,&me)) nomem();
  x = env_get("QMAILDEFAULTHOST");
  if (x) if (!stralloc_copys(&defaulthost,x)) nomem();

  r = control_readline(&plusdomain,"control/plusdomain");
  if (r == -1) die_control();
  if (!r) if (!stralloc_copy(&plusdomain,&me)) nomem();
  x = env_get("QMAILPLUSDOMAIN");
  if (x) if (!stralloc_copys(&plusdomain,x)) nomem();

  if (fchdir(fddir) == -1)
    strerr_die2sys(111,FATAL,"unable to set current directory: ");
}

stralloc cbuf = {0};
token822_alloc toks = {0};
token822_alloc tokaddr = {0};
stralloc address = {0};

void gotincl()
{
  token822_reverse(&tokaddr);
  if (token822_unquote(&address,&tokaddr) != 1) nomem();
  tokaddr.len = 0;
  doincl(address.s,address.len);
}

void gotaddr()
{
  int i;
  int j;
  int flaghasat;
 
  token822_reverse(&tokaddr);
  if (token822_unquote(&address,&tokaddr) != 1) nomem();
 
  flaghasat = 0;
  for (i = 0;i < tokaddr.len;++i)
    if (tokaddr.t[i].type == TOKEN822_AT)
      flaghasat = 1;
 
  tokaddr.len = 0;

  if (!address.len) return;

  if (!flaghasat)
    if (address.s[0] == '/') {
      if (!stralloc_0(&address)) nomem();
      strerr_die4x(111,FATAL,"file delivery ",address.s," not supported");
    }
  if (!flaghasat)
    if (address.s[0] == '|') {
      if (!stralloc_0(&address)) nomem();
      strerr_die4x(111,FATAL,"program delivery ",address.s," not supported");
    }

  if (!flaghasat) {
    if (!stralloc_cats(&address,"@")) nomem();
    if (!stralloc_cat(&address,&defaulthost)) nomem();
  }
  if (address.s[address.len - 1] == '+') {
    address.s[address.len - 1] = '.';
    if (!stralloc_cat(&address,&plusdomain)) nomem();
  }
  j = 0;
  for (i = 0;i < address.len;++i) if (address.s[i] == '@') j = i;
  for (i = j;i < address.len;++i) if (address.s[i] == '.') break;
  if (i == address.len) {
    if (!stralloc_cats(&address,".")) nomem();
    if (!stralloc_cat(&address,&defaultdomain)) nomem();
  }

  dorecip(address.s,address.len);
}


stralloc line = {0};
int match;

void parseerr()
{
  if (!stralloc_0(&line)) nomem();
  strerr_die3x(111,FATAL,"unable to parse this line: ",line.s);
}

void parseline()
{
  int wordok;
  struct token822 *t;
  struct token822 *beginning;
 
  switch(token822_parse(&toks,&line,&cbuf)) {
    case -1: nomem();
    case 0: parseerr();
  }

  beginning = toks.t;
  t = toks.t + toks.len;
  wordok = 1;
 
  if (!token822_readyplus(&tokaddr,1)) nomem();
  tokaddr.len = 0;
 
  while (t > beginning)
    switch((--t)->type) {
      case TOKEN822_SEMI:
        break; /*XXX*/
      case TOKEN822_COLON:
        if (t >= beginning + 2)
          if (t[-2].type == TOKEN822_COLON)
            if (t[-1].type == TOKEN822_ATOM)
              if (t[-1].slen == 7)
                if (!byte_diff(t[-1].s,7,"include")) {
                  gotincl();
                  t -= 2;
                }
        break; /*XXX*/
      case TOKEN822_RIGHT:
        if (tokaddr.len) gotaddr();
        while ((t > beginning) && (t[-1].type != TOKEN822_LEFT))
          if (!token822_append(&tokaddr,--t)) nomem();
        gotaddr();
        if (t <= beginning) parseerr();
        --t;
        while ((t > beginning) && ((t[-1].type == TOKEN822_COMMENT) || (t[-1].type == TOKEN822_ATOM) || (t[-1].type == TOKEN822_QUOTE) || (t[-1].type == TOKEN822_AT) || (t[-1].type == TOKEN822_DOT)))
          --t;
        wordok = 0;
        continue;
      case TOKEN822_ATOM: case TOKEN822_QUOTE: case TOKEN822_LITERAL:
        if (!wordok) if (tokaddr.len) gotaddr();
        wordok = 0;
        if (!token822_append(&tokaddr,t)) nomem();
        continue;
      case TOKEN822_COMMENT:
        /* comment is lexically a space; shouldn't affect wordok */
        break;
      case TOKEN822_COMMA:
        if (tokaddr.len) gotaddr();
        wordok = 1;
        break;
      default:
        wordok = 1;
        if (!token822_append(&tokaddr,t)) nomem();
        continue;
    }
  if (tokaddr.len) gotaddr();
}

 
int main(argc,argv)
int argc;
char **argv;
{
  int fd;

  umask(033);
  readcontrols();

  fnlist = argv[1]; if (!fnlist) usage();

  if (!stralloc_copys(&bin,fnlist)) nomem();
  if (!stralloc_cats(&bin,".bin")) nomem();
  if (!stralloc_0(&bin)) nomem();

  if (!stralloc_copys(&tmp,fnlist)) nomem();
  if (!stralloc_cats(&tmp,".tmp")) nomem();
  if (!stralloc_0(&tmp)) nomem();

  fd = open_read(fnlist);
  if (fd == -1) readerr();
  substdio_fdbuf(&sslist,read,fd,listbuf,sizeof(listbuf));

  fd = open_trunc(fntmp);
  if (fd == -1) writeerr();
  substdio_fdbuf(&sstmp,write,fd,tmpbuf,sizeof(tmpbuf));

  for (;;) {
    if (getln(&sslist,&line,&match,'\n') == -1) readerr();
    if (!line.len) break;
    if (line.s[0] != '#') parseline();
    if (!match) break;
  }

  if (substdio_flush(&sstmp) == -1) writeerr();
  if (fsync(fd) == -1) writeerr();
  if (close(fd) == -1) writeerr(); /* NFS stupidity */

  if (rename(fntmp,fnbin) == -1)
    strerr_die6sys(111,FATAL,"unable to move ",fntmp," to ",fnbin,": ");
  
  _exit(0);
}
