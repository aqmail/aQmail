#include "case.h"
#include "str.h"

int case_diffrs(register char *s,register char *t)
{
  register unsigned char x;
  register unsigned char y;
  unsigned int lens = str_len(s); 
  unsigned int lent = str_len(t); 

  while (lens > 0 && lent > 0) {
    x = s[--lens] - 'A';
    if (x <= 'Z' - 'A') x += 'a'; else x += 'A';
    y = t[--lent] - 'A';
    if (y <= 'Z' - 'A') y += 'a'; else y += 'A';
    if (x != y) break;
    if (!x) break;
    if (!y) break;
  }
  return ((int)(unsigned int) x) - ((int)(unsigned int) y);
}
