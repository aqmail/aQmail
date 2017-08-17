#include "ucspissl.h"

SSL_CTX *ssl_context(const SSL_METHOD *m)
{
  SSL_CTX *ctx;

  SSL_library_init();
  ctx = SSL_CTX_new(m);
#ifdef SSL_TWEAKING
  SSL_CTX_set_options(ctx,SSL_OP_SINGLE_DH_USE|SSL_OP_NO_COMPRESSION|SSL_OP_CIPHER_SERVER_PREFERENCE);
#else
  SSL_CTX_set_options(ctx,SSL_OP_SINGLE_DH_USE);
#endif
#ifdef SSLv2_DISABLE
  SSL_CTX_set_options(ctx,SSL_OP_NO_SSLv2);
#endif
#ifdef SSLv3_DISABLE
  SSL_CTX_set_options(ctx,SSL_OP_NO_SSLv3);
#endif
#ifdef TLSv1_DISABLE
  SSL_CTX_set_options(ctx,SSL_OP_NO_TLSv1);
#endif
  return ctx;
}

