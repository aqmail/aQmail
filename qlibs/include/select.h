#ifndef SELECT_H
#define SELECT_H

#include <sys/types.h>
#include <sys/time.h>

#ifdef HAS_SELECT_H
#include <sys/select.h>
#endif

extern int select();

#endif
