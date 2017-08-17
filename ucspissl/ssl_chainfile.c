/** 
  @file ssl_certchainfile.c
  @author feh
  @brief Enables Certificate chain file presentation for sslserver
*/
#include "ucspissl.h"

int ssl_chainfile(SSL_CTX *ctx,const char *certchainfile,const char *keyfile,pem_password_cb *passwd_cb)
{
  if (!certchainfile) return 0;
  if (!keyfile) return 0;

  if (SSL_CTX_use_certificate_chain_file(ctx,certchainfile) <= 0)
    return -1;

  SSL_CTX_set_default_passwd_cb(ctx,passwd_cb);
  if (SSL_CTX_use_RSAPrivateKey_file(ctx,keyfile,SSL_FILETYPE_PEM) != 1)
    return -2;

  if (SSL_CTX_check_private_key(ctx) != 1)
    return -3;

  return 0;
}
