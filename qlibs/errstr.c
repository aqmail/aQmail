/*  Replacement for error_str.c, extended replacement of 'error_temp'
 *
 *  Revision 20170420, Kai Peter
 *  - new file:
*/
#include "error.h"

#define X(e,s) if (code == e) return s;   /* genial! ;-) */

char *errstr(int code)
{
  X(0,"no error")
  X(ESOFT,"internal code")   // djb compat
  X(EHARD,"internal code")   // djb compat
  X(error_intr,"interrupted system call")        // EINTR
  X(EINTR,            "interrupted system call")
  X(error_nomem,"out of memory")                 // ENOMEM
  X(ENOMEM,           "out of memory")
  X(error_noent,"file does not exist")           // ENOENT
  X(ENOENT,           "file does not exist")
  X(error_txtbsy,"text busy")                    // ETXTBSY
  X(ETXTBSY,          "text busy")
  X(error_io,"input/output error")               // EIO
  X(EIO,              "input/output error")
  X(error_exist,"file already exists")           // EEXISTS
  X(EEXIST,           "file already exists")
  X(error_timeout,"timed out")                   // ETIMEDOUT
  X(ETIMEDOUT,        "timed out")
  X(error_inprogress,"operation in progress")    // EINPROGRESS
  X(EINPROGRESS,      "operation in progress")
  X(error_again,"temporary failure")             // EAGAIN
  X(EAGAIN,           "temporary failure")
  X(error_wouldblock,"input/output would block") // EWOULDBLOCK (intern EAGAIN)
  X(EWOULDBLOCK,      "input/output would block")
  X(error_pipe,"broken pipe")                    // EPIPE
  X(EPIPE,            "broken pipe")
  X(error_perm,"permission denied")              // EPERM
  X(EPERM,            "permission denied")
  X(error_acces,"access denied")                 // EACCES
  X(EACCES,           "access denied")

  X(ESRCH,            "no such process")

//  X(error_nodevice,"device not configured")      // ENXIO
  X(ENXIO,            "device not configured")

  X(E2BIG,            "argument list too long")
  X(ENOEXEC,          "exec format error")
  X(EBADF,            "file descriptor not open")
  X(ECHILD,           "no child processes")
  X(EDEADLK,          "operation would cause deadlock")
  X(EFAULT,           "bad address")
  X(ENOTBLK,          "not a block device")
  X(EBUSY,            "device busy")
  X(EXDEV,            "cross-device link")
  X(ENODEV,           "device does not support operation")
//  X(error_notdir,"not a directory")              // ENOTDIR
  X(ENOTDIR,          "not a directory")
  X(error_isdir,"is a directory")                // EISDIR
  X(EISDIR,           "is a directory")
  X(EINVAL,           "invalid argument")
  X(ENFILE,           "system cannot open more files")
  X(EMFILE,           "process cannot open more files")
  X(ENOTTY,           "not a tty")
  X(EFBIG,            "file too big")
  X(ENOSPC,           "out of disk space")
  X(ESPIPE,           "unseekable descriptor")
//  X(error_rofs,"read-only file system")          // EROFS
  X(EROFS,            "read-only file system")
  X(EMLINK,           "too many links")
  X(EDOM,             "input out of range")
  X(ERANGE,           "output out of range")
  X(EALREADY,         "operation already in progress")
  X(ENOTSOCK,         "not a socket")
  X(EDESTADDRREQ,     "destination address required")
  X(EMSGSIZE,         "message too long")
  X(EPROTOTYPE,       "incorrect protocol type")
  X(ENOPROTOOPT,      "protocol not available")
  X(EPROTONOSUPPORT,  "protocol not supported")
  X(ESOCKTNOSUPPORT,  "socket type not supported")
  X(EOPNOTSUPP,       "operation not supported")
  X(EPFNOSUPPORT,     "protocol family not supported")
  X(EAFNOSUPPORT,     "address family not supported")
  X(EADDRINUSE,       "address already used")
  X(EADDRNOTAVAIL,    "address not available")
  X(ENETDOWN,         "network down")
  X(ENETUNREACH,      "network unreachable")
  X(ENETRESET,        "network reset")
  X(ECONNABORTED,     "connection aborted")
  X(error_connreset,"connection reset")          // ECONNRESET
  X(ECONNRESET,       "connection reset")
  X(ENOBUFS,          "out of buffer space")
  X(EISCONN,          "already connected")
  X(ENOTCONN,         "not connected")
  X(ESHUTDOWN,        "socket shut down")
  X(ETOOMANYREFS,     "too many references")
  X(error_connrefused,"connection refused")      // ECONNREFUSED
  X(ECONNREFUSED,     "connection refused")
  X(ELOOP,            "symbolic link loop")
  X(ENAMETOOLONG,     "file name too long")
  X(EHOSTDOWN,        "host down")
  X(EHOSTUNREACH,     "host unreachable")
  X(ENOTEMPTY,        "directory not empty")
  X(EUSERS,           "too many users")
  X(EDQUOT,           "disk quota exceeded")
  X(ESTALE,           "stale NFS file handle")

  /* BSD only (all BSD's, NOT on Linux) */
//  X(EPROCLIM,         "too many processes")			// -L +FB +OB +NB
//  X(EBADRPC,          "RPC structure is bad")			// -L +FB +OB +NB
//  X(ERPCMISMATCH,     "RPC version mismatch")			// -L +FB +OB +NB
//  X(EPROGUNAVAIL,     "RPC program unavailable")		// -L +FB +OB +NB
//  X(EPROGMISMATCH,    "program version mismatch")		// -L +FB +OB +NB
//  X(EPROCUNAVAIL,     "bad procedure for program")	// -L +FB +OB +NB
//  X(EFTYPE,           "bad file type")				// -L +FB +OB +NB

  X(ENOLCK,           "no locks available")
  X(ENOSYS,           "system call not available")
  X(ENOMSG,           "no message of desired type")
  X(EIDRM,            "identifier removed")

//  X(ERREMOTE,         "object not local")				// -L -FB -OB -NB
  X(EREMOTE,          "object not local")     // Linux: "Object is remote"
//  X(EREMOTE,          "too many levels of remote in path")

  /* Linux only */
//  X(ENONET,           "machine not on network")		// +L -FB -OB -NB
//  X(EADV,             "advertise error")				// +L -FB -OB -NB
//  X(ESRMNT,           "srmount error")				// +L -FB -OB -NB
//  X(ECOMM,            "communication error")			// +L -FB -OB -NB
//  X(EREMCHG,          "remote address changed")		// +L -FB -OB -NB

  X(error_proto,"protocol error")                // EPROTO
  /* EPROTO: see 'error.h' for OpenBSD compat */
  X(EPROTO,           "protocol error")					// +L +FB -OB +NB

  /* Linux and NetBSD */
//  X(ENOSTR,           "not a stream device")			// +L -FB -OB +NB
//  X(ETIME,            "timer expired")				// +L -FB -OB +NB
//  X(ENOSR,            "out of stream resources")		// +L -FB -OB +NB

  /* FreeBSD and NetBSD */
//  X(EAUTH,            "authentication error")			// -L +FB -OB +NB
//  X(ENEEDAUTH,        "not authenticated")			// -L +FB -OB +NB

  /* NOT on OpenBSD */
//  X(EBADMSG,          "bad message type")				// +L +FB -OB +NB
//  X(ENOLINK,          "link severed")					// +L +FB -OB +NB
//  X(EMULTIHOP,        "multihop attempted")			// +L +FB -OB +NB


  return "unknown error";   /* worst case */
}
