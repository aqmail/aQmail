#ifndef QUOTE_H
#define QUOTE_H

#include "stralloc.h"

int quote_need(char *,unsigned int);
int quote(stralloc *, stralloc *);
int quote2(stralloc *,char *);

#endif
