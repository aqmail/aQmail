#include "fmt.h"
#include "scan.h"
#include "ip.h"
#include "byte.h"
#include "str.h"
#include "stralloc.h"

/**
 @file ip.c
 @brief IPv4 functions: ip4_fmt, ip4_scan, ip4_scanbracket
        IPv6 functions: ip6_fmt, ip6_scan, ip6_scanbracket
        conversion functions: ip4_bitstring, ip6_bitstring, intfromhex
 */

/**
 @brief ip4_fmt
        converts IPv4 address to dotted decimal string format
 @param input:  pointer to IPv4 ip_address struct
        output: pointer to IPv4 address string
 @return int length of address (ok > 0)
 */

unsigned int ip4_fmt(char *s,struct ip_address *ip)
{
  unsigned int len;
  unsigned int i;
 
  len = 0;
  i = fmt_ulong(s,(unsigned long) ip->d[0]); len += i; if (s) s += i;
  i = fmt_str(s,"."); len += i; if (s) s += i;
  i = fmt_ulong(s,(unsigned long) ip->d[1]); len += i; if (s) s += i;
  i = fmt_str(s,"."); len += i; if (s) s += i;
  i = fmt_ulong(s,(unsigned long) ip->d[2]); len += i; if (s) s += i;
  i = fmt_str(s,"."); len += i; if (s) s += i;
  i = fmt_ulong(s,(unsigned long) ip->d[3]); len += i; if (s) s += i;
  return len;
}

/**
 @brief ip4_scan
        parse IPv4 address string and convert to ip_address
 @param input:  pointer to IPv4 address string
        output: pointer to IPv4 ip_address struct
 @return int lenth of ip_address (ok > 0)
 */

unsigned int ip4_scan(struct ip_address *ip,char *s)
{
  unsigned int i;
  unsigned int len;
  unsigned long u;
 
  len = 0;
  i = scan_ulong(s,&u); if (!i) return 0; if (u > 255) return 0; ip->d[0] = u; s += i; len += i;
  if (*s != '.') return 0; ++s; ++len;
  i = scan_ulong(s,&u); if (!i) return 0; if (u > 255) return 0; ip->d[1] = u; s += i; len += i;
  if (*s != '.') return 0; ++s; ++len;
  i = scan_ulong(s,&u); if (!i) return 0; if (u > 255) return 0; ip->d[2] = u; s += i; len += i;
  if (*s != '.') return 0; ++s; ++len;
  i = scan_ulong(s,&u); if (!i) return 0; if (u > 255) return 0; ip->d[3] = u; s += i; len += i;
  return len;
}

/**
 @brief ip4_scanbracket
        parse IPv4 address string enclosed in brackets and convert to ip_address struct
 @param input:  pointer to IPv4 address string
        output: pointer to IPv4 ip_address struct
 @return int lenth of ip_address (ok > 0)
 */

unsigned int ip4_scanbracket(struct ip_address *ip,char *s)
{
  unsigned int len;
 
  if (*s != '[') return 0;
  len = ip4_scan(ip,s + 1);
  if (!len) return 0;
  if (s[len + 1] != ']') return 0;
  return len + 2;
}

/**
 @brief ip6_fmt
        convert IPv6 address to compactified IPv6 address string
 @param input:  pointer to IPv6 ip6_address struct
        output: pointer to IPv6 address string
 @return int length of address (ok > 0)
 */

unsigned int ip6_fmt(char *s,struct ip6_address *ip)
{
  unsigned int len;
  unsigned int i;
  unsigned int temp, temp2;
  unsigned int compressing;
  unsigned int compressed;
  int j;
  struct ip_address ip4;

  len = 0; 
  compressing = 0; 
  compressed = 0;

  for (j = 0; j < 16; j += 2) {
    if (j == 12 && byte_equal(ip,12,V4mappedprefix)) {
      for (i = 0; i < 4; i++) {
        ip4.d[i] = ip->d[i+12];
      }
      len += ip4_fmt(s,&ip4);
      break;
    }

    temp = ((unsigned long) (unsigned char) ip->d[j] << 8) +
            (unsigned long) (unsigned char) ip->d[j+1];	

    temp2 = 0;			/* test the following octet [ip.h - 3.b)] */
    if (!compressing && j < 16)
      temp2 = ((unsigned long) (unsigned char) ip->d[j+2] << 8) +
               (unsigned long) (unsigned char) ip->d[j+3];	
   
    if (temp == 0 && temp2 == 0 && !compressed) {
      if (!compressing) {
	compressing = 1;
	if (j == 0) {
	  if (s) *s++=':'; 
          ++len;
	}
      }
    } else {
      if (compressing) {
	compressing = 0; 
        ++compressed;
	if (s) *s++ = ':'; 
        ++len;
      }
      i = fmt_xlong(s,temp);
      len += i; 
      if (s) s += i;
      if (j < 14) {
	if (s) *s++ = ':';
	++len;
      }
    }
  }
  if (compressing) { *s++ = ':'; ++len; }

  if (s) *s = 0; 
  return len;
}

/**
 @brief ip6_scan 
        parse compactified IPv6 address string and convert to ip6_address struct
 @param input:  pointer to IPv6 address string
        output: pointer to ip6_address struct
 @return int length of ip6_address/ip 
 */

unsigned int ip6_scan(struct ip6_address *ip,char *s)
{
  unsigned int i;
  unsigned int len = 0;
  unsigned long u;
  char suffix[16];
  int prefixlen = 0;
  int suffixlen = 0;

  unsigned int x;
  struct ip_address ip4;

  for (x = 0;x < 4;x++) {	/* Mapped IPv4 addresses */
    ip4.d[x] = ip->d[x+12];
  }

  if ((i = ip4_scan(&ip4,s))) {
    const char *c = V4mappedprefix;
    if (byte_equal(ip4.d,4,V6any)) c = V6any;
    for (len = 0;len < 12;++len) 
      ip->d[len] = c[len];
    return i;
  }

  for (i = 0;i < 16;i++) 
   ip->d[i] = 0;

  for (;;) {
    if (*s == ':') {
      len++;
      if (s[1] == ':') {	/* Found "::", skip to part 2 */
        s+=2;
        len++;
        break;
      }
      s++;
    }
    i = scan_xlong(s,&u);
    if (!i) return 0;

    if (prefixlen == 12 && s[i] == '.') {
      /* the last 4 bytes may be written as IPv4 address */
      i = ip4_scan(&ip4,s);
      if (i) {
        /* copy into ip->d+12 from ip4 */
        for (x = 0;x < 4;x++) {
          ip->d[x+12] = ip4.d[x];
        }
        return i+len;
      } else
        return 0;
    }
    ip->d[prefixlen++] = (u >> 8);
    ip->d[prefixlen++] = (u & 255);
    s += i; len += i;

    if (prefixlen == 16)
      return len;
  }

/* part 2, after "::" */
  for (;;) {
    if (*s == ':') {
      if (suffixlen == 0)
	break;
      s++;
      len++;
    } else if (suffixlen != 0)
      break;
    i = scan_xlong(s,&u);
    if (!i) {
      len--;
      break;
    }
    if (suffixlen+prefixlen <= 12 && s[i] == '.') {
      int j = ip4_scan(&ip4,s);
      if (j) {
        byte_copy(suffix+suffixlen,4,ip4.d);
	suffixlen += 4;
	len += j;
	break;
      } else
        prefixlen = 12-suffixlen; /* make end-of-loop test true */
    }
    suffix[suffixlen++] = (u >> 8);
    suffix[suffixlen++] = (u & 255);
    s += i; len += i;
    if (prefixlen+suffixlen == 16)
      break;
  }
  for (i = 0;i < suffixlen;i++)
    ip->d[16-suffixlen+i] = suffix[i];

  return len;
}

/**
 @brief ip6_scanbracket 
        parse IPv6 string address enclosed in brackets
 @param input:  pointer to IPv6 address string
        output: pointer to IPv6 address struct
 @return int length of ip_address (ok > 0)
 */

unsigned int ip6_scanbracket(struct ip6_address *ip6,char *s)
{
  unsigned int len;
 
  if (*s != '[') return 0;
  len = ip6_scan(ip6,s + 1);
  if (!len) return 0;
  if (s[len + 1] != ']') return 0;
  return len + 2;
}

static inline int fromhex(unsigned char c) {
  if (c>='0' && c<='9')
    return c-'0';
  else if (c>='A' && c<='F')
    return c-'A'+10;
  else if (c>='a' && c<='f')
    return c-'a'+10;
  return -1;
}

/**
 @brief ip4_bitstring
        parse IPv4 address and represent as char string with length prefix
 @param input:  pointer to struct IPv4 address, prefix length
        output: pointer to stralloc bitstring
 @return 0, if ok; 1, memory shortage; 2, input error
 */

unsigned int ip4_bitstring(stralloc *ip4string,struct ip_address *ip4,int prefix)
{
  int i, j;
  unsigned char number;

  ip4string->len = 0;
  if (!stralloc_copys(ip4string,"")) return 1;
  if (!stralloc_readyplus(ip4string,32)) return 1;

  for (i = 0; i < 4; i++) { 
    number = (unsigned char) ip4->d[i];
    if (number > 255) return 2;
   
    for (j = 7; j >= 0; j--) {
      if (number & (1<<j)) {
        if (!stralloc_cats(ip4string,"1")) return 1;
      } else {
        if (!stralloc_cats(ip4string,"0")) return 1;
      }
      if (prefix == 0) {
        if (!stralloc_0(ip4string)) return 1;
        else return 0;
      }
      prefix--;
    }
  }
  if (!stralloc_0(ip4string)) return 1;

  return 0;
}

/**
 @brief ip6_bitstring
        parse IPv6 address and represent as char string with length prefix
 @param input:  pointer to struct IPv6 address, prefix length
        output: pointer to stralloc bit string;
 @return 0, if ok; 1, memory shortage
 */

unsigned int ip6_bitstring(stralloc *ip6string,struct ip6_address *ip6,int prefix)
{
  int i, j;
  unsigned char lowbyte, highbyte;

  ip6string->len = 0;
  if (!stralloc_copys(ip6string,"")) return 1;
  if (!stralloc_readyplus(ip6string,128)) return 1;

  for (i = 0; i < 16; i++) {
    lowbyte = (unsigned char) (ip6->d[i]) & 0x0f;
    highbyte = (unsigned char) (ip6->d[i] >> 4) & 0x0f;

    for (j = 3; j >= 0; j--) {
      if (highbyte & (1<<j)) {
        if (!stralloc_cats(ip6string,"1")) return 1;
      } else {
        if (!stralloc_cats(ip6string,"0")) return 1;
      }
      if (prefix == 0) {
        if (!stralloc_0(ip6string)) return 1;
        else return 0;
      }
      prefix--;
    }
    for (j = 3; j >= 0; j--) {
      if (lowbyte & (1<<j)) {
        if (!stralloc_cats(ip6string,"1")) return 1;
      } else {
        if (!stralloc_cats(ip6string,"0")) return 1;
      }
      if (prefix == 0) {
        if (!stralloc_0(ip6string)) return 1;
        else return 0;
      }
      prefix--;
    }
  } 
  if (!stralloc_0(ip6string)) return 1;

  return 0;
}
