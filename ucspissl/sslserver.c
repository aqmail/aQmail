/**
  @file  sslserver.c
  @author web, fefe, feh
  @brief IPv6 enabled sslserver
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>  //* check!
#include <sys/param.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>
#include "ucspissl.h"
//#include "uint16.h"
#include "uint_t.h"
#include "str.h"
#include "byte.h"
#include "fmt.h"
#include "scan.h"
//#include "ip4.h"
//#include "ip6.h"
#include "ip.h"
#include "fd.h"
#include "exit.h"
#include "env.h"
#include "prot.h"
#include "open.h"
#include "wait.h"
#include "stralloc.h"
#include "alloc.h"
#include "buffer.h"
#include "getln.h"
#include "error.h"
#include "strerr.h"
//#include "sgetopt.h"
#include "getoptb.h"
#include "pathexec.h"
#include "socket.h"
#include "ndelay.h"
#include "remoteinfo.h"
#include "rules.h"
#include "sig.h"
#include "dns.h"
#include "auto_cafile.h"
#include "auto_cadir.h"
#include "auto_ccafile.h"
#include "auto_dhfile.h"
#include "auto_certfile.h"
#include "auto_certchainfile.h"
#include "auto_keyfile.h"
#include "auto_ciphers.h"
#include "fmt.h"
// @Kai: added
#include "socket.h"

int ipv4;   // declared in ucspi-ssl's 'socket.h'
#define V4MAPPREFIX "::ffff:"    // declared in ucspi-ssl's 'ip6.h'

//typedef ssize_t buffer_0_read; int buffer_unixread;

// next comes from ucspi-ssl' buffer lib:
int buffer_unixread(int fd,char *buf,unsigned int len);
int buffer_unixwrite(int fd,const char *buf,unsigned int len);
#include "buffer_read.c"
#include "buffer_write.c"
// end Kai added

int ipv6 = 0;
int verbosity = 1;
int flagkillopts = 1;
int flagdelay = 0;
const char *banner = "";
int flagremoteinfo = 0;
int flagremotehost = 1;
int flagparanoid = 0;
int flagclientcert = 0;
int flagsslenv = 0;
int flagtcpenv = 0;
int flagsslwait = 0;
unsigned long timeout = 26;
unsigned long ssltimeout = 26;
unsigned int progtimeout = 3600;
uint32 netif = 0;

static stralloc tcpremoteinfo;

uint16 localport;
char localportstr[FMT_ULONG];
char localip[16];
char localipstr[IP6_FMT];
static stralloc localhostsa;
const char *localhost = 0;

uint16 remoteport;
char remoteportstr[FMT_ULONG];
char remoteip[16];
char remoteipstr[IP6_FMT];
char remoteip6str[IP6_FMT];
static stralloc remotehostsa;
char *remotehost = 0;
char *verifyhost = 0;

unsigned long uid = 0;
unsigned long gid = 0;

char strnum[FMT_ULONG];
char strnum2[FMT_ULONG];

static stralloc tmp;
static stralloc fqdn;
static stralloc addresses;

char bspace[16];
buffer b;

SSL_CTX *ctx;
const char *certchainfile = auto_certchainfile;
const char *certfile = auto_certfile;
const char *keyfile = auto_keyfile;
stralloc password = {0};
int match = 0;
const char *cafile = auto_cafile;
const char *ccafile = auto_ccafile;
const char *cadir = auto_cadir;
const char *ciphers = auto_ciphers;
int verifydepth = 1;
const char *dhfile = auto_dhfile;
int rsalen = SSL_RSA_LEN;

char * const *prog;

int pi[2];
int po[2];
int pt[2];

stralloc envsa = {0};

X509 *cert;
char buf[SSL_NAME_LEN];

#define FATAL "sslserver: fatal: "


/* ---------------------------- child */

#define DROP "sslserver: warning: dropping SSL connection, "

int flagdeny = 0;
int flagallownorules = 0;
const char *fnrules = 0;

void drop_nomem(void)
{
  strerr_die2sys(111,DROP,"out of memory");
}
void cats(const char *s)
{
  if (!stralloc_cats(&tmp,s)) drop_nomem();
}
void append(const char *ch)
{
  if (!stralloc_append(&tmp,ch)) drop_nomem();
}
void safecats(const char *s) {
  char ch;
  int i;

  for (i = 0;i < 100;++i) {
    ch = s[i];
    if (!ch) return;
    if (ch < 33) ch = '?';
    if (ch > 126) ch = '?';
    if (ch == '%') ch = '?'; /* logger stupidity */
    append(&ch);
  }
  cats("...");
}
void env(const char *s,const char *t) {
  if (!pathexec_env(s,t)) drop_nomem();
}
int error_warn(const char *x) {
  if (!x) return 0;
  strerr_warn2("sslserver: ",x,0);
  return 0;
}
void drop_rules(void) {
  strerr_die4sys(111,DROP,"unable to read ",fnrules,": ");
}

void found(char *data,unsigned int datalen) {
  unsigned int next0;
  unsigned int split;

  while ((next0 = byte_chr(data,datalen,0)) < datalen) {
    switch(data[0]) {
      case 'D':
	flagdeny = 1;
	break;
      case '+':
	split = str_chr(data + 1,'=');
	if (data[1 + split] == '=') {
	  data[1 + split] = 0;
	  env(data + 1,data + 1 + split + 1);
	}
	break;
    }
    ++next0;
    data += next0; datalen -= next0;
  }
}

void doit(int t) {
  int j;
  SSL *ssl = 0;
  int wstat;
  int sslctl[2];
  char *s;
  unsigned long tmp_long;
  char ssl_cmd;
  stralloc ssl_env = {0};
  int bytesleft;
  char envbuf[8192];
  int childpid;
  int mappedv4 = 0;
  uint32 scope_id;
  char *stripaddr;
  stralloc tlsinfo = {0};

  if (pipe(pi) == -1) strerr_die2sys(111,DROP,"unable to create pipe: ");
  if (pipe(po) == -1) strerr_die2sys(111,DROP,"unable to create pipe: ");
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sslctl) == -1) 
    strerr_die2sys(111,DROP,"unable to create socketpair: ");

/* Get remote IP and FQDN to validate X.509 cert */

  if (!ipv6 && ip6_isv4mapped(remoteip)) mappedv4 = 1;

  if (ipv6 && !ipv4)		/* standard IPv6 (mapped) address */
    remoteip6str[ip6_compactaddr(remoteip6str,remoteip)] = 0;
  if (mappedv4)			/* convert IPv4-mapped IPv6 address to IP4; cdb lookup */
    remoteipstr[ip4_fmt(remoteipstr,remoteip+12)] = 0;
  else				/* IPv4 address */
    remoteipstr[ip6_compactaddr(remoteipstr,remoteip)] = 0;

  switch(childpid=fork()) {
    case -1:
      strerr_die2sys(111,DROP,"unable to fork: ");
    case 0:
      /* Child */
      close(sslctl[0]);
      break;
    default:
      /* Parent */

      close(pi[0]); close(po[1]); close(sslctl[1]);

      if ((s=env_get("SSL_CHROOT")))
        if (chroot(s) == -1) {
          kill(childpid, SIGTERM);
          strerr_die2x(111,DROP,"unable to chroot");
        }

      if ((s=env_get("SSL_GID"))) {
        scan_ulong(s,&tmp_long);
        gid = tmp_long;
      }
      if (gid) if (prot_gid(gid) == -1) {
        kill(childpid, SIGTERM);
        strerr_die2sys(111,FATAL,"unable to set gid: ");
      }

      if ((s=env_get("SSL_UID"))) {
        scan_ulong(s,&tmp_long);
        uid = tmp_long;
      }
      if (uid)
        if (prot_uid(uid) == -1) {
          kill(childpid, SIGTERM);
          strerr_die2sys(111,FATAL,"unable to set uid: ");
        }

/* Read the TLS command socket. This will block until/unless TLS is requested. */

      if (read(sslctl[0],&ssl_cmd,1) == 1) {
        ssl = ssl_new(ctx,t);
        if (!ssl) {
          kill(childpid, SIGTERM);
          strerr_die2x(111,DROP,"unable to create SSL instance");
        }
        if (ndelay_on(t) == -1) {
          kill(childpid, SIGTERM);
          strerr_die2sys(111,DROP,"unable to set socket options: ");
        }
        if (ssl_timeoutaccept(ssl,ssltimeout) == -1) {
          kill(childpid, SIGTERM);
          strerr_die3x(111,DROP,"unable to accept SSL: ",ssl_error_str(ssl_errno));
        }
      }

      if (flagclientcert) {
        if (flagclientcert == 2) verifyhost = 0;
        switch(ssl_verify(ssl,verifyhost)) {
          case -1:
        kill(childpid, SIGTERM);
            strerr_die2x(111,DROP,"no client certificate");
          case -2:
        kill(childpid, SIGTERM);
            strerr_die2x(111,DROP,"missing credentials (CA) or unable to validate client certificate");
          case -3:
        kill(childpid, SIGTERM);
            strerr_die3x(111,DROP,"client hostname does not match certificate: ",verifyhost);
          default: break;
        }
      }

/* Request SSL */

      if (ssl_cmd == 'Y') {
        ssl_server_env(ssl, &ssl_env);
        if(!stralloc_0(&ssl_env)) drop_nomem(); /* Add another NUL */
        env("SSLCTL",ssl_env.s); 

        for(bytesleft = ssl_env.len; bytesleft>0; bytesleft-=j)
          if ( (j=write(sslctl[0], ssl_env.s, bytesleft)) < 0) {
            kill(childpid, SIGTERM);
            strerr_die2sys(111, FATAL, "unable to write SSL environment: ");
          }
      }

      if (verbosity >= 2) {
        strnum[fmt_ulong(strnum,getpid())] = 0;
        if (verbosity >= 3 && ssl_env.len > 1) {
          s = ssl_env.s;
          if ((j = str_chr(s,'=')))
            if (!stralloc_copys(&tlsinfo,s+j+1)) drop_nomem();
          if (!stralloc_cats(&tlsinfo,":")) drop_nomem();
          s = s + str_len(s) + 1;
          s = s + str_len(s) + 1;
          if ((j = str_chr(s,'=')))
            if (!stralloc_cats(&tlsinfo,s+j+1)) drop_nomem();
          if (!stralloc_0(&tlsinfo)) drop_nomem();
          strerr_warn4("sslserver: ssl ",strnum," accept ",tlsinfo.s,0);
        } else
          strerr_warn3("sslserver: ssl ",strnum," accept ",0);
      }

      if (ssl_cmd == 'Y' || ssl_cmd == 'y') {
        if (ssl_io(ssl,pi[1],po[0],progtimeout) != 0) {
          kill(childpid, SIGTERM);
          strerr_die3x(111,DROP,"unable to speak SSL: ",ssl_error_str(ssl_errno));
        }
        if (wait_nohang(&wstat) > 0)
          _exit(wait_exitcode(wstat)); 
        ssl_close(ssl);
      }
      kill(childpid, SIGTERM);
      _exit(0);
  }

/* Child-only below this point */

  if (verbosity >= 2) {
    strnum[fmt_ulong(strnum,getpid())] = 0;
    strerr_warn4("sslserver: pid ",strnum," from ",remoteipstr,0);
  }

  if (socket_local4(t,localip,&localport) == -1)
    strerr_die2sys(111,DROP,"unable to get local address: ");

  if (socket_local6(t,localip,&localport,&scope_id) == -1)
    strerr_die2sys(111,DROP,"unable to get local address: ");

/* Local IP information */

  if (mappedv4)
    localipstr[ip4_fmt(localipstr,&localip[12])] = 0;
  else
    localipstr[ip6_compactaddr(localipstr,localip)] = 0;
    
  if (!localhost)
    if (dns_name6(&localhostsa,localip) == 0)
      if (localhostsa.len) {
	if (!stralloc_0(&localhostsa)) drop_nomem();
	localhost = localhostsa.s;
      }

/* Remote IP information */

  stripaddr=remoteipstr;
  if (!ipv6 && byte_equal(remoteipstr,7,V4MAPPREFIX))
    stripaddr = remoteipstr+7;
    
  remoteportstr[fmt_ulong(remoteportstr,remoteport)] = 0;

  if (flagremotehost)
    if (dns_name6(&remotehostsa,remoteip) == 0)
      if (remotehostsa.len) {
        if (flagparanoid) {
          verifyhost = remoteipstr;
          if (!ipv4) {
            if (dns_ip6(&tmp,&remotehostsa) == 0)
              for (j = 0;j + 16 <= tmp.len;j += 16)
                if (byte_equal(remoteip,16,tmp.s + j)) {
                  flagparanoid = 0;
                  break;
                }
          } else if (dns_ip4(&tmp,&remotehostsa) == 0) {
              for (j = 0;j + 4 <= tmp.len;j += 4)
                if (byte_equal(stripaddr,4,tmp.s + j)) {
                  flagparanoid = 0;
                  break;
                }
          }
        }
        if (!flagparanoid) {
          if (!stralloc_0(&remotehostsa)) drop_nomem();
            remotehost = remotehostsa.s;
            verifyhost = remotehostsa.s;
        }
      }

  if (flagremoteinfo) {
    if (remoteinfo6(&tcpremoteinfo,remoteip,remoteport,localip,localport,timeout,netif) == -1)
      flagremoteinfo = 0;
    if (!stralloc_0(&tcpremoteinfo)) drop_nomem();
  }

/* Setup environment variables */

  env("PROTO","SSL");
  env("SSLLOCALIP",localipstr);
  env("SSLLOCALPORT",localportstr);
  env("SSLLOCALHOST",localhost);
  if (flagtcpenv) {
    env("TCPLOCALIP",localipstr);
    env("TCPLOCALPORT",localportstr);
    env("TCPLOCALHOST",localhost);
  }

  env("SSLREMOTEIP",remoteipstr);
  env("SSLREMOTEPORT",remoteportstr);
  env("SSLREMOTEHOST",remotehost);
  if (flagtcpenv) {
    env("PROTO",mappedv4?"TCP":"TCP6");
    if (!ipv4) {
      remoteipstr[ip6_compactaddr(remoteip6str,remoteip)] = 0;
      env("TCP6REMOTEIP",remoteip6str);
      env("TCP6REMOTEPORT",remoteportstr);
      env("TCP6REMOTEHOST",remotehost);
    }
    env("TCPREMOTEIP",stripaddr);
    env("TCPREMOTEPORT",remoteportstr);
    env("TCPREMOTEHOST",remotehost);
    if (!mappedv4) {
      env("TCP6LOCALIP",localipstr);
      env("TCP6LOCALHOST",localhost);
      env("TCP6LOCALPORT",localportstr);
      if (scope_id)
        env("TCP6INTERFACE",socket_getifname(scope_id));
    }
  }

  env("SSLREMOTEINFO",flagremoteinfo ? tcpremoteinfo.s : 0);
  if (flagtcpenv)
    env("TCPREMOTEINFO",flagremoteinfo ? tcpremoteinfo.s : 0);

/* Evaluate rules cdb */

  if (fnrules) {
    int fdrules;
    fdrules = open_read(fnrules);
    if (fdrules == -1) {
      if (errno != error_noent) drop_rules();
      if (!flagallownorules) drop_rules();
    } else {
      if (rules(found,fdrules,stripaddr,remotehost,flagremoteinfo ? tcpremoteinfo.s : 0) == -1) drop_rules();
      close(fdrules);
    }
  }

  if (verbosity >= 2) {
    strnum[fmt_ulong(strnum,getpid())] = 0;
    if (!stralloc_copys(&tmp,"sslserver: ")) drop_nomem();
    safecats(flagdeny ? "deny" : "ok");
    cats(" "); safecats(strnum);
    cats(" "); if (localhost) safecats(localhost);
    cats(":"); safecats(localipstr);
    cats(":"); safecats(localportstr);
    cats(" "); if (remotehost) safecats(remotehost);
    cats(":"); safecats(stripaddr);
    cats(":"); if (flagremoteinfo) safecats(tcpremoteinfo.s);
    cats(":"); safecats(remoteportstr);
    cats("\n");
    buffer_putflush(buffer_2,tmp.s,tmp.len);
  }

  if (flagdeny) _exit(100);

  if (gid) if (prot_gid(gid) == -1)
    strerr_die2sys(111,FATAL,"unable to set gid: ");
  if (uid) if (prot_uid(uid) == -1)
    strerr_die2sys(111,FATAL,"unable to set uid: ");

  close(pi[1]); close(po[0]); close(sslctl[0]);

  sig_uncatch(sig_child);
  sig_unblock(sig_child);
  sig_uncatch(sig_term);
  sig_uncatch(sig_pipe);

  if (fcntl(sslctl[1],F_SETFD,0) == -1)
    strerr_die2sys(111,FATAL,"unable to clear close-on-exec flag");
  strnum[fmt_ulong(strnum,sslctl[1])]=0;
  env("SSLCTLFD",strnum);

  if (fcntl(pi[0],F_SETFD,0) == -1)
    strerr_die2sys(111,FATAL,"unable to clear close-on-exec flag");
  strnum[fmt_ulong(strnum,pi[0])]=0;
  env("SSLREADFD",strnum);

  if (fcntl(po[1],F_SETFD,0) == -1)
    strerr_die2sys(111,FATAL,"unable to clear close-on-exec flag");
  strnum[fmt_ulong(strnum,po[1])]=0;
  env("SSLWRITEFD",strnum);

  if (flagsslwait) {
    if (fd_copy(0,t) == -1)
      strerr_die2sys(111,DROP,"unable to set up descriptor 0: ");
    if (fd_copy(1,t) == -1)
      strerr_die2sys(111,DROP,"unable to set up descriptor 1: ");
  } else {
    if (fd_move(0,pi[0]) == -1)
      strerr_die2sys(111,DROP,"unable to set up descriptor 0: ");
    if (fd_move(1,po[1]) == -1)
      strerr_die2sys(111,DROP,"unable to set up descriptor 1: ");
  }

  if (flagkillopts) {
    socket_ipoptionskill(t);
    socket_ip6optionskill(t);
  }
  
  if (!flagdelay)
    socket_tcpnodelay(t);

  if (*banner) {
    buffer_init(&b,buffer_unixwrite,1,bspace,sizeof bspace);
    if (buffer_putsflush(&b,banner) == -1)
      strerr_die2sys(111,DROP,"unable to print banner: ");
  }

  if (!flagsslwait) {
    ssl_cmd = flagsslenv ? 'Y' : 'y';
    if (write(sslctl[1], &ssl_cmd, 1) < 1)
      strerr_die2sys(111,DROP,"unable to start SSL: ");
    if (flagsslenv) {
      while ((j=read(sslctl[1],envbuf,8192)) > 0) {
        stralloc_catb(&ssl_env,envbuf,j);
        if (ssl_env.len >= 2 && ssl_env.s[ssl_env.len-2]==0 && ssl_env.s[ssl_env.len-1]==0)
          break;
      }
      if (j < 0)
        strerr_die2sys(111,DROP,"unable to read SSL environment: ");
      pathexec_multienv(&ssl_env);
    }
  }
      
  pathexec(prog);
  strerr_die4sys(111,DROP,"unable to run ",*prog,": ");
}



/* ---------------------------- parent */

void usage(void)
{
  strerr_warn1("\
sslserver: usage: sslserver \
[ -1346UXpPhHrRoOdDqQvVIeEsSnNmzZ ] \
[ -c limit ] \
[ -x rules.cdb ] \
[ -B banner ] \
[ -g gid ] \
[ -u uid ] \
[ -b backlog ] \
[ -l localname ] \
[ -t timeout ] \
[ -I interface ] \
[ -T ssltimeout ] \
[ -w progtimeout ] \
host port program",0);
  _exit(100);
}

unsigned long limit = 40;
unsigned long numchildren = 0;

int flag1 = 0;
int flag3 = 0;
unsigned long backlog = 20;

void printstatus(void)
{
  if (verbosity < 2) return;
  strnum[fmt_ulong(strnum,numchildren)] = 0;
  strnum2[fmt_ulong(strnum2,limit)] = 0;
  strerr_warn4("sslserver: status: ",strnum,"/",strnum2,0);
}

void sigterm(void)
{
  _exit(0);
}

void sigchld(void) {
  int wstat;
  int pid;
 
  while ((pid = wait_nohang(&wstat)) > 0) {
    if (verbosity >= 2) {
      strnum[fmt_ulong(strnum,pid)] = 0;
      strnum2[fmt_ulong(strnum2,wstat)] = 0;
      strerr_warn4("sslserver: end ",strnum," status ",strnum2,0);
    }
    if (numchildren) --numchildren; printstatus();
  }
}

void read_passwd(void) {
  if (!password.len) {
    buffer_init(&b,buffer_unixread,3,bspace,sizeof bspace);
    if (getln(&b,&password,&match,'\0') == -1)
      strerr_die2sys(111,FATAL,"unable to read password: ");
    close(3);
    if (match) --password.len;
  }
}

int passwd_cb(char *buff,int size,int rwflag,void *userdata) {
  if (size < password.len)
    strerr_die2x(111,FATAL,"password too long");

  byte_copy(buff,password.len,password.s);
  return password.len;
}

int main(int argc,char * const *argv) {
  const char *hostname;
  int opt;
  struct servent *se;
  char *x;
  unsigned long u;
  int s;
  int t;
 
  while ((opt = getopt(argc,argv,"1346dDvVqQhHrRUXx:t:T:u:g:l:b:B:c:pPoOIEeSsw:nNzZm")) != opteof)
    switch(opt) {
      case 'b': scan_ulong(optarg,&backlog); break;
      case 'c': scan_ulong(optarg,&limit); break;
      case 'X': flagallownorules = 1; break;
      case 'x': fnrules = optarg; break;
      case 'B': banner = optarg; break;
      case 'd': flagdelay = 1; break;
      case 'D': flagdelay = 0; break;
      case 'v': verbosity = 2; break;
      case 'V': verbosity = 3; break;
      case 'q': verbosity = 0; break;
      case 'Q': verbosity = 1; break;
      case 'P': flagparanoid = 0; break;
      case 'p': flagparanoid = 1; break;
      case 'O': flagkillopts = 1; break;
      case 'o': flagkillopts = 0; break;
      case 'H': flagremotehost = 0; break;
      case 'h': flagremotehost = 1; break;
      case 'R': flagremoteinfo = 0; break;
      case 'r': flagremoteinfo = 1; break;
      case 't': scan_ulong(optarg,&timeout); break;
      case 'T': scan_ulong(optarg,&ssltimeout); break;
      case 'w': scan_uint(optarg,&progtimeout); break;
      case 'U': x = env_get("UID"); if (x) scan_ulong(x,&uid);
		x = env_get("GID"); if (x) scan_ulong(x,&gid); break;
      case 'u': scan_ulong(optarg,&uid); break;
      case 'g': scan_ulong(optarg,&gid); break;
      case 'I': netif=socket_getifidx(optarg); break;
      case 'l': localhost = optarg; break;
      case '1': flag1 = 1; break;
      case '3': flag3 = 1; break;
      case '4': ipv4 = 1; break;
      case '6': ipv6 = 1; break;
      case 'Z': flagclientcert = 0; break;
      case 'z': flagclientcert = 1; break;
      case 'm': flagclientcert = 2; break;
      case 'S': flagsslenv = 0; break;
      case 's': flagsslenv = 1; break;
      case 'E': flagtcpenv = 0; break;
      case 'e': flagtcpenv = 1; break;
      case 'n': flagsslwait = 1; break;
      case 'N': flagsslwait = 0; break;
      default: usage();
    }
  argc -= optind;
  argv += optind;

  if (!verbosity)
    buffer_2->fd = -1;
 
  hostname = *argv++;
  if (!hostname) usage();
  if (str_equal(hostname,"")) hostname = "0";

  x = *argv++;
  if (!x) usage();
  prog = argv;
  if (!*argv) usage();
  if (!x[scan_ulong(x,&u)])
    localport = u;
  else {
    se = getservbyname(x,"tcp");
    if (!se)
      strerr_die3x(111,FATAL,"unable to figure out port number for ",x);
    uint16_unpack_big((char *)&se->s_port,&localport);
  }

  if ((x = env_get("VERIFYDEPTH"))) {
    scan_ulong(x,&u);
    verifydepth = u;
  }

  if ((x = env_get("CAFILE"))) cafile = x;
  if (cafile && str_equal(cafile,"")) cafile = 0;

  if ((x = env_get("CCAFILE"))) ccafile = x;
  if (ccafile && str_equal(ccafile,"")) ccafile = 0;
  if (ccafile && str_equal(ccafile,"-")) flagclientcert = 0;
  if (!flagclientcert) ccafile = 0;

  if ((x = env_get("CADIR"))) cadir = x;
  if (cadir && str_equal(cadir,"")) cadir= 0;

  if ((x = env_get("CERTCHAINFILE"))) certchainfile = x;
  if (certchainfile && str_equal(certchainfile,"")) certchainfile = 0;

  if ((x = env_get("CERTFILE"))) certfile = x;
  if (certfile && str_equal(certfile,"")) certfile = 0;

  if ((x = env_get("KEYFILE"))) keyfile = x;
  if (keyfile && str_equal(keyfile,"")) keyfile = 0;

  if ((x = env_get("DHFILE"))) dhfile = x;
  if (dhfile && str_equal(dhfile,"")) dhfile = 0;

  if ((x = env_get("CIPHERS"))) ciphers = x;
  if (ciphers && str_equal(ciphers,"")) ciphers = 0;

  sig_block(sig_child);
  sig_catch(sig_child,sigchld);
  sig_catch(sig_term,sigterm);
  sig_ignore(sig_pipe);
 
  if (str_equal(hostname,"0")) {
      byte_zero(localip,sizeof localip);
  } else {
      if (!stralloc_copys(&tmp,hostname))
        strerr_die2x(111,FATAL,"out of memory");
      if (dns_ip6_qualify(&addresses,&fqdn,&tmp) == -1)
        strerr_die4sys(111,FATAL,"temporarily unable to figure out IP address for ",hostname,": ");
      if (addresses.len < 16)
        strerr_die3x(111,FATAL,"no IP address for ",hostname);
      byte_copy(localip,16,addresses.s);
      if (ip6_isv4mapped(localip))
        ipv4=1;
  }

  s = socket_tcp6();
  if (s == -1)
    strerr_die2sys(111,FATAL,"unable to create socket: ");

  if (socket_bind6_reuse(s,localip,localport,netif) == -1)
    strerr_die2sys(111,FATAL,"unable to bind: ");

  if (socket_local6(s,localip,&localport,&netif) == -1)
    strerr_die2sys(111,FATAL,"unable to get local address: ");

  if (socket_listen(s,backlog) == -1)
    strerr_die2sys(111,FATAL,"unable to listen: ");
  ndelay_off(s);

  localportstr[fmt_ulong(localportstr,localport)] = 0;
  if (flag1) {
    buffer_init(&b,buffer_unixwrite,1,bspace,sizeof bspace);
    buffer_puts(&b,localportstr);
    buffer_puts(&b,"\n");
    buffer_flush(&b);
  }
 
  if (flag3) read_passwd();

  ctx = ssl_server();
  ssl_errstr();
  if (!ctx) strerr_die2x(111,FATAL,"unable to create SSL context");

  if (certchainfile) {
    switch (ssl_chainfile(ctx,certchainfile,keyfile,passwd_cb)) {
      case -1: strerr_die2x(111,FATAL,"unable to load certificate chain file");
      case -2: strerr_die2x(111,FATAL,"unable to load key");
      case -3: strerr_die2x(111,FATAL,"key does not match certificate");
      default: break;
    }
  } 
  else {  
    switch (ssl_certkey(ctx,certfile,keyfile,passwd_cb)) {
      case -1: strerr_die2x(111,FATAL,"unable to load certificate");
      case -2: strerr_die2x(111,FATAL,"unable to load key");
      case -3: strerr_die2x(111,FATAL,"key does not match certificate");
      default: break;
    }
  }

  if (!ssl_ca(ctx,cafile,cadir,verifydepth))
    strerr_die2x(111,FATAL,"unable to load CA list");

  if (!ssl_cca(ctx,ccafile))
    strerr_die2x(111,FATAL,"unable to load client CA list");

  if (!ssl_params_rsa(ctx,rsalen))
    strerr_die2x(111,FATAL,"unable to set RSA parameters");
  if (!ssl_params_dh(ctx,dhfile))
    strerr_die2x(111,FATAL,"unable to set DH parameters");
  if (!ssl_params_ecdh(ctx))
    strerr_warn1("unable to set ECDH parameters",0);

  if (!ssl_ciphers(ctx,ciphers))
    strerr_die2x(111,FATAL,"unable to set cipher list");

  if (verbosity >= 2) {
    strnum[fmt_ulong(strnum,getpid())] = 0;
    strnum2[fmt_ulong(strnum2,rsalen)] = 0;
    strerr_warn4("sslserver: cafile ",strnum," ",cafile,0);
    strerr_warn4("sslserver: ccafile ",strnum," ",ccafile,0);
    strerr_warn4("sslserver: cadir ",strnum," ",cadir,0);
    strerr_warn4("sslserver: chainfile ",strnum," ",certchainfile,0);
    strerr_warn4("sslserver: cert ",strnum," ",certfile,0);
    strerr_warn4("sslserver: key ",strnum," ",keyfile,0);
    strerr_warn6("sslserver: param ",strnum," ",dhfile," ",strnum2,0);
  }

  close(0); open_read("/dev/null");
  close(1); open_append("/dev/null");

  printstatus();
 
  for (;;) {
    while (numchildren >= limit) sig_pause();

    sig_unblock(sig_child);
    t = socket_accept6(s,remoteip,&remoteport,&netif);
    sig_block(sig_child);

    if (t == -1) continue;
    ++numchildren; printstatus();
 
    switch(fork()) {
      case 0:
        close(s);
        doit(t);
	strerr_die4sys(111,DROP,"unable to run ",*argv,": ");
      case -1:
        strerr_warn2(DROP,"unable to fork: ",&strerr_sys);
        --numchildren; printstatus();
    }
    close(t);
  }
}

/* taken from 0.68 */
char *ssl_error_str(int e)
{
  SSL_load_error_strings();
  return ERR_error_string(e,0);
}
