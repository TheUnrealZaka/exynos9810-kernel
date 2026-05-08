#ifndef _LINUX_HASH_H
#define _LINUX_HASH_H


#include <asm/types.h>
#include <linux/compiler.h>


#if BITS_PER_LONG == 32
#define GOLDEN_RATIO_PRIME GOLDEN_RATIO_32
#define hash_long(val, bits) hash_32(val, bits)
#elif BITS_PER_LONG == 64
#define hash_long(val, bits) hash_64(val, bits)
#define GOLDEN_RATIO_PRIME GOLDEN_RATIO_64
#else
#error Wordsize not 32 or 64
#endif


#define GOLDEN_RATIO_32 0x61C88647
#define GOLDEN_RATIO_64 0x61C8864680B583EBull

#ifdef CONFIG_HAVE_ARCH_HASH

#include <asm/hash.h>
#endif


#ifndef HAVE_ARCH__HASH_32
#define __hash_32 __hash_32_generic
#endif
static inline u32 __hash_32_generic(u32 val)
{
	return val * GOLDEN_RATIO_32;
}

#ifndef HAVE_ARCH_HASH_32
#define hash_32 hash_32_generic
#endif
static inline u32 hash_32_generic(u32 val, unsigned int bits)
{
	
	return __hash_32(val) >> (32 - bits);
}

#ifndef HAVE_ARCH_HASH_64
#define hash_64 hash_64_generic
#endif
static __always_inline u32 hash_64_generic(u64 val, unsigned int bits)
{
#if BITS_PER_LONG == 64
	
	return val * GOLDEN_RATIO_64 >> (64 - bits);
#else
	
	return hash_32((u32)val ^ __hash_32(val >> 32), bits);
#endif
}

static inline u32 hash_ptr(const void *ptr, unsigned int bits)
{
	return hash_long((unsigned long)ptr, bits);
}


static inline u32 hash32_ptr(const void *ptr)
{
	unsigned long val = (unsigned long)ptr;

#if BITS_PER_LONG == 64
	val ^= (val >> 32);
#endif
	return (u32)val;
}

#endif 
