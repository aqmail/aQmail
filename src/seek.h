#ifndef SEEK_H
#define SEEK_H

typedef unsigned long seek_pos;

seek_pos seek_cur();

int seek_set();
int seek_end();

int seek_trunc();

#define seek_begin(fd) (seek_set((fd),(seek_pos) 0))

#endif
