(progn
  (defvar *pack40* 1107298102)
  (defvar *cstring* 1107308232)
  (defvar *Serial* 1070226264)
  (defvar *Serial-write* 1107296758)
  (defvar *test_cfun* 1107296614)
  (defvar *testClassInstance* 1070246644)
  (defvar *TestClass-testString* 1007391748)
  (defvar *TestClass-foo* 1007392956)
  (defvar *getAllHookFunList* 1107302960)
  (defvar *malloc* 1077468840)
  (defvar *free* 1077468848)
  (defvar *memset* 1073742676)
)

;call c fun
(call-c-fun *test_cfun* 1 97)

;call cpp method Serial.write
(call-c-fun *Serial-write* *Serial* 97)
(call-c-fun *Serial-write* *Serial* (char-code (char "b" 0)))
;call TestClass.foo will coredump, do not know why!!!
(call-c-fun *TestClass-foo* *testClassInstance* 1 2) 

;call char *cstring (object *form, char *buffer, int buflen)
;with char *buffer output
(let ((c-str-addr (call-c-fun *malloc* 32))
      (lisp-str "abcd"))
  (unless (= 0 c-str-addr)
    (unwind-protect
	(progn
	  (print (cons "c-str-addr" c-str-addr))
	  (call-c-fun *memset* c-str-addr 0 32)
	  (print (cons "pack40 empty" (call-c-fun *pack40* c-str-addr)))
	  (call-c-fun2 '(nil (t nil nil)) *cstring* lisp-str c-str-addr 32)
	  ;c'char* at c-str-addr has been change
	  (print (cons "pack40" (call-c-fun *pack40* c-str-addr)))
	  )
      (call-c-fun *free* c-str-addr))))
