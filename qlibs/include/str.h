/*
 *  Revision 20170501, Kai Peter
 *  - added '*str_append' and 'str_cat'
*/
#ifndef STR_H
#define STR_H

#include "stralloc.h"

extern unsigned int str_copy();
extern int str_diff();
extern int str_diffn();
extern unsigned int str_len();
extern unsigned int str_chr();
extern unsigned int str_rchr();
extern int str_start();

#define str_equal(s,t) (!str_diff((s),(t)))

extern char *str_append(char *out, char *s);
#define str_cat(s,t) str_append(s,t)

#endif
