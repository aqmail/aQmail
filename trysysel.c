#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h> /* SVR4 silliness */
#include <stdio.h>
#include "select.h"

int main()
{
    printf("FD_SETSIZE:%d\n",FD_SETSIZE);
    return 0;
}
