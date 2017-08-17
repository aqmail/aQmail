/* 
 *  Revision 20170504, Kai Peter
 *  - replaced 'GEN_ALLOC_{ready,readyplus,append}' by functions from libowfat
*/
#include "byte.h"
#include "str.h"
#include "stralloc.h"
#include "alloc.h"
#include <stdlib.h>
/* Consolidate the "stralloc_*.c" functions into one source file. Original
   files shipped with qmail-1.03.
   These functions will be linked to "stralloc.a" only!
   Included files:     Size (bytes)    Date
     - stralloc_arts.c         196     19980615
     - stralloc_cat.c          161     19980615
     - stralloc_catb.c         320     19980615
     - stralloc_cats.c         150     19980615
     - stralloc_copy.c         163     19980615
     - stralloc_eady.c         203     19980615
     - stralloc_opyb.c         255     19980615
     - stralloc_opys.c         152     19980615
     - stralloc_pend.c         153     19980615                         */

/* file: stralloc_arts.c ********************************************** */
int stralloc_starts(stralloc *sa,const char *s) {
  int len;
  len = str_len(s);
  return (sa->len >= len) && byte_equal(s,len,sa->s);
}
/* file: stralloc_cat.c *********************************************** */
int stralloc_cat(stralloc *sato,stralloc *safrom) {
  return stralloc_catb(sato,safrom->s,safrom->len);
}
/* file: stralloc_catb.c ********************************************** */
int stralloc_catb(stralloc *sa,const char *s,unsigned int n) {
  if (!sa->s) return stralloc_copyb(sa,s,n);
  if (!stralloc_readyplus(sa,n + 1)) return 0;
  byte_copy(sa->s + sa->len,n,s);
  sa->len += n;
  sa->s[sa->len] = 'Z'; /* ``offensive programming'' */
  return 1;
}
/* file: stralloc_cats.c ********************************************** */
int stralloc_cats(stralloc *sa,const char *s) {
  return stralloc_catb(sa,s,str_len(s));
}
/* file: stralloc_copy.c ********************************************** */
int stralloc_copy(stralloc *sato,stralloc *safrom) {
  return stralloc_copyb(sato,safrom->s,safrom->len);
}

/* file: stralloc_eady.c ********************************************** */
//GEN_ALLOC_ready(stralloc,char,s,len,a,i,n,x,30,stralloc_ready)
//GEN_ALLOC_readyplus(stralloc,char,s,len,a,i,n,x,30,stralloc_readyplus)
/* replacements from libowfat: */

/* stralloc_ready makes sure that sa has enough space allocated to hold
 * len bytes: If sa is not allocated, stralloc_ready allocates at least
 * len bytes of space, and returns 1. If sa is already allocated, but
 * not enough to hold len bytes, stralloc_ready allocates at least len
 * bytes of space, copies the old string into the new space, frees the
 * old space, and returns 1. Note that this changes sa.s. */
int stralloc_ready(stralloc *sa,size_t len) {
  register size_t wanted=len+(len>>3)+30; /* heuristic from djb */
  if (wanted<len) wanted=len;
  if (!sa->s || sa->a<len) {
    register char* tmp;
    if (!(tmp=realloc(sa->s,wanted)))    // !!! needs stdlib (realloc)
      return 0;
    sa->a=wanted;
    sa->s=tmp;
  }
  return 1;
}
/* stralloc_readyplus is like stralloc_ready except that, if sa is already
 * allocated, stralloc_readyplus adds the current length of sa to len.  */
int stralloc_readyplus(stralloc *sa,size_t len) {
  if (sa->s) {
    if (sa->len + len < len) return 0;  /* catch integer overflow */
    return stralloc_ready(sa,sa->len+len);
  } else
    return stralloc_ready(sa,len);
}

/* file: stralloc_opyb.c ********************************************** */
int stralloc_copyb(stralloc *sa,const char *s,unsigned int n) {
  if (!stralloc_ready(sa,n + 1)) return 0;
  byte_copy(sa->s,n,s);
  sa->len = n;
  sa->s[n] = 'Z'; /* ``offensive programming'' */
  return 1;
}
/* *** file: stralloc_opys.c ****************************************** */
int stralloc_copys(stralloc *sa,const char *s) {
  return stralloc_copyb(sa,s,str_len(s));
}
/* file: stralloc_pend.c ********************************************** */
//GEN_ALLOC_append(stralloc,char,s,len,a,i,n,x,30,stralloc_readyplus,stralloc_append)
/* replacement from libowfat: */
/* stralloc_append adds one byte in[0] to the end of the string stored
 * in sa. It is the same as stralloc_catb(&sa,in,1). */
int stralloc_append(stralloc *sa,const char *in) {
  if (stralloc_readyplus(sa,1)) {
    sa->s[sa->len]=*in;
    ++sa->len;
    return 1;
  }
  return 0;
}
