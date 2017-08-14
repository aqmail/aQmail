#ifndef MAILDIR_H
#define MAILDIR_H

#include "strerr.h"
extern struct strerr maildir_chdir_err;
extern struct strerr maildir_scan_err;

int maildir_chdir(void);
void maildir_clean(stralloc *);
int maildir_scan(prioq *,stralloc *,int,int);
#endif
