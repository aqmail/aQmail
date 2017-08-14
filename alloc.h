#ifndef ALLOC_H
#define ALLOC_H

extern /*@null@*//*@out@*/char *alloc();
void alloc_free(char *);
int alloc_re(char **,unsigned int,unsigned int);

#endif
