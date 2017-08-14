#include <sys/types.h>
#include <sys/stat.h>
#include "readwrite.h"
#include "sig.h"
#include "exit.h"
#include "open.h"
#include "seek.h"
#include "fmt.h"
#include "alloc.h"
#include "substdio.h"
#include "datetime.h"
#include "now.h"
#include "triggerpull.h"
#include "extra.h"
#include "auto_qmail.h"
#include "auto_uids.h"
#include "date822fmt.h"
#include "fmtqfn.h"
#include "env.h"
#include "fork.h"
#include "wait.h"

#define DEATH 86400 /* 24 hours; _must_ be below q-s's OSSIFIED (36 hours) */
#define ADDR 1003

char inbuf[2048];
struct substdio ssin;
char outbuf[256];
struct substdio ssout;

datetime_sec starttime;
struct datetime dt;
unsigned long mypid;
unsigned long uid;
char *pidfn;
struct stat pidst;
unsigned long messnum;
char *messfn;
char *todofn;
char *intdfn;
int messfd;
int intdfd;
int flagmademess = 0;
int flagmadeintd = 0;

void cleanup()
{
 if (flagmadeintd)
  {
   seek_trunc(intdfd,0);
   if (unlink(intdfn) == -1) return;
  }
 if (flagmademess)
  {
   seek_trunc(messfd,0);
   if (unlink(messfn) == -1) return;
  }
}

void die(int e) { _exit(e); }
void die_qhpsi() { cleanup(); die(71); }
void die_write() { cleanup(); die(53); }
void die_read() { cleanup(); die(54); }
void sigalrm() { /* thou shalt not clean up here */ die(52); }
void sigbug() { die(81); }

unsigned int receivedlen;
char *received;
/* "Received: (qmail-queue invoked by alias); 26 Sep 1995 04:46:54 -0000\n" */

static unsigned int receivedfmt(char *s)
{
 unsigned int i;
 unsigned int len;
 len = 0;
 i = fmt_str(s,"Received: (qmail "); len += i; if (s) s += i;
 i = fmt_ulong(s,mypid); len += i; if (s) s += i;
 i = fmt_str(s," invoked "); len += i; if (s) s += i;
 if (uid == auto_uida)
  { i = fmt_str(s,"by alias"); len += i; if (s) s += i; }
 else if (uid == auto_uidd)
  { i = fmt_str(s,"from network"); len += i; if (s) s += i; }
 else if (uid == auto_uids)
  { i = fmt_str(s,"for bounce"); len += i; if (s) s += i; }
 else
  {
   i = fmt_str(s,"by uid "); len += i; if (s) s += i;
   i = fmt_ulong(s,uid); len += i; if (s) s += i;
  }
 i = fmt_str(s,"); "); len += i; if (s) s += i;
 i = date822fmt(s,&dt); len += i; if (s) s += i;
 return len;
}

void received_setup()
{
 receivedlen = receivedfmt((char *) 0);
 received = alloc(receivedlen + 1);
 if (!received) die(51);
 receivedfmt(received);
}

unsigned int pidfmt(char *s,unsigned long seq)
{
 unsigned int i;
 unsigned int len;

 len = 0;
 i = fmt_str(s,"pid/"); len += i; if (s) s += i;
 i = fmt_ulong(s,mypid); len += i; if (s) s += i;
 i = fmt_str(s,"."); len += i; if (s) s += i;
 i = fmt_ulong(s,starttime); len += i; if (s) s += i;
 i = fmt_str(s,"."); len += i; if (s) s += i;
 i = fmt_ulong(s,seq); len += i; if (s) s += i;
 ++len; if (s) *s++ = 0;

 return len;
}

char *fnnum(char *dirslash,int flagsplit)
{
 char *s;

 s = alloc(fmtqfn((char *) 0,dirslash,messnum,flagsplit));
 if (!s) die(51);
 fmtqfn(s,dirslash,messnum,flagsplit);
 return s;
}

void pidopen()
{
 unsigned int len;
 unsigned long seq;

 seq = 1;
 len = pidfmt((char *) 0,seq);
 pidfn = alloc(len);
 if (!pidfn) die(51);

 for (seq = 1;seq < 10;++seq)
  {
   if (pidfmt((char *) 0,seq) > len) die(81); /* paranoia */
   pidfmt(pidfn,seq);
   messfd = open_excl(pidfn);
   if (messfd != -1) return;
  }

 die(63);
}

char *qhpsi;

void qhpsiprog(char *arg)
{
 int wstat;
 int child;
 char *qhpsiargs[6] = { 0, 0, 0, 0, 0, 0 };
 char *x;
 unsigned long u; 
 int childrc; 
 int qhpsirc = 1;
 unsigned int size;
 unsigned int qhpsiminsize = 0;
 unsigned int qhpsimaxsize = 0;

 struct stat st;

 if (stat(messfn,&st) == -1) die(63);
 size = (unsigned int) st.st_size;

 x = env_get("QHPSIMINSIZE");
 if (x) { scan_ulong(x,&u); qhpsiminsize = (int) u; }
 if (qhpsiminsize) if (size < qhpsiminsize) return;
 x = env_get("QHPSIMAXSIZE");
 if (x) { scan_ulong(x,&u); qhpsimaxsize = (int) u; }
 if (qhpsimaxsize) if (size > qhpsimaxsize) return; 

 if (*arg) {
   switch(child = fork()) {
     case -1:
       die_qhpsi();
     case 0:
       qhpsiargs[0] = arg; 
       qhpsiargs[1] = messfn;
       qhpsiargs[2] = env_get("QHPSIARG1");
       if (!qhpsiargs[2]) qhpsiargs[2] = 0; 
       qhpsiargs[3] = env_get("QHPSIARG2");
       if (!qhpsiargs[3]) qhpsiargs[3] = 0; 
       qhpsiargs[4] = env_get("QHPSIARG3");
       if (!qhpsiargs[4]) qhpsiargs[4] = 0; 
       x = env_get("QHPSIRC");
       if (x) { scan_ulong(x,&u); qhpsirc = (int) u; }
       execvp(*qhpsiargs,qhpsiargs);
       die_qhpsi();
   }
   if (wait_pid(&wstat,child) == -1) die_qhpsi();
   if (wait_crashed(wstat)) die_qhpsi();
   childrc = wait_exitcode(wstat); 
   if (childrc == qhpsirc) { cleanup(); die(32); }
   else if (childrc != 0) die_qhpsi(); 
  }
}

char tmp[FMT_ULONG];

int main()
{
 unsigned int len;
 char ch;

 sig_blocknone();
 umask(033);
 if (chdir(auto_qmail) == -1) die(61);
 if (chdir("queue") == -1) die(62);

 mypid = getpid();
 uid = getuid();
 starttime = now();
 datetime_tai(&dt,starttime);
 qhpsi = env_get("QHPSI");

 received_setup();

 sig_pipeignore();
 sig_miscignore();
 sig_alarmcatch(sigalrm);
 sig_bugcatch(sigbug);

 alarm(DEATH);

 pidopen();
 if (fstat(messfd,&pidst) == -1) die(63);

 messnum = pidst.st_ino;
 messfn = fnnum("mess/",1);
 todofn = fnnum("todo/",1);
 intdfn = fnnum("intd/",1);

 if (link(pidfn,messfn) == -1) die(64);
 if (unlink(pidfn) == -1) die(63);
 flagmademess = 1;

 substdio_fdbuf(&ssout,write,messfd,outbuf,sizeof(outbuf));
 substdio_fdbuf(&ssin,read,0,inbuf,sizeof(inbuf));

 if (substdio_bput(&ssout,received,receivedlen) == -1) die_write();

 switch(substdio_copy(&ssout,&ssin))
  {
   case -2: die_read();
   case -3: die_write();
  }

 if (substdio_flush(&ssout) == -1) die_write();
 if (fsync(messfd) == -1) die_write();

 intdfd = open_excl(intdfn);
 if (intdfd == -1) die(65);
 flagmadeintd = 1;

 substdio_fdbuf(&ssout,write,intdfd,outbuf,sizeof(outbuf));
 substdio_fdbuf(&ssin,read,1,inbuf,sizeof(inbuf));

 if (substdio_bput(&ssout,"u",1) == -1) die_write();
 if (substdio_bput(&ssout,tmp,fmt_ulong(tmp,uid)) == -1) die_write();
 if (substdio_bput(&ssout,"",1) == -1) die_write();

 if (substdio_bput(&ssout,"p",1) == -1) die_write();
 if (substdio_bput(&ssout,tmp,fmt_ulong(tmp,mypid)) == -1) die_write();
 if (substdio_bput(&ssout,"",1) == -1) die_write();

 if (substdio_get(&ssin,&ch,1) < 1) die_read();
 if (ch != 'F') die(91);
 if (substdio_bput(&ssout,&ch,1) == -1) die_write();
 for (len = 0;len < ADDR;++len)
  {
   if (substdio_get(&ssin,&ch,1) < 1) die_read();
   if (substdio_put(&ssout,&ch,1) == -1) die_write();
   if (!ch) break;
  }
 if (len >= ADDR) die(11);

 if (substdio_bput(&ssout,QUEUE_EXTRA,QUEUE_EXTRALEN) == -1) die_write();

 for (;;)
  {
   if (substdio_get(&ssin,&ch,1) < 1) die_read();
   if (!ch) break;
   if (ch == 'Q') { qhpsi = 0; break; }
   if (ch != 'T') die(91);
   if (substdio_bput(&ssout,&ch,1) == -1) die_write();
   for (len = 0;len < ADDR;++len)
    {
     if (substdio_get(&ssin,&ch,1) < 1) die_read();
     if (substdio_bput(&ssout,&ch,1) == -1) die_write();
     if (!ch) break;
    }
   if (len >= ADDR) die(11);
  }

 if (qhpsi) qhpsiprog(qhpsi);

 if (substdio_flush(&ssout) == -1) die_write();
 if (fsync(intdfd) == -1) die_write();

 if (link(intdfn,todofn) == -1) die(66);

 triggerpull();
 die(0);
}
