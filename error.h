#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
//extern int errno;

extern int error_intr;		/*  1 */
extern int error_nomem;		/*  2 */
extern int error_noent;		/*  3 */
extern int error_txtbsy;	/*  4 */
extern int error_io;		/*  5 */
extern int error_exist;		/*  6 */
extern int error_timeout;	/*  7 */
extern int error_inprogress;	/*  8 */
extern int error_wouldblock;	/*  9 */
extern int error_again;		/* 10 */
extern int error_pipe;		/* 11 */
extern int error_perm;		/* 12 */
extern int error_acces;		/* 13 */
extern int error_nodevice;	/* 14 */
extern int error_proto;		/* 15 */
extern int error_isdir;		/* 16 */
extern int error_connrefused;	/* 17 */

char *error_str();
int error_temp();

#endif
