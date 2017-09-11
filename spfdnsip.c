#include "qlibs/include/stralloc.h"
#include "strsalloc.h"
#include "qlibs/include/alloc.h"
#include "ip.h"
#include "ipalloc.h"
#include "ipme.h"
#include "qlibs/include/str.h"
#include "qlibs/include/fmt.h"
#include "scan.h"
#include "qlibs/include/byte.h"
#include "now.h"
#include "dns.h"
#include "qlibs/include/case.h"
#include "spf.h"

extern stralloc dnsname;
extern struct ip_address ip4remote;
extern struct ip6_address ip6remote;
extern int flagip6;

/**
 @brief  match_ip
         compares IPv4/IPv6 addreses up to prefix length
 @param  input:  ip_address1,prefix length, ip_address2
 @return 1 ok; 0 failure
 */

int match_ip4(struct ip_address *ip1,int prefix,struct ip_address *ip2)
{
  stralloc iptest1 = {0};
  stralloc iptest2 = {0};

  if (flagip6) return 0;  

   if (!ip4_bitstring(&iptest1,ip1,prefix)) 
     if (!ip4_bitstring(&iptest2,ip2,prefix)) {
       if (byte_diff(iptest1.s,prefix,iptest2.s)) return 0;
      }

  return 1;
}

int match_ip6(struct ip6_address *ip1,int prefix,struct ip6_address *ip2)
{
  stralloc iptest1 = {0};
  stralloc iptest2 = {0};

  if (!flagip6) return 0;

  if (!ip6_bitstring(&iptest1,ip1,prefix))
    if (!ip6_bitstring(&iptest2,ip2,prefix)) {
      if (byte_diff(iptest1.s,prefix,iptest2.s)) return 0;
  }

  return 1;
}

/**
 @brief  get_prefix
         return integer value of prefix length
 @param  input:  pointer to prefix
 @return (int) length of prefix
 */

int get_prefix(char *prefix)
{
  unsigned int r;
  int pos;

  if (!prefix) {
    if (flagip6 == 0) return 32;
    if (flagip6 == 1) return 128;
  }

  pos = scan_ulong(prefix,&r);
  if (!pos || (prefix[pos] && !(prefix[pos] == '/'))) return SPF_SYNTAX;
  if (flagip6 == 0 && r > 32) return SPF_SYNTAX;
  if (flagip6 == 1 && r > 128) return SPF_SYNTAX;

  return r;
}

/* DNS Record:  -------------------------------------- Fetch multiple SPF TXT RRs */

/**
 @brief  spf_records
         get TXT records for domain
 @param  input:  pointer stralloc domain
         output: pointer to stralloc spf records
 @return SPF_OK, SPF_NONE; SPF_MULTIRR, SPF_DNSSOFT, SPF_NOMEM
 */

int spf_records(stralloc *spf,stralloc *domain)
{
  strsalloc ssa = {0};
  int begin, pos, i, j;
  int r = SPF_NONE;

  spf->len = 0;

  switch(dns_txt(&ssa,domain)) {
    case DNS_MEM:  return SPF_NOMEM;
    case DNS_SOFT: return SPF_DNSSOFT; /* return 2main */
    case DNS_HARD: return SPF_NONE; break;
  }

  for (j = 0; j < ssa.len; ++j) {
    pos = 0;
    NXTOK(begin,pos,&ssa.sa[j]);

    if (str_len(ssa.sa[j].s + begin) < 6) continue;
    if (!byte_equal(ssa.sa[j].s + begin,6,"v=spf1")) continue;
    if (ssa.sa[j].s[begin + 6]) {
      /* check for minor version */
      if (ssa.sa[j].s[begin + 6] != '.') continue;

      for (i = begin + 7;; ++i)
        if (!(ssa.sa[j].s[i] >= '0' && ssa.sa[j].s[i] <= '9')) break;

      if (i == (begin + 7)) continue;
      if (ssa.sa[j].s[i]) continue;
    }

    if (spf->len > 0) {
      spf->len = 0;
      r = SPF_MULTIRR; /* return 2main */
      break;
    }
    if (!stralloc_0(&ssa.sa[j])) return SPF_NOMEM;
    if (!stralloc_copys(spf,ssa.sa[j].s + pos)) return SPF_NOMEM;
    r = SPF_OK;
  }

  return r;
}

/* Mechanisms:  -------------------------------------- Lookup functions */

/**
 @brief  spf_a  (a; a:fqdns; a:fqdns/56) 
         compares A + AAAA records for SPF info and client host
 @param  input:  pointer to spfspecification, pointer to prefix 
 @return SPF_OK, SPF_NONE; SPF_DNSSOFT, SPF_NOMEM
 */

int spf_a(char *spfspec,char *prefix)
{
  stralloc sa = {0};
  ipalloc ia = {0};
  int ipprefix, r, q, j;

  ipprefix = get_prefix(prefix);
  if (ipprefix < 0) return SPF_SYNTAX;

  if (!stralloc_copys(&sa,spfspec)) return SPF_NOMEM;
  if (!stralloc_readyplus(&ia,0)) return SPF_NOMEM;
  if (!spf_info("MA/AAAA=",spfspec)) return SPF_NOMEM; 

  switch (dns_ip(&ia,&sa)) {
    case DNS_MEM:  return SPF_NOMEM;
    case DNS_SOFT: r = SPF_DNSSOFT; break;
    case DNS_HARD: r = SPF_NONE; break;
    default:
      r = SPF_NONE;
      for (j = 0; j < ia.len; ++j) {
        if (flagip6) {
          q = match_ip6(&ia.ix[j].addr.ip6,ipprefix,&ip6remote);
        } else {
          q = match_ip4(&ia.ix[j].addr.ip,ipprefix,&ip4remote);
        }
        if (q) { r = SPF_OK; break; }
      }
  }

  return r;
}

/**
 @brief  spf_mx (mx; mx:domain; mx:domain/24)
         compares MX records for SPF info and client host
 @param  input:  pointer to spfspecification, pointer to prefix 
 @return SPF_OK, SPF_NONE; SPF_DNSSOFT, SPF_NOMEM
 */

int spf_mx(char *spfspec,char *prefix)
{
  stralloc sa = {0};
  ipalloc ia = {0};
  int ipprefix, random, i, j;
  int r, q;

  ipprefix = get_prefix(prefix);
  if (ipprefix < 0) return SPF_SYNTAX;

  random = now() + (getpid() << 16);

  if (!stralloc_copys(&sa,spfspec)) return SPF_NOMEM;
  if (!stralloc_readyplus(&ia,0)) return SPF_NOMEM;
  if (!spf_info("MMX=",spfspec)) return SPF_NOMEM; 

  switch(dns_mxip(&ia,&sa,random)) {
    case DNS_MEM:  return SPF_NOMEM;
    case DNS_SOFT: r = SPF_DNSSOFT; break;
    case DNS_HARD: r = SPF_NONE; break;
    default:
      r = SPF_NONE;
      for (j = 0; j < ia.len; ++j) {
        if (flagip6) {
          q = match_ip6(&ia.ix[j].addr.ip6,ipprefix,&ip6remote);
        } else {
          q = match_ip4(&ia.ix[j].addr.ip,ipprefix,&ip4remote);
        }
        if (q) { r = SPF_OK; break; }
      }
  }

  return r;
}

/**
 @brief  spf_ptr (ptr; ptr:fqdn)
         compares PTR records from SPF info and client host
 @param  input:  pointer to spfspecification; prefix not used
 @return SPF_OK, SPF_NONE; SPF_DNSSOFT, SPF_NOMEM
 */

int spf_ptr(char *spfspec,char *prefix)
{
  strsalloc ssa = {0};
  ipalloc ia = {0};
  int len = str_len(spfspec);
  int rc, r;
  int j, k, q;
  int pos;

  /* we didn't find host with the matching IP before */
  if (dnsname.len == 7 && str_equal(dnsname.s,"unknown"))
    return SPF_NONE;

  if (!spf_info("MPTR=",spfspec)) return SPF_NOMEM;

  /* the hostname found will probably be the same as before */
  while (dnsname.len) {
    pos = dnsname.len - len;
    if (pos < 0) break;
    if (pos > 0 && dnsname.s[pos - 1] != '.') break;
    if (case_diffb(dnsname.s + pos,len,spfspec)) break;
    return SPF_OK;
  }

  /* ok, either it's the first test or it's a very weired setup */

  if (!stralloc_readyplus(&ssa,0)) return SPF_NOMEM;
  if (!stralloc_readyplus(&ia,0)) return SPF_NOMEM;

  if (flagip6) { rc = dns_ptr6(&ssa,&ip6remote); }
  else { rc = dns_ptr(&ssa,&ip4remote); }

  switch (rc) {
    case DNS_MEM:  return SPF_NOMEM;
    case DNS_SOFT: r = SPF_DNSSOFT; break;
    case DNS_HARD: r = SPF_NONE; break;
    default:
      r = SPF_NONE;
      for (j = 0; j < ssa.len; ++j) {
        if (j > LOOKUP_LIMIT) { r = SPF_ERROR; break; }
        switch (dns_ip(&ia,&ssa.sa[j])) {
          case DNS_MEM:  return SPF_NOMEM;
          case DNS_SOFT: r = SPF_DNSSOFT; break;
          case DNS_HARD: break;
          default:
            for (k = 0; k < ia.len; ++k) {  
              if (k > LOOKUP_LIMIT) { r = SPF_ERROR;  break; }
              if (flagip6) {
                q = match_ip6(&ia.ix[j].addr.ip6,128,&ip6remote);
              } else {
                q = match_ip4(&ia.ix[j].addr.ip,32,&ip4remote);
              }
              if (q) {
                if (!dnsname.len)
                  if (!stralloc_copy(&dnsname,&ssa.sa[j])) return SPF_NOMEM;

                pos = ssa.sa[j].len - len;
                if (pos < 0) continue;
                if (pos > 0 && ssa.sa[j].s[pos - 1] != '.') continue;
                if (case_diffb(ssa.sa[j].s + pos,len,spfspec)) continue;

                stralloc_copy(&dnsname,&ssa.sa[j]);
                r = SPF_OK;
                break;
             }
           }
        }  
        if (r == SPF_ERROR) break;
      }
  }

  if (!dnsname.len)
    if (!stralloc_copys(&dnsname,"unknown")) return SPF_NOMEM;

  return r;
}

/**
 @brief  spf_ip4 (ip4; ip4:fqdn; ip4:fqdn/24)
         compares A records for SPF info and client host
 @param  input:  pointer to spfspecification, pointer to prefix 
 @return SPF_OK, SPF_NONE; SPF_DNSSOFT, SPF_NOMEM
 */

int spf_ip4(char *spfspec,char *prefix)
{
  struct ip_address spfip;

  if (flagip6) return SPF_NONE;
  int ipprefix = get_prefix(prefix);

  if (ipprefix < 0) return SPF_SYNTAX;
  if (!ip4_scan(&spfip,spfspec)) return SPF_SYNTAX;

  if (!spf_info("MIPv4=",spfspec)) return SPF_NOMEM;
  if (!match_ip4(&spfip,ipprefix,&ip4remote)) return SPF_NONE;

  return SPF_OK;
}

/**
 @brief  spf_ip6 (ip6; ip6:fqdn; ip6:fqdn/56)
         compares AAAA records for SPF info and client host
 @param  input:  pointer to spfspecification, pointer to prefix 
 @return SPF_OK, SPF_NONE; SPF_DNSSOFT, SPF_NOMEM
 */

int spf_ip6(char *spfspec,char *prefix)
{
  struct ip6_address spfip;

  if (!flagip6) return SPF_NONE;
  int ipprefix = get_prefix(prefix);

  if (ipprefix < 0) return SPF_SYNTAX;
  if (!ip6_scan(&spfip,spfspec)) return SPF_SYNTAX;

  if (!spf_info("MIPv6=",spfspec)) return SPF_NOMEM;
  if (!match_ip6(&spfip,ipprefix,&ip6remote)) return SPF_NONE;

  return SPF_OK;
}

/**
 @brief  spf_exists (exists; exists:fqdn)
         simply looks for a A records only for SPF info and client host
 @param  input:  pointer to spfspecification, prefix not used
 @return SPF_OK, SPF_NONE; SPF_DNSSOFT, SPF_NOMEM
 */

int spf_exists(char *spfspec,char *prefix)
{
  stralloc sa = {0};
  ipalloc ia = {0};
  int r;

  if (!stralloc_copys(&sa,spfspec)) return SPF_NOMEM;
  if (!stralloc_readyplus(&ia,0)) return SPF_NOMEM;

  if (!spf_info("MExists=",spfspec)) return SPF_NOMEM;

  switch (dns_ip(&ia,&sa)) {
    case DNS_MEM:  return SPF_NOMEM;
    case DNS_SOFT: r = SPF_DNSSOFT; break;
    case DNS_HARD: r = SPF_NONE; break;
    default:       return SPF_OK; 
  }

  return r;
}
