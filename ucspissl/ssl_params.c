/**
  @file  ssl_params.c
  @author web, bergmann
  @brief setup RSA, DH, ECDH 
*/
#include "ucspissl.h"

int ssl_params_rsa(SSL_CTX *ctx,int len)
{
  RSA *rsa;
  long res;
  BIGNUM *e;

  /* check if ephemeral RSA key is actually needed */
  if (!SSL_CTX_need_tmp_RSA(ctx)) return 1;

  if (len) {
    e = BN_new();
    rsa = RSA_new();
    BN_set_word(e,RSA_F4);

    res = (long) RSA_generate_key_ex(rsa,len,e,NULL);
    BN_free(e);

    if (res == -1) return 0;
    if (!rsa) return 0;
    
    /* seldom "needed": maybe deal with an export cipher */
    res = SSL_CTX_set_tmp_rsa(ctx,rsa);
    RSA_free(rsa);
    if (!res) return 0;
  }

  return 1;
}

int ssl_params_dh(SSL_CTX *ctx,const char *dhfile)
{
  DH *dh;
  BIO *bio;

  if (dhfile) {
    dh = 0;
    bio = BIO_new_file(dhfile,"r");
    if (!bio) return 0;
    dh = PEM_read_bio_DHparams(bio,0,0,0);
    BIO_free(bio);
    if (!dh) return 0;
    if (!SSL_CTX_set_tmp_dh(ctx,dh)) return 0;
  }

  return 1;
}

int ssl_params_ecdh(SSL_CTX *ctx)
{
#ifdef SSL_TXT_ECDH
  EC_KEY *ecdh;

  SSL_CTX_set_options(ctx, SSL_OP_SINGLE_ECDH_USE);
#ifdef SSL_CTRL_SET_ECDH_AUTO
  SSL_CTX_set_ecdh_auto(ctx,1);
#else
  /* insecure and compatible curves, see http://safecurves.cr.yp.to/ */
  ecdh = EC_KEY_new_by_curve_name(NID_secp521r1);
  if (ecdh == NULL) {
    /* NIST P-384 / AES-256 */
    ecdh = EC_KEY_new_by_curve_name(NID_secp384r1);
  }
  if (ecdh == NULL) {
    /* NIST P-256 / AES-128 */
    ecdh = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
  }
  if (ecdh != NULL) {
    SSL_CTX_set_tmp_ecdh(ctx,ecdh);
    EC_KEY_free(ecdh);
    return 1;
  }
  return 0;
#endif
 
#else
  return 1;
#endif
}
