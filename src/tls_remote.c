#include <unistd.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/ssl.h>
#include <openssl/asn1.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include "fmt.h"
#include "ucspitls.h"
#include "stralloc.h"
#include "str.h"
#include "byte.h"
#include "case.h"
#include "strerr.h"
#include "tls_remote.h"
#include "tls_errors.h"

extern char *tlsdestinfo;
extern struct constmap maptlsdestinations;
extern char *tlsdomaininfo;
extern struct constmap mapdomaincerts;
extern stralloc ciphers;

/** @file  tls_remote.c -- TLS client functions
    @brief connection functions: tls_conn, tls_exit;
           verification functions: tls_certkey, tls_checkpeer, tls_fingerprint;
           tls_destinations, tls_domaincert
           dummy functions: tls_crlcheck
*/

int tls_certkey(SSL_CTX *ctx,const char *cert,const char *key,char *ppwd) 
{
  if (!cert) return 0;

  if (SSL_CTX_use_certificate_chain_file(ctx,cert) != 1)
    return -1;

  if (!key) key = cert;

  if (ppwd) SSL_CTX_set_default_passwd_cb_userdata(ctx,ppwd);

  if (SSL_CTX_use_PrivateKey_file(ctx,key,SSL_FILETYPE_PEM) != 1)
    return -2;

  if (SSL_CTX_check_private_key(ctx) != 1)
    return -3;

  return 0;
}

int tls_conn(SSL *ssl,int smtpfd)
{
  SSL_set_options(ssl,SSL_OP_NO_SSLv2);
  SSL_set_options(ssl,SSL_OP_NO_SSLv3);
  return SSL_set_fd(ssl,smtpfd);
}

int tls_checkpeer(SSL *ssl,const char *hostname,int hlen,const int flag,const int verify)
{
  X509 *cert;
  STACK_OF(GENERAL_NAME) *extensions;
  const GENERAL_NAME *ext;
  char buf[SSL_NAME_LEN];
  char *dnsname;
  int dname = 0;
  int num;
  int len;
  int fflag; 
  int i;

  fflag = flag;
  if (flag > 20) fflag = flag - 20;
  if (flag > 10) fflag = flag - 10;

  if (fflag == 1 || fflag == 3) return 0;       /* ADH - no Cert required */

  cert = SSL_get_peer_certificate(ssl);
  if (!cert) return -1;

  if (hostname && fflag > 4) {
    extensions = (GENERAL_NAME *)X509_get_ext_d2i(cert,NID_subject_alt_name,0,0);
    num = sk_GENERAL_NAME_num(extensions);	/* num = 0, if no SAN extensions */

    for (i = 0; i < num; ++i) {
      ext = sk_GENERAL_NAME_value(extensions,i);
      if (ext->type == GEN_DNS) {
        if (ASN1_STRING_type(ext->d.ia5) != V_ASN1_IA5STRING) continue;
        dnsname = (char *)ASN1_STRING_data(ext->d.ia5);
        len = ASN1_STRING_length(ext->d.ia5);
        dname = 1;
      }
    }

    if (!dname) {
      X509_NAME_get_text_by_NID(X509_get_subject_name(cert),NID_commonName,buf,sizeof(buf));
      buf[SSL_NAME_LEN - 1] = 0;
      dnsname = buf;
      len = SSL_NAME_LEN - 1;
    }

    switch (fflag) {
      case 5: if (dnsname[0] == '*' && dnsname[1] == '.') 
                if (!case_diffrs(hostname,dnsname+1)) return -3; break;
      case 6: if (str_diffn(hostname,dnsname,hlen)) return -3; break;
    }
  }

  if (fflag > 3 && verify > 0) 	/* Root CA must be availble */
    if (SSL_get_verify_result(ssl) != X509_V_OK) return -2;

  return 0;
}

int tls_checkcrl(SSL *ssl)
{

  return 0;
}

int tls_fingerprint(SSL *ssl,const char *fingerprint,int dlen)
{
  X509 *cert;
  const EVP_MD *methodsha1 = EVP_sha1();
  const EVP_MD *methodsha224 = EVP_sha224();
  const EVP_MD *methodsha256 = EVP_sha256();
  const EVP_MD *methodsha512 = EVP_sha512();

  unsigned char digest[EVP_MAX_MD_SIZE];
  unsigned int len;
  stralloc slop = {0};
  unsigned char digascii[129];
  static char hextab[]="0123456789abcdef";
  int j;

  cert = SSL_get_peer_certificate(ssl);
  if (!cert) return -1;

  switch (dlen) {			/* fetch digest from cert */
    case  40: if (!X509_digest(cert,methodsha1,digest,&len)) return -2; break;
    case  56: if (!X509_digest(cert,methodsha224,digest,&len)) return -2; break;
    case  64: if (!X509_digest(cert,methodsha256,digest,&len)) return -2; break;
    case 128: if (!X509_digest(cert,methodsha512,digest,&len)) return -2; break;
    default: return -3;
  }

  if (!stralloc_copys(&slop,"")) temp_nomem();

  for (j = 0; j < dlen; j++) {
    digascii[2*j] = hextab[digest[j] >> 4];
    digascii[2*j+1] = hextab[digest[j] & 0x0f];
  }   
  digascii[dlen] = 0;

  if (!stralloc_catb(&slop,digascii,dlen+1)) temp_nomem(); 
  if (!str_diffn(slop.s,fingerprint,dlen)) return 0;

  return 1;
}

int tls_exit(SSL *ssl)
{
  if (SSL_shutdown(ssl) == 0)
    SSL_shutdown(ssl);

  return 0;
}

/** @brief tls_destinations
    @param  pointer to remote fqdn, length of fqdn

    @return values: | ADH Cert *DN FQDN Hash
          ----------+-----------------------
       optional TLS |  1   2    -    -    -
      mandatory TLS |  3   4    5    6    7 
  */

int tls_destination(const char *hostname,const int len)
{
  int i;
  stralloc tlsdest = {0};

// Host rules

  for (i = 0; i <= len; ++i) // exclude host/domain
    if ((i == 0) || (i == len) || (hostname[i] == '.')) {
      if (!stralloc_copys(&tlsdest,"!")) temp_nomem();
      if (!stralloc_catb(&tlsdest,hostname+i,len-i)) temp_nomem();
      if (!stralloc_0(&tlsdest)) temp_nomem();
      if (tlsdestinfo = constmap(&maptlsdestinations,tlsdest.s,tlsdest.len-1)) return -1;
    }

  if (!stralloc_copys(&tlsdest,"%")) temp_nomem(); // CERT + hash 
  if (!stralloc_catb(&tlsdest,hostname,len)) temp_nomem();
  if (!stralloc_0(&tlsdest)) temp_nomem();
  if (tlsdestinfo = constmap(&maptlsdestinations,tlsdest.s,tlsdest.len-1)) return 7;

  if (!stralloc_copys(&tlsdest,"=")) temp_nomem();  // CERT + FQDN
  if (!stralloc_catb(&tlsdest,hostname,len)) temp_nomem();
  if (!stralloc_0(&tlsdest)) temp_nomem();
  if (tlsdestinfo = constmap(&maptlsdestinations,tlsdest.s,tlsdest.len-1)) return 6;
  
  for (i = 0; i <= len; ++i)  // CERT + *DN
    if ((i == 0) || (i == len) || (hostname[i] == '.')) {
      if (!stralloc_copys(&tlsdest,"~")) temp_nomem();
      if (!stralloc_catb(&tlsdest,hostname+i,len-i)) temp_nomem();
      if (!stralloc_0(&tlsdest)) temp_nomem();
      if (tlsdestinfo = constmap(&maptlsdestinations,tlsdest.s,tlsdest.len-1)) return 5;
    }

  for (i = 0; i <= len; ++i) // CERT
    if ((i == 0) || (i == len) || (hostname[i] == '.')) {
      if (!stralloc_copys(&tlsdest,"")) temp_nomem();
      if (!stralloc_catb(&tlsdest,hostname+i,len-i)) temp_nomem();
      if (!stralloc_0(&tlsdest)) temp_nomem();
      if (tlsdestinfo = constmap(&maptlsdestinations,tlsdest.s,tlsdest.len-1)) return 4;
    }

  for (i = 0; i <= len; ++i)  // ADH per host/domain
    if ((i == 0) || (i == len) || (hostname[i] == '.')) {
      if (!stralloc_copys(&tlsdest,"-")) temp_nomem();
      if (!stralloc_catb(&tlsdest,hostname+i,len-i)) temp_nomem();
      if (!stralloc_0(&tlsdest)) temp_nomem();
      if (tlsdestinfo = constmap(&maptlsdestinations,tlsdest.s,tlsdest.len-1)) return 3;
    }

// General rules

  if (tlsdestinfo = constmap(&maptlsdestinations,"=*",2)) return 6; // CERT + FQDN
  if (tlsdestinfo = constmap(&maptlsdestinations,"~*",2)) return 5; // CERT + Wild
  if (tlsdestinfo = constmap(&maptlsdestinations,"+*",2)) return 4;
  if (tlsdestinfo = constmap(&maptlsdestinations,"-*",2)) return 3;

// Fall thru rules

  if (tlsdestinfo = constmap(&maptlsdestinations,"*",1)) return 2; // CERT
  if (tlsdestinfo = constmap(&maptlsdestinations,"-",1)) return 1; // ADH

  return 0;
}

int tls_domaincerts(const char *domainname,const int len)
{
  int i;

/* Our Certs - per domain */

  for (i = 0; i <= len; ++i)
    if ((i == 0) || (i == len) || (domainname[i] == '.'))
      if (tlsdomaininfo = constmap(&mapdomaincerts,domainname+i,len-i)) return 2;

/* Standard Cert (if any) */
 
  if (tlsdomaininfo = constmap(&mapdomaincerts,"*",1)) return 1;
 
  return 0;
}
