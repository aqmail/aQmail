#ifndef GETLN_H
#define GETLN_H
#include "substdio.h"
#include "stralloc.h"

extern int getln(substdio *,stralloc *,int *,int);
extern int getln2(substdio *,stralloc *,char **,unsigned int *,int);

#endif
