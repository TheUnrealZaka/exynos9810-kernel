#ifndef _ASM_WORD_AT_A_TIME_H
#define _ASM_WORD_AT_A_TIME_H

#include <linux/kernel.h>
#include <asm/byteorder.h>

#ifdef __BIG_ENDIAN

struct word_at_a_time {
	const unsigned long high_bits, low_bits;
};

#define WORD_AT_A_TIME_CONSTANTS { REPEAT_BYTE(0xfe) + 1, REPEAT_BYTE(0x7f) }


static inline long prep_zero_mask(unsigned long val, unsigned long rhs, const struct word_at_a_time *c)
{
	unsigned long mask = (val & c->low_bits) + c->low_bits;
	return ~(mask | rhs);
}

#define create_zero_mask(mask) (mask)

static inline long find_zero(unsigned long mask)
{
	long byte = 0;
#ifdef CONFIG_64BIT
	if (mask >> 32)
		mask >>= 32;
	else
		byte = 4;
#endif
	if (mask >> 16)
		mask >>= 16;
	else
		byte += 2;
	return (mask >> 8) ? byte : byte + 1;
}

static inline bool has_zero(unsigned long val, unsigned long *data, const struct word_at_a_time *c)
{
	unsigned long rhs = val | c->low_bits;
	*data = rhs;
	return (val + c->high_bits) & ~rhs;
}

#ifndef zero_bytemask
#define zero_bytemask(mask) (~1ul << __fls(mask))
#endif

#else


struct word_at_a_time {
	const unsigned long one_bits, high_bits;
};

#define WORD_AT_A_TIME_CONSTANTS { REPEAT_BYTE(0x01), REPEAT_BYTE(0x80) }

#ifdef CONFIG_64BIT


static inline long count_masked_bytes(unsigned long mask)
{
	return mask*0x0001020304050608ul >> 56;
}

#else	


static inline long count_masked_bytes(long mask)
{
	
	long a = (0x0ff0001+mask) >> 23;
	
	return a & mask;
}

#endif


static inline unsigned long has_zero(unsigned long a, unsigned long *bits, const struct word_at_a_time *c)
{
	unsigned long mask = ((a - c->one_bits) & ~a) & c->high_bits;
	*bits = mask;
	return mask;
}

static inline unsigned long prep_zero_mask(unsigned long a, unsigned long bits, const struct word_at_a_time *c)
{
	return bits;
}

static inline unsigned long create_zero_mask(unsigned long bits)
{
	bits = (bits - 1) & ~bits;
	return bits >> 7;
}


#define zero_bytemask(mask) (mask)

static inline unsigned long find_zero(unsigned long mask)
{
	return count_masked_bytes(mask);
}

#endif 

#endif 
