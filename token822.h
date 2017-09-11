#ifndef TOKEN822_H
#define TOKEN822_H

struct token822
 {
  int type;
  char *s;
  int slen;
 }
;

#include "stralloc.h"
GEN_ALLOC_typedef(token822_alloc,struct token822,t,len,a)

int token822_parse();
int token822_addrlist();
int token822_unquote();
int token822_unparse();
void token822_free();
void token822_reverse();
int token822_ready();
int token822_readyplus();
int token822_append();

#define TOKEN822_ATOM 1
#define TOKEN822_QUOTE 2
#define TOKEN822_LITERAL 3
#define TOKEN822_COMMENT 4
#define TOKEN822_LEFT 5
#define TOKEN822_RIGHT 6
#define TOKEN822_AT 7
#define TOKEN822_COMMA 8
#define TOKEN822_SEMI 9
#define TOKEN822_COLON 10
#define TOKEN822_DOT 11

#endif
