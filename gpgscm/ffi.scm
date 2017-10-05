;; FFI interface for TinySCHEME.
;; Copyright (C) 2016 g10 Code GmbH
;;
;; This file is part of Libgpg-error.
;;
;; Libgpg-error is free software; you can redistribute it and/or
;; modify it under the terms of the GNU Lesser General Public License
;; as published by the Free Software Foundation; either version 2.1 of
;; the License, or (at your option) any later version.
;;
;; Libgpg-error is distributed in the hope that it will be useful, but
;; WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;; Lesser General Public License for more details.
;;
;; You should have received a copy of the GNU Lesser General Public
;; License along with this program; if not, see <https://www.gnu.org/licenses/>.


;; Foreign function wrapper.  Expects F to return a list with the
;; first element being the `error_t' value returned by the foreign
;; function.  The error is thrown, or the cdr of the result is
;; returned.
(define (ffi-apply name f args)
  (let ((result (apply f args)))
    (cond
     ((string? result)
      (ffi-fail name args result))
     ((not (= (car result) 0))
      (ffi-fail name args (strerror (car result))))
     ((and (= (car result) 0) (pair? (cdr result))) (cadr result))
     ((= (car result) 0) '())
     (else
      (throw (list "Result violates FFI calling convention: " result))))))

(define (ffi-fail name args message)
  (let ((args' (open-output-string)))
    (write (cons (string->symbol name) args) args')
    (throw (get-output-string args') message)))

;; Pseudo-definitions for foreign functions.  Evaluates to no code,
;; but serves as documentation.
(macro (ffi-define form))

;; Runtime support.

;; Low-level mechanism to terminate the process.
(ffi-define (_exit status))

;; Get the current time in seconds since the epoch.
(ffi-define (get-time))
