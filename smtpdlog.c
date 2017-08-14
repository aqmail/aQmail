#include "readwrite.h"
#include "substdio.h"
#include "str.h"
#include "byte.h"
#include "env.h"
#include "fmt.h"
#include "exit.h"
#include "smtpdlog.h"
#define FDLOG 2

char *reply550hlo;
char *reply550mbx;
char *reply552siz;
char *reply553bmf;
char *reply553brt;
char *reply553ngw;
char *reply553env;
char *reply553inv;
char *reply554cnt;

char strnum[FMT_ULONG];
char sslogbuf[512];
substdio sslog = SUBSTDIO_FDBUF(write,FDLOG,sslogbuf,sizeof(sslogbuf));

void smtpdlog_init()
{
  reply550hlo = env_get("REPLY_HELO");
  reply550mbx = env_get("REPLY_MAILBOX");
  reply552siz = env_get("REPLY_MAXSIZE");
  reply553bmf = env_get("REPLY_BADMAILFROM");
  reply553brt = env_get("REPLY_BADRCPTTO");
  reply553env = env_get("REPLY_SENDEREXIST");
  reply553ngw = env_get("REPLY_NOGATEWAY");
  reply553inv = env_get("REPLY_SENDERINVALID");
  reply554cnt = env_get("REPLY_CONTENT");
}

static void logs(char *s) { if (substdio_puts(&sslog,s) == -1) _exit(1); }  /* single string */
static void logp(char *s) { logs(" P:"); logs(s); }				/* protocol */
static void logh(char *s1,char *s2,char *s3) { logs(" S:"); logs(s1); logs(":"); logs(s2); logs(" H:"); logs(s3); }  /* host */
static void logf(char *s) { logs(" F:"); logs(s); }                         /* mailfrom */
static void logt(char *s) { logs(" T:"); logs(s); }                         /* rcptto */
static void logi(char *s) { logs(" '"); logs(s); logs("'"); }               /* information */
static void logn(char *s) { if (substdio_puts(&sslog,s) == -1) _exit(1); if (substdio_flush(&sslog) == -1) _exit(1); } /* end */
static void logpid() { strnum[fmt_ulong(strnum,getpid())] = 0; logs("qmail-smtpd: pid "); logs(strnum); logs(" "); }

void smtp_loga(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7,char *s8,char *s9)
  { logpid(); logs(s1); logs(s9); logp(s2); logh(s3,s4,s5); logf(s6); logt(s7), logs(" ?~"); logi(s8); logn("\n"); }	/* Auth info */
void smtp_logb(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7)
  { logpid(); logs(s1); logs(s7); logp(s2); logh(s3,s4,s5); logs(" ?~"); logi(s6); logn("\n"); }	/* Auth info */
void smtp_logg(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7)
  { logpid(); logs(s1); logp(s2); logh(s3,s4,s5); logf(s6); logt(s7); logn("\n"); }			/* Generic */
void smtp_logh(char *s1,char *s2,char *s3,char *s4,char *s5)
  { logpid(); logs(s1); logp(s2); logh(s3,s4,s5); logn("\n"); }                                         /* Host */
void smtp_logi(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7,char *s8)
  { logpid(); logs(s1); logp(s2); logh(s3,s4,s5); logf(s6); logt(s7); logi(s8); logn("\n"); }		/* Generic + Info */
void smtp_logr(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7,char *s8)
  { logpid(); logs(s1); logs(s2); logp(s3); logh(s4,s5,s6); logf(s7); logt(s8); logn("\n"); }		/* Recipient */

void die_read() { _exit(1); }
void die_alarm() { out("451 timeout (#4.4.2)\r\n"); flush(); _exit(1); }
void die_nomem() { out("421 out of memory (#4.3.0)\r\n"); flush(); _exit(1); }
void die_control() { out("421 unable to read controls (#4.3.0)\r\n"); flush(); _exit(1); }
void die_ipme() { out("421 unable to figure out my IP addresses (#4.3.0)\r\n"); flush(); _exit(1); }
void die_starttls() { out("454 TLS not available due to temporary reason (#5.7.3)\r\n"); flush(); _exit(1); }
void die_recipients() { out("421 unable to check recipients (#4.3.0)\r\n"); flush(); _exit(1); }

void err_unimpl() { out("500 unimplemented (#5.5.1)\r\n"); }
void err_syntax() { out("555 syntax error (#5.5.4)\r\n"); }
void err_noop() { out("250 ok\r\n"); }
void err_vrfy() { out("252 send some mail, i'll try my best\r\n"); }
void err_qqt() { out("451 qqt failure (#4.3.0)\r\n"); }

int err_child() { out("454 problem with child and I can't auth (#4.3.0)\r\n"); return -1; }
int err_fork() { out("454 child won't start and I can't auth (#4.3.0)\r\n"); return -1; }
int err_pipe() { out("454 unable to open pipe and I can't auth (#4.3.0)\r\n"); return -1; }
int err_write() { out("454 unable to write pipe and I can't auth (#4.3.0)\r\n"); return -1; }

/* TLS */

int err_starttls() 
{ 
  out("454 TLS not available due to temporary reason (#5.7.3)\r\n");
  return -1; 
}
void err_tlsreq(char *s1,char *s2,char *s3,char *s4,char *s5)
{
  out("535 STARTTLS required (#5.7.1)\r\n"); 
  smtp_logh(s1,s2,s3,s4,s5); 
}

/* Helo */

void err_helo(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7,char *s8)
{
  out("550 sorry, invalid HELO/EHLO greeting ");
  if (reply550hlo) out(reply550hlo);
  out(" (#5.7.1)\r\n");
  smtp_logi(s1,s2,s3,s4,s5,s6,s7,s8);
 }

/* Auth */

void err_authd() 
{ 
  out("503 you're already authenticated (#5.5.0)\r\n"); 
}
void err_authmail() 
{ 
  out("503 no auth during mail transaction (#5.5.0)\r\n");
}
void err_authfail(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7)
{
  out("535 authentication failed (#5.7.1)\r\n"); smtp_logb(s1,s2,s3,s4,s5,s6,s7); 
}
void err_authreq(char *s1,char *s2,char *s3,char *s4,char *s5)
{
  out("535 authentication required (#5.7.1)\r\n"); smtp_logh(s1,s2,s3,s4,s5); 
}
void err_submission(char *s1,char *s2,char *s3,char *s4,char *s5)
{ 
  out("530 Authorization required (#5.7.1) \r\n"); smtp_logh(s1,s2,s3,s4,s5);
}

int err_authabort() 
{ 
  out("501 auth exchange canceled (#5.0.0)\r\n"); 
  return -1;
}
int err_authinput() 
{ 
  out("501 malformed auth input (#5.5.4)\r\n"); 
  return -1;
}
int err_noauth() 
{ 
  out("504 auth type unimplemented (#5.5.1)\r\n"); 
  return -1;
}

/* Mail From: */

void err_wantmail() { out("503 MAIL first (#5.5.1)\r\n"); }

void err_mav(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7)
{
  out("553 sorry, invalid sender address specified ");
  if (reply553inv) out(reply553inv);
  out(" (#5.7.1)\r\n");
  smtp_logg(s1,s2,s3,s4,s5,s6,s7);
}
void err_bmf(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7,char *s8)
{
  out("553 sorry, your envelope sender is in my badmailfrom list ");
  if (reply553bmf) out(reply553bmf);
  out(" (#5.7.1)\r\n");
  smtp_logi(s1,s2,s3,s4,s5,s6,s7,s8);
}
void err_mfdns(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7) 
{
  out("553 sorry, your envelope sender must exist ");
  if (reply553env) out(reply553env);
  out(" (#5.7.1)\r\n");
  smtp_logg(s1,s2,s3,s4,s5,s6,s7);
}

/* SPF */

void err_spf(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7,char *msg) 
{
  int i, j;
  int len = str_len(msg);

  for (i = 0; i < len; i = j + 1) {
    j = byte_chr(msg + i, len - i, '\n') + i;
    if (j < len) {
      out("550-");
      msg[j] = 0;
      out(msg);
      msg[j] = '\n';
    } else {
      out("550 ");
      out(msg);
    }
  } 
  out(" (#5.7.1)\r\n");

  smtp_logg(s1,s2,s3,s4,s5,s6,s7);
}

/* Rcpt To: */

void err_wantrcpt() { out("503 RCPT first (#5.5.1)\r\n"); }

void err_nogateway(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7) 
{
  out("553 sorry, that domain isn't in my list of allowed rcpthosts ");
  if (reply553ngw) out(reply553ngw);
  out(" (#5.7.1)\r\n");
  smtp_logg(s1,s2,s3,s4,s5,s6,s7);
}
void err_brt(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7) 
{
  out("553 sorry, your envelope recipient is in my badrcptto list ");
  if (reply553brt) out(reply553brt);
  out(" (#5.7.1)\r\n");
  smtp_logg(s1,s2,s3,s4,s5,s6,s7);
}
void err_rcpts(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7) 
{
  out("452 sorry, too many recipients (#4.5.3)\r\n");   /* RFC 5321 */
  smtp_logg(s1,s2,s3,s4,s5,s6,s7);
}
void err_recipient(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7) 
{
  if (env_get("RECIPIENTS450"))
    out("450 sorry, mailbox currently unavailable (#4.2.1)\r\n");
  else {
    out("550 sorry, no mailbox by that name ");
    if (reply550mbx) out(reply550mbx); out(" (#5.7.1)\r\n");
  }
  smtp_logg(s1,s2,s3,s4,s5,s6,s7);
}

/* Data */

void straynewline() 
{ 
  out("451 Bare Line Feeds (LF) are not accepted in SMTP; CRLF is required according to RFC 2822.\r\n"); 
  flush(); 
  _exit(1); 
}
void err_notorious() 
{ 
  out("503 DATA command not accepted at this time (#5.5.1)\r\n");
  flush();
  _exit(1);
}
void err_size(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7) 
{
  out("552 sorry, that message size exceeds my databytes limit (#5.3.4)\r\n"); 
  smtp_logg(s1,s2,s3,s4,s5,s6,s7);
}
void err_data(char *s1,char *s2,char *s3,char *s4,char *s5,char *s6,char *s7,char *s8) 
{
  out("554 sorry, invalid message content ");
  if (reply554cnt) out(reply554cnt);
  out(" (#5.3.2)\r\n");
  smtp_logi(s1,s2,s3,s4,s5,s6,s7,s8);
}
