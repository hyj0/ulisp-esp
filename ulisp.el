(defun call-c-fun2 (&rest r) nil)
(defun call-c-fun (&rest r) nil)

(defvar printAddr 1107319286)
(defvar SerialBT_flush 1107392426)
(defvar SerialBT_begin0 1107392956)
(defvar SerialBT_connected 1107393362)
(defvar SerialBT_read 1107392438)
(defvar SerialBT_write 1107392736)
(defvar SerialBT_writebuff 1107392784)
(defvar SerialBT_available 1107392406)
(defvar Freespace 1070257536)
(defvar BleKeyboard_begin 1107297336)
(defvar wifi_client 1070236536)
(defvar WiFiClient-connect 1107378576)
(defvar WiFiClient-connected 1107379034)
(defvar WiFiClient-stop 1107380768)
(defvar WiFiClient-available 1107379394)
(defvar WiFiClient-read 1107378508)
(defvar WiFiClient-write 1107378482)
(defvar WiFiwrite 1107335212)
(defvar LastChar 1070257572)
(defvar cstring 1107309068)
(defvar Flags 1070144028)

(defvar pinMode1 1107397962)
(defvar digitalWrite1 1107398112)
(defvar test_cfun 1107296638)
(defvar test_cfun1 1107296680)
(defvar testClass 1070257620)
(defvar handleInterrupts 1107329828)
(defvar autorunimage 1107326502)
(defvar __pserial 1107297588)
(defvar set_pserial 1107296288)
(defvar reset_pserial 1107296294)
(defvar set_call_c_fun_debug 1107296596)
(defvar read2 1107333926)
(defvar gserial 1107334638)
(defvar testVaFun 1107297190)
(defvar __hookFun00 1107327488)
(defvar __hookFun01 1107327544)
(defvar __hookFun02 1107327600)
(defvar __hookFunN0 1107327656)
(defvar __hookFunN1 1107328380)
(defvar __hookFunN2 1107329104)
(defvar getAllHookFunList 1107303796)
(defvar getWifiServerConnect 1107335378)
(defvar markUse 1107296730)
(defvar BleKeyboard_begin 1107297336)
(defvar BleKeyboard_end 1107297340)
(defvar BleKeyboard_isConnected 1107297344)
(defvar BleKeyboard_press 1107297348)
(defvar BleKeyboard_release 1107297356)
(defvar SerialBT_begin 1107297316)
(defvar BLEDeviceClose 1107297364)
(defvar getMemChar 1107296590)
(defvar SerialBT 1070162576)
(defvar bleKeyboard 1070236424)
(defvar memset 1073742676)
(defvar malloc 1077478532)
(defvar free 1077478540)
(defvar SerialBT_pserial 1107335962)
(defvar SerialBT_pserial_send 1107335912)

(progn
  (defvar stringlength-addr #x42000d08)
  (let* ((lisp-string "abcd")
	 (length (call-c-fun2 '(nil (t)) stringlength-addr lisp-string)))
    (print (cons "length" length))))

;bug run with wifi repl will coredump!
(defvar printobject-addr 1107305334)
(let* ((lisp-obj "xyz")
       (my-printc (getHookFun '(nil (nil)) (lambda (c)
					     (print "get c")
					     (print c)
					     1)))
       (my-printc-addr (second my-printc))
       (my-printc-name (first my-printc)))
  (print my-printc)
  (unwind-protect
      (call-c-fun2 '(nil (t nil)) printobject-addr lisp-obj my-printc-addr)
    (releaseHootFun my-printc-name)))
;ok 
(call-c-fun2 '(nil (t nil)) printobject-addr  "xyz" 1107329104)
(releaseHootFun __hookFunN2)


;return (name addr arglen)
(defvar g_hook_fun nil)

(defun getHookFun (sign fun)
  (let* ((all (call-c-fun2 '(t ()) getAllHookFunList))
	 (lst0 (mapcan (lambda (x) (if (assoc (car x) g_hook_fun)
				     nil
				   (list x)))
		     all))
	 (lst1 (mapcan (lambda (x) (if (= 0 (length (second sign)))
				       (if (= 0 (third x))
					   (list x)
					 nil)
				     (if (= 1 (third x))
					 (list x)
				       nil)))
		       lst0)))
    (if lst1
	(let ((one (first lst1)))
	  (setq g_hook_fun (cons (list (first one) sign fun) g_hook_fun))
	  one)
      nil)))

(defun releaseHootFun (name)
  (setq g_hook_fun (mapcan (lambda (x) (if (eq name (car x))
					   nil
					 (list x)))
			   g_hook_fun)))

(progn
  (let* ((one
	 (getHookFun '(nil (nil nil nil nil)) (lambda (x y a b) (+ x y)))))
    (print one)
    (let ((ret (call-c-fun (second one) 1 2 3 4)))
      (releaseHootFun (car one))
      ret)
    ))



(getHookFun '(t (nil )) (lambda (x) (+ x 1)))
(call-c-fun2 '(t (nil)) 1107329104 33)
(releaseHootFun '__hookFunN2)



(progn
  (let* ((one
	  (getHookFun '(nil ()) (lambda () (+ 1 3)))))
    (print one)
    (print (call-c-fun (second one) ))
    (releaseHootFun (car one))
    "okkkk"))


(progn
  (let* ((one
	 (getHookFun '(nil (nil nil)) (lambda (x y) (+ x y)))))
    (print one)
    (let ((ret (call-c-fun (second one) 1 2)))
      (releaseHootFun (car one))
      ret)
    ))

(progn
  (let* ((one
	 (getHookFun '(nil (nil nil nil nil)) (lambda (x y a b) (+ x y)))))
    (print one)
    (let ((ret (call-c-fun (second one) 1 2 3 4)))
      (releaseHootFun (car one))
      ret)
    ))



(defun string-to-list (str)
  (let ((lst nil))
    (dotimes (index (length str))
      (setq lst (cons (char-code (char str index)) lst)))
    (reverse lst)))
(string-to-list "abcd")

(defvar *bt-repl-buff* "")
(defvar *bt-repl-buff2* nil)
(defvar *bt-repl-buff-errors* 0)

(defun bt-repl-try (buff)
  (let* ((c-read-fun (getHookFun '(nil ()) (lambda ()
					     (let ((lastc (call-c-fun getMemChar LastChar))
						   (c (car *bt-repl-buff2*))
						   (res (cdr *bt-repl-buff2*)))
					       (if (not (= 0 lastc))
						   (progn
						     (call-c-fun memset LastChar 0 1)
						     lastc)
						 (if c
						     (progn
						       (setq *bt-repl-buff2* res)
						       c)
						   (progn
						     (setq *bt-repl-buff-errors* (+ 1 *bt-repl-buff-errors*))
						     (if (> *bt-repl-buff-errors* 5)
							 (error "lisp parse incomplete")
						       -1)))))
					    )))
	 (addr (second c-read-fun))
	 (name (first c-read-fun)))
    (unwind-protect
	(progn
	  (setq *bt-repl-buff2*  (string-to-list buff))
	  (print *bt-repl-buff2*)
	  (setq *bt-repl-buff-errors* 0)
	  (let ((line (unwind-protect
			  (progn
			    (call-c-fun set_pserial SerialBT_pserial)
			    (call-c-fun2 '(t (nil)) read2 addr))
			(progn
			  (call-c-fun SerialBT_pserial_send)
			  (call-c-fun reset_pserial)
			  (if (< *bt-repl-buff-errors* 5)
			      (setq *bt-repl-buff* "")
			    nil)))))
	    (call-c-fun memset LastChar 0 1)
	    (print (cons "line" line))
	    (if (eq nothing line)
		nil
	      (unwind-protect
		  (progn
		    (setq *bt-repl-buff* "")
		    (call-c-fun set_pserial SerialBT_pserial)
		    (eval line))
		(progn
		  (call-c-fun SerialBT_pserial_send)
		  (call-c-fun reset_pserial))))))
      (progn
	(releaseHootFun name)))))

(progn (setq  *bt-repl-buff* ")")
       (bt-repl-try *bt-repl-buff*))
*bt-repl-buff*
(progn (setq  *bt-repl-buff* "(abc")
       (bt-repl-try *bt-repl-buff*))
(progn (setq  *bt-repl-buff* "(abc)")
       (bt-repl-try *bt-repl-buff*))
(progn (setq  *bt-repl-buff* "(+ 1 3)")
       (bt-repl-try *bt-repl-buff*))


(defun bt-loop-read ()
  (loop
   (let ((c (call-c-fun SerialBT_read SerialBT)))
     (if (< c 0)
	 (return)
       (progn
	 (print (cons "getchar " (cons c (code-char c))))
	 (setq *bt-repl-buff* (concatenate 'string *bt-repl-buff* (princ-to-string (code-char c))))
	 )))))
(defun bt-repl-timer-fun ()
  (if (= 0 (call-c-fun SerialBT_connected SerialBT))
      (setq *bt-repl-buff* "")
    (unless (= 0 (call-c-fun SerialBT_available SerialBT))
      (let ((c (call-c-fun SerialBT_read SerialBT)))
	(print (cons "getchar " (cons c (code-char c))))
	(setq *bt-repl-buff* (concatenate 'string *bt-repl-buff* (princ-to-string (code-char c))))
	(bt-loop-read)
	(let ((ret (bt-repl-try *bt-repl-buff*)))
	  (if ret
	      (let ((str (string ret)))
		(print (cons "str" str))
		(dotimes (i (length str))
		  (call-c-fun SerialBT_pserial (char-code (char str i))))
		(call-c-fun SerialBT_pserial_send)
		(setq *bt-repl-buff* ""))
	    nil))))))

(setq *bt-repl-buff* "")
(progn
  (bt-repl-timer-fun )
  *bt-repl-buff*)



(progn (setq  *wifi-repl-buff* ")")
       (wifi-repl-try *wifi-repl-buff*))
(progn (setq  *wifi-repl-buff* "(abc")
       (wifi-repl-try *wifi-repl-buff*))
*wifi-repl-buff*
(progn (setq  *wifi-repl-buff* "(abc)")
       (wifi-repl-try *wifi-repl-buff*))
(progn (setq  *wifi-repl-buff* "(+ 1 3)")
       (wifi-repl-try *wifi-repl-buff*))
(progn (setq  *wifi-repl-buff* (concatenate 'string  "(let* ((lisp-string \"abcd\")       (length (call-c-fun2 '(nil (t)) stringlength-addr lisp-string)))  (print (cons \"length\" length)))" (string (code-char 13)) (string  (code-char 10))))
       (wifi-repl-try *wifi-repl-buff*))


(defvar *wifi-repl-buff* "")
(defun wifi-repl-try (buff)
  (let* ((c-read-fun (getHookFun '(nil ()) (lambda ()
					     (let ((lastc (call-c-fun getMemChar LastChar))
						   (c (car *bt-repl-buff2*))
						   (res (cdr *bt-repl-buff2*)))
					       (if (not (= 0 lastc))
						   (progn
						     (call-c-fun memset LastChar 0 1)
						     lastc)
						 (if c
						     (progn
						       (setq *bt-repl-buff2* res)
						       c)
						   (progn
						     (setq *bt-repl-buff-errors* (+ 1 *bt-repl-buff-errors*))
						     (if (> *bt-repl-buff-errors* 5)
							 (error "lisp parse incomplete")
						       -1)))))
					    )))
	 (addr (second c-read-fun))
	 (name (first c-read-fun)))
    (unwind-protect
	(progn
	  (setq *bt-repl-buff2*  (string-to-list buff))
	  (print (cons "*bt-repl-buff2*" *bt-repl-buff2*))
	  (setq *bt-repl-buff-errors* 0)
	  (print (cons "c-read-fun" c-read-fun))
	  (let ((line (unwind-protect
			  (progn
			    (call-c-fun set_pserial WiFiwrite)
			    (call-c-fun2 '(t (nil)) read2 addr))
			(progn
			  (call-c-fun reset_pserial)
			  (if (< *bt-repl-buff-errors* 5)
			      (setq *wifi-repl-buff* "")
			    nil)))))
	    (call-c-fun memset LastChar 0 1)
	    (print (cons "line" line))
	    (if (eq nothing line)
		nil
	      (unwind-protect
		  (progn
		    (let ((line1 (read-from-string buff)))
		      (print (cons "line1" line1))
		      (setq line line1))
		    (setq *wifi-repl-buff* "")
		    (call-c-fun set_pserial WiFiwrite)
		    (eval line))
		(progn
		  (call-c-fun reset_pserial))))))
      (progn
	(releaseHootFun name)))))


(defvar *wifi-client-inited* nil)
(defun wifi-loop-read ()
  (loop
   (let ((c (call-c-fun WiFiClient-read wifi_client)))
     (if (< c 0)
	 (return)
       (progn
	 (print (cons "getchar " (cons c (code-char c))))
	 (setq *wifi-repl-buff* (concatenate 'string *wifi-repl-buff* (princ-to-string (code-char c))))
	 )))))

(defun wifi-repl-timer-fun ()
  (if (not (and *wifi-client-inited* (= 1 (call-c-fun WiFiClient-connected wifi_client))))
      (progn
	(wifi-accept)
	(setq *wifi-repl-buff* ""))
    (unless (= 0 (call-c-fun WiFiClient-available wifi_client))
      (progn
	(wifi-loop-read)
	(print (cons "buff" *wifi-repl-buff*))
	(let ((ret (wifi-repl-try *wifi-repl-buff*)))
	  (if ret
	      (let ((str (string ret)))
		(print (cons "str" str))
		(dotimes (i (length str))
		  (call-c-fun WiFiwrite  (char-code (char str i))))
		(call-c-fun WiFiwrite 10)
		(setq *wifi-repl-buff* ""))
	    nil))))))
(defun wifi-accept ()
  (if (= 1 (call-c-fun getWifiServerConnect))
      (progn
	(setq *wifi-client-inited* t)
	t)
    (progn
      (setq *wifi-client-inited* nil)
      nil)))

(defun wifi-connect2 (ip port)
  (let* ((ipbuff (call-c-fun2 '(nil (t nil nil)) cstring ip (call-c-fun malloc 32) 32)))
    (unwind-protect
	(progn
	  (let ((ret (call-c-fun WiFiClient-connect wifi_client ipbuff port 9000)))
	    (if (= 1 ret)
		(progn
		  (setq *wifi-client-inited* t)
		  t)
	      (progn
		(setq *wifi-client-inited* nil)
		nil))))
      (progn
	(call-c-fun free ipbuff)))))

(wifi-localip)
(wifi-connect2 "192.168.31.142" 8899)
(wifi-repl-timer-fun )

(defun wifi-disconnect ()
  (call-c-fun WiFiClient-stop wifi_client))

(call-c-fun WiFiClient-connected wifi_client)
(call-c-fun WiFiClient-available wifi_client)
(call-c-fun WiFiClient-read wifi_client)
(call-c-fun WiFiwrite (char-code (char "abc" 0)))
(call-c-fun WiFiwrite 10)

(defun do-key (ret key)
  (when (not (= 0 (call-c-fun BleKeyboard_isConnected bleKeyboard)))
    (if ret
	(call-c-fun BleKeyboard_press bleKeyboard key)
      (call-c-fun BleKeyboard_release bleKeyboard key))))

(defun check-key ()
  (let ((ret (digitalRead 19)))
    (if (eq *last-stat* ret)
	nil
      (progn
	(print (cons *last-stat* ret))
	(do-key ret 128)
	(setq *last-stat* ret)))))
(defun check-key1 ()
  (let ((ret (digitalRead 18)))
    (if (eq *last-stat1* ret)
	nil
      (progn
	(print (cons *last-stat1* ret))
	(do-key ret 130)
	(setq *last-stat1* ret)))))

(defvar *ble-state* 1)
(defun switch-to-keyboard()
  (progn
    (setq *ble-state* 1)
    (pinmode 18 :output)
    (digitalWrite 18 :low)
    (delay 2000)
    (pinmode 18 :input)
    (digitalWrite 18 :low)
    (call-c-fun BleKeyboard_end bleKeyboard)
    (call-c-fun BLEDeviceClose)
    (delay 3000)
    (call-c-fun set_call_c_fun_debug 0)
    (call-c-fun BleKeyboard_begin bleKeyboard)))

(defun switch-to-bleserial ()
  (progn
    (call-c-fun SerialBT_flush SerialBT)
    (setq *ble-state* 2)
    ;release key before switch
    (do-key nil 130)
    (pinmode 18 :output)
    (digitalWrite 18 :low)
    (delay 2000)
    (pinmode 18 :input)
    (digitalWrite 18 :low)
    (call-c-fun BLEDeviceClose)
    (delay 3000)
    (call-c-fun SerialBT_begin)
    (call-c-fun set_call_c_fun_debug 0)
    ))
(defun ble-all ()
  (call-c-fun BLEDeviceClose)
  (call-c-fun BleKeyboard_begin bleKeyboard)
  (call-c-fun SerialBT_begin))

(defun set-output-error ()
  (let* ((old-flag (call-c-fun getMemChar Flags))
	(new-flag (logxor old-flag (expt 2 7))))
    (call-c-fun memset Flags new-flag 1)))

(defun foo1 (n m)
  (ignore-errors
    (when (= 0  (mod m 500))
      (check-key-bleserial))
    (when (= 1 *ble-state*)
      (progn
	(check-key1)
	(check-key)))
    (check-key-bleserial2))
  (ignore-errors
    (set-output-error)
    (wifi-repl-timer-fun))
  (when (= 2 *ble-state*)
    (bt-repl-timer-fun)
    ))


(defun check-key-bleserial ()
  nil)

(defun check-key () nil)

(defvar *flag-ble-switch* 0)
(defun check-key-bleserial2 ()
  (if (digitalRead 18)
      (setq  *flag-ble-switch* (+ 1 *flag-ble-switch*))
    (setq *flag-ble-switch* 0))
  (when (> *flag-ble-switch* 200)
    (setq *flag-ble-switch* 0)
    (if (= 1 *ble-state*)
	(progn
	  (switch-to-bleserial))
      (progn
	(switch-to-keyboard)))
    (delay 2000)))

(defun boot ()
  (setq *ble-state* 1)
  (call-c-fun BleKeyboard_begin bleKeyboard)
  (call-c-fun set_call_c_fun_debug 0)
  (defvar *last-stat* nil)
  (defvar *last-stat1* nil)

  (pinmode 19 :input)
  (digitalWrite 19 :low)
  (pinmode 18 :input)
  (digitalWrite 18 :low)

  (pinmode 9 :input)
  (digitalWrite 9 :low)

  (wifi-softap "esp32-ap"  "87654320a" )
  (wifi-localip)
  (wifi-server)
  (attach-interrupt 0 foo1 10))

(defun noboot ()
  nil)

(attach-interrupt 0 foo1 10)
(attach-interrupt 0 nil 1500)
(boot)
(foo1 1 0)
(save-image 'boot)
(save-image 'noboot)
(length  (globals))

(call-c-fun printAddr)
(register Freespace)
(switch-to-bleserial)
(call-c-fun set_call_c_fun_debug 1)



(defvar nc-proc (start-process "nc8899" "nc89" "nc" "-l" "8899"))
(defvar nc-proc (start-process "nc8899" "nc89" "E:/cygwin64/bin/telnet.exe" "192.168.31.227" "80"))

(progn
  (defun send-str-to-buffer (str)
    (process-send-string nc-proc str)
    )
  (defun send-to-bt0 ()
    (let* ((start (save-excursion (backward-sexp) (point)))
	   (end (point))
	   (buff (with-current-buffer (current-buffer)
		   (buffer-substring start end)))
	   (buff1 (replace-regexp-in-string "\n" "" buff)))
      (print buff)
      (send-str-to-buffer buff)))
  (defun send-to-bt ()
    (interactive)
    (if (and (bound-and-true-p evil-mode) (evil-normal-state-p))
	(progn
	  (evil-insert-state)
	  (goto-char (+ 1 (point)))
	  (send-to-bt0)
	  (evil-normal-state))
      (send-to-bt0))))

(defun cut-s-exp0 ()
  (let* ((start (save-excursion (backward-sexp) (point)))
	 (end (point))
	 (buff (with-current-buffer (current-buffer)
		 (buffer-substring start end)))
	 (buff1 (replace-regexp-in-string "\n" "" buff)))
    (print buff1)
    (kill-new buff)))

(defun cut-s-exp ()
  (interactive)
  (if (and (bound-and-true-p evil-mode) (evil-normal-state-p))
      (progn
	(evil-insert-state)
	(goto-char (+ 1 (point)))
	(cut-s-exp0)
	(evil-normal-state))
    (cut-s-exp0)))

;test
(locals)

(defun foo (n)
  (cond ((= 0 n) 1)
	(t (* n (foo (- n 1))))))
(foo 12)
