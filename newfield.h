#ifndef NEWFIELD_H
#define NEWFIELD_H

#include "stralloc.h"

extern stralloc newfield_date;
int newfield_datemake();

extern stralloc newfield_msgid;
int newfield_msgidmake();

#endif
