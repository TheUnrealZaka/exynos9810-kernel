
#ifndef __ASM_WORD_AT_A_TIME_H
#define __ASM_WORD_AT_A_TIME_H

#include <asm/uaccess.h>

#ifndef __AARCH64EB__

#include <linux/kernel.h>

struct word_at_a_time {
	const unsigned long one_bits, high_bits;
};

#define WORD_AT_A_TIME_CONSTANTS { REPEAT_BYTE(0x01), REPEAT_BYTE(0x80) }

static inline unsigned long has_zero(unsigned long a, unsigned long *bits,
				     const struct word_at_a_time *c)
{
	unsigned long mask = ((a - c->one_bits) & ~a) & c->high_bits;
	*bits = mask;
	return mask;
}

#define prep_zero_mask(a, bits, c) (bits)

static inline unsigned long create_zero_mask(unsigned long bits)
{
	bits = (bits - 1) & ~bits;
	return bits >> 7;
}

static inline unsigned long find_zero(unsigned long mask)
{
	return fls64(mask) >> 3;
}

#define zero_bytemask(mask) (mask)

#else	
#include <asm-generic/word-at-a-time.h>
#endif


static inline unsigned long load_unaligned_zeropad(const void *addr)
{
	unsigned long ret, offset;

	
	asm(
	"1:	ldr	%0, %3\n"
	"2:\n"
	"	.pushsection .fixup,\"ax\"\n"
	"	.align 2\n"
	"3:	and	%1, %2, #0x7\n"
	"	bic	%2, %2, #0x7\n"
	"	ldr	%0, [%2]\n"
	"	lsl	%1, %1, #0x3\n"
#ifndef __AARCH64EB__
	"	lsr	%0, %0, %1\n"
#else
	"	lsl	%0, %0, %1\n"
#endif
	"	b	2b\n"
	"	.popsection\n"
	_ASM_EXTABLE(1b, 3b)
	: "=&r" (ret), "=&r" (offset)
	: "r" (addr), "Q" (*(unsigned long *)addr));

	return ret;
}

#endif 
