/* Public domain. */

#ifndef TAI_H
#define TAI_H

#include "uint64.h"

struct tai {
  uint64 x;
} ;

#define tai_unix(t,u) ((void) ((t)->x = 4611686018427387914ULL + (uint64) (u)))

void tai_now(struct tai *);

#define tai_approx(t) ((double) ((t)->x))

void tai_add(struct tai *,const struct tai *,const struct tai *);
void tai_sub(struct tai *,const struct tai *,const struct tai *);
#define tai_less(t,u) ((t)->x < (u)->x)
#define tai_equal(t,u) ((t)->x == (u)->x)

#define TAI_PACK 8
void tai_pack(char *,const struct tai *);
void tai_unpack(const char *,struct tai *);

void tai_uint(struct tai *,unsigned int);

#endif
