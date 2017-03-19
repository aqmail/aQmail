#ifndef RECEIVED_H
#define RECEIVED_H

#include "qmail.h"

void received(struct qmail *,char *,char *,char *,char *,char *,char *,char *,char *); 
void spfheader(struct qmail *,char *,char *,char *,char *,char *);

#endif
