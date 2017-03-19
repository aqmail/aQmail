#ifndef STRALLOC_H
#define STRALLOC_H

#include "gen_alloc.h"

GEN_ALLOC_typedef(stralloc,char,s,len,a)

int stralloc_ready();
int stralloc_readyplus();
int stralloc_copy(stralloc *,stralloc *);
int stralloc_cat(stralloc *,stralloc *);
int stralloc_copys(stralloc *,char *);
int stralloc_cats(stralloc *,char *);
int stralloc_copyb(stralloc *,char *,unsigned int);
int stralloc_catb(stralloc *,char *,unsigned int);
int stralloc_append(); /* beware: this takes a pointer to 1 char */
int stralloc_starts(stralloc *,char *);

#define stralloc_0(sa) stralloc_append(sa,"")

#endif
