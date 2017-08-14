#ifndef CONTROL_H
#define CONTROL_H

#include "stralloc.h"

int control_init(void);
int control_readline(stralloc *,char *);
int control_rldef(stralloc *,char *,int,char *);
int control_readint(int *,char *);
int control_readfile(stralloc *,char *,int);

#endif
