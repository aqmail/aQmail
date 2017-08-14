/* errmsg.c - print error messages to stderr
 *
 *  Revision 20170502, Kai Peter
 *  - added handling for negative error codes
 *  Revision 20170422, Kai Peter
 *  - new file
*/
#include <unistd.h>
#include "error.h"
#include "buffer.h"
#include "fmt.h"
#include "str.h"

unsigned int loglevel;

int errmsg(char *who, int code, unsigned int severity, char *msg) {
  char strnum[FMT_ULONG];
  char *codestr;
  char *errorstr;
  char *logstr;

  if (!loglevel) loglevel = DEF_LOGLEVEL;

  errno = 0;   /* re-initialize errno, value is in 'code' now */
  if (loglevel < severity) return 0;

  switch(severity) {
  case FATAL:  logstr = "fatal: ";    break;   // hard
  case ALERT:  logstr = "alert: ";    break;
  case CRIT:   logstr = "critical: "; break;
  case ERROR:  logstr = "error: ";    break;   // temp/soft
  case WARN:   logstr = "warning: ";  break;
  case NOTICE: logstr = "notice: ";   break;
  case INFO:   logstr = "info: ";     break;
  case DEBUG:  logstr = "debug: ";    break;
  default: /* shouldn't happen */
      severity = WARN;
      logstr="unknown: ";
    break;
  }

  /* handle the (error) code */
  if (code != 0) {
    codestr = "";
    errorstr = errstr(code);     /* worst case: "unknown error" */
    if (code < 0) {           /* check for negative error codes */
      code = (code * (-1));
      codestr = "-";
    }
    strnum[fmt_ulong(strnum,code)] = 0;    /* format for output */
    char *temp = strnum;
    codestr = str_cat(codestr,temp);
  }

  /* if msg is empty, try to get a system message */
  if (severity > ERROR)
    if (str_equal(msg,"")) msg = errstr(code);  /* worst case: "no error" */

  buffer_puts(buffer_2,who);
  buffer_puts(buffer_2,logstr);
  /* log the next content if an error exists only */
  if (severity < WARN) {     // better ??? !severity > DEF_LOGLEVEL
    buffer_puts(buffer_2,errorstr);
    buffer_puts(buffer_2," (");
    buffer_puts(buffer_2,codestr);
    buffer_puts(buffer_2,")");
    if (!str_equal(msg,""))
      buffer_puts(buffer_2,": ");
  }
  buffer_puts(buffer_2,msg);
  buffer_puts(buffer_2,"\n");
  buffer_flush(buffer_2);

  if(severity > ERROR) return 0;  /* continue on informal severity */
  _exit(code);
}
