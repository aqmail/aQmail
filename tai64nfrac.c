#include "strerr.h"
#include "stralloc.h"
#include "substdio.h"
#include "exit.h"
#include "readwrite.h"
#include "open.h"
#include "scan.h"
#include "fmt.h"
#include "getln.h"

#define TAI64NLEN 24

/** @file tai64nfrac
    @brief Read a TAI64N external format timestamp from stdin and 
           write fractional seconds since epoch (TAI, not UTC) to stdout.  
           Return the characters after the timestamp. 
  */

char ssoutbuf[64];
substdio ssout = SUBSTDIO_FDBUF(write,1,ssoutbuf,sizeof(ssoutbuf));

static void putsa(stralloc *sa)
{
  substdio_putflush(&ssout,sa->s,sa->len);
}

static void puts(char *s)
{
  if (substdio_puts(&ssout,s) == -1) _exit(1);
  if (substdio_flush(&ssout) == -1) _exit(1);
}

static void puti(int i)
{
  char num[FMT_ULONG];

  if (substdio_bput(&ssout,num,fmt_ulong(num,(unsigned long) i)) == -1) _exit(1);
  if (substdio_flush(&ssout) == -1) _exit(1);
}

char ssinbuf[1024];
substdio ssin = SUBSTDIO_FDBUF(read,0,ssinbuf,sizeof(ssinbuf));

int main(void)
{
  int c;
  int i;
  int match;
  unsigned long u;
  unsigned long seconds;  
  unsigned long nanoseconds;
  stralloc line = {0};

/* Read from stdin */

  substdio_fdbuf(&ssin,read,0,ssinbuf,sizeof(ssinbuf));

  for (;;) {
    if (getln(&ssin,&line,&match,'\n') != 0) _exit(1);
    if (!match) break;
    if (!stralloc_0(&line)) _exit(1);

    seconds = 0;
    nanoseconds = 0;
    
    if (line.s[0] == '@') {     /* tai64 timestamp */  
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
      puti(seconds); puts("."); puti(nanoseconds); puts(line.s+i); puts("\n");
    } else {
      puts("tai64nfrac: fatal: Wrong TAI64N input format."); puts("\n");
      _exit(1);
    }
  }

  _exit(0);
}
