#include "tls_errors.h"

/** @file tls_errors.c 
    @brief temp_tls* routines are used for error messges
*/

/* TLS error messages */

void temp_tlscertfp() { 
  out("Z Received X.509 certificate from: ");
  outsafe(&host); 
  out (" does not match fingerprint: "); 
  outsafe(&cafile); 
  out(". (#4.4.1)\n"); zerodie(); 
}

void temp_invaliddigest() { 
  out("Z Invalid digest length provided for: ");
  outsafe(&host); 
  out(". (#4.4.1)\n"); zerodie(); 
}

void temp_tlsdigest() { 
  out("Z Received X.509 certificate from: ");
  outsafe(&host); 
  out (" posses an unknown digest method"); 
  out(". (#4.4.1)\n"); zerodie(); 
}

void temp_tlscert() { 
  out("Z Can't load X.509 certificate: "); 
  outsafe(&certfile); 
  out(". (#4.4.1)\n"); zerodie(); 
}

void temp_tlskey() { 
  out("Z Can't load X.509 private key: "); 
  outsafe(&keyfile); 
  out(". (#4.4.1)\n"); 
  zerodie(); 
}

void temp_tlschk() { 
  out("Z Keyfile does not match X.509 certificate: "); 
  outsafe(&keypwd); 
  out(". (#4.4.1)\n"); 
  zerodie(); 
}

void temp_tlsctx() { 
  out("Z I wasn't able to create TLS context for: ");
  outsafe(&host); 
  out(". (#4.4.1)\n"); 
  zerodie(); 
}

void temp_tlscipher() {
  out("Z I wasn't able to process the TLS ciphers: "); 
  outsafe(&ciphers); 
  out (" (#4.4.1)\n"); 
  zerodie(); 
}

void temp_tlsca() { 
  out("Z I wasn't able to set up CAFILE: "); 
  outsafe(&cafile); 
  out(" or CADIR: ");
  outsafe(&cadir); 
  out(" for TLS. (#4.4.1)\n"); 
  zerodie(); 
}

void temp_tlspeercert() { 
  out("Z Unable to obtain X.500 certificate from: "); 
  outsafe(&host); 
  out(". (#4.4.1)\n"); 
  zerodie(); 
}

void temp_tlspeervalid() { 
  out("Z Unable to validate X.500 certificate Subject for: "); 
  outsafe(&host); 
  out(". (#4.4.1)\n"); 
  zerodie(); 
}

void temp_tlspeerverify() { 
  out("Z Unable to verify X.500 certificate from: "); 
  outsafe(&host); 
  out(". (#4.4.1)\n"); 
  zerodie(); 
}

void temp_tlscon() { 
  out("Z I wasn't able to establish a TLS connection with: "); 
  outsafe(&host); 
  out(". (#4.4.1)\n"); 
  zerodie(); 
}

void temp_tlserr() { 
  out("Z TLS connection/protocol error for host: "); 
  outsafe(&host); 
  out(". (#4.4.1)\n"); 
  zerodie(); 
}

void temp_tlsexit() { 
  out("Z I wasn't able to gracefully close the TLS connection with: "); 
  outsafe(&host); 
  out(". (#4.4.1)\n"); 
  zerodie(); 
}

void temp_tlshost() { 
  out("Z I wasn't able to negotiate a TLS connection with: "); 
  outsafe(&host); 
  out(". (#4.4.1)\n"); 
  zerodie(); 
}
