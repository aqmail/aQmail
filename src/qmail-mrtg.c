#include "strerr.h"
#include "stralloc.h"
#include "substdio.h"
#include "getln.h"
#include "exit.h"
#include "readwrite.h"
#include "open.h"
#include "scan.h"
#include "fmt.h"
#include "case.h"
#include "now.h"
#include "str.h"
#include "datetime.h"

#define TAI64NLEN 24

/** @file qmail-mrtg.c
    @return 0: ok
            1: Error: No TAI64N timestamp available
            2: Warning: Not enough time left between calls
*/

/* qmail-send */

int local = 0;
int remote = 0;
int success = 0;
int failure = 0;
int bytes = 0;
int tlstrans = 0;
int deferral = 0;
int bounces = 0;
int triples = 0;
int qmtp = 0;
int qmtps = 0;

/* qmail-smtpd */

int asessions = 0;
int rsessions = 0;
int aorig = 0;
int arcpt = 0;
int rsend = 0;
int rhelo = 0;
int rorigbad = 0;
int rorigdns = 0;
int rrcptbad = 0;
int rrcptfail = 0;
int rsize = 0;
int rmime = 0;
int rloader = 0;
int rvirus = 0;
int rspam = 0;
int aauth = 0;
int rauth = 0;
int atls = 0;
int rtls = 0;
int spfpass = 0;
int spfail = 0;

/* qmail-pop3d */

int apop = 0;
int rpop = 0;
int pok = 0;
int pdeny = 0;

/* *server + rblsmtpd  */

int sok = 0;
int sdeny = 0;
int greet = 0;
int rbl = 0;

char ssoutbuf[64];
substdio ssout = SUBSTDIO_FDBUF(write,1,ssoutbuf,sizeof(ssoutbuf));

static void putsa(stralloc *sa)
{
  substdio_putflush(&ssout,sa->s,sa->len);
}

static void puts(char *s) 
{
  if (substdio_puts(&ssout,s) == -1) _exit(1);
  if (substdio_puts(&ssout,"\n") == -1) _exit(1);
  if (substdio_flush(&ssout) == -1) _exit(1);
}

static void puti(int i) 
{
  char num[FMT_ULONG];

  if (substdio_bput(&ssout,num,fmt_ulong(num,(unsigned long) i)) == -1) _exit(1); 
  if (substdio_puts(&ssout,"\n") == -1) _exit(1);
  if (substdio_flush(&ssout) == -1) _exit(1);
}

char ssinbuf[1024];
substdio ssin = SUBSTDIO_FDBUF(read,0,ssinbuf,sizeof(ssinbuf)); 

void mrtg_results(char flag) 
{
  switch (flag) {
    case  '1': puti(success); puti(tlstrans); break;
    case  '2': bytes = bytes/1024; puti(bytes); puti(bytes); break;
    case  '3': puti(local); puti(remote); break;
    case  '4': puti(failure); puti(deferral); break;
    case  '5': puti(bounces); puti(triples); break;
    case  '6': qmtps += qmtp; puti(qmtp); puti(qmtps); break; /* QMTP */ 

    case  'a': puti(asessions); puti(rsessions); break; /* total */
    case  'b': puti(aorig); puti(arcpt); break; /* accepted */
    case  'c': puti(rsend); puti(rhelo); break; /* rejected MTA */
    case  'd': puti(rorigbad); puti(rorigdns); break; /* Orig */
    case  'e': puti(rrcptbad); puti(rrcptfail); break; /* Recipient */
    case  'f': puti(rmime); puti(rloader); break; /* Warlord */
    case  'g': puti(rvirus); puti(rspam); break; /* Infected/Spam */
    case  'h': puti(aauth); puti(rauth); break; /* Auth */
    case  'i': puti(atls); puti(rtls); break;  /* TLS */  
    case  'j': puti(spfpass); puti(spfail); break; /* SPF */
    case  'z': sdeny +=rbl; puti(sok); puti(sdeny); break; /* reject session */

    case  'A': puti(apop); puti(rpop); break;
    case  'B': puti(pok); puti(pdeny); break;

    default: break;
  }
}

void mrtg_sendlog(char *in, char flag) 
{
  int i, j, k = 0;

  switch (flag) { 
    case  '1': if (case_starts(in,"delivery")) {
                 i = str_chr(in,':') + 2;
                 if (case_starts(in + i,"success:")) success++;
                 i = str_chr(in,'T');
                 if (case_starts(in + i,"TLS_transmitted")) tlstrans++;
               }; break;
    case  '2': if (case_starts(in,"info msg")) {
                  i = str_chr(in,':') + 8;
                  if (j = str_chr(in + i,' ')) in[i + j] = '\0';
                  bytes += atoi(in + i); 
               }; break;
    case  '3': if (case_starts(in,"status:")) {
                 i = str_rchr(in,'c') + 4;
                 k = str_rchr(in,'r') + 7;
                 if (j = str_chr(in + i,'/')) in[i + j] = '\0';
                 if (atoi(in + i) > local) local = atoi(in + i); 
                 if (j = str_chr(in + k,'/')) in[k + j] = '\0';
                 if (atoi(in + k) > remote) remote = atoi(in + k); 
               }; break;
    case  '4': if (case_starts(in,"delivery")) {
                 i = str_chr(in,':') + 2;
                 if (case_starts(in + i,"failure:")) failure++; 
                 if (case_starts(in + i,"deferral:")) deferral++; 
               }; break;
    case  '5': if (case_starts(in,"bounce msg")) bounces++;
               if (case_starts(in,"triple bounce:")) triples++;
               break;
    case  '6': if (case_starts(in,"delivery")) {
                 i = str_chr(in,'q');
                 if (case_starts(in + i,"qmtp:_ok")) qmtp++;
                 if (case_starts(in + i,"qmtps:_ok")) qmtps++;
               }; break;
    default:   break;
  }
}

void mrtg_smtplog(char *in, char flag) 
{
  int i, j, k = 0;

  i = str_chr(in,'A');
  j = str_chr(in,'R');
  k = str_chr(in,'P');

  switch (flag) {
    case  'a': if (case_starts(in + i,"Accept")) asessions++;
               if (case_starts(in + j,"Reject")) rsessions++;
               break;
    case  'b': if (case_starts(in + i,"Accept::ORIG:")) aorig++; 
               if (case_starts(in + i,"Accept::RCPT:")) arcpt++; 
               break;
    case  'c': if (case_starts(in + j,"Reject::SNDR::Invalid_Relay")) rsend++;
               if (case_starts(in + j,"Reject::SNDR::Bad_Helo")) rhelo++; 
               if (case_starts(in + j,"Reject::SNDR::DNS_Helo")) rhelo++; 
               break;
    case  'd': if (case_starts(in + j,"Reject::ORIG::Bad_Mailfrom")) rorigbad++;
               if (case_starts(in + j,"Reject::ORIG::DNS_MF")) rorigdns++;
               break;
    case  'e': if (case_starts(in + j,"Reject::RCPT::Bad_Rcptto")) rrcptbad++;
               if (case_starts(in + j,"Reject::RCPT::Failed_Rcptto")) rrcptfail++; 
               break;
    case  'f': if (case_starts(in + j,"Reject::DATA::Invalid_Size")) rsize++;
               if (case_starts(in + j,"Reject::DATA::Bad_MIME")) rmime++;
               if (case_starts(in + j,"Reject::DATA::MIME_Attach")) rmime++;
               if (case_starts(in + j,"Reject::DATA::Bad_Loader")) rloader++;
               break;
    case  'g': if (case_starts(in + j,"Reject::DATA::Spam_Message")) rspam++;
               if (case_starts(in + j,"Reject::DATA::Virus_Infected")) rvirus++;
               break;
    case  'h': if (case_starts(in + i,"Accept::AUTH:")) aauth++;
               if (case_starts(in + j,"Reject::AUTH:")) rauth++;
               break;
    case  'i': if (case_starts(in + k,"P:ESMTPS")) atls++;
               if (case_starts(in + j,"Reject::TLS:")) rtls++;
               break;
    case  'j': if (case_starts(in + i,"Accept::SPF:")) spfpass++;
               if (case_starts(in + j,"Reject::SPF:")) spfail++;
               break;
    case  'z': if (case_starts(in,"tcpserver") || case_starts(in,"sslserver") || case_starts(in,"rblsmtpd")) { 
                 i = str_chr(in,':') + 2;
                 if (case_starts(in + i,"ok")) sok++;
                 if (case_starts(in + i,"deny")) sdeny++;
                 j = str_chr(in+i,':') + 2;
                 if (case_starts(in + i + j,"451")) rbl++;
                 if (case_starts(in + i + j,"553")) rbl++;
                 if (case_starts(in + i + j,"greetdelay:")) greet++;
               } break;
    default:   break;
  }
} 

void mrtg_pop3log(char *in, char flag) 
{
  int i, j = 0;

  switch (flag) {
    case  'A': i = str_chr(in,'A'); j = str_chr(in,'R');
               if (case_starts(in + i,"Accept::AUTH:")) apop++;
               if (case_starts(in + j,"Reject::AUTH:")) rpop++;
               break;
    case  'B': if (case_starts(in,"tcpserver:") || case_starts(in,"sslserver:")) { 
                 i = str_chr(in,':') + 2;
                 if (case_starts(in + i,"ok")) pok++;
                 if (case_starts(in + i,"deny")) pdeny++;
               }; break;
    default:   break;
  }
}

int main(int argc, char **argv) 
{
  int i;
  int c;
  int match;
  int enoughtime = 0;
  unsigned long u;
  unsigned long calltime;
  unsigned long seconds;
  unsigned long nanoseconds;
  unsigned int history = 305;
  char flag;

  stralloc line = {0};
  calltime = now();

  if (argc < 2) {
    puts("Usage: qmail-mrtg [ -1 | -2 | -3 | -4 | -5 | -6 | -a | -b | -c | -d | -e | -f | -g | -h | -i | -j | -z | -A | -B ] [time]");
    puts("       qmail-mrtg needs to be called every [time] minutes (i.e. by crontab) -- default 305 secs");
    _exit(2);
  }
  flag = *(argv[1] + 1);
  if (argc == 3) { scan_ulong(argv[2],&u); history = 60 * u; }

/* Read input lines sequentially */

  substdio_fdbuf(&ssin,read,0,ssinbuf,sizeof(ssinbuf));

  for (;;) {
    if (getln(&ssin,&line,&match,'\n') != 0) _exit(1);
    if (!match) break;
    if (!stralloc_0(&line)) _exit(1);

    seconds = 0;
    nanoseconds = 0;

    if (line.s[0] == '@') {	/* tai64 timestamp */
      for (i = 1; i <= TAI64NLEN; i++) {
        c = (int)line.s[i];
        u = c - '0';
        if (u >= 10) {
          u = c - 'a';
          if (u >= 6) break;
          u += 10;
        }
        seconds <<= 4;
        seconds += nanoseconds >> 28;
        nanoseconds &= 0xfffffff;
        nanoseconds <<= 4;
        nanoseconds += u;
      }
      seconds -= 4611686018427387914ULL;
      seconds = seconds > 0 ? seconds : 0;
    } else {
      puts("Error: No TAI64N timestamp available.");
      _exit(1);
    }

    if (enoughtime) {		/* default history = 305 sec => evaluate logs every ~5 mins */
      if (seconds <= calltime && seconds >= (calltime - history)) {
        if (flag >= '1' && flag <= '9') mrtg_sendlog(line.s+TAI64NLEN+2,flag);
        else if (flag >= 'a' && flag <= 'z') mrtg_smtplog(line.s+TAI64NLEN+2,flag);
        else if (flag >= 'A' && flag <= 'Z') mrtg_pop3log(line.s+TAI64NLEN+2,flag);
      }
    } else { 
      if (seconds) {
        enoughtime++;
      } else {
        puts("Warning: Not enough time left between calls");
        _exit(1);
      } 
    }
  }

  mrtg_results(flag);
  
  _exit(0);
}
