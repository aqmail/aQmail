#ifndef FMT_H
#define FMT_H

#define FMT_ULONG 40 /* enough space to hold 2^128 - 1 in decimal, plus \0 */
#define FMT_LEN ((char *) 0) /* convenient abbreviation */

unsigned int fmt_uint(char *,unsigned int);
unsigned int fmt_uint0(char *,unsigned int, unsigned int);
unsigned int fmt_xint();
unsigned int fmt_nbbint();
unsigned int fmt_ushort();
unsigned int fmt_xshort();
unsigned int fmt_nbbshort();
unsigned int fmt_ulong(char *,unsigned long);
unsigned int fmt_xlong(char *,unsigned long);
unsigned int fmt_nbblong();

unsigned int fmt_plusminus();
unsigned int fmt_minus();
unsigned int fmt_0x();

unsigned int fmt_str(char *,char *);
unsigned int fmt_strn(char *,char *,unsigned int);

#endif
