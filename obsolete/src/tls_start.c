#include "scan.h"
#include "env.h"
#include "open.h"
#include "stralloc.h"

int starttls_init(void)
{
  unsigned long fd;
  char *fdstr;
   
  if (!(fdstr=env_get("SSLCTLFD")))
    return 0;
  if (!scan_ulong(fdstr,&fd))
    return 0;
  if (write((int)fd, "Y", 1) < 1)
    return 0;
 
  if (!(fdstr=env_get("SSLREADFD")))
    return 0;
  if (!scan_ulong(fdstr,&fd))
    return 0;
  if (dup2((int)fd,0) == -1)
    return 0;
 
  if (!(fdstr=env_get("SSLWRITEFD")))
    return 0; 
  if (!scan_ulong(fdstr,&fd))
    return 0;
  if (dup2((int)fd,1) == -1)
    return 0;
 
  return 1;
}

int starttls_info(void)
{
  unsigned long fd;
  char *fdstr;
  char envbuf[8192];
  char *x;
  int j;

  stralloc ssl_env   = {0};
  stralloc ssl_parm  = {0};
  stralloc ssl_value = {0};

  if (!(fdstr=env_get("SSLCTLFD")))
    return 0;
  if (!scan_ulong(fdstr,&fd))
    return 0;

  while ((j=read(fd,envbuf,8192)) > 0 ) {
    stralloc_catb(&ssl_env,envbuf,j);
      if (ssl_env.len >= 2 && ssl_env.s[ssl_env.len-2]==0 && ssl_env.s[ssl_env.len-1]==0)
        break;
  }
  if (j <= 0)
    die_nomem();

  x = ssl_env.s; 

  for (j=0;j < ssl_env.len-1;++j) {
    if ( *x != '=' ) {
      if (!stralloc_catb(&ssl_parm,x,1)) die_nomem();
      x++; }
    else {
      if (!stralloc_0(&ssl_parm)) die_nomem();
      x++;

      for (;j < ssl_env.len-j-1;++j) {
        if ( *x != '\0' ) {
          if (!stralloc_catb(&ssl_value,x,1)) die_nomem();
          x++; }
        else {
          if (!stralloc_0(&ssl_value)) die_nomem();
          x++;
          if (!env_put2(ssl_parm.s,ssl_value.s)) die_nomem();
          ssl_parm.len = 0; 
          ssl_value.len = 0;
          break; 
        }
      }
    }
  }
  return j;
}
