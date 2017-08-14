#ifndef TCPTO_H
#define TCPTO_H

#define	TCPTO_BUFSIZ 1024	

/* persistency structure: record 
struct tcpto {		
  unsigned char af;        --  1 byte -- IPv6: x'2' / IPv6: x'a' (10)
  unsigned char nul[3];    --  3 byte
  unsigned char flag;      --  1 byte -- err_timeout || err_conrefused
  unsigned char nul[3];    --  3 byte
  unsigned long when;      --  8 byte		
  union {
    struct ip_address ip;
    struct ip6_address ip6;
    unsigned char nul[16];  -- 16 byte -- IPv4: filled up with '.' = x'2e'
  } addr;		
};               total:       32 byte
*/

int tcpto();
void tcpto_err();
void tcpto_clean();

#endif
