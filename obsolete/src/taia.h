/* Public domain. */

#ifndef TAIA_H
#define TAIA_H

#include "tai.h"

struct taia {
  struct tai sec;
  unsigned long nano; /* 0...999999999 */
  unsigned long atto; /* 0...999999999 */
} ;

void taia_tai(const struct taia *,struct tai *);

void taia_now(struct taia *);

double taia_approx(const struct taia *);
double taia_frac(const struct taia *);

void taia_add(struct taia *,const struct taia *,const struct taia *);
void taia_addsec(struct taia *,const struct taia *,int);
void taia_sub(struct taia *,const struct taia *,const struct taia *);
void taia_half(struct taia *,const struct taia *);
int taia_less(const struct taia *,const struct taia *);

#define TAIA_PACK 16
void taia_pack(char *,const struct taia *);
void taia_unpack(const char *,struct taia *);

#define TAIA_FMTFRAC 19
unsigned int taia_fmtfrac(char *,const struct taia *);

void taia_uint(struct taia *,unsigned int);

#endif
