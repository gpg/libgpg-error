/* gpg-error.h - Public interface to libgpg-error.
   Copyright (C) 2003 g10 Code GmbH

   This file is part of libgpg-error.
 
   libgpg-error is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.
 
   libgpg-error is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
 
   You should have received a copy of the GNU Lesser General Public
   License along with libgpgme-error; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#ifndef GPG_ERROR_H
#define GPG_ERROR_H	1

/* The GnuPG project consists of many components.  Error codes are
   exchanged between all components.  The common error codes and their
   user-presentable descriptions are kept into a shared library to
   allow adding new error codes and components without recompiling any
   of the other components.  The interface will not change in a
   backward incompatible way.

   An error code together with an error source build up an error
   value.  As the error value is been passed from one component to
   another, it preserver the information about the source and nature
   of the error.

   A component of the GnuPG project can define the following macro to
   tune the behaviour of the library:

   GPG_ERR_SOURCE_DEFAULT: Define to an error source of type
   gpg_err_source_t to make that source the default for gpg_error().
   Otherwise GPG_ERR_SOURCE_UNKNOWN is used as default.  */


/* The error source type gpg_err_source_t.

   Where as the Poo out of a welle small
   Taketh his firste springing and his sours.
					--Chaucer.  */

/* Only use free slots, never change or reorder the existing
   entries.  */
typedef enum
  {
    GPG_ERR_SOURCE_UNKNOWN = 0,
    GPG_ERR_SOURCE_GCRYPT = 1,
    GPG_ERR_SOURCE_GPG = 2,
    GPG_ERR_SOURCE_GPGSM = 3,
    GPG_ERR_SOURCE_GPGAGENT = 4,
    GPG_ERR_SOURCE_PINENTRY = 5,
    GPG_ERR_SOURCE_SCD = 6,
    GPG_ERR_SOURCE_GPGME = 7,
    GPG_ERR_SOURCE_KEYBOX = 8,

    /* 9 to 31 are free to be used.  */

    /* Free for use by non-GnuPG components.  */
    GPG_ERR_SOURCE_USER_1 = 32,
    GPG_ERR_SOURCE_USER_2 = 33,
    GPG_ERR_SOURCE_USER_3 = 34,
    GPG_ERR_SOURCE_USER_4 = 35,

    /* 36 to 255 are free to be used.  */

    /* This is one more than the largest allowed entry.  */
    GPG_ERR_SOURCE_DIM = 256
  } gpg_err_source_t;


/* The error code type gpg_err_code_t.  */

/* Only use free slots, never change or reorder the existing
   entries.  */
typedef enum
  {
    GPG_ERR_NO_ERROR = 0,		/* An error that is not an error.  */
    GPG_ERR_GENERAL = 1,
    GPG_ERR_UNKNOWN_PACKET = 2,
    GPG_ERR_UNKNOWN_VERSION = 3,	/* Unknown version (in packet).  */
    GPG_ERR_PUBKEY_ALGO	= 4,		/* Invalid public key algorithm.  */
    GPG_ERR_DIGEST_ALGO = 5,		/* Invalid digest algorithm.  */
    GPG_ERR_BAD_PUBKEY = 6,		/* Bad public key.  */
    GPG_ERR_BAD_SECKEY = 7,		/* Bad secret key.  */
    GPG_ERR_BAD_SIGNATURE = 8,		/* Bad signature.  */
    GPG_ERR_NO_PUBKEY = 9,		/* Public key not found.  */
    GPG_ERR_CHECKSUM = 10,		/* Checksum error.  */
    GPG_ERR_BAD_PASSPHRASE = 11,	/* Bad passphrase.  */
    GPG_ERR_CIPHER_ALGO = 12,		/* Invalid cipher algorithm.  */
    GPG_ERR_KEYRING_OPEN = 13,
    GPG_ERR_INV_PACKET = 14,
    GPG_ERR_INV_ARMOR = 15,
    GPG_ERR_NO_USER_ID = 16,
    GPG_ERR_NO_SECKEY = 17,		/* Secret key not available.  */
    GPG_ERR_WRONG_SECKEY = 18,		/* Wrong secret key used.  */
    GPG_ERR_BAD_KEY = 19,		/* Bad (session) key.  */
    GPG_ERR_COMPR_ALGO = 20,		/* Unknown compress algorithm.  */

    /* Codes 21 to 29 are free to be used.  */

    GPG_ERR_BAD_MPI = 30,		/* Problem with an MPI's value.  */
    GPG_ERR_INV_PASSPHRASE = 31,	/* Invalid passphrase.  */
    GPG_ERR_SIG_CLASS = 32,
    GPG_ERR_RESOURCE_LIMIT = 33,
    GPG_ERR_INV_KEYRING = 34,
    GPG_ERR_TRUSTDB = 35,		/* A problem with the trustdb.  */
    GPG_ERR_BAD_CERT = 36,		/* Bad certificate.  */
    GPG_ERR_INV_USER_ID = 37,
    GPG_ERR_UNEXPECTED = 38,
    GPG_ERR_TIME_CONFLICT = 39,
    GPG_ERR_KEYSERVER = 40,
    GPG_ERR_WRONG_PUBKEY_ALGO = 41,	/* Wrong public key algorithm.  */
    GPG_ERR_TRIBUTE_TO_D_A = 42,
    GPG_ERR_WEAK_KEY = 43,		/* Weak encryption key.  */
    GPG_ERR_INV_KEYLEN = 44,		/* Invalid length of a key.  */
    GPG_ERR_INV_ARG = 45,		/* Invalid argument.  */
    GPG_ERR_BAD_URI = 46,		/* Syntax error in URI.  */
    GPG_ERR_INV_URI = 47,		/* Unsupported scheme and similar.  */
    GPG_ERR_NETWORK = 48,		/* General network error.  */
    GPG_ERR_UNKNOWN_HOST = 49,
    GPG_ERR_SELFTEST_FAILED = 50,
    GPG_ERR_NOT_ENCRYPTED = 51,
    GPG_ERR_NOT_PROCESSED = 52,
    GPG_ERR_UNUSABLE_PUBKEY = 53,
    GPG_ERR_UNUSABLE_SECKEY = 54,
    GPG_ERR_INV_VALUE = 55,
    GPG_ERR_BAD_CERT_CHAIN = 56,
    GPG_ERR_MISSING_CERT = 57,
    GPG_ERR_NO_DATA = 58,
    GPG_ERR_BUG = 59,
    GPG_ERR_NOT_SUPPORTED = 60,
    GPG_ERR_INV_OP = 61,		/* Invalid operation code.  */
    GPG_ERR_TIMEOUT = 62,               /* Something timed out. */
    GPG_ERR_INTERNAL = 63,		/* Internal error.  */
    GPG_ERR_EOF_GCRYPT = 64,		/* Compatibility for gcrypt.  */
    GPG_ERR_INV_OBJ = 65,		/* An object is not valid.  */
    GPG_ERR_TOO_SHORT = 66,		/* Provided object is too short.  */
    GPG_ERR_TOO_LARGE = 67,		/* Provided object is too large.  */
    GPG_ERR_NO_OBJ = 68,		/* Missing item in an object.  */
    GPG_ERR_NOT_IMPLEMENTED = 69,	/* Not implemented.  */
    GPG_ERR_CONFLICT = 70,		/* Conflicting use.  */
    GPG_ERR_INV_CIPHER_MODE = 71,	/* Invalid cipher mode.  */ 
    GPG_ERR_INV_FLAG = 72,		/* Invalid flag.  */
    GPG_ERR_INV_HANDLE = 73,            /* Invalid handle.  */

    /* Code 74 is free to be used.  */

    GPG_ERR_INCOMPLETE_LINE = 75,
    GPG_ERR_INV_RESPONSE = 76,
    GPG_ERR_NO_AGENT = 77,
    GPG_ERR_AGENT = 78,
    GPG_ERR_INV_DATA = 79,
    GPG_ERR_ASSUAN_SERVER_FAULT = 80,
    GPG_ERR_ASSUAN = 81,		/* Catch all assuan error.  */
    GPG_ERR_INV_SESSION_KEY = 82,
    GPG_ERR_INV_SEXP = 83,
    GPG_ERR_UNSUPPORTED_ALGORITHM = 84,
    GPG_ERR_NO_PIN_ENTRY = 85,
    GPG_ERR_PIN_ENTRY = 86,
    GPG_ERR_BAD_PIN = 87,
    GPG_ERR_INV_NAME = 88,
    GPG_ERR_BAD_DATA = 89,
    GPG_ERR_INV_PARAMETER = 90,

    GPG_ERR_WRONG_CARD = 91, 
    
    GPG_ERR_NO_DIRMNGR = 92,
    GPG_ERR_DIRMNGR = 93,
    GPG_ERR_CERT_REVOKED = 94,
    GPG_ERR_NO_CRL_KNOWN = 95,
    GPG_ERR_CRL_TOO_OLD = 96,
    GPG_ERR_LINE_TOO_LONG = 97,
    GPG_ERR_NOT_TRUSTED = 98,
    GPG_ERR_CANCELED = 99,
    GPG_ERR_BAD_CA_CERT = 100,
    GPG_ERR_CERT_EXPIRED = 101,		/* Key signature expired.  */
    GPG_ERR_CERT_TOO_YOUNG = 102,
    GPG_ERR_UNSUPPORTED_CERT = 103,
    GPG_ERR_UNKNOWN_SEXP = 104,
    GPG_ERR_UNSUPPORTED_PROTECTION = 105,
    GPG_ERR_CORRUPTED_PROTECTION = 106,
    GPG_ERR_AMBIGUOUS_NAME = 107,
    GPG_ERR_CARD = 108,
    GPG_ERR_CARD_RESET = 109,
    GPG_ERR_CARD_REMOVED = 110,
    GPG_ERR_INV_CARD = 111,
    GPG_ERR_CARD_NOT_PRESENT = 112,
    GPG_ERR_NO_PKCS15_APP = 113,
    GPG_ERR_NOT_CONFIRMED = 114,
    GPG_ERR_CONFIGURATION = 115,
    GPG_ERR_NO_POLICY_MATCH = 116,
    GPG_ERR_INV_INDEX = 117,
    GPG_ERR_INV_ID = 118,
    GPG_ERR_NO_SCDAEMON = 119,
    GPG_ERR_SCDAEMON = 120,
    GPG_ERR_UNSUPPORTED_PROTOCOL = 121,
    GPG_ERR_BAD_PIN_METHOD = 122,
    GPG_ERR_CARD_NOT_INITIALIZED = 123,
    GPG_ERR_UNSUPPORTED_OPERATION = 124,
    GPG_ERR_WRONG_KEY_USAGE = 125,
    GPG_ERR_NOTHING_FOUND = 126,        /* Operation failed due to an
                                           unsuccessful find operation.  */
    GPG_ERR_WRONG_BLOB_TYPE = 127,      /* Keybox BLOB of wrong type.  */
    GPG_ERR_MISSING_VALUE = 128,        /* A required value is missing.  */

    /* 129 to 149 are free to be used.  */

    GPG_ERR_INV_ENGINE = 150,
    GPG_ERR_PUBKEY_NOT_TRUSTED = 151,
    GPG_ERR_DECRYPT_FAILED = 152,
    GPG_ERR_KEY_EXPIRED = 153,
    GPG_ERR_SIG_EXPIRED = 154,		/* Data signature expired.  */

    /* 155 to 200 are free to be used.  */

    /* Error codes pertaining to S-expressions.  */
    GPG_ERR_SEXP_INV_LEN_SPEC = 201,
    GPG_ERR_SEXP_STRING_TOO_LONG = 202,
    GPG_ERR_SEXP_UNMATCHED_PAREN = 203, 
    GPG_ERR_SEXP_NOT_CANONICAL = 204, 
    GPG_ERR_SEXP_BAD_CHARACTER = 205, 
    GPG_ERR_SEXP_BAD_QUOTATION = 206,	/* Or invalid hex or octal value.  */
    GPG_ERR_SEXP_ZERO_PREFIX = 207,	/* First character of length is 0.  */
    GPG_ERR_SEXP_NESTED_DH = 208,	/* Nested display hints.  */
    GPG_ERR_SEXP_UNMATCHED_DH = 209,	/* Unmatched display hint.  */
    GPG_ERR_SEXP_UNEXPECTED_PUNC = 210,	/* Unexpected reserved punctuation. */
    GPG_ERR_SEXP_BAD_HEX_CHAR = 211,
    GPG_ERR_SEXP_ODD_HEX_NUMBERS = 212,
    GPG_ERR_SEXP_BAD_OCT_CHAR = 213,

    /* 213 to 1023 are free to be used.  */

    /* For free use by non-GnuPG components.  */
    GPG_ERR_USER_1 = 1024,
    GPG_ERR_USER_2 = 1025,
    GPG_ERR_USER_3 = 1026,
    GPG_ERR_USER_4 = 1027,
    GPG_ERR_USER_5 = 1028,
    GPG_ERR_USER_6 = 1029,
    GPG_ERR_USER_7 = 1030,
    GPG_ERR_USER_8 = 1031,
    GPG_ERR_USER_9 = 1032,
    GPG_ERR_USER_10 = 1033,
    GPG_ERR_USER_11 = 1034,
    GPG_ERR_USER_12 = 1035,
    GPG_ERR_USER_13 = 1036,
    GPG_ERR_USER_14 = 1037,
    GPG_ERR_USER_15 = 1038,
    GPG_ERR_USER_16 = 1039,

    /* 1040 to 16381 are free to be used.  */

    GPG_ERR_UNKNOWN_ERRNO = 16382,
    GPG_ERR_EOF = 16383,		/* This was once a -1.  Pity.  */

    /* The following error codes are used to map system errors.  */
    GPG_ERR_E2BIG = 16384,
    GPG_ERR_EACCES = 16385,
    GPG_ERR_EADDRINUSE = 16386,
    GPG_ERR_EADDRNOTAVAIL = 16387,
    GPG_ERR_EADV = 16388,
    GPG_ERR_EAFNOSUPPORT = 16389,
    GPG_ERR_EAGAIN = 16390,
    GPG_ERR_EALREADY = 16391,
    GPG_ERR_EAUTH = 16392,
    GPG_ERR_EBACKGROUND = 16393,
    GPG_ERR_EBADE = 16394,
    GPG_ERR_EBADF = 16395,
    GPG_ERR_EBADFD = 16396,
    GPG_ERR_EBADMSG = 16397,
    GPG_ERR_EBADR = 16398,
    GPG_ERR_EBADRPC = 16399,
    GPG_ERR_EBADRQC = 16400,
    GPG_ERR_EBADSLT = 16401,
    GPG_ERR_EBFONT = 16402,
    GPG_ERR_EBUSY = 16403,
    GPG_ERR_ECANCELED = 16404,
    GPG_ERR_ECHILD = 16405,
    GPG_ERR_ECHRNG = 16406,
    GPG_ERR_ECOMM = 16407,
    GPG_ERR_ECONNABORTED = 16408,
    GPG_ERR_ECONNREFUSED = 16409,
    GPG_ERR_ECONNRESET = 16410,
    GPG_ERR_ED = 16411,
    GPG_ERR_EDEADLK = 16412,
    GPG_ERR_EDEADLOCK = 16413,
    GPG_ERR_EDESTADDRREQ = 16414,
    GPG_ERR_EDIED = 16415,
    GPG_ERR_EDOM = 16416,
    GPG_ERR_EDOTDOT = 16417,
    GPG_ERR_EDQUOT = 16418,
    GPG_ERR_EEXIST = 16419,
    GPG_ERR_EFAULT = 16420,
    GPG_ERR_EFBIG = 16421,
    GPG_ERR_EFTYPE = 16422,
    GPG_ERR_EGRATUITOUS = 16423,
    GPG_ERR_EGREGIOUS = 16424,
    GPG_ERR_EHOSTDOWN = 16425,
    GPG_ERR_EHOSTUNREACH = 16426,
    GPG_ERR_EIDRM = 16427,
    GPG_ERR_EIEIO = 16428,
    GPG_ERR_EILSEQ = 16429,
    GPG_ERR_EINPROGRESS = 16430,
    GPG_ERR_EINTR = 16431,
    GPG_ERR_EINVAL = 16432,
    GPG_ERR_EIO = 16433,
    GPG_ERR_EISCONN = 16434,
    GPG_ERR_EISDIR = 16435,
    GPG_ERR_EISNAM = 16436,
    GPG_ERR_EL2HLT = 16437,
    GPG_ERR_EL2NSYNC = 16438,
    GPG_ERR_EL3HLT = 16439,
    GPG_ERR_EL3RST = 16440,
    GPG_ERR_ELIBACC = 16441,
    GPG_ERR_ELIBBAD = 16442,
    GPG_ERR_ELIBEXEC = 16443,
    GPG_ERR_ELIBMAX = 16444,
    GPG_ERR_ELIBSCN = 16445,
    GPG_ERR_ELNRNG = 16446,
    GPG_ERR_ELOOP = 16447,
    GPG_ERR_EMEDIUMTYPE = 16448,
    GPG_ERR_EMFILE = 16449,
    GPG_ERR_EMLINK = 16450,
    GPG_ERR_EMSGSIZE = 16451,
    GPG_ERR_EMULTIHOP = 16452,
    GPG_ERR_ENAMETOOLONG = 16453,
    GPG_ERR_ENAVAIL = 16454,
    GPG_ERR_ENEEDAUTH = 16455,
    GPG_ERR_ENETDOWN = 16456,
    GPG_ERR_ENETRESET = 16457,
    GPG_ERR_ENETUNREACH = 16458,
    GPG_ERR_ENFILE = 16459,
    GPG_ERR_ENOANO = 16460,
    GPG_ERR_ENOBUFS = 16461,
    GPG_ERR_ENOCSI = 16462,
    GPG_ERR_ENODATA = 16463,
    GPG_ERR_ENODEV = 16464,
    GPG_ERR_ENOENT = 16465,
    GPG_ERR_ENOEXEC = 16466,
    GPG_ERR_ENOLCK = 16467,
    GPG_ERR_ENOLINK = 16468,
    GPG_ERR_ENOMEDIUM = 16469,
    GPG_ERR_ENOMEM = 16470,
    GPG_ERR_ENOMSG = 16471,
    GPG_ERR_ENONET = 16472,
    GPG_ERR_ENOPKG = 16473,
    GPG_ERR_ENOPROTOOPT = 16474,
    GPG_ERR_ENOSPC = 16475,
    GPG_ERR_ENOSR = 16476,
    GPG_ERR_ENOSTR = 16477,
    GPG_ERR_ENOSYS = 16478,
    GPG_ERR_ENOTBLK = 16479,
    GPG_ERR_ENOTCONN = 16480,
    GPG_ERR_ENOTDIR = 16481,
    GPG_ERR_ENOTEMPTY = 16482,
    GPG_ERR_ENOTNAM = 16483,
    GPG_ERR_ENOTSOCK = 16484,
    GPG_ERR_ENOTSUP = 16485,
    GPG_ERR_ENOTTY = 16486,
    GPG_ERR_ENOTUNIQ = 16487,
    GPG_ERR_ENXIO = 16488,
    GPG_ERR_EOPNOTSUPP = 16489,
    GPG_ERR_EOVERFLOW = 16490,
    GPG_ERR_EPERM = 16491,
    GPG_ERR_EPFNOSUPPORT = 16492,
    GPG_ERR_EPIPE = 16493,
    GPG_ERR_EPROCLIM = 16494,
    GPG_ERR_EPROCUNAVAIL = 16495,
    GPG_ERR_EPROGMISMATCH = 16496,
    GPG_ERR_EPROGUNAVAIL = 16497,
    GPG_ERR_EPROTO = 16498,
    GPG_ERR_EPROTONOSUPPORT = 16499,
    GPG_ERR_EPROTOTYPE = 16500,
    GPG_ERR_ERANGE = 16501,
    GPG_ERR_EREMCHG = 16502,
    GPG_ERR_EREMOTE = 16503,
    GPG_ERR_EREMOTEIO = 16504,
    GPG_ERR_ERESTART = 16505,
    GPG_ERR_EROFS = 16506,
    GPG_ERR_ERPCMISMATCH = 16507,
    GPG_ERR_ESHUTDOWN = 16508,
    GPG_ERR_ESOCKTNOSUPPORT = 16509,
    GPG_ERR_ESPIPE = 16510,
    GPG_ERR_ESRCH = 16511,
    GPG_ERR_ESRMNT = 16512,
    GPG_ERR_ESTALE = 16513,
    GPG_ERR_ESTRPIPE = 16514,
    GPG_ERR_ETIME = 16515,
    GPG_ERR_ETIMEDOUT = 16516,
    GPG_ERR_ETOOMANYREFS = 16517,
    GPG_ERR_ETXTBSY = 16518,
    GPG_ERR_EUCLEAN = 16519,
    GPG_ERR_EUNATCH = 16520,
    GPG_ERR_EUSERS = 16521,
    GPG_ERR_EWOULDBLOCK = 16522,
    GPG_ERR_EXDEV = 16523,
    GPG_ERR_EXFULL = 16524,

    /* 16525 to 32677 are free to be used for more system errors.  */

    /* This is one more than the largest allowed entry.  */
    GPG_ERR_CODE_DIM = 32768
  } gpg_err_code_t;


/* The error value type gpg_error_t.  */

/* We would really like to use bit-fields in a struct, but using
   structs as return values can cause binary compatibility issues, in
   particular if you want to do it effeciently (also see
   -freg-struct-return option to GCC).  */
typedef unsigned int gpg_error_t;

/* We use the lowest 16 bits of gpg_error_t for error codes.  The 17th
   bit indicates system errors.  */
#define GPG_ERR_CODE_MASK	(GPG_ERR_CODE_DIM - 1)
#define GPG_ERR_SYSTEM_ERROR	16384

/* Bits 18 to 24 are reserved.  */

/* We use the upper 8 bits of gpg_error_t for error sources.  */
#define GPG_ERR_SOURCE_MASK	(GPG_ERR_SOURCE_DIM - 1)
#define GPG_ERR_SOURCE_SHIFT	24


/* Constructor and accessor functions.  */

/* Construct an error value from an error code and source.  Within a
   subsystem, use gpg_error.  */
static __inline__ gpg_error_t
gpg_err_make (gpg_err_source_t source, gpg_err_code_t code)
{
  return code == GPG_ERR_NO_ERROR ? GPG_ERR_NO_ERROR
    : (((source & GPG_ERR_SOURCE_MASK) << GPG_ERR_SOURCE_SHIFT)
       | (code & GPG_ERR_CODE_MASK));
}


/* The user should define GPG_ERR_SOURCE_DEFAULT before including this
   file to specify a default source for gpg_error.  */
#ifndef GPG_ERR_SOURCE_DEFAULT
#define GPG_ERR_SOURCE_DEFAULT	GPG_ERR_SOURCE_UNKNOWN
#endif

static __inline__ gpg_error_t
gpg_error (gpg_err_code_t code)
{
  return gpg_err_make (GPG_ERR_SOURCE_DEFAULT, code);
}


/* Retrieve the error code from an error value.  */
static __inline__ gpg_err_code_t
gpg_err_code (gpg_error_t err)
{
  return err & GPG_ERR_CODE_MASK;
}


/* Retrieve the error source from an error value.  */
static __inline__ gpg_err_source_t
gpg_err_source (gpg_error_t err)
{
  return (err >> GPG_ERR_SOURCE_SHIFT) & GPG_ERR_SOURCE_MASK;
}


/* String functions.  */

/* Return a pointer to a string containing a description of the error
   code in the error value ERR.  */
const char *gpg_strerror (gpg_error_t err);

/* Return a pointer to a string containing a description of the error
   source in the error value ERR.  */
const char *gpg_strsource (gpg_error_t err);


/* Mapping of system errors (errno).  */

/* Retrieve the error code for the system error ERR.  This returns
   GPG_ERR_UNKNOWN_ERRNO if the system error is not mapped (report
   this).  */
gpg_err_code_t gpg_err_code_from_errno (int err);


/* Retrieve the system error for the error code CODE.  This returns 0
   if CODE is not a system error code.  */
int gpg_err_code_to_errno (gpg_err_code_t code);


/* Self-documenting convenience functions.  */

static __inline__ gpg_error_t
gpg_err_make_from_errno (gpg_err_source_t source, int err)
{
  return gpg_err_make (source, gpg_err_code_from_errno (err));
}


static __inline__ gpg_error_t
gpg_error_from_errno (int err)
{
  return gpg_error (gpg_err_code_from_errno (err));
}

#endif	/* GPG_ERROR_H */
