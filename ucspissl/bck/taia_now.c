/* Public domain. */

#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
//#include "taia.h"
#include "taia2.h"

int taia_now(struct taia *t)
{
  struct timeval now;

  if (gettimeofday(&now,(struct timezone *) 0) == 0) {
    tai_unix(&t->sec,now.tv_sec);
    t->nano = 1000 * now.tv_usec + 500;
    t->atto = 0;
    return 0;
  } 
  t->nano = 0;
  t->atto = 0;
  return -1;
}
