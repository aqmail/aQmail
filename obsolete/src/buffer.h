#ifndef BUFFER_H
#define BUFFER_H

typedef struct buffer {
  char *x;
  unsigned int p;
  unsigned int n;
  int fd;
  int (*op)();
} buffer;

#define BUFFER_INIT(op,fd,buf,len) { (buf), 0, (len), (fd), (op) }
#define BUFFER_INSIZE 8192
#define BUFFER_OUTSIZE 8192

void buffer_init(buffer *,int (*)(),int,char *,unsigned int);

int buffer_flush(buffer *);
int buffer_put(buffer *,char *,unsigned int);
int bufner_putalign(buffer *,char *,unsigned int);
int buffer_putflush(buffer *,char *,unsigned int);
int buffer_puts(buffer *,char *);
int buffer_putsalign(buffer *,char *);
int buffer_putsflush(buffer *,char *);

#define buffer_PUTC(s,c) \
  ( ((s)->n != (s)->p) \
    ? ( (s)->x[(s)->p++] = (c), 0 ) \
    : buffer_put((s),&(c),1) \
  )

int buffer_get(buffer *,char *,unsigned int);
int buffer_bget(buffer *,char *,unsigned int);
int buffer_feed(buffer *);

char *buffer_peek(buffer *);
void buffer_seek(buffer *,unsigned int);

#define buffer_PEEK(s) ( (s)->x + (s)->n )
#define buffer_SEEK(s,len) ( ( (s)->p -= (len) ) , ( (s)->n += (len) ) )

#define buffer_GETC(s,c) \
  ( ((s)->p > 0) \
    ? ( *(c) = (s)->x[(s)->n], buffer_SEEK((s),1), 1 ) \
    : buffer_get((s),(c),1) \
  )

int buffer_copy(buffer *,buffer *);

extern buffer *buffer_0;
extern buffer *buffer_0small;
extern buffer *buffer_1;
extern buffer *buffer_1small;
extern buffer *buffer_2;

#endif
