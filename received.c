#include "fmt.h"
#include "qmail.h"
#include "now.h"
#include "datetime.h"
#include "date822fmt.h"
#include "received.h"
#include "str.h"
#include "stralloc.h"
#include "byte.h"
#include "str.h"

static int issafe(char ch)
{
  if (ch == ' ') return 1;  /* accept empty spaces */
  if (ch == '.') return 1;
  if (ch == '@') return 1;
  if (ch == '%') return 1;
  if (ch == '+') return 1;
  if (ch == '/') return 1;
  if (ch == '=') return 1;
  if (ch == ':') return 1;
  if (ch == '-') return 1;
  if ((ch >= 'a') && (ch <= 'z')) return 1;
  if ((ch >= 'A') && (ch <= 'Z')) return 1;
  if ((ch >= '0') && (ch <= '9')) return 1;
  return 0;
}

void safeput(struct qmail *qqt,char *s)
{
  char ch;
  while (ch = *s++) {
    if (!issafe(ch)) ch = '?';
    qmail_put(qqt,&ch,1);
  }
}

static char buf[DATE822FMT];

/* "Received: from relay1.uu.net ([E]HELO uunet.uu.net) (user@192.48.96.5)" */
/* "  de/crypted with tls-version: cipher [used/perm] DN=dn" */
/* "  by silverton.berkeley.edu with [UTF8][E]SMTP[SA]; 26 Sep 1995 04:46:54 -0000" */
/* "X-RBL-Info: http://www.spamhaus.org/query/bl?ip=127.0.0.2 */

void received(struct qmail *qqt,char *protocol,char *local,char *remoteip,char *remotehost,char *remoteinfo,char *helo,char *tlsinfo,char *rblinfo)
{
  struct datetime dt;
  int i;

  qmail_puts(qqt,"Received: from ");
  safeput(qqt,remotehost);
  if (helo) {
    qmail_puts(qqt," (HELO ");
    safeput(qqt,helo);
    qmail_puts(qqt,")");
  }
  qmail_puts(qqt," (");
  if (remoteinfo) {
    safeput(qqt,remoteinfo);
    qmail_puts(qqt,"@");
  }
  safeput(qqt,remoteip);
  qmail_puts(qqt,")");

  if (tlsinfo) {
    qmail_puts(qqt,"\n  de/crypted with ");
    qmail_puts(qqt,tlsinfo);
  }
  qmail_puts(qqt,"\n  by ");
  safeput(qqt,local);
  qmail_puts(qqt," with ");
  qmail_puts(qqt,protocol);
  qmail_puts(qqt,"; ");
  datetime_tai(&dt,now());
  qmail_put(qqt,buf,date822fmt(buf,&dt));

  if (rblinfo) {
    i = str_chr(rblinfo,']'); 
    if (i) { 
      qmail_puts(qqt,"X-RBL-Info: "); 
      safeput(qqt,rblinfo+i+2);
      qmail_puts(qqt,"\n");
    }
  }
}

/* "Received-SPF: pass (Helogreeting: domain of Identity " */
/* "  designates Clientip as permitted sender) receiver=Hostname " */
/* "  client-ip=Clientip; envelope-from=Mailfrom; " */

void spfheader(struct qmail *qqt,char *spfinfo,char *local,char *remoteip,char *helohost,char *mailfrom)
{
  char *result = 0;
  char *identity = 0;
  char *clientip = 0;
  char *helo = 0;
  char *envelopefrom = 0;
  char *receiver = 0;
  char *problem = 0;
  char *mechanism = 0;
  int i, j = 0;
  int len;

  len = str_len(spfinfo);
  if (!len) return;

  for (i = 0; i < len; i++)
    if (spfinfo[i] == ' ') spfinfo[i] = '\0'; 
 
  for (i = 0; i < len; i++) {
    if (spfinfo[i] == '\0') {
      switch (spfinfo[i + 1]) {   
        case 'S': clientip = spfinfo + i + 3; break;
        case 'O': envelopefrom = spfinfo + i + 3; break;
        case 'C': identity = spfinfo + i + 3; break;
        case 'H': helo = spfinfo + i + 3; break;
        case 'T': receiver = spfinfo + i + 3; break;
        case 'P': problem = spfinfo + i + 3; break;
        case 'M': if (j = str_chr(spfinfo + i,'=')) spfinfo[i+j] = '\0';  
                  mechanism = spfinfo + i + 1; break;
        case 'R': result = spfinfo + i + 3; break;
        default: break;
      }
    }
  }

  if (!result || *result == 0) result = "o";
  if (!clientip || *clientip == 0) clientip = remoteip;
  if (!helo || *helo == 0) helo = helohost;
  if (!envelopefrom || *envelopefrom == 0) envelopefrom = mailfrom;
  if (!receiver || *receiver == 0) receiver = local;
  if (!problem || *problem == 0) problem = "unknown";
  if (!mechanism || *mechanism == 0) mechanism = "unknown";
  j = str_rchr(envelopefrom,'@');
  if (!identity || *identity == 0) 
    if (envelopefrom[j] == '@') identity = envelopefrom + j + 1;
    else identity = "unknown";

  qmail_puts(qqt,"Received-SPF: ");
  switch (*result) {
    case '+': qmail_puts(qqt," pass ("); safeput(qqt,helo);
              qmail_puts(qqt,": domain of "); safeput(qqt,identity); qmail_puts(qqt,"\n");
              qmail_puts(qqt,"  designates "); safeput(qqt,clientip); qmail_puts(qqt," as permitted sender)\n");
              qmail_puts(qqt,"  receiver="); safeput(qqt,receiver); 
              qmail_puts(qqt,"; client-ip="); safeput(qqt,clientip); qmail_puts(qqt,"\n");
              qmail_puts(qqt,"  envelope-from=<"); safeput(qqt,envelopefrom); qmail_puts(qqt,">\n"); break;
    case '-': qmail_puts(qqt," fail ("); safeput(qqt,helo);
              qmail_puts(qqt,": domain of "); safeput(qqt,identity); qmail_puts(qqt,"\n");
              qmail_puts(qqt,"  does not designate "); safeput(qqt,clientip); qmail_puts(qqt," as permitted sender)\n"); break;
    case '~': qmail_puts(qqt," softfail ("); safeput(qqt,helo);
              qmail_puts(qqt,": domain of transitioning "); safeput(qqt,identity); qmail_puts(qqt,"\n");
              qmail_puts(qqt,"  does not designate "); safeput(qqt,clientip); qmail_puts(qqt," as permitted sender)\n"); break;
    case '?': qmail_puts(qqt," neutral ("); safeput(qqt,helo); qmail_puts(qqt,"; client-ip="); safeput(qqt,clientip);
              qmail_puts(qqt," is neither permitted \n"); qmail_puts(qqt,"  nor denied by domain of "); safeput(qqt,identity),
              qmail_puts(qqt,")\n"); break;
    case 'o': qmail_puts(qqt," none ("); safeput(qqt,helo);
              qmail_puts(qqt,": domain of "); safeput(qqt,identity); qmail_puts(qqt," does\n");
              qmail_puts(qqt,"  not designate permitted sender hosts)\n"); break;
    case 't': qmail_puts(qqt," temperror ("); safeput(qqt,helo);
              qmail_puts(qqt,": domain of "); safeput(qqt,identity); qmail_puts(qqt," evaluated\n"); 
              qmail_puts(qqt,"  with error: "); safeput(qqt,problem); qmail_puts(qqt," for mechanism: "); safeput(qqt,mechanism);
              qmail_puts(qqt,")\n"); break;
    case 'e': qmail_puts(qqt," permerror ("); safeput(qqt,helo);
              qmail_puts(qqt,": domain of "); safeput(qqt,identity); qmail_puts(qqt," evaluated\n");
              qmail_puts(qqt,"  with error: "); safeput(qqt,problem); qmail_puts(qqt," for mechanism: "); safeput(qqt,mechanism);
              qmail_puts(qqt,")\n"); break;
    default:  qmail_puts(qqt," unknown (results for "); safeput(qqt,helo);
              qmail_puts(qqt,": domain of "); safeput(qqt,identity);
              qmail_puts(qqt,"  follow an unknown mechanism: "); safeput(qqt,mechanism); qmail_puts(qqt,")\n"); break;
  }
}
