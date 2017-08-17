#include <unistd.h>
#include <openssl/x509.h>
#include <string.h>
#include "fmt.h"
#include "pathexec.h"
#include "ucspissl.h"
#include "stralloc.h"
#include "str.h"

static char strnum[FMT_ULONG];
static stralloc ctemp = {0};
static stralloc *envsa = 0;
static stralloc btemp = {0};
static stralloc etemp = {0};

#define set_env_id(n,e,v) \
if (!set_env_name_entry((n),(e),(v))) return 0

static int env_val(const char *env,const void *val,int len) {
  const char *v = val;
  if (envsa) {
    if (!stralloc_cats(envsa,env)) return 0;
    if (!stralloc_catb(envsa,"=",1)) return 0;
    if (!stralloc_catb(envsa,v,len)) return 0;
    if (!stralloc_0(envsa)) return 0;
    return 1;
  }
  if (!stralloc_copyb(&etemp,v,len)) return 0;
  if (!stralloc_0(&etemp)) return 0;
  return pathexec_env(env,etemp.s);
}

static int env_str(const char *env,const char *val) {
  if (envsa) {
    return env_val(env,val,str_len(val));
    if (!stralloc_cats(envsa,env)) return 0;
    if (!stralloc_catb(envsa,"=",1)) return 0;
    if (!stralloc_catb(envsa,val,str_len(val) + 1)) return 0;
    return 1;
  }
  return pathexec_env(env,val);
}

static int set_env_name_entry(X509_NAME *xname,const char *env,int nid) {
  X509_NAME_ENTRY *xne;
  int m;
  int n;

  if (!env) return 1;
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  for (m = 0; m < sk_X509_NAME_ENTRY_num(xname->entries); m++) {
    xne = sk_X509_NAME_ENTRY_value(xname->entries,m);
    n = OBJ_obj2nid(xne->object);
    if (n == nid)
      if (!env_val(env,xne->value->data,xne->value->length)) return 0;
#else
  for (m = 0; m < X509_NAME_entry_count(xname); m++) {
    xne = X509_NAME_get_entry(xname,m);
    n = OBJ_obj2nid(X509_NAME_ENTRY_get_object(xne));
    if (n == nid)
      if (!env_val(env,X509_NAME_ENTRY_get_data(xne)->data,X509_NAME_ENTRY_get_data(xne)->length)) return 0;
#endif
  }
  return 1;
}

int ssl_session_vars(SSL *ssl) {
  unsigned char *x;
  SSL_SESSION *session;
  int n = 0;
  int m;
  const SSL_CIPHER *cipher;
  unsigned char u;
  unsigned char c;

  if (!env_str("SSL_PROTOCOL",SSL_get_version(ssl)))
    return 0;

#if OPENSSL_VERSION_NUMBER < 0x10100000L
  session = SSL_get_session(ssl);
  x = session->session_id;
  n = session->session_id_length;
#else
  session = SSL_get1_session(ssl);
  x = SSL_SESSION_get_id(session,&n);
#endif

  if (!stralloc_ready(&btemp,2 * n)) return 0;
  btemp.len = 2 * n;
  while (n--) {
    u = x[n];
    c = '0' + (u & 15); if (c > '0' + 9) c += 'a' - '0' - 10;
    btemp.s[2 * n + 1] = c;
    u >>= 4;
    c = '0' + (u & 15); if (c > '0' + 9) c += 'a' - '0' - 10;
    btemp.s[2 * n] = c;
  }
  if (!env_val("SSL_SESSION_ID",btemp.s,btemp.len)) return 0;

  if (!env_str("SSL_CIPHER",SSL_get_cipher_name(ssl))) return 0;
  
  cipher = SSL_get_current_cipher(ssl);
  if (!cipher) return 0;
  n = SSL_CIPHER_get_bits(cipher,&m);
  if (!env_str("SSL_CIPHER_EXPORT",n < 56 ? "true" : "false")) return 0;
  if (!env_val("SSL_CIPHER_USEKEYSIZE",strnum,fmt_ulong(strnum,n))) return 0;
  if (!env_val("SSL_CIPHER_ALGKEYSIZE",strnum,fmt_ulong(strnum,m))) return 0;

  if (!env_str("SSL_VERSION_INTERFACE","ucspi-ssl")) return 0;
  if (!env_str("SSL_VERSION_LIBRARY",OPENSSL_VERSION_TEXT)) return 0;

  return 1;
}

static int ssl_client_bio_vars(X509 *cert,STACK_OF(X509) *chain,BIO *bio) {
  ASN1_STRING *astring;
  int n;
  int m;

  astring = X509_get_notBefore(cert);
  if (!ASN1_UTCTIME_print(bio,astring)) return 0;
  n = BIO_pending(bio);
  if (!stralloc_ready(&btemp,n)) return 0;
  btemp.len = n;
  n = BIO_read(bio,btemp.s,n);
  if (n != btemp.len) return 0;
  if (!env_val("SSL_CLIENT_V_START",btemp.s,btemp.len)) return 0;

  astring = X509_get_notAfter(cert);
  if (!ASN1_UTCTIME_print(bio,astring)) return 0;
  n = BIO_pending(bio);
  if (!stralloc_ready(&btemp,n)) return 0;
  btemp.len = n;
  n = BIO_read(bio,btemp.s,n);
  if (n != btemp.len) return 0;
  if (!env_val("SSL_CLIENT_V_END",btemp.s,btemp.len)) return 0;

  if (!PEM_write_bio_X509(bio,cert)) return 0;
  n = BIO_pending(bio);
  if (!stralloc_ready(&btemp,n)) return 0;
  btemp.len = n;
  n = BIO_read(bio,btemp.s,n);
  if (n != btemp.len) return 0;
  if (!env_val("SSL_CLIENT_CERT",btemp.s,btemp.len)) return 0;

  if (chain) {
    for (m = 0;m < sk_X509_num(chain);m++) {
      if (!stralloc_copys(&ctemp,"SSL_CLIENT_CERT_CHAIN_")) return 0;
      if (!stralloc_catb(&ctemp,strnum,fmt_ulong(strnum,m))) return 0;
      if (!stralloc_0(&ctemp)) return 0;

      if (m < sk_X509_num(chain)) {
	if (!PEM_write_bio_X509(bio,sk_X509_value(chain,m))) return 0;
	n = BIO_pending(bio);
	if (!stralloc_ready(&btemp,n)) return 0;
	btemp.len = n;
	n = BIO_read(bio,btemp.s,n);
	if (n != btemp.len) return 0;
	if (!env_val(ctemp.s,btemp.s,btemp.len)) return 0;
      }
    }
  }

  return 1;
}

static int ssl_client_vars(X509 *cert,STACK_OF(X509) *chain) {
  X509_NAME *xname;
  X509_PUBKEY *pubkey;
  const X509_ALGOR *sigalg;
  const ASN1_OBJECT *algoid;
  BIGNUM *bn;
  BIO *bio;
  char *x;
  int n;

  if (!cert) return 1;

  if (!env_val("SSL_CLIENT_M_VERSION",strnum,fmt_ulong(strnum,X509_get_version(cert) + 1)))
    return 0;

  bn = ASN1_INTEGER_to_BN(X509_get_serialNumber(cert), 0);
  x = BN_bn2dec(bn);
  BN_free(bn);
  if (!env_val("SSL_CLIENT_M_SERIAL",x,strlen(x)))
    return 0;
  OPENSSL_free(x);

  xname = X509_get_subject_name(cert);
  x = X509_NAME_oneline(xname,0,0);
  n = env_str("SSL_CLIENT_S_DN",x);
  free(x);
  if (!n) return 0;

  set_env_id(xname,"SSL_CLIENT_S_DN_C",NID_countryName);
  set_env_id(xname,"SSL_CLIENT_S_DN_ST",NID_stateOrProvinceName);
  set_env_id(xname,"SSL_CLIENT_S_DN_L",NID_localityName);
  set_env_id(xname,"SSL_CLIENT_S_DN_O",NID_organizationName);
  set_env_id(xname,"SSL_CLIENT_S_DN_OU",NID_organizationalUnitName);
  set_env_id(xname,"SSL_CLIENT_S_DN_CN",NID_commonName);
  set_env_id(xname,"SSL_CLIENT_S_DN_T",NID_title);
  set_env_id(xname,"SSL_CLIENT_S_DN_I",NID_initials);
  set_env_id(xname,"SSL_CLIENT_S_DN_G",NID_givenName);
  set_env_id(xname,"SSL_CLIENT_S_DN_S",NID_surname);
  set_env_id(xname,"SSL_CLIENT_S_DN_D",NID_description);
  set_env_id(xname,"SSL_CLIENT_S_DN_UID",NID_x500UniqueIdentifier);
  set_env_id(xname,"SSL_CLIENT_S_DN_Email",NID_pkcs9_emailAddress);

  xname = X509_get_issuer_name(cert);
  x = X509_NAME_oneline(xname,0,0);
  n = env_str("SSL_CLIENT_I_DN",x);
  free(x);
  if (!n) return 0;

  set_env_id(xname,"SSL_CLIENT_I_DN_C",NID_countryName);
  set_env_id(xname,"SSL_CLIENT_I_DN_ST",NID_stateOrProvinceName);
  set_env_id(xname,"SSL_CLIENT_I_DN_L",NID_localityName);
  set_env_id(xname,"SSL_CLIENT_I_DN_O",NID_organizationName);
  set_env_id(xname,"SSL_CLIENT_I_DN_OU",NID_organizationalUnitName);
  set_env_id(xname,"SSL_CLIENT_I_DN_CN",NID_commonName);
  set_env_id(xname,"SSL_CLIENT_I_DN_T",NID_title);
  set_env_id(xname,"SSL_CLIENT_I_DN_I",NID_initials);
  set_env_id(xname,"SSL_CLIENT_I_DN_G",NID_givenName);
  set_env_id(xname,"SSL_CLIENT_I_DN_S",NID_surname);
  set_env_id(xname,"SSL_CLIENT_I_DN_D",NID_description);
  set_env_id(xname,"SSL_CLIENT_I_DN_UID",NID_x500UniqueIdentifier);
  set_env_id(xname,"SSL_CLIENT_I_DN_Email",NID_pkcs9_emailAddress);

/* Signature Algorithm for PubKey */
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  n = OBJ_obj2nid(cert->cert_info->signature->algorithm);
#else
  sigalg = X509_get0_tbs_sigalg(cert);
  X509_ALGOR_get0(&algoid,0,0,sigalg);
  n = OBJ_obj2nid(algoid);
#endif
  if (!env_str("SSL_CLIENT_A_SIG",(n == NID_undef) ? "UNKNOWN" : OBJ_nid2ln(n)))
    return 0;

/* Algorithm for PubKey */
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  n = OBJ_obj2nid(cert->cert_info->key->algor->algorithm);
#else
  pubkey = X509_get_X509_PUBKEY(cert);
  X509_PUBKEY_get0_param(&algoid,0,0,0,pubkey);
  n = OBJ_obj2nid(algoid);
#endif
  if (!env_str("SSL_CLIENT_A_KEY",(n == NID_undef) ? "UNKNOWN" : OBJ_nid2ln(n)))
    return 0;

  bio = BIO_new(BIO_s_mem());
  if (!bio) return 0;
  n = ssl_client_bio_vars(cert,chain,bio);
  BIO_free(bio);
  if (!n) return 0;

  return 1;
}

static int ssl_server_bio_vars(X509 *cert,STACK_OF(X509) *chain,BIO *bio) {
  ASN1_STRING *astring;
  int n;
  int m;

  astring = X509_get_notBefore(cert);
  if (!ASN1_UTCTIME_print(bio,astring)) return 0;
  n = BIO_pending(bio);
  if (!stralloc_ready(&btemp,n)) return 0;
  btemp.len = n;
  n = BIO_read(bio,btemp.s,n);
  if (n != btemp.len) return 0;
  if (!env_val("SSL_SERVER_V_START",btemp.s,btemp.len)) return 0;

  astring = X509_get_notAfter(cert);
  if (!ASN1_UTCTIME_print(bio,astring)) return 0;
  n = BIO_pending(bio);
  if (!stralloc_ready(&btemp,n)) return 0;
  btemp.len = n;
  n = BIO_read(bio,btemp.s,n);
  if (n != btemp.len) return 0;
  if (!env_val("SSL_SERVER_V_END",btemp.s,btemp.len)) return 0;

  if (!PEM_write_bio_X509(bio,cert)) return 0;
  n = BIO_pending(bio);
  if (!stralloc_ready(&btemp,n)) return 0;
  btemp.len = n;
  n = BIO_read(bio,btemp.s,n);
  if (n != btemp.len) return 0;
  if (!env_val("SSL_SERVER_CERT",btemp.s,btemp.len)) return 0;

  if (chain) {
    for (m = 0;m < sk_X509_num(chain);m++) {
      if (!stralloc_copys(&ctemp,"SSL_SERVER_CERT_CHAIN_")) return 0;
      if (!stralloc_catb(&ctemp,strnum,fmt_ulong(strnum,m))) return 0;
      if (!stralloc_0(&ctemp)) return 0;

      if (m < sk_X509_num(chain)) {
	if (!PEM_write_bio_X509(bio,sk_X509_value(chain,m))) return 0;
	n = BIO_pending(bio);
	if (!stralloc_ready(&btemp,n)) return 0;
	btemp.len = n;
	n = BIO_read(bio,btemp.s,n);
	if (n != btemp.len) return 0;
	if (!env_val(ctemp.s,btemp.s,btemp.len)) return 0;
      }
    }
  }

  return 1;
}

static int ssl_server_vars(X509 *cert,STACK_OF(X509) *chain) {
  X509_NAME *xname;
  X509_PUBKEY *pubkey;
  const X509_ALGOR *sigalg; 
  const ASN1_OBJECT *algoid;
  BIGNUM *bn;
  BIO *bio;
  char *x;
  int n;

  if (!cert) return 1;

  if (!env_val("SSL_SERVER_M_VERSION",strnum,fmt_ulong(strnum,X509_get_version(cert) + 1)))
    return 0;

  bn = ASN1_INTEGER_to_BN(X509_get_serialNumber(cert), 0);
  x = BN_bn2dec(bn);
  BN_free(bn);
  if (!env_val("SSL_SERVER_M_SERIAL",x,strlen(x))) return 0;
  OPENSSL_free(x);

  xname = X509_get_subject_name(cert);
  x = X509_NAME_oneline(xname,0,0);
  n = env_str("SSL_SERVER_S_DN",x);
  free(x);
  if (!n) return 0;

  set_env_id(xname,"SSL_SERVER_S_DN_C",NID_countryName);
  set_env_id(xname,"SSL_SERVER_S_DN_ST",NID_stateOrProvinceName);
  set_env_id(xname,"SSL_SERVER_S_DN_L",NID_localityName);
  set_env_id(xname,"SSL_SERVER_S_DN_O",NID_organizationName);
  set_env_id(xname,"SSL_SERVER_S_DN_OU",NID_organizationalUnitName);
  set_env_id(xname,"SSL_SERVER_S_DN_CN",NID_commonName);
  set_env_id(xname,"SSL_SERVER_S_DN_T",NID_title);
  set_env_id(xname,"SSL_SERVER_S_DN_I",NID_initials);
  set_env_id(xname,"SSL_SERVER_S_DN_G",NID_givenName);
  set_env_id(xname,"SSL_SERVER_S_DN_S",NID_surname);
  set_env_id(xname,"SSL_SERVER_S_DN_D",NID_description);
  set_env_id(xname,"SSL_SERVER_S_DN_UID",NID_x500UniqueIdentifier);
  set_env_id(xname,"SSL_SERVER_S_DN_Email",NID_pkcs9_emailAddress);

  xname = X509_get_issuer_name(cert);
  x = X509_NAME_oneline(xname,0,0);
  n = env_str("SSL_SERVER_I_DN",x);
  free(x);
  if (!n) return 0;

  set_env_id(xname,"SSL_SERVER_I_DN_C",NID_countryName);
  set_env_id(xname,"SSL_SERVER_I_DN_ST",NID_stateOrProvinceName);
  set_env_id(xname,"SSL_SERVER_I_DN_L",NID_localityName);
  set_env_id(xname,"SSL_SERVER_I_DN_O",NID_organizationName);
  set_env_id(xname,"SSL_SERVER_I_DN_OU",NID_organizationalUnitName);
  set_env_id(xname,"SSL_SERVER_I_DN_CN",NID_commonName);
  set_env_id(xname,"SSL_SERVER_I_DN_T",NID_title);
  set_env_id(xname,"SSL_SERVER_I_DN_I",NID_initials);
  set_env_id(xname,"SSL_SERVER_I_DN_G",NID_givenName);
  set_env_id(xname,"SSL_SERVER_I_DN_S",NID_surname);
  set_env_id(xname,"SSL_SERVER_I_DN_D",NID_description);
  set_env_id(xname,"SSL_SERVER_I_DN_UID",NID_x500UniqueIdentifier);
  set_env_id(xname,"SSL_SERVER_I_DN_Email",NID_pkcs9_emailAddress);

/* Signature Algorithm of PubKey */
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  n = OBJ_obj2nid(cert->cert_info->signature->algorithm);
#else
  sigalg = X509_get0_tbs_sigalg(cert);
  X509_ALGOR_get0(&algoid,0,0,sigalg);
  n = OBJ_obj2nid(algoid);
#endif
  if (!env_str("SSL_SERVER_A_SIG",(n == NID_undef) ? "UNKNOWN" : OBJ_nid2ln(n)))
    return 0;

/* Algorithm of PubKey */
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  n = OBJ_obj2nid(cert->cert_info->key->algor->algorithm);
#else
  pubkey = X509_get_X509_PUBKEY(cert);
  X509_PUBKEY_get0_param(&algoid,0,0,0,pubkey);
  n = OBJ_obj2nid(algoid);
#endif
  if (!env_str("SSL_SERVER_A_KEY",(n == NID_undef) ? "UNKNOWN" : OBJ_nid2ln(n)))
    return 0;

  bio = BIO_new(BIO_s_mem());
  if (!bio) return 0;
  n = ssl_server_bio_vars(cert,chain,bio);
  BIO_free(bio);

  if (!n) return 0;

  return 1;
}

int ssl_client_env(SSL *ssl,stralloc *sa) {
  envsa = sa;
  if (!ssl_session_vars(ssl)) return 0;
  if (!ssl_client_vars(SSL_get_certificate(ssl),0))
    return 0;
  if (!ssl_server_vars(SSL_get_peer_certificate(ssl),SSL_get_peer_cert_chain(ssl)))
    return 0;
  return 1;
}

int ssl_server_env(SSL *ssl,stralloc *sa) {
  envsa = sa;
  if (!ssl_session_vars(ssl)) return 0;
  if (!ssl_server_vars(SSL_get_certificate(ssl),0))
    return 0;
  if (!ssl_client_vars(SSL_get_peer_certificate(ssl),SSL_get_peer_cert_chain(ssl)))
    return 0;
  return 1;
}


