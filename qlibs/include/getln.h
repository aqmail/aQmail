#ifndef GETLN_H
#define GETLN_H

#include "buffer.h"
#include "stralloc.h"

extern int getln(buffer *b,stralloc *sa,int *match,int sep);
extern int sgetln(buffer *b,stralloc *sa,char **cont,unsigned int *clen,int sep);

#endif
