/* Output of mkerrcodes.awk.  DO NOT EDIT.  */

static struct
  {
    int err;
    const char *err_sym;
  } err_table[] = 
{
  { 7, "GPG_ERR_E2BIG" },
  { 13, "GPG_ERR_EACCES" },
  { 10013L, "GPG_ERR_EACCES" },
  { 100, "GPG_ERR_EADDRINUSE" },
  { 10048L, "GPG_ERR_EADDRINUSE" },
  { 101, "GPG_ERR_EADDRNOTAVAIL" },
  { 10049L, "GPG_ERR_EADDRNOTAVAIL" },
  { 102, "GPG_ERR_EAFNOSUPPORT" },
  { 10047L, "GPG_ERR_EAFNOSUPPORT" },
  { 11, "GPG_ERR_EAGAIN" },
  { 103, "GPG_ERR_EALREADY" },
  { 10037L, "GPG_ERR_EALREADY" },
  { 9, "GPG_ERR_EBADF" },
  { 10009L, "GPG_ERR_EBADF" },
  { 104, "GPG_ERR_EBADMSG" },
  { 16, "GPG_ERR_EBUSY" },
  { 105, "GPG_ERR_ECANCELED" },
  { 10, "GPG_ERR_ECHILD" },
  { 106, "GPG_ERR_ECONNABORTED" },
  { 10053L, "GPG_ERR_ECONNABORTED" },
  { 107, "GPG_ERR_ECONNREFUSED" },
  { 10061L, "GPG_ERR_ECONNREFUSED" },
  { 108, "GPG_ERR_ECONNRESET" },
  { 10054L, "GPG_ERR_ECONNRESET" },
  { 36, "GPG_ERR_EDEADLK" },
  { 36, "GPG_ERR_EDEADLOCK" },
  { 109, "GPG_ERR_EDESTADDRREQ" },
  { 10039L, "GPG_ERR_EDESTADDRREQ" },
  { 33, "GPG_ERR_EDOM" },
  { 10069L, "GPG_ERR_EDQUOT" },
  { 17, "GPG_ERR_EEXIST" },
  { 14, "GPG_ERR_EFAULT" },
  { 10014L, "GPG_ERR_EFAULT" },
  { 27, "GPG_ERR_EFBIG" },
  { 10064L, "GPG_ERR_EHOSTDOWN" },
  { 110, "GPG_ERR_EHOSTUNREACH" },
  { 10065L, "GPG_ERR_EHOSTUNREACH" },
  { 111, "GPG_ERR_EIDRM" },
  { 42, "GPG_ERR_EILSEQ" },
  { 112, "GPG_ERR_EINPROGRESS" },
  { 10036L, "GPG_ERR_EINPROGRESS" },
  { 4, "GPG_ERR_EINTR" },
  { 10004L, "GPG_ERR_EINTR" },
  { 22, "GPG_ERR_EINVAL" },
  { 10022L, "GPG_ERR_EINVAL" },
  { 5, "GPG_ERR_EIO" },
  { 113, "GPG_ERR_EISCONN" },
  { 10056L, "GPG_ERR_EISCONN" },
  { 21, "GPG_ERR_EISDIR" },
  { 114, "GPG_ERR_ELOOP" },
  { 10062L, "GPG_ERR_ELOOP" },
  { 24, "GPG_ERR_EMFILE" },
  { 10024L, "GPG_ERR_EMFILE" },
  { 31, "GPG_ERR_EMLINK" },
  { 115, "GPG_ERR_EMSGSIZE" },
  { 10040L, "GPG_ERR_EMSGSIZE" },
  { 38, "GPG_ERR_ENAMETOOLONG" },
  { 10063L, "GPG_ERR_ENAMETOOLONG" },
  { 116, "GPG_ERR_ENETDOWN" },
  { 10050L, "GPG_ERR_ENETDOWN" },
  { 117, "GPG_ERR_ENETRESET" },
  { 10052L, "GPG_ERR_ENETRESET" },
  { 118, "GPG_ERR_ENETUNREACH" },
  { 10051L, "GPG_ERR_ENETUNREACH" },
  { 23, "GPG_ERR_ENFILE" },
  { 119, "GPG_ERR_ENOBUFS" },
  { 10055L, "GPG_ERR_ENOBUFS" },
  { 120, "GPG_ERR_ENODATA" },
  { 19, "GPG_ERR_ENODEV" },
  { 2, "GPG_ERR_ENOENT" },
  { 8, "GPG_ERR_ENOEXEC" },
  { 39, "GPG_ERR_ENOLCK" },
  { 121, "GPG_ERR_ENOLINK" },
  { 12, "GPG_ERR_ENOMEM" },
  { 122, "GPG_ERR_ENOMSG" },
  { 123, "GPG_ERR_ENOPROTOOPT" },
  { 10042L, "GPG_ERR_ENOPROTOOPT" },
  { 28, "GPG_ERR_ENOSPC" },
  { 124, "GPG_ERR_ENOSR" },
  { 125, "GPG_ERR_ENOSTR" },
  { 40, "GPG_ERR_ENOSYS" },
  { 126, "GPG_ERR_ENOTCONN" },
  { 10057L, "GPG_ERR_ENOTCONN" },
  { 20, "GPG_ERR_ENOTDIR" },
  { 41, "GPG_ERR_ENOTEMPTY" },
  { 10066L, "GPG_ERR_ENOTEMPTY" },
  { 128, "GPG_ERR_ENOTSOCK" },
  { 10038L, "GPG_ERR_ENOTSOCK" },
  { 129, "GPG_ERR_ENOTSUP" },
  { 25, "GPG_ERR_ENOTTY" },
  { 6, "GPG_ERR_ENXIO" },
  { 130, "GPG_ERR_EOPNOTSUPP" },
  { 10045L, "GPG_ERR_EOPNOTSUPP" },
  { 132, "GPG_ERR_EOVERFLOW" },
  { 1, "GPG_ERR_EPERM" },
  { 10046L, "GPG_ERR_EPFNOSUPPORT" },
  { 32, "GPG_ERR_EPIPE" },
  { 10067L, "GPG_ERR_EPROCLIM" },
  { 134, "GPG_ERR_EPROTO" },
  { 135, "GPG_ERR_EPROTONOSUPPORT" },
  { 10043L, "GPG_ERR_EPROTONOSUPPORT" },
  { 136, "GPG_ERR_EPROTOTYPE" },
  { 10041L, "GPG_ERR_EPROTOTYPE" },
  { 34, "GPG_ERR_ERANGE" },
  { 10071L, "GPG_ERR_EREMOTE" },
  { 30, "GPG_ERR_EROFS" },
  { 10058L, "GPG_ERR_ESHUTDOWN" },
  { 10044L, "GPG_ERR_ESOCKTNOSUPPORT" },
  { 29, "GPG_ERR_ESPIPE" },
  { 3, "GPG_ERR_ESRCH" },
  { 10070L, "GPG_ERR_ESTALE" },
  { 137, "GPG_ERR_ETIME" },
  { 138, "GPG_ERR_ETIMEDOUT" },
  { 10060L, "GPG_ERR_ETIMEDOUT" },
  { 10059L, "GPG_ERR_ETOOMANYREFS" },
  { 139, "GPG_ERR_ETXTBSY" },
  { 10068L, "GPG_ERR_EUSERS" },
  { 140, "GPG_ERR_EWOULDBLOCK" },
  { 10035L, "GPG_ERR_EWOULDBLOCK" },
  { 18, "GPG_ERR_EXDEV" },
};
