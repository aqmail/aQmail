#ifndef SCAN_H
#define SCAN_H

unsigned int scan_uint();
unsigned int scan_xint();
unsigned int scan_nbbint();
unsigned int scan_ushort();
unsigned int scan_xshort();
unsigned int scan_nbbshort();
unsigned int scan_ulong(register char *,register unsigned long *);
unsigned int scan_xlong(const char *,unsigned long *);
unsigned int scan_nbblong();

unsigned int scan_plusminus();
unsigned int scan_0x();

unsigned int scan_whitenskip();
unsigned int scan_nonwhitenskip();
unsigned int scan_charsetnskip();
unsigned int scan_noncharsetnskip();

unsigned int scan_strncmp();
unsigned int scan_memcmp();

unsigned int scan_long();

#endif
