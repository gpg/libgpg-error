/* gpg/error.h - Public interface to libgpg-error.
   Copyright (C) 2003 g10 Code GmbH

   This file is part of libgpg-error.
 
   libgpg-error is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.
 
   libgpg-error is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with GPGME; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

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


/* The error source type gpg_err_source_t.  */

/* Only add to the end of the list, never change or reorder the
   existing entries.  */
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

    /* This is one more than the largest allowed entry.  */
    GPG_ERR_SOURCE_DIM = 256
  } gpg_err_source_t;


/* The error code type gpg_err_code_t.  */

/* Only add to the end of the list, never change or reorder the
   existing entries.  */
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
    GPG_ERR_FILE_READ = 21,
    GPG_ERR_FILE_WRITE = 22,
    GPG_ERR_FILE_OPEN = 23,
    GPG_ERR_FILE_CREATE = 24,
    GPG_ERR_FILE_CLOSE = 25,
    GPG_ERR_FILE_RENAME = 26,
    GPG_ERR_FILE_DELETE = 27,
    GPG_ERR_FILE_EXISTS = 28,
    GPG_ERR_FILE = 29,
    GPG_ERR_BAD_MPI = 30,		/* Problem with an MPI's value.  */
    GPG_ERR_INV_PASSPHRASE = 31,	/* Invalid passphrase.  */
    GPG_ERR_SIG_CLASS = 32,
    GPG_ERR_RESOURCE_LIMIT = 33,
    GPG_ERR_INV_KEYRING = 34,
    GPG_ERR_TRUSTDB = 35,		/* A problem with the trustdb.  */
    GPG_ERR_BAD_CERT = 36,		/* Bad certicate.  */
    GPG_ERR_INV_USER_ID = 37,
    GPG_ERR_UNEXPECTED = 38,
    GPG_ERR_TIME_CONFLICT = 39,
    GPG_ERR_KEYSERVER = 40,
    GPG_ERR_WRONG_PUBKEY_ALGO = 41,	/* Wrong public key algorithm.  */
    GPG_ERR_IO = 42,			/* I/O error.  */
    GPG_ERR_WEAK_KEY = 43,		/* Weak encryption key.  */
    GPG_ERR_INV_KEYLEN = 44,		/* Invalid length of a key.  */
    GPG_ERR_INV_ARG = 45,		/* Invalid argument.  */
    GPG_ERR_BAD_URI = 46,		/* Syntax error in URI.  */
    GPG_ERR_INV_URI = 47,		/* Unsupported scheme and similar.  */
    GPG_ERR_NETWORK = 48,		/* General network error.  */
    GPG_ERR_UNKNOWN_HOST = 49,
    GPG_ERR_SELFTEST_FAILED = 50,	/* Selftest failed.  */
    GPG_ERR_NOT_ENCRYPTED = 51,
    GPG_ERR_NOT_PROCESSED = 52,
    GPG_ERR_UNUSABLE_PUBKEY = 53,
    GPG_ERR_UNUSABLE_SECKEY = 54,
    GPG_ERR_INV_VALUE = 55,		/* Invalid value.  */
    GPG_ERR_BAD_CERT_CHAIN = 56,
    GPG_ERR_MISSING_CERT = 57,
    GPG_ERR_NO_DATA = 58,
    GPG_ERR_BUG = 59,
    GPG_ERR_NOT_SUPPORTED = 60,
    GPG_ERR_INV_OP = 61,		/* Invalid operation code.  */
    GPG_ERR_OUT_OF_CORE = 62,		/* Out of core ("Keine Kerne").  */
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
    GPG_ERR_READ_ERROR = 73,
    GPG_ERR_WRITE_ERROR = 74,
    GPG_ERR_INCOMPLETE_LINE = 75,
    GPG_ERR_INVALID_RESPONSE = 76,
    GPG_ERR_NO_AGENT = 77,
    GPG_ERR_AGENT_ERROR = 78,
    GPG_ERR_INV_DATA = 79,
    GPG_ERR_ASSUAN_SERVER_FAULT = 80,
    GPG_ERR_ASSUAN_ERROR = 81,		/* Catch all assuan error.  */
    GPG_ERR_INV_SESSION_KEY = 82,
    GPG_ERR_INV_SEXP = 83,
    GPG_ERR_UNSUPPORTED_ALGORITHM = 84,
    GPG_ERR_NO_PIN_ENTRY = 85,
    GPG_ERR_PIN_ENTRY_ERROR = 86,
    GPG_ERR_BAD_PIN = 87,
    GPG_ERR_INV_NAME = 88,
    GPG_ERR_BAD_DATA = 89,
    GPG_ERR_INV_PARAMETER = 90,
    GPG_ERR_TRIBUTE_TO_D_A = 91,
    GPG_ERR_NO_DIRMNGR = 92,
    GPG_ERR_DIRMNGR_ERROR = 93,
    GPG_ERR_CERT_REVOKED = 94,
    GPG_ERR_NO_CRL_KNOWN = 95,
    GPG_ERR_CRL_TOO_OLD = 96,
    GPG_ERR_LINE_TOO_LONG = 97,
    GPG_ERR_NOT_TRUSTED = 98,
    GPG_ERR_CANCELED = 109,
    GPG_ERR_BAD_CA_CERT = 100,
    GPG_ERR_CERT_EXPIRED = 101,
    GPG_ERR_CERT_TOO_YOUNG = 102,
    GPG_ERR_UNSUPPORTED_CERT = 103,
    GPG_ERR_UNKNOWN_SEXP = 104,
    GPG_ERR_UNSUPPORTED_PROTECTION = 105,
    GPG_ERR_CORRUPTED_PROTECTION = 106,
    GPG_ERR_AMBIGUOUS_NAME = 107,
    GPG_ERR_CARD_ERROR = 108,
    GPG_ERR_CARD_RESET = 109,
    GPG_ERR_CARD_REMOVED = 110,
    GPG_ERR_INVALID_CARD = 111,
    GPG_ERR_CARD_NOT_PRESENT = 112,
    GPG_ERR_NO_PKCS15_APP = 113,
    GPG_ERR_NOT_CONFIRMED = 114,
    GPG_ERR_CONFIGURATION_ERROR = 115,
    GPG_ERR_NO_POLICY_MATCH = 116,
    GPG_ERR_INVALID_INDEX = 117,
    GPG_ERR_INVALID_ID = 118,
    GPG_ERR_NO_SCDAEMON = 119,
    GPG_ERR_SCDAEMON_ERROR = 120,
    GPG_ERR_UNSUPPORTED_PROTOCOL = 121,
    GPG_ERR_BAD_PIN_METHOD = 122,
    GPG_ERR_CARD_NOT_INITIALIZED = 123,
    GPG_ERR_UNSUPPORTED_OPERATION = 124,
    GPG_ERR_WRONG_KEY_USAGE = 125,

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

    GPG_ERR_EOF = 16383,		/* This was once a -1.  Pity.  */

    /* This is one more than the largest allowed entry.  */
    GPG_ERR_CODE_DIM = 16384
  } gpg_err_code_t;


/* The error value type gpg_error_t.  */

/* We would really like to use bit-fields in a struct, but using
   structs as return values can cause binary compatibility issues, in
   particular if you want to do it effeciently (also see
   -freg-struct-return option to GCC).  */
typedef unsigned long gpg_error_t;

/* We use the lowest 16 bits of gpg_error_t for error codes.  */
#define GPG_ERR_CODE_MASK	(GPG_ERR_CODE_DIM - 1)

/* Bits 17 to 24 are reserved.  */

/* We use the upper 8 bits of gpg_error_t for error sources.  */
#define GPG_ERR_SOURCE_MASK	(GPG_ERR_SOURCE_DIM - 1)
#define GPG_ERR_SOURCE_SHIFT	24


/* Constructor and accessor functions.  */

/* Construct an error value from an error code and source.  Within a
   subsystem, use gpg_error.  */
static __inline__ gpg_error_t
gpg_make_error (gpg_err_source_t source, gpg_err_code_t code)
{
  return ((source & GPG_ERR_SOURCE_MASK) << GPG_ERR_SOURCE_SHIFT)
    | (code & GPG_ERR_CODE_MASK);
}


/* The user should define GPG_ERR_SOURCE_DEFAULT before including this
   file to specify a default source for gpg_error.  */
#ifndef GPG_ERR_SOURCE_DEFAULT
#define GPG_ERR_SOURCE_DEFAULT	GPG_ERR_SOURCE_UNKNOWN
#endif

static __inline__ gpg_error_t
gpg_error (gpg_err_code_t code)
{
  return gpg_make_error (GPG_ERR_SOURCE_DEFAULT, code);
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

#endif	/* GPG_ERROR_H */
