#ifndef _LINUX_BITOPS_H
#define _LINUX_BITOPS_H
#include <asm/types.h>
#include <linux/bits.h>

#define BITS_PER_TYPE(type) (sizeof(type) * BITS_PER_BYTE)
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_TYPE(long))

extern unsigned int __sw_hweight8(unsigned int w);
extern unsigned int __sw_hweight16(unsigned int w);
extern unsigned int __sw_hweight32(unsigned int w);
extern unsigned long __sw_hweight64(__u64 w);


#include <asm/bitops.h>

#define for_each_set_bit(bit, addr, size) \
	for ((bit) = find_first_bit((addr), (size));		\
	     (bit) < (size);					\
	     (bit) = find_next_bit((addr), (size), (bit) + 1))


#define for_each_set_bit_from(bit, addr, size) \
	for ((bit) = find_next_bit((addr), (size), (bit));	\
	     (bit) < (size);					\
	     (bit) = find_next_bit((addr), (size), (bit) + 1))

#define for_each_clear_bit(bit, addr, size) \
	for ((bit) = find_first_zero_bit((addr), (size));	\
	     (bit) < (size);					\
	     (bit) = find_next_zero_bit((addr), (size), (bit) + 1))


#define for_each_clear_bit_from(bit, addr, size) \
	for ((bit) = find_next_zero_bit((addr), (size), (bit));	\
	     (bit) < (size);					\
	     (bit) = find_next_zero_bit((addr), (size), (bit) + 1))

static inline int get_bitmask_order(unsigned int count)
{
	int order;

	order = fls(count);
	return order;	
}

static __always_inline unsigned long hweight_long(unsigned long w)
{
	return sizeof(w) == 4 ? hweight32(w) : hweight64((__u64)w);
}


static inline __u64 rol64(__u64 word, unsigned int shift)
{
	return (word << (shift & 63)) | (word >> ((-shift) & 63));
}


static inline __u64 ror64(__u64 word, unsigned int shift)
{
	return (word >> (shift & 63)) | (word << ((-shift) & 63));
}


static inline __u32 rol32(__u32 word, unsigned int shift)
{
	return (word << (shift & 31)) | (word >> ((-shift) & 31));
}


static inline __u32 ror32(__u32 word, unsigned int shift)
{
	return (word >> (shift & 31)) | (word << ((-shift) & 31));
}


static inline __u16 rol16(__u16 word, unsigned int shift)
{
	return (word << (shift & 15)) | (word >> ((-shift) & 15));
}


static inline __u16 ror16(__u16 word, unsigned int shift)
{
	return (word >> (shift & 15)) | (word << ((-shift) & 15));
}


static inline __u8 rol8(__u8 word, unsigned int shift)
{
	return (word << (shift & 7)) | (word >> ((-shift) & 7));
}


static inline __u8 ror8(__u8 word, unsigned int shift)
{
	return (word >> (shift & 7)) | (word << ((-shift) & 7));
}


static inline __s32 sign_extend32(__u32 value, int index)
{
	__u8 shift = 31 - index;
	return (__s32)(value << shift) >> shift;
}


static inline __s64 sign_extend64(__u64 value, int index)
{
	__u8 shift = 63 - index;
	return (__s64)(value << shift) >> shift;
}

static inline unsigned fls_long(unsigned long l)
{
	if (sizeof(l) == 4)
		return fls(l);
	return fls64(l);
}

static inline int get_count_order(unsigned int count)
{
	int order;

	order = fls(count) - 1;
	if (count & (count - 1))
		order++;
	return order;
}


static inline int get_count_order_long(unsigned long l)
{
	if (l == 0UL)
		return -1;
	else if (l & (l - 1UL))
		return (int)fls_long(l);
	else
		return (int)fls_long(l) - 1;
}


static inline unsigned long __ffs64(u64 word)
{
#if BITS_PER_LONG == 32
	if (((u32)word) == 0UL)
		return __ffs((u32)(word >> 32)) + 32;
#elif BITS_PER_LONG != 64
#error BITS_PER_LONG not 32 or 64
#endif
	return __ffs((unsigned long)word);
}

#ifdef __KERNEL__

#ifndef set_mask_bits
#define set_mask_bits(ptr, _mask, _bits)	\
({								\
	const typeof(*ptr) mask = (_mask), bits = (_bits);	\
	typeof(*ptr) old, new;					\
								\
	do {							\
		old = ACCESS_ONCE(*ptr);			\
		new = (old & ~mask) | bits;			\
	} while (cmpxchg(ptr, old, new) != old);		\
								\
	new;							\
})
#endif

#ifndef bit_clear_unless
#define bit_clear_unless(ptr, _clear, _test)	\
({								\
	const typeof(*ptr) clear = (_clear), test = (_test);	\
	typeof(*ptr) old, new;					\
								\
	do {							\
		old = ACCESS_ONCE(*ptr);			\
		new = old & ~clear;				\
	} while (!(old & test) &&				\
		 cmpxchg(ptr, old, new) != old);		\
								\
	!(old & test);						\
})
#endif

#ifndef find_last_bit

extern unsigned long find_last_bit(const unsigned long *addr,
				   unsigned long size);
#endif

#endif 
#endif
