#ifndef STRSALLOC_H
#define STRSALLOC_H

#include "stralloc.h"
#include "gen_alloc.h"

GEN_ALLOC_typedef(strsalloc,stralloc,sa,len,a)
int strsalloc_readyplus();
int strsalloc_append();

#endif
