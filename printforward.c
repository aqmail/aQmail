/*
 *  Revision 20170914, Kai Peter
 *  - added function 'cdb_unpack()'
 *  - switched to 'buffer'
*/
#include "buffer.h"
#include "strerr.h"
#include "stralloc.h"
#include "exit.h"
#include "uint_t.h"

#define FATAL "printforward: fatal: "

/* this function comes from a quite early cdb lib */
uint32 cdb_unpack(unsigned char *buf)
{
  uint32 num;
  num = buf[3]; num <<= 8;
  num += buf[2]; num <<= 8;
  num += buf[1]; num <<= 8;
  num += buf[0];
  return num;
}

void badformat()
{
  strerr_die2x(100,FATAL,"bad database format");
}
void nomem()
{
  strerr_die2x(111,FATAL,"out of memory");
}

void getch(ch)
char *ch;
{
  int r;
//  r = substdio_get(subfdinsmall,ch,1);
  r = buffer_get(buffer_0,ch,1);
  if (r == -1)
    strerr_die2sys(111,FATAL,"unable to read input: ");
  if (r == 0)
    badformat();
}

void putch(ch)
char *ch;
{
//  if (substdio_put(subfdoutsmall,ch,1) == -1)
  if (buffer_put(buffer_1,ch,1) == -1)
    strerr_die2x(111,FATAL,"unable to write output: ");
}

void print(buf)
char *buf;
{
  while (*buf)
    putch(buf++);
}

void printsafe(buf,len)
char *buf;
int len;
{
  char ch;

  while (len) {
    ch = *buf;
    if ((ch <= 32) || (ch == ',') || (ch == ':') || (ch == ';') || (ch == '\\') || (ch == '#'))
      putch("\\");
    putch(&ch);
    ++buf;
    --len;
  }
}

stralloc key = {0};
stralloc data = {0};

int main()
{
  uint32 eod;
  uint32 pos;
  uint32 klen;
  uint32 dlen;
  char buf[8];
  char ch;
  int i;
  int j;

  for (i = 0;i < 4;++i) getch(buf + i);
  eod = cdb_unpack((unsigned char *)buf);

  for (i = 4;i < 2048;++i) getch(&ch);

  pos = 2048;
  while (pos < eod) {
    if (eod - pos < 8) badformat();
    pos += 8;
    for (i = 0;i < 8;++i) getch(buf + i);
    klen = cdb_unpack((unsigned char *)buf);
    dlen = cdb_unpack((unsigned char *)buf + 4);

    if (!stralloc_copys(&key,"")) nomem();
    if (eod - pos < klen) badformat();
    pos += klen;
    while (klen) {
      --klen;
      getch(&ch);
      if (!stralloc_append(&key,&ch)) nomem();
    }

    if (eod - pos < dlen) badformat();
    pos += dlen;
    if (!stralloc_copys(&data,"")) nomem();
    while (dlen) {
      --dlen;
      getch(&ch);
      if (!stralloc_append(&data,&ch)) nomem();
    }

    if (!key.len) badformat();
    if (key.s[0] == '?') {
      printsafe(key.s + 1,key.len - 1);
      print(": ?");
      printsafe(data.s,data.len);
      print(";\n");
    }
    else if (key.s[0] == ':') {
      printsafe(key.s + 1,key.len - 1);
      print(":\n");

      i = 0;
      for (j = 0;j < data.len;++j)
        if (!data.s[j]) {
          if ((data.s[i] == '.') || (data.s[i] == '/')) {
            print(", ");
            printsafe(data.s + i,j - i);
            print("\n");
          }
          else if ((data.s[i] == '|') || (data.s[i] == '!')) {
            print(", ");
            printsafe(data.s + i,j - i);
            print("\n");
          }
          else if ((data.s[i] == '&') && (j - i < 900)) {
            print(", ");
            printsafe(data.s + i,j - i);
            print("\n");
          }
          else badformat();
          i = j + 1;
        }
      if (i != j) badformat();
      print(";\n");
    }
    else badformat();
  }

//  if (substdio_flush(subfdoutsmall) == -1)
  if (buffer_flush(buffer_1) == -1)
    strerr_die2sys(111,FATAL,"unable to write output: ");
  _exit(0);
}
