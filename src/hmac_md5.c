#include "global.h"
#include "md5.h"
#include "str.h"
#include "byte.h"

/**
@file   hmac_md5
@brief  caculates HMAC digest from challenge + password (DJB version)
@param input:  unsigned char *text : pointer to challenge
               int text_len        : length of challenge
               unsigned char *key  : pointer to password
               int key_len         : length of password
      output: unsigned char *digest: pointer to calculated digest 
*/

void hmac_md5(unsigned char *text,int text_len,unsigned char * key,int key_len,unsigned char *digest)
{
   MD5_CTX context;
   unsigned char k_ipad[65];    /* inner padding - key XORd with ipad */
   unsigned char k_opad[65];    /* outer padding - key XORd with opad */
   unsigned char tk[16];
   int i;

   if (key_len > 64) {
     MD5_CTX tctx;
     MD5Init(&tctx);
     MD5Update(&tctx,key,key_len);
     MD5Final(tk,&tctx);
     key = tk;
     key_len = 16;
   }

   byte_zero(k_ipad,sizeof(k_ipad));
   byte_zero(k_opad,sizeof(k_opad));
   byte_copy(k_ipad,key_len,key);
   byte_copy(k_opad,key_len,key);

   for (i = 0; i < 64; i++) {
      k_ipad[i] ^= 0x36;
      k_opad[i] ^= 0x5c;
   }

   MD5Init(&context);                   /* init context for 1st pass */
   MD5Update(&context,k_ipad,64);       /* start with inner pad */
   MD5Update(&context,text,text_len);   /* then text of datagram */
   MD5Final(digest,&context);           /* finish up 1st pass */

   MD5Init(&context);                   /* init context for 2nd pass */
   MD5Update(&context,k_opad,64);       /* start with outer pad */
   MD5Update(&context,digest,16);       /* then results of 1st hash */
   MD5Final(digest,&context);           /* finish up 2nd pass */
}
