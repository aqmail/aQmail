/*
 *  Revision 20170502, Kai Peter
 *  - consolidated files buffer.c, buffer_{0,1,2}.c
 *  - changed '&it' to '&it0', '&it1', '&it2'
 *  Revision 20160708, Kai Peter
 *  - replaced 'readwrite.h' by 'unistd.h'
*/
#include <unistd.h>
#include "buffer.h"

/* file: buffer.c **************************************************************** */
void buffer_init(buffer *s,ssize_t (*op)(),int fd,char *buf,unsigned int len)
{
  s->x = buf;
  s->fd = fd;
  s->op = op;
  s->p = 0;
  s->n = len;
}

/* file: buffer_0.c ************************************************************** */
ssize_t buffer_0_read(int fd,char *buf,int len) {
  if (buffer_flush(buffer_1) == -1) return -1;
  return read(fd,buf,len);
}

char buffer_0_space[BUFFER_INSIZE];
static buffer it0 = BUFFER_INIT(buffer_0_read,0,buffer_0_space,sizeof buffer_0_space);
buffer *buffer_0 = &it0;

/* file: buffer_0.c ************************************************************** */
char buffer_1_space[BUFFER_OUTSIZE];
static buffer it1 = BUFFER_INIT(write,1,buffer_1_space,sizeof buffer_1_space);
buffer *buffer_1 = &it1;

/* file: buffer_0.c ************************************************************** */
char buffer_2_space[256];
static buffer it2 = BUFFER_INIT(write,2,buffer_2_space,sizeof buffer_2_space);
buffer *buffer_2 = &it2;
