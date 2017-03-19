#ifndef SUBSTDIO_H
#define SUBSTDIO_H

typedef struct substdio {
  char *x;
  int p;
  int n;
  int fd;
  int (*op)();
} substdio;

#define SUBSTDIO_FDBUF(op,fd,buf,len) { (buf), 0, (len), (fd), (op) }

void substdio_fdbuf();

int substdio_flush();
int substdio_put();
int substdio_bput();
int substdio_putflush();
int substdio_puts();
int substdio_bputs();
int substdio_putsflush();

int substdio_get();
int substdio_bget();
int substdio_feed();

char *substdio_peek();
void substdio_seek();

#define substdio_fileno(s) ((s)->fd)

#define SUBSTDIO_INSIZE 8192
#define SUBSTDIO_OUTSIZE 8192

#define substdio_PEEK(s) ( (s)->x + (s)->n )
#define substdio_SEEK(s,len) ( ( (s)->p -= (len) ) , ( (s)->n += (len) ) )

#define substdio_BPUTC(s,c) \
  ( ((s)->n != (s)->p) \
    ? ( (s)->x[(s)->p++] = (c), 0 ) \
    : substdio_bput((s),&(c),1) \
  )

int substdio_copy();

#endif
