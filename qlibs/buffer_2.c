/*
 *  Revision 20160708, Kai Peter
 *  - replaced 'readwrite.h' by 'unistd.h'
*/
#include <unistd.h>
#include "buffer.h"

char buffer_2_space[256];
static buffer it = BUFFER_INIT(write,2,buffer_2_space,sizeof buffer_2_space);
buffer *buffer_2 = &it;
