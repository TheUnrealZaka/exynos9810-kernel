


#ifndef _CRYPTO_B128OPS_H
#define _CRYPTO_B128OPS_H

#include <linux/types.h>

typedef struct {
	u64 a, b;
} u128;

typedef struct {
	__be64 a, b;
} be128;

typedef struct {
	__le64 b, a;
} le128;

static inline void u128_xor(u128 *r, const u128 *p, const u128 *q)
{
	r->a = p->a ^ q->a;
	r->b = p->b ^ q->b;
}

static inline void be128_xor(be128 *r, const be128 *p, const be128 *q)
{
	u128_xor((u128 *)r, (u128 *)p, (u128 *)q);
}

static inline void le128_xor(le128 *r, const le128 *p, const le128 *q)
{
	u128_xor((u128 *)r, (u128 *)p, (u128 *)q);
}

#endif 
