;; *******************************************************************************************************************
;; -----------------
;; Bootloader demo
;; -----------------
;; This is a very simple demo of 'loading' user application and transferring control
;; The demo copies user's code from __source to __ram_start (this can be replaced with flash read or anything else).
;; Then caches are flushed and IVT address is changed to __ram_start (assuming that user's application has it's own IVT
;; and startup code.
;;
;; Then bootloader jumps to user's reset handler __start (we can't just generate reset).
;;
;; As current bootloader's code doesn't globals or subroutines and never receive control back,
;; it does not update SP/GP/FP registers.
;; *******************************************************************************************************************
	.file "crt0.s"

	.section .ivt, "a",@progbits
;; *******************************************************************************************************************
;// handler's name              type      number  name                    offset in IVT (hex/dec)
.word   __start             ;   exception   0   program entry point offset  0x0     0
.rep 15
	.word	_exit_halt
.endr

;;	.extern main
	.text

	.global __start
	.type __start, @function
__start:

;; copy user's code
	mov %r0, @__ram_start
	mov %r1, @__source_start
	mov %r2, @__source_end
	sub %r2, %r2, %r1 ; calculate how many words to copy
	asr %r2, %r2
	asr %r2, %r2
	mov %lp_count, %r2
	lp	copy_end
	ld.ab %r2, [%r1,4]
	st.ab %r2, [%r0,4]
copy_end:

;; invalidate  instruction cache
	sr 0xdeadbeef, [0x10]
	nop
	nop
	nop
;; invalidate data cache
	lr %r5,  [0x48]
	sr 0xc2, [0x48]
	sr 0x01, [0x47]
wait:
	nop
	lr %r4, [0x48]
	bbit1 %r4, 8, wait
	sr %r5, [0x48]

;; soft reset to user's handler
	sr 0x10000000, [0x25]	;	setup user app interrupt vector
	ld %r0, [0x10000000]    ;	load address of user's _reset handler
	j [%r0]			;	jump to user's reset handler

	.global abort
	.type abort, @function
	.align 4
abort:
	mov r0, 1
	j @_exit_halt

_exit_halt          :
	; r0 contains exit code
	flag 0x01
	nop
	nop
	nop
	j @_exit_halt
	nop
