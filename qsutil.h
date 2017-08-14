#ifndef QSUTIL_H
#define QSUTIL_H

#include "stralloc.h"

void log1s(char *);
void log2s(char *,char *);
void log3s(char *,char *,char *);
void log4s(char *,char *,char *,char *);
void log5s(char *,char *,char *,char *,char *);
void logsa(stralloc *);
void nomem();
void pausedir(char *);
void logsafe(char *);
int issafe(char);

#endif
