/** 
  @file  ssl_verify.c
  @author web, feh -- parts of code borrowed from Pavel Shramov; tx Peter Conrad
  @brief Compares 'hostname' against SubAltName DNS:hostname + DN: /CN=hostname
*/
#include "ucspissl.h"
#include "case.h"
#include "strerr.h"
#include "str.h"
#include <openssl/x509v3.h>

int ssl_verify(SSL *ssl,const char *hostname)
{
  X509 *cert;
  STACK_OF(GENERAL_NAME) *extensions;
  const GENERAL_NAME *ext;
  char buf[SSL_NAME_LEN];
  char *dnsname;
  int i;
  int num; 
  int len;
  int dname = 0;

  cert = SSL_get_peer_certificate(ssl);
  if (!cert) return -1;

  if (SSL_get_verify_result(ssl) != X509_V_OK) return -2;

  if (hostname) {
    extensions = X509_get_ext_d2i(cert,NID_subject_alt_name,0,0);
    num = sk_GENERAL_NAME_num(extensions); 	/* num = 0, if no SAN extensions */

    for (i = 0; i < num; ++i) {
      ext = sk_GENERAL_NAME_value(extensions,i);
      if (ext->type == GEN_DNS) {
//#if OPENSSL_VERSION_NUMBER < 0x10100000L
        if (ASN1_STRING_type(ext->d.ia5) != V_ASN1_IA5STRING) continue;
//#else 
        if (OBJ_sn2nid((const char *)ext->d.ia5) != V_ASN1_IA5STRING) continue;
//#endif
        dnsname = (char *)ASN1_STRING_data(ext->d.ia5);
        len = ASN1_STRING_length(ext->d.ia5);
        if (len != str_len(dnsname)) continue;
        if (case_diffs((char *)hostname,dnsname) == 0) return 0;
        dname = 1;
      }
    }

    if (!dname) {
      X509_NAME_get_text_by_NID(X509_get_subject_name(cert),NID_commonName,buf,sizeof buf);
      buf[SSL_NAME_LEN - 1] = 0;
      if (case_diffs((char *)hostname,buf) == 0) return 0;
    }

    return -3;
  }
  return 0;
}

