#ifndef PRIOQ_H
#define PRIOQ_H

#include "datetime.h"
#include "stralloc.h"

struct prioq_elt { datetime_sec dt; unsigned long id; } ;

GEN_ALLOC_typedef(prioq,struct prioq_elt,p,len,a)

int prioq_insert();
int prioq_min();
void prioq_delmin();

#endif
