#ifndef UCSPITLS_H
#define UCSPITLS_H

#include <openssl/ssl.h>
#include <openssl/opensslv.h>
#include "stralloc.h"

#define SSL_NAME_LEN 256

extern int ssl_errno;

int ssl_io(SSL *,int,int,unsigned int);
SSL_CTX *ssl_context(SSL_METHOD *);
int ssl_timeoutconn(SSL *,unsigned int);
int ssl_timeoutaccept(SSL *,unsigned int);
SSL *ssl_new(SSL_CTX *,int);
int ssl_certkey(SSL_CTX *,const char *,const char *,pem_password_cb *);
int ssl_ca(SSL_CTX *,const char *,const char *,int);
int ssl_cca(SSL_CTX *,const char *);
int ssl_ciphers(SSL_CTX *,const char *);
int ssl_verify(SSL *,const char *);
int ssl_params(SSL_CTX *,const char *,int);
int ssl_server_env(SSL *,stralloc *);
int ssl_client_env(SSL *,stralloc *);
char *ssl_error_str(int);

#if defined (LIBRESSL_VERSION_NUMBER)
#define ssl_client() (ssl_context(TLS_client_method()))
#define ssl_server() (ssl_context(TLS_server_method()))
#else
#define ssl_client() (ssl_context(SSLv23_client_method()))
#define ssl_server() (ssl_context(SSLv23_server_method()))
#endif
#define ssl_errstr() (SSL_load_error_strings())
#define ssl_free(ssl) (SSL_free((ssl)))
#define ssl_close(ssl) (close(SSL_get_fd((ssl))))

#define ssl_pending(ssl) (SSL_pending((ssl)))
#define ssl_shutdown(ssl) (SSL_shutdown((ssl)))
#define ssl_shutdown_pending(ssl) (SSL_get_shutdown((ssl)) & SSL_RECEIVED_SHUTDOWN)
#define ssl_shutdown_sent(ssl) (SSL_get_shutdown((ssl)) & SSL_SENT_SHUTDOWN)

#endif
