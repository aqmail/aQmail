/*
 *  Revision 20160708, Kai Peter
 *  - replaced 'readwrite.h' by 'unistd.h'
*/
#include <unistd.h>
#include "buffer.h"

ssize_t buffer_0_read(fd,buf,len) int fd; char *buf; int len;
{
  if (buffer_flush(buffer_1) == -1) return -1;
  return read(fd,buf,len);
}

char buffer_0_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT(buffer_0_read,0,buffer_0_space,sizeof buffer_0_space);
buffer *buffer_0 = &it;
