#ifndef READSUBDIR_H
#define READSUBDIR_H

#include "direntry.h"

typedef struct readsubdir
 {
  DIR *dir;
  int pos;
  char *name;
  void (*pause)();
 }
readsubdir;

void readsubdir_init();
int readsubdir_next();

#define READSUBDIR_NAMELEN 10

#endif
