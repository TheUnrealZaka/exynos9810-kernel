/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _LINUX_PRANDOM_H
#define _LINUX_PRANDOM_H

#include <linux/types.h>
#include <linux/percpu.h>
#include <linux/siphash.h>

u32 prandom_u32(void);
void prandom_bytes(void *buf, size_t nbytes);
void prandom_seed(u32 seed);
void prandom_reseed_late(void);

#if BITS_PER_LONG == 64

#define PRND_SIPROUND(v0, v1, v2, v3) SIPHASH_PERMUTATION(v0, v1, v2, v3)

#define PRND_K0 (SIPHASH_CONST_0 ^ SIPHASH_CONST_2)
#define PRND_K1 (SIPHASH_CONST_1 ^ SIPHASH_CONST_3)

#elif BITS_PER_LONG == 32

#define PRND_SIPROUND(v0, v1, v2, v3) HSIPHASH_PERMUTATION(v0, v1, v2, v3)
#define PRND_K0 (HSIPHASH_CONST_0 ^ HSIPHASH_CONST_2)
#define PRND_K1 (HSIPHASH_CONST_1 ^ HSIPHASH_CONST_3)

#else
#error Unsupported BITS_PER_LONG
#endif

struct rnd_state {
	__u32 s1, s2, s3, s4;
};

u32 prandom_u32_state(struct rnd_state *state);
void prandom_bytes_state(struct rnd_state *state, void *buf, size_t nbytes);
void prandom_seed_full_state(struct rnd_state __percpu *pcpu_state);

#define prandom_init_once(pcpu_state)			\
	DO_ONCE(prandom_seed_full_state, (pcpu_state))


static inline u32 prandom_u32_max(u32 ep_ro)
{
	return (u32)(((u64) prandom_u32() * ep_ro) >> 32);
}


static inline u32 __seed(u32 x, u32 m)
{
	return (x < m) ? x + m : x;
}


static inline void prandom_seed_state(struct rnd_state *state, u64 seed)
{
	u32 i = ((seed >> 32) ^ (seed << 10) ^ seed) & 0xffffffffUL;

	state->s1 = __seed(i,   2U);
	state->s2 = __seed(i,   8U);
	state->s3 = __seed(i,  16U);
	state->s4 = __seed(i, 128U);
}


static inline u32 next_pseudo_random32(u32 seed)
{
	return seed * 1664525 + 1013904223;
}

#endif
