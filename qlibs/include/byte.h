#ifndef BYTE_H
#define BYTE_H

extern unsigned int byte_chr();
extern unsigned int byte_rchr();
extern void byte_copy();
extern void byte_copyr();
extern int byte_diff();
extern void byte_zero();

#define byte_equal(s,n,t) (!byte_diff((s),(n),(t)))

/* better declarations would be:
extern unsigned int byte_chr(char *,unsigned int,int);
extern unsigned int byte_rchr(char *,unsigned int,int);
extern void byte_copy(char *,unsigned int,char *);
extern void byte_copyr(char *,unsigned int,char *);
extern int byte_diff(char *,unsigned int,char *);
extern void byte_zero(char *,unsigned int);

--> but this produces a lot of warnings !!!
*/


#endif
