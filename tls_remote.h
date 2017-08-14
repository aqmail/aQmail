#ifndef TLS_REMOTE_H
#define TLS_REMOTE_H

#include <openssl/ssl.h>

/* the version is like this: 0xMNNFFPPS: major minor fix patch status */
#if OPENSSL_VERSION_NUMBER < 0x00908000L
# error "Need OpenSSL version at least 0.9.8"
#endif

SSL *ssl;
SSL_CTX *ctx;

/* extern char *tlsdestinfo;
extern struct constmap maptlsdestinations;
extern char *tlsdomaininfo;
extern struct constmap mapdomaincerts; */

int tls_domaincerts(const char *,const int);
int tls_destination(const char *,const int);
int tls_fingerprint(SSL *ssl,const char *,const int);
int tls_chainfile(SSL_CTX *ctx,const char *);
int tls_certkey(SSL_CTX *ctx,const char *,const char *,char *);
int tls_conn(SSL *ssl,int);
int tls_setup(int,char *,char *);
int tls_checkpeer(SSL *ssl,const char *,int,const int,const int);
int tls_checkcrl(SSL *ssl);
int tls_error(void);
int tls_exit(SSL *ssl);

#endif
