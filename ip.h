#ifndef IP_H
#define IP_H

#include "uint32.h"
#include "stralloc.h"

#define IPFMT 40
#define V4MAPPREFIX "::ffff:"
#define HOSTNAMELEN 1025

struct ip_address { unsigned char d[4]; };	/* 4 decimal pieces */
struct ip6_address { unsigned char d[16]; };	/* 16 hex pieces */

unsigned int ip4_fmt(char *,struct ip_address *);
unsigned int ip4_scan(struct ip_address *,char *);
unsigned int ip4_scanbracket(struct ip_address *,char *);
unsigned int ip4_bitstring(stralloc *,struct ip_address *,int);

const static char ip4loopback[4] = {127,0,0,1};

/**
 * Compactified IPv6 addresses are really ugly to parse.
 * Syntax: (h = hex digit) [RFC 5952]
 *   1. hhhh:hhhh:hhhh:hhhh:hhhh:hhhh:hhhh:hhhh 
 *   2. leading 0s in any octet can be suppressed
 *   3. any number of 0000 may be abbreviated as "::" 
 *      a) but only once; 
 *      b) the longest piece or on equal match the first piece,
 *      c) a single instance of 0000 as to displayed as 0
 *   4. The last two words may be written as IPv4 address
 *
 * Flat ip6 address syntax:
 *   hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh (32 hex digits)
 *
 * struct ip6_address format:
 *   cccccccccccccccc (16 chars; d[16]) -- each char swallows two hex values
 *
 * Bitstring representation with length prefix: 
 *   bbbbbbb.........bbbb (max 128 bits); stralloc(ip6string); b = 0 || 1
 *
 */

unsigned int ip6_fmt(char *,struct ip6_address *);
unsigned int ip6_scan(struct ip6_address *,char *);
unsigned int ip6_scanbracket(struct ip6_address *,char *);
unsigned int ip6_bitstring(stralloc *,struct ip6_address *,int);

const static unsigned char V4mappedprefix[12]={0,0,0,0,0,0,0,0,0,0,0xff,0xff};
const static unsigned char V6loopback[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
const static unsigned char V6any[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

#endif
