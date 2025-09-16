#ifndef __ASM_ASM_UACCESS_H
#define __ASM_ASM_UACCESS_H


	.macro	clear_address_tag, dst, addr
	tst	\addr, #(1 << 55)
	bic	\dst, \addr, #(0xff << 56)
	csel	\dst, \dst, \addr, eq
	.endm

#endif
