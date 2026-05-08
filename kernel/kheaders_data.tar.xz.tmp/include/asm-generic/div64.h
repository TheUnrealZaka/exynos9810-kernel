#ifndef _ASM_GENERIC_DIV64_H
#define _ASM_GENERIC_DIV64_H


#include <linux/types.h>
#include <linux/compiler.h>

#if BITS_PER_LONG == 64

# define do_div(n,base) ({					\
	uint32_t __base = (base);				\
	uint32_t __rem;						\
	__rem = ((uint64_t)(n)) % __base;			\
	(n) = ((uint64_t)(n)) / __base;				\
	__rem;							\
 })

#elif BITS_PER_LONG == 32

#include <linux/log2.h>



#ifndef __div64_const32_is_OK
#define __div64_const32_is_OK (__GNUC__ >= 4)
#endif

#define __div64_const32(n, ___b)					\
({									\
									\
	uint64_t ___res, ___x, ___t, ___m, ___n = (n);			\
	uint32_t ___p, ___bias;						\
									\
						\
	___p = 1 << ilog2(___b);					\
									\
				\
	___m = (~0ULL / ___b) * ___p;					\
	___m += (((~0ULL % ___b + 1) * ___p) + ___b - 1) / ___b;	\
									\
			\
	___x = ~0ULL / ___b * ___b - 1;					\
									\
			\
	___res = ((___m & 0xffffffff) * (___x & 0xffffffff)) >> 32;	\
	___t = ___res += (___m & 0xffffffff) * (___x >> 32);		\
	___res += (___x & 0xffffffff) * (___m >> 32);			\
	___t = (___res < ___t) ? (1ULL << 32) : 0;			\
	___res = (___res >> 32) + ___t;					\
	___res += (___m >> 32) * (___x >> 32);				\
	___res /= ___p;							\
									\
				\
	if (~0ULL % (___b / (___b & -___b)) == 0) {			\
				\
		___n /= (___b & -___b);					\
		___m = ~0ULL / (___b / (___b & -___b));			\
		___p = 1;						\
		___bias = 1;						\
	} else if (___res != ___x / ___b) {				\
									\
		___bias = 1;						\
						\
		___m = (~0ULL / ___b) * ___p;				\
		___m += ((~0ULL % ___b + 1) * ___p) / ___b;		\
	} else {							\
									\
		uint32_t ___bits = -(___m & -___m);			\
		___bits |= ___m >> 32;					\
		___bits = (~___bits) << 1;				\
									\
		if (!___bits) {						\
			___p /= (___m & -___m);				\
			___m /= (___m & -___m);				\
		} else {						\
			___p >>= ilog2(___bits);			\
			___m >>= ilog2(___bits);			\
		}							\
							\
		___bias = 0;						\
	}								\
									\
									\
	___res = __arch_xprod_64(___m, ___n, ___bias);			\
									\
	___res /= ___p;							\
})

#ifndef __arch_xprod_64

static inline uint64_t __arch_xprod_64(const uint64_t m, uint64_t n, bool bias)
{
	uint32_t m_lo = m;
	uint32_t m_hi = m >> 32;
	uint32_t n_lo = n;
	uint32_t n_hi = n >> 32;
	uint64_t res, tmp;

	if (!bias) {
		res = ((uint64_t)m_lo * n_lo) >> 32;
	} else if (!(m & ((1ULL << 63) | (1ULL << 31)))) {
		
		res = (m + (uint64_t)m_lo * n_lo) >> 32;
	} else {
		res = m + (uint64_t)m_lo * n_lo;
		tmp = (res < m) ? (1ULL << 32) : 0;
		res = (res >> 32) + tmp;
	}

	if (!(m & ((1ULL << 63) | (1ULL << 31)))) {
		
		res += (uint64_t)m_lo * n_hi;
		res += (uint64_t)m_hi * n_lo;
		res >>= 32;
	} else {
		tmp = res += (uint64_t)m_lo * n_hi;
		res += (uint64_t)m_hi * n_lo;
		tmp = (res < tmp) ? (1ULL << 32) : 0;
		res = (res >> 32) + tmp;
	}

	res += (uint64_t)m_hi * n_hi;

	return res;
}
#endif

#ifndef __div64_32
extern uint32_t __div64_32(uint64_t *dividend, uint32_t divisor);
#endif


# define do_div(n,base) ({				\
	uint32_t __base = (base);			\
	uint32_t __rem;					\
	(void)(((typeof((n)) *)0) == ((uint64_t *)0));	\
	if (__builtin_constant_p(__base) &&		\
	    is_power_of_2(__base)) {			\
		__rem = (n) & (__base - 1);		\
		(n) >>= ilog2(__base);			\
	} else if (__div64_const32_is_OK &&		\
		   __builtin_constant_p(__base) &&	\
		   __base != 0) {			\
		uint32_t __res_lo, __n_lo = (n);	\
		(n) = __div64_const32(n, __base);	\
		 \
		__res_lo = (n);				\
		__rem = __n_lo - __res_lo * __base;	\
	} else if (likely(((n) >> 32) == 0)) {		\
		__rem = (uint32_t)(n) % __base;		\
		(n) = (uint32_t)(n) / __base;		\
	} else 						\
		__rem = __div64_32(&(n), __base);	\
	__rem;						\
 })

#else 

# error do_div() does not yet support the C64

#endif 

#endif 
