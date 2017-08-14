#ifndef SUBFD_H
#define SUBFD_H

#include "substdio.h"

extern substdio *subfdin;
extern substdio *subfdinsmall;
extern substdio *subfdout;
extern substdio *subfdoutsmall;
extern substdio *subfderr;

int subfd_read();
int subfd_readsmall();

#endif
