#ifndef SHA256_H
#define SHA256_H

typedef struct
{
  uint8_t data[64];
  uint32_t datalen;
  uint32_t bitlen[2];
  uint32_t state[8];
} SHA256_CTX;

static void sha256_transform(SHA256_CTX *ctx, uint8_t *data);
static void sha256_init(SHA256_CTX *ctx);
static void sha256_update(SHA256_CTX *ctx, uint8_t *data, uint32_t len);
static void sha256_final(SHA256_CTX *ctx, uint8_t *hash);
extern void sha256_hash(const char *data, size_t len, char *res);

#endif
