
cert=$1
[ -f "$cert" ] || (echo "$0: Provide path to certificate as argument."; exit 2;)

openssl x509 -sha1 -in $cert -noout -fingerprint | tr -d ':'
openssl x509 -sha224 -in $cert -noout -fingerprint | tr -d ':'
openssl x509 -sha256 -in $cert -noout -fingerprint | tr -d ':'
openssl x509 -sha512 -in $cert -noout -fingerprint | tr -d ':'

exit $?
