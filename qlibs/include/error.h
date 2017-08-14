#ifndef ERROR_H
#define ERROR_H

#include <errno.h>

#ifndef EPROTO             /* OpenBSD compat */
#define EPROTO EINTR
#endif

/* djb backwards compatibility - deprecated */
                                          /* Comparison of error codes and constants:
                                             intern   Linux  FreeBSD              */
#define error_intr          EINTR         /*    -1       4       4     */
#define error_nomem         ENOMEM        /*    -2      12      12     */
#define error_noent         ENOENT        /*    -3       2       2     */
#define error_txtbsy        ETXTBSY       /*    -4      26      26     */
#define error_io            EIO           /*    -5       5       5     */
#define error_exist         EEXIST        /*    -6      17      17     */
#define error_timeout       ETIMEDOUT     /*    -7     110      60     */
#define error_inprogress    EINPROGRESS   /*    -8     115      36     */
#define error_wouldblock    EWOULDBLOCK   /*    -9    EAGAIN  EAGAIN   */
#define error_again         EAGAIN        /*   -10      11      35     */
#define error_pipe          EPIPE         /*   -11      32      32     */
#define error_perm          EPERM         /*   -12       1       1     */
#define error_acces         EACCES        /*   -13      13      13     */
//extern int error_nodevice;              /*   -14      (6)     (6)    */
#define error_proto         EPROTO        /*   -15      71      92     */
#define error_isdir         EISDIR        /*   -16      21      21     */
#define error_connrefused   ECONNREFUSED  /*   -17     111      61     */
//extern int error_notdir;                /*   -18      20      20     */
#define error_rofs          EROFS         /*   -19      30      30     */
#define error_connreset     ECONNRESET    /*   -20     104      54     */

#define error_str(i) errstr(i)
extern int error_temp();   /* deprecated */
/* end djb backwards compatibility */


/* severity constants (similar to RfC 5424) */
#define FATAL     0     /* ERMERGENCY (FATAL) */
#define ALERT     1     /* ALERT */
#define CRIT      2     /* CRITICAL */
#define ERROR     3     /* ERROR */
#define WARN      4     /* WARNING */
#define NOTICE    5     /* NOTICE */
#define INFO      6     /* INFO */
#define DEBUG     7     /* DEBUG */

#define DEF_LOGLEVEL WARN

extern unsigned int loglevel;

extern int errmsg(char *who, int errorcode, unsigned int severity, char *emsg);
extern char *errstr(int code);

/* djb uses some internal codes */
#define ESOFT  -100   /* soft error, reversed negative */
#define EHARD  -111   /* hard error, reversed negative */

/* useful combinations of params */
/* severity: FATAL (hard) - system error */
#define err_sys(c) errmsg(WHO,c,FATAL,"")
#define err_sys_plus(c,s) errmsg(WHO,c,FATAL,s)
/* severity: ERROR (temp) - user error */
#define err_tmp(c) errmsg(WHO,c,ERROR,"")
#define err_tmp_plus(c,s) errmsg(WHO,c,ERROR,s)
/* severity: facultative ('int'ernal definition */
#define err_int(c,l) errmsg(WHO,c,l,"")
#define err_int_plus(c,l,s) errmsg(WHO,c,l,s)
/* log messages */
#define log(l,s) errmsg(WHO,0,l,s)
#define log_anon(l,s) errmsg("",0,l,s)

#endif
