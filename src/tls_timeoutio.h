#ifndef TLS_TIMEOUTIO_H
#define TLS_TIMEOUTIO_H

#include <openssl/ssl.h>

/* the version is like this: 0xMNNFFPPS: major minor fix patch status */
#if OPENSSL_VERSION_NUMBER < 0x00906000L
# error "Need OpenSSL version at least 0.9.7"
#endif

int tls_timeoutconn(int t, int rfd, int wfd, SSL *tls);
int tls_timeoutaccept(int t, int rfd, int wfd, SSL *tls);
int tsl_timeoutrehandshake(int t, int rfd, int wfd, SSL *tls);

int tls_timeoutread(int t, int rfd, int wfd, SSL *tls, char *buf, int len);
int tls_timeoutwrite(int t, int rfd, int wfd, SSL *tls, char *buf, int len);

int tls_timeoutio(int (*fun)(), int t, int rfd, int wfd, SSL *tls, char *buf, int len);

#endif
