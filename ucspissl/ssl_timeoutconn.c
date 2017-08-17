#include "ucspissl.h"
#include "iopause.h"
#include "error.h"

int ssl_timeoutconn(SSL *ssl,unsigned int timeout)
{
  struct taia now;
  struct taia deadline;
  iopause_fd x;
  int r;
  int rfd;
  int wfd;

  if (taia_now(&now) == -1) {
    errno = error_timeout;
    return -1;
  }
  taia_uint(&deadline,timeout);
  taia_add(&deadline,&now,&deadline);

  rfd = SSL_get_fd(ssl); /* XXX */
  wfd = SSL_get_fd(ssl); /* XXX */

  SSL_set_connect_state(ssl);
  for (;;) {
    r = SSL_connect(ssl);
    errno = error_proto;
    if (r == 1) return 0;
    ssl_errno = SSL_get_error(ssl,r);
    if ((ssl_errno != SSL_ERROR_WANT_READ) && (ssl_errno != SSL_ERROR_WANT_WRITE))
      return -1;
    if (ssl_errno == SSL_ERROR_WANT_READ) {
      x.events = IOPAUSE_READ;
      x.fd = rfd;
      if (x.fd == -1) return -1;
    }
    else {
      x.events = IOPAUSE_WRITE;
      x.fd = wfd;
      if (x.fd == -1) return -1;
    }
    for (;;) {
      if (taia_now(&now) == -1) {
        errno = error_timeout;
        return -1;
      }
      iopause(&x,1,&deadline,&now);
      if (x.revents) break;
      if (taia_less(&deadline,&now))
    return -1;
    }
  }
}
