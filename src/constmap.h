#ifndef CONSTMAP_H
#define CONSTMAP_H

typedef unsigned long constmap_hash;

struct constmap {
  int num;
  constmap_hash mask;
  constmap_hash *hash;
  int *first;
  int *next;
  char **input;
  int *inputlen;
} ;

int constmap_init(struct constmap *,char *,int,int);
int constmap_init_char(struct constmap *,char *,int,int,char);
void constmap_free();
char *constmap();

#endif
