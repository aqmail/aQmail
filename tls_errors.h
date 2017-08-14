#ifndef TLS_CLIENTS_H
#define TLS_CLIENTS_H

#include "stralloc.h"

extern stralloc host;
extern stralloc cafile;
extern stralloc cadir;
extern stralloc ciphers;
extern stralloc certfile;
extern stralloc keyfile;
extern stralloc keypwd;
extern stralloc fiprint;

void temp_nomem(void);
void temp_tlsctx(void);
void temp_tlsca(void);
void temp_tlscipher(void);
void temp_tlscert(void);
void temp_tlscertfp(void);
void temp_tlsdigest(void);
void temp_tlshost(void);
void temp_tlskey(void);
void temp_tlschk(void);
void temp_tlsctx(void);
void temp_tlserr(void);
void temp_tlsepeercert(void);
void temp_tlsepeerverify(void);
void temp_invaliddigest(void);

#endif
