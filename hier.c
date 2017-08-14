#include "auto_qmail.h"
#include "auto_split.h"
#include "auto_uids.h"
#include "fmt.h"
#include "fifo.h"
#include "ipalloc.h"
#include "tcpto.h"

char buf[100 + FMT_ULONG];

void dsplit(char *base,int uid,int mode) /* base must be under 100 bytes */
{
  char *x;
  unsigned long i;

  d(auto_qmail,base,uid,auto_gidq,mode);

  for (i = 0;i < auto_split;++i) {
    x = buf;
    x += fmt_str(x,base);
    x += fmt_str(x,"/");
    x += fmt_ulong(x,i);
    *x = 0;

    d(auto_qmail,buf,uid,auto_gidq,mode);
  }
}

void hier()
{
  h(auto_qmail,auto_uido,auto_gidq,0755);

  d(auto_qmail,"control",auto_uido,auto_gidq,0755);
  d(auto_qmail,"users",auto_uido,auto_gidq,0755);
  d(auto_qmail,"bin",auto_uido,auto_gidq,0755);
  d(auto_qmail,"alias",auto_uida,auto_gidq,02755);

  d(auto_qmail,"queue",auto_uidq,auto_gidq,0750);
  d(auto_qmail,"queue/pid",auto_uidq,auto_gidq,0700);
  d(auto_qmail,"queue/intd",auto_uidq,auto_gidq,0700);
  d(auto_qmail,"queue/todo",auto_uidq,auto_gidq,0750);
  d(auto_qmail,"queue/bounce",auto_uids,auto_gidq,0700);

  dsplit("queue/mess",auto_uidq,0750);
  dsplit("queue/todo",auto_uidq,0750);
  dsplit("queue/intd",auto_uidq,0700);
  dsplit("queue/info",auto_uids,0700);
  dsplit("queue/local",auto_uids,0700);
  dsplit("queue/remote",auto_uids,0700);

  d(auto_qmail,"queue/lock",auto_uidq,auto_gidq,0750);
  z(auto_qmail,"queue/lock/tcpto",TCPTO_BUFSIZ,auto_uidr,auto_gidq,0644);
  z(auto_qmail,"queue/lock/sendmutex",0,auto_uids,auto_gidq,0600);
  p(auto_qmail,"queue/lock/trigger",auto_uids,auto_gidq,0622);

  c(auto_qmail,"bin","qmail-queue",auto_uidq,auto_gidq,04711);
  c(auto_qmail,"bin","qmail-lspawn",auto_uido,auto_gidq,0700);
  c(auto_qmail,"bin","qmail-start",auto_uido,auto_gidq,0700);
  c(auto_qmail,"bin","qmail-getpw",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-local",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-remote",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-smtpam",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-rspawn",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-clean",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-send",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-todo",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","splogger",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-newu",auto_uido,auto_gidq,0700);
  c(auto_qmail,"bin","qmail-newmrh",auto_uido,auto_gidq,0700);

  c(auto_qmail,"bin","qmail-authuser",auto_uido,auto_gidq,06711);
  c(auto_qmail,"bin","qmail-vmailuser",auto_uido,auto_gidq,06711);
  c(auto_qmail,"bin","qmail-badloadertypes",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-badmimetypes",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-recipients",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-mfrules",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-mrtg",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qmail-mrtg-queue",auto_uido,auto_gidq,0755);

  c(auto_qmail,"bin","qmail-pw2u",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-inject",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qmail-showctl",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qmail-qread",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qmail-qstat",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qmail-tcpto",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qmail-tcpok",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qmail-pop3d",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qmail-popup",auto_uido,auto_gidq,0711);
  c(auto_qmail,"bin","qmail-qmqpc",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qmail-qmqpd",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qmail-qmtpd",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qmail-smtpd",auto_uido,auto_gidq,0755);

  c(auto_qmail,"bin","predate",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","datemail",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","mailsubj",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","sendmail",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qreceipt",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","qbiff",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","forward",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","preline",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","condredirect",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","bouncesaying",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","except",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","maildirmake",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","maildir2mbox",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","maildirwatch",auto_uido,auto_gidq,0755);

  c(auto_qmail,"bin","fastforward",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","printforward",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","setforward",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","newaliases",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","printmaillist",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","setmaillist",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","newinclude",auto_uido,auto_gidq,0755);

  c(auto_qmail,"bin","ipmeprint",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","spfquery",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","dnscname",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","dnsfq",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","dnsmxip",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","dnsptr",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","dnstxt",auto_uido,auto_gidq,0755);

  c(auto_qmail,"bin","columnt",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","ddist",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","deferrals",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","failures",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","matchup",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","recipients",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","rhosts",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","rxdelay",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","senders",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","successes",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","suids",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","tai64nfrac",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","xqp",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","xrecipient",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","xsender",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zddist",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zdeferrals",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zfailures",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zfailures",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zoverall",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zrecipients",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zrhosts",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zrxdelay",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zsenders",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zsendmail",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zsuccesses",auto_uido,auto_gidq,0755);
  c(auto_qmail,"bin","zsuids",auto_uido,auto_gidq,0755);
}
