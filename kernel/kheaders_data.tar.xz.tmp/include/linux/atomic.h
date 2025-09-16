
#ifndef _LINUX_ATOMIC_H
#define _LINUX_ATOMIC_H
#include <asm/atomic.h>
#include <asm/barrier.h>



#ifndef atomic_read_acquire
#define  atomic_read_acquire(v)		smp_load_acquire(&(v)->counter)
#endif

#ifndef atomic_set_release
#define  atomic_set_release(v, i)	smp_store_release(&(v)->counter, (i))
#endif


#ifndef __atomic_op_acquire
#define __atomic_op_acquire(op, args...)				\
({									\
	typeof(op##_relaxed(args)) __ret  = op##_relaxed(args);		\
	smp_mb__after_atomic();						\
	__ret;								\
})
#endif

#ifndef __atomic_op_release
#define __atomic_op_release(op, args...)				\
({									\
	smp_mb__before_atomic();					\
	op##_relaxed(args);						\
})
#endif

#ifndef __atomic_op_fence
#define __atomic_op_fence(op, args...)					\
({									\
	typeof(op##_relaxed(args)) __ret;				\
	smp_mb__before_atomic();					\
	__ret = op##_relaxed(args);					\
	smp_mb__after_atomic();						\
	__ret;								\
})
#endif


#ifndef atomic_add_return_relaxed
#define  atomic_add_return_relaxed	atomic_add_return
#define  atomic_add_return_acquire	atomic_add_return
#define  atomic_add_return_release	atomic_add_return

#else 

#ifndef atomic_add_return_acquire
#define  atomic_add_return_acquire(...)					\
	__atomic_op_acquire(atomic_add_return, __VA_ARGS__)
#endif

#ifndef atomic_add_return_release
#define  atomic_add_return_release(...)					\
	__atomic_op_release(atomic_add_return, __VA_ARGS__)
#endif

#ifndef atomic_add_return
#define  atomic_add_return(...)						\
	__atomic_op_fence(atomic_add_return, __VA_ARGS__)
#endif
#endif 


#ifndef atomic_inc_return_relaxed
#define  atomic_inc_return_relaxed	atomic_inc_return
#define  atomic_inc_return_acquire	atomic_inc_return
#define  atomic_inc_return_release	atomic_inc_return

#else 

#ifndef atomic_inc_return_acquire
#define  atomic_inc_return_acquire(...)					\
	__atomic_op_acquire(atomic_inc_return, __VA_ARGS__)
#endif

#ifndef atomic_inc_return_release
#define  atomic_inc_return_release(...)					\
	__atomic_op_release(atomic_inc_return, __VA_ARGS__)
#endif

#ifndef atomic_inc_return
#define  atomic_inc_return(...)						\
	__atomic_op_fence(atomic_inc_return, __VA_ARGS__)
#endif
#endif 


#ifndef atomic_sub_return_relaxed
#define  atomic_sub_return_relaxed	atomic_sub_return
#define  atomic_sub_return_acquire	atomic_sub_return
#define  atomic_sub_return_release	atomic_sub_return

#else 

#ifndef atomic_sub_return_acquire
#define  atomic_sub_return_acquire(...)					\
	__atomic_op_acquire(atomic_sub_return, __VA_ARGS__)
#endif

#ifndef atomic_sub_return_release
#define  atomic_sub_return_release(...)					\
	__atomic_op_release(atomic_sub_return, __VA_ARGS__)
#endif

#ifndef atomic_sub_return
#define  atomic_sub_return(...)						\
	__atomic_op_fence(atomic_sub_return, __VA_ARGS__)
#endif
#endif 


#ifndef atomic_dec_return_relaxed
#define  atomic_dec_return_relaxed	atomic_dec_return
#define  atomic_dec_return_acquire	atomic_dec_return
#define  atomic_dec_return_release	atomic_dec_return

#else 

#ifndef atomic_dec_return_acquire
#define  atomic_dec_return_acquire(...)					\
	__atomic_op_acquire(atomic_dec_return, __VA_ARGS__)
#endif

#ifndef atomic_dec_return_release
#define  atomic_dec_return_release(...)					\
	__atomic_op_release(atomic_dec_return, __VA_ARGS__)
#endif

#ifndef atomic_dec_return
#define  atomic_dec_return(...)						\
	__atomic_op_fence(atomic_dec_return, __VA_ARGS__)
#endif
#endif 



#ifndef atomic_fetch_add_relaxed
#define atomic_fetch_add_relaxed	atomic_fetch_add
#define atomic_fetch_add_acquire	atomic_fetch_add
#define atomic_fetch_add_release	atomic_fetch_add

#else 

#ifndef atomic_fetch_add_acquire
#define atomic_fetch_add_acquire(...)					\
	__atomic_op_acquire(atomic_fetch_add, __VA_ARGS__)
#endif

#ifndef atomic_fetch_add_release
#define atomic_fetch_add_release(...)					\
	__atomic_op_release(atomic_fetch_add, __VA_ARGS__)
#endif

#ifndef atomic_fetch_add
#define atomic_fetch_add(...)						\
	__atomic_op_fence(atomic_fetch_add, __VA_ARGS__)
#endif
#endif 


#ifndef atomic_fetch_inc_relaxed

#ifndef atomic_fetch_inc
#define atomic_fetch_inc(v)	        atomic_fetch_add(1, (v))
#define atomic_fetch_inc_relaxed(v)	atomic_fetch_add_relaxed(1, (v))
#define atomic_fetch_inc_acquire(v)	atomic_fetch_add_acquire(1, (v))
#define atomic_fetch_inc_release(v)	atomic_fetch_add_release(1, (v))
#else 
#define atomic_fetch_inc_relaxed	atomic_fetch_inc
#define atomic_fetch_inc_acquire	atomic_fetch_inc
#define atomic_fetch_inc_release	atomic_fetch_inc
#endif 

#else 

#ifndef atomic_fetch_inc_acquire
#define atomic_fetch_inc_acquire(...)					\
	__atomic_op_acquire(atomic_fetch_inc, __VA_ARGS__)
#endif

#ifndef atomic_fetch_inc_release
#define atomic_fetch_inc_release(...)					\
	__atomic_op_release(atomic_fetch_inc, __VA_ARGS__)
#endif

#ifndef atomic_fetch_inc
#define atomic_fetch_inc(...)						\
	__atomic_op_fence(atomic_fetch_inc, __VA_ARGS__)
#endif
#endif 


#ifndef atomic_fetch_sub_relaxed
#define atomic_fetch_sub_relaxed	atomic_fetch_sub
#define atomic_fetch_sub_acquire	atomic_fetch_sub
#define atomic_fetch_sub_release	atomic_fetch_sub

#else 

#ifndef atomic_fetch_sub_acquire
#define atomic_fetch_sub_acquire(...)					\
	__atomic_op_acquire(atomic_fetch_sub, __VA_ARGS__)
#endif

#ifndef atomic_fetch_sub_release
#define atomic_fetch_sub_release(...)					\
	__atomic_op_release(atomic_fetch_sub, __VA_ARGS__)
#endif

#ifndef atomic_fetch_sub
#define atomic_fetch_sub(...)						\
	__atomic_op_fence(atomic_fetch_sub, __VA_ARGS__)
#endif
#endif 


#ifndef atomic_fetch_dec_relaxed

#ifndef atomic_fetch_dec
#define atomic_fetch_dec(v)	        atomic_fetch_sub(1, (v))
#define atomic_fetch_dec_relaxed(v)	atomic_fetch_sub_relaxed(1, (v))
#define atomic_fetch_dec_acquire(v)	atomic_fetch_sub_acquire(1, (v))
#define atomic_fetch_dec_release(v)	atomic_fetch_sub_release(1, (v))
#else 
#define atomic_fetch_dec_relaxed	atomic_fetch_dec
#define atomic_fetch_dec_acquire	atomic_fetch_dec
#define atomic_fetch_dec_release	atomic_fetch_dec
#endif 

#else 

#ifndef atomic_fetch_dec_acquire
#define atomic_fetch_dec_acquire(...)					\
	__atomic_op_acquire(atomic_fetch_dec, __VA_ARGS__)
#endif

#ifndef atomic_fetch_dec_release
#define atomic_fetch_dec_release(...)					\
	__atomic_op_release(atomic_fetch_dec, __VA_ARGS__)
#endif

#ifndef atomic_fetch_dec
#define atomic_fetch_dec(...)						\
	__atomic_op_fence(atomic_fetch_dec, __VA_ARGS__)
#endif
#endif 


#ifndef atomic_fetch_or_relaxed
#define atomic_fetch_or_relaxed	atomic_fetch_or
#define atomic_fetch_or_acquire	atomic_fetch_or
#define atomic_fetch_or_release	atomic_fetch_or

#else 

#ifndef atomic_fetch_or_acquire
#define atomic_fetch_or_acquire(...)					\
	__atomic_op_acquire(atomic_fetch_or, __VA_ARGS__)
#endif

#ifndef atomic_fetch_or_release
#define atomic_fetch_or_release(...)					\
	__atomic_op_release(atomic_fetch_or, __VA_ARGS__)
#endif

#ifndef atomic_fetch_or
#define atomic_fetch_or(...)						\
	__atomic_op_fence(atomic_fetch_or, __VA_ARGS__)
#endif
#endif 


#ifndef atomic_fetch_and_relaxed
#define atomic_fetch_and_relaxed	atomic_fetch_and
#define atomic_fetch_and_acquire	atomic_fetch_and
#define atomic_fetch_and_release	atomic_fetch_and

#else 

#ifndef atomic_fetch_and_acquire
#define atomic_fetch_and_acquire(...)					\
	__atomic_op_acquire(atomic_fetch_and, __VA_ARGS__)
#endif

#ifndef atomic_fetch_and_release
#define atomic_fetch_and_release(...)					\
	__atomic_op_release(atomic_fetch_and, __VA_ARGS__)
#endif

#ifndef atomic_fetch_and
#define atomic_fetch_and(...)						\
	__atomic_op_fence(atomic_fetch_and, __VA_ARGS__)
#endif
#endif 

#ifdef atomic_andnot

#ifndef atomic_fetch_andnot_relaxed
#define atomic_fetch_andnot_relaxed	atomic_fetch_andnot
#define atomic_fetch_andnot_acquire	atomic_fetch_andnot
#define atomic_fetch_andnot_release	atomic_fetch_andnot

#else 

#ifndef atomic_fetch_andnot_acquire
#define atomic_fetch_andnot_acquire(...)					\
	__atomic_op_acquire(atomic_fetch_andnot, __VA_ARGS__)
#endif

#ifndef atomic_fetch_andnot_release
#define atomic_fetch_andnot_release(...)					\
	__atomic_op_release(atomic_fetch_andnot, __VA_ARGS__)
#endif

#ifndef atomic_fetch_andnot
#define atomic_fetch_andnot(...)						\
	__atomic_op_fence(atomic_fetch_andnot, __VA_ARGS__)
#endif
#endif 
#endif 


#ifndef atomic_fetch_xor_relaxed
#define atomic_fetch_xor_relaxed	atomic_fetch_xor
#define atomic_fetch_xor_acquire	atomic_fetch_xor
#define atomic_fetch_xor_release	atomic_fetch_xor

#else 

#ifndef atomic_fetch_xor_acquire
#define atomic_fetch_xor_acquire(...)					\
	__atomic_op_acquire(atomic_fetch_xor, __VA_ARGS__)
#endif

#ifndef atomic_fetch_xor_release
#define atomic_fetch_xor_release(...)					\
	__atomic_op_release(atomic_fetch_xor, __VA_ARGS__)
#endif

#ifndef atomic_fetch_xor
#define atomic_fetch_xor(...)						\
	__atomic_op_fence(atomic_fetch_xor, __VA_ARGS__)
#endif
#endif 



#ifndef atomic_xchg_relaxed
#define  atomic_xchg_relaxed		atomic_xchg
#define  atomic_xchg_acquire		atomic_xchg
#define  atomic_xchg_release		atomic_xchg

#else 

#ifndef atomic_xchg_acquire
#define  atomic_xchg_acquire(...)					\
	__atomic_op_acquire(atomic_xchg, __VA_ARGS__)
#endif

#ifndef atomic_xchg_release
#define  atomic_xchg_release(...)					\
	__atomic_op_release(atomic_xchg, __VA_ARGS__)
#endif

#ifndef atomic_xchg
#define  atomic_xchg(...)						\
	__atomic_op_fence(atomic_xchg, __VA_ARGS__)
#endif
#endif 


#ifndef atomic_cmpxchg_relaxed
#define  atomic_cmpxchg_relaxed		atomic_cmpxchg
#define  atomic_cmpxchg_acquire		atomic_cmpxchg
#define  atomic_cmpxchg_release		atomic_cmpxchg

#else 

#ifndef atomic_cmpxchg_acquire
#define  atomic_cmpxchg_acquire(...)					\
	__atomic_op_acquire(atomic_cmpxchg, __VA_ARGS__)
#endif

#ifndef atomic_cmpxchg_release
#define  atomic_cmpxchg_release(...)					\
	__atomic_op_release(atomic_cmpxchg, __VA_ARGS__)
#endif

#ifndef atomic_cmpxchg
#define  atomic_cmpxchg(...)						\
	__atomic_op_fence(atomic_cmpxchg, __VA_ARGS__)
#endif
#endif 

#ifndef atomic_try_cmpxchg

#define __atomic_try_cmpxchg(type, _p, _po, _n)				\
({									\
	typeof(_po) __po = (_po);					\
	typeof(*(_po)) __r, __o = *__po;				\
	__r = atomic_cmpxchg##type((_p), __o, (_n));			\
	if (unlikely(__r != __o))					\
		*__po = __r;						\
	likely(__r == __o);						\
})

#define atomic_try_cmpxchg(_p, _po, _n)		__atomic_try_cmpxchg(, _p, _po, _n)
#define atomic_try_cmpxchg_relaxed(_p, _po, _n)	__atomic_try_cmpxchg(_relaxed, _p, _po, _n)
#define atomic_try_cmpxchg_acquire(_p, _po, _n)	__atomic_try_cmpxchg(_acquire, _p, _po, _n)
#define atomic_try_cmpxchg_release(_p, _po, _n)	__atomic_try_cmpxchg(_release, _p, _po, _n)

#else 
#define atomic_try_cmpxchg_relaxed	atomic_try_cmpxchg
#define atomic_try_cmpxchg_acquire	atomic_try_cmpxchg
#define atomic_try_cmpxchg_release	atomic_try_cmpxchg
#endif 


#ifndef cmpxchg_relaxed
#define  cmpxchg_relaxed		cmpxchg
#define  cmpxchg_acquire		cmpxchg
#define  cmpxchg_release		cmpxchg

#else 

#ifndef cmpxchg_acquire
#define  cmpxchg_acquire(...)						\
	__atomic_op_acquire(cmpxchg, __VA_ARGS__)
#endif

#ifndef cmpxchg_release
#define  cmpxchg_release(...)						\
	__atomic_op_release(cmpxchg, __VA_ARGS__)
#endif

#ifndef cmpxchg
#define  cmpxchg(...)							\
	__atomic_op_fence(cmpxchg, __VA_ARGS__)
#endif
#endif 


#ifndef cmpxchg64_relaxed
#define  cmpxchg64_relaxed		cmpxchg64
#define  cmpxchg64_acquire		cmpxchg64
#define  cmpxchg64_release		cmpxchg64

#else 

#ifndef cmpxchg64_acquire
#define  cmpxchg64_acquire(...)						\
	__atomic_op_acquire(cmpxchg64, __VA_ARGS__)
#endif

#ifndef cmpxchg64_release
#define  cmpxchg64_release(...)						\
	__atomic_op_release(cmpxchg64, __VA_ARGS__)
#endif

#ifndef cmpxchg64
#define  cmpxchg64(...)							\
	__atomic_op_fence(cmpxchg64, __VA_ARGS__)
#endif
#endif 


#ifndef xchg_relaxed
#define  xchg_relaxed			xchg
#define  xchg_acquire			xchg
#define  xchg_release			xchg

#else 

#ifndef xchg_acquire
#define  xchg_acquire(...)		__atomic_op_acquire(xchg, __VA_ARGS__)
#endif

#ifndef xchg_release
#define  xchg_release(...)		__atomic_op_release(xchg, __VA_ARGS__)
#endif

#ifndef xchg
#define  xchg(...)			__atomic_op_fence(xchg, __VA_ARGS__)
#endif
#endif 


static inline int atomic_add_unless(atomic_t *v, int a, int u)
{
	return __atomic_add_unless(v, a, u) != u;
}


#ifndef atomic_inc_not_zero
#define atomic_inc_not_zero(v)		atomic_add_unless((v), 1, 0)
#endif

#ifndef atomic_andnot
static inline void atomic_andnot(int i, atomic_t *v)
{
	atomic_and(~i, v);
}

static inline int atomic_fetch_andnot(int i, atomic_t *v)
{
	return atomic_fetch_and(~i, v);
}

static inline int atomic_fetch_andnot_relaxed(int i, atomic_t *v)
{
	return atomic_fetch_and_relaxed(~i, v);
}

static inline int atomic_fetch_andnot_acquire(int i, atomic_t *v)
{
	return atomic_fetch_and_acquire(~i, v);
}

static inline int atomic_fetch_andnot_release(int i, atomic_t *v)
{
	return atomic_fetch_and_release(~i, v);
}
#endif


#ifndef atomic_inc_not_zero_hint
static inline int atomic_inc_not_zero_hint(atomic_t *v, int hint)
{
	int val, c = hint;

	
	if (!hint)
		return atomic_inc_not_zero(v);

	do {
		val = atomic_cmpxchg(v, c, c + 1);
		if (val == c)
			return 1;
		c = val;
	} while (c);

	return 0;
}
#endif

#ifndef atomic_inc_unless_negative
static inline int atomic_inc_unless_negative(atomic_t *p)
{
	int v, v1;
	for (v = 0; v >= 0; v = v1) {
		v1 = atomic_cmpxchg(p, v, v + 1);
		if (likely(v1 == v))
			return 1;
	}
	return 0;
}
#endif

#ifndef atomic_dec_unless_positive
static inline int atomic_dec_unless_positive(atomic_t *p)
{
	int v, v1;
	for (v = 0; v <= 0; v = v1) {
		v1 = atomic_cmpxchg(p, v, v - 1);
		if (likely(v1 == v))
			return 1;
	}
	return 0;
}
#endif


#ifndef atomic_dec_if_positive
static inline int atomic_dec_if_positive(atomic_t *v)
{
	int c, old, dec;
	c = atomic_read(v);
	for (;;) {
		dec = c - 1;
		if (unlikely(dec < 0))
			break;
		old = atomic_cmpxchg((v), c, dec);
		if (likely(old == c))
			break;
		c = old;
	}
	return dec;
}
#endif

#ifdef CONFIG_GENERIC_ATOMIC64
#include <asm-generic/atomic64.h>
#endif

#ifndef atomic64_read_acquire
#define  atomic64_read_acquire(v)	smp_load_acquire(&(v)->counter)
#endif

#ifndef atomic64_set_release
#define  atomic64_set_release(v, i)	smp_store_release(&(v)->counter, (i))
#endif


#ifndef atomic64_add_return_relaxed
#define  atomic64_add_return_relaxed	atomic64_add_return
#define  atomic64_add_return_acquire	atomic64_add_return
#define  atomic64_add_return_release	atomic64_add_return

#else 

#ifndef atomic64_add_return_acquire
#define  atomic64_add_return_acquire(...)				\
	__atomic_op_acquire(atomic64_add_return, __VA_ARGS__)
#endif

#ifndef atomic64_add_return_release
#define  atomic64_add_return_release(...)				\
	__atomic_op_release(atomic64_add_return, __VA_ARGS__)
#endif

#ifndef atomic64_add_return
#define  atomic64_add_return(...)					\
	__atomic_op_fence(atomic64_add_return, __VA_ARGS__)
#endif
#endif 


#ifndef atomic64_inc_return_relaxed
#define  atomic64_inc_return_relaxed	atomic64_inc_return
#define  atomic64_inc_return_acquire	atomic64_inc_return
#define  atomic64_inc_return_release	atomic64_inc_return

#else 

#ifndef atomic64_inc_return_acquire
#define  atomic64_inc_return_acquire(...)				\
	__atomic_op_acquire(atomic64_inc_return, __VA_ARGS__)
#endif

#ifndef atomic64_inc_return_release
#define  atomic64_inc_return_release(...)				\
	__atomic_op_release(atomic64_inc_return, __VA_ARGS__)
#endif

#ifndef atomic64_inc_return
#define  atomic64_inc_return(...)					\
	__atomic_op_fence(atomic64_inc_return, __VA_ARGS__)
#endif
#endif 



#ifndef atomic64_sub_return_relaxed
#define  atomic64_sub_return_relaxed	atomic64_sub_return
#define  atomic64_sub_return_acquire	atomic64_sub_return
#define  atomic64_sub_return_release	atomic64_sub_return

#else 

#ifndef atomic64_sub_return_acquire
#define  atomic64_sub_return_acquire(...)				\
	__atomic_op_acquire(atomic64_sub_return, __VA_ARGS__)
#endif

#ifndef atomic64_sub_return_release
#define  atomic64_sub_return_release(...)				\
	__atomic_op_release(atomic64_sub_return, __VA_ARGS__)
#endif

#ifndef atomic64_sub_return
#define  atomic64_sub_return(...)					\
	__atomic_op_fence(atomic64_sub_return, __VA_ARGS__)
#endif
#endif 


#ifndef atomic64_dec_return_relaxed
#define  atomic64_dec_return_relaxed	atomic64_dec_return
#define  atomic64_dec_return_acquire	atomic64_dec_return
#define  atomic64_dec_return_release	atomic64_dec_return

#else 

#ifndef atomic64_dec_return_acquire
#define  atomic64_dec_return_acquire(...)				\
	__atomic_op_acquire(atomic64_dec_return, __VA_ARGS__)
#endif

#ifndef atomic64_dec_return_release
#define  atomic64_dec_return_release(...)				\
	__atomic_op_release(atomic64_dec_return, __VA_ARGS__)
#endif

#ifndef atomic64_dec_return
#define  atomic64_dec_return(...)					\
	__atomic_op_fence(atomic64_dec_return, __VA_ARGS__)
#endif
#endif 



#ifndef atomic64_fetch_add_relaxed
#define atomic64_fetch_add_relaxed	atomic64_fetch_add
#define atomic64_fetch_add_acquire	atomic64_fetch_add
#define atomic64_fetch_add_release	atomic64_fetch_add

#else 

#ifndef atomic64_fetch_add_acquire
#define atomic64_fetch_add_acquire(...)					\
	__atomic_op_acquire(atomic64_fetch_add, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_add_release
#define atomic64_fetch_add_release(...)					\
	__atomic_op_release(atomic64_fetch_add, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_add
#define atomic64_fetch_add(...)						\
	__atomic_op_fence(atomic64_fetch_add, __VA_ARGS__)
#endif
#endif 


#ifndef atomic64_fetch_inc_relaxed

#ifndef atomic64_fetch_inc
#define atomic64_fetch_inc(v)		atomic64_fetch_add(1, (v))
#define atomic64_fetch_inc_relaxed(v)	atomic64_fetch_add_relaxed(1, (v))
#define atomic64_fetch_inc_acquire(v)	atomic64_fetch_add_acquire(1, (v))
#define atomic64_fetch_inc_release(v)	atomic64_fetch_add_release(1, (v))
#else 
#define atomic64_fetch_inc_relaxed	atomic64_fetch_inc
#define atomic64_fetch_inc_acquire	atomic64_fetch_inc
#define atomic64_fetch_inc_release	atomic64_fetch_inc
#endif 

#else 

#ifndef atomic64_fetch_inc_acquire
#define atomic64_fetch_inc_acquire(...)					\
	__atomic_op_acquire(atomic64_fetch_inc, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_inc_release
#define atomic64_fetch_inc_release(...)					\
	__atomic_op_release(atomic64_fetch_inc, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_inc
#define atomic64_fetch_inc(...)						\
	__atomic_op_fence(atomic64_fetch_inc, __VA_ARGS__)
#endif
#endif 


#ifndef atomic64_fetch_sub_relaxed
#define atomic64_fetch_sub_relaxed	atomic64_fetch_sub
#define atomic64_fetch_sub_acquire	atomic64_fetch_sub
#define atomic64_fetch_sub_release	atomic64_fetch_sub

#else 

#ifndef atomic64_fetch_sub_acquire
#define atomic64_fetch_sub_acquire(...)					\
	__atomic_op_acquire(atomic64_fetch_sub, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_sub_release
#define atomic64_fetch_sub_release(...)					\
	__atomic_op_release(atomic64_fetch_sub, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_sub
#define atomic64_fetch_sub(...)						\
	__atomic_op_fence(atomic64_fetch_sub, __VA_ARGS__)
#endif
#endif 


#ifndef atomic64_fetch_dec_relaxed

#ifndef atomic64_fetch_dec
#define atomic64_fetch_dec(v)		atomic64_fetch_sub(1, (v))
#define atomic64_fetch_dec_relaxed(v)	atomic64_fetch_sub_relaxed(1, (v))
#define atomic64_fetch_dec_acquire(v)	atomic64_fetch_sub_acquire(1, (v))
#define atomic64_fetch_dec_release(v)	atomic64_fetch_sub_release(1, (v))
#else 
#define atomic64_fetch_dec_relaxed	atomic64_fetch_dec
#define atomic64_fetch_dec_acquire	atomic64_fetch_dec
#define atomic64_fetch_dec_release	atomic64_fetch_dec
#endif 

#else 

#ifndef atomic64_fetch_dec_acquire
#define atomic64_fetch_dec_acquire(...)					\
	__atomic_op_acquire(atomic64_fetch_dec, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_dec_release
#define atomic64_fetch_dec_release(...)					\
	__atomic_op_release(atomic64_fetch_dec, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_dec
#define atomic64_fetch_dec(...)						\
	__atomic_op_fence(atomic64_fetch_dec, __VA_ARGS__)
#endif
#endif 


#ifndef atomic64_fetch_or_relaxed
#define atomic64_fetch_or_relaxed	atomic64_fetch_or
#define atomic64_fetch_or_acquire	atomic64_fetch_or
#define atomic64_fetch_or_release	atomic64_fetch_or

#else 

#ifndef atomic64_fetch_or_acquire
#define atomic64_fetch_or_acquire(...)					\
	__atomic_op_acquire(atomic64_fetch_or, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_or_release
#define atomic64_fetch_or_release(...)					\
	__atomic_op_release(atomic64_fetch_or, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_or
#define atomic64_fetch_or(...)						\
	__atomic_op_fence(atomic64_fetch_or, __VA_ARGS__)
#endif
#endif 


#ifndef atomic64_fetch_and_relaxed
#define atomic64_fetch_and_relaxed	atomic64_fetch_and
#define atomic64_fetch_and_acquire	atomic64_fetch_and
#define atomic64_fetch_and_release	atomic64_fetch_and

#else 

#ifndef atomic64_fetch_and_acquire
#define atomic64_fetch_and_acquire(...)					\
	__atomic_op_acquire(atomic64_fetch_and, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_and_release
#define atomic64_fetch_and_release(...)					\
	__atomic_op_release(atomic64_fetch_and, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_and
#define atomic64_fetch_and(...)						\
	__atomic_op_fence(atomic64_fetch_and, __VA_ARGS__)
#endif
#endif 

#ifdef atomic64_andnot

#ifndef atomic64_fetch_andnot_relaxed
#define atomic64_fetch_andnot_relaxed	atomic64_fetch_andnot
#define atomic64_fetch_andnot_acquire	atomic64_fetch_andnot
#define atomic64_fetch_andnot_release	atomic64_fetch_andnot

#else 

#ifndef atomic64_fetch_andnot_acquire
#define atomic64_fetch_andnot_acquire(...)					\
	__atomic_op_acquire(atomic64_fetch_andnot, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_andnot_release
#define atomic64_fetch_andnot_release(...)					\
	__atomic_op_release(atomic64_fetch_andnot, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_andnot
#define atomic64_fetch_andnot(...)						\
	__atomic_op_fence(atomic64_fetch_andnot, __VA_ARGS__)
#endif
#endif 
#endif 


#ifndef atomic64_fetch_xor_relaxed
#define atomic64_fetch_xor_relaxed	atomic64_fetch_xor
#define atomic64_fetch_xor_acquire	atomic64_fetch_xor
#define atomic64_fetch_xor_release	atomic64_fetch_xor

#else 

#ifndef atomic64_fetch_xor_acquire
#define atomic64_fetch_xor_acquire(...)					\
	__atomic_op_acquire(atomic64_fetch_xor, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_xor_release
#define atomic64_fetch_xor_release(...)					\
	__atomic_op_release(atomic64_fetch_xor, __VA_ARGS__)
#endif

#ifndef atomic64_fetch_xor
#define atomic64_fetch_xor(...)						\
	__atomic_op_fence(atomic64_fetch_xor, __VA_ARGS__)
#endif
#endif 



#ifndef atomic64_xchg_relaxed
#define  atomic64_xchg_relaxed		atomic64_xchg
#define  atomic64_xchg_acquire		atomic64_xchg
#define  atomic64_xchg_release		atomic64_xchg

#else 

#ifndef atomic64_xchg_acquire
#define  atomic64_xchg_acquire(...)					\
	__atomic_op_acquire(atomic64_xchg, __VA_ARGS__)
#endif

#ifndef atomic64_xchg_release
#define  atomic64_xchg_release(...)					\
	__atomic_op_release(atomic64_xchg, __VA_ARGS__)
#endif

#ifndef atomic64_xchg
#define  atomic64_xchg(...)						\
	__atomic_op_fence(atomic64_xchg, __VA_ARGS__)
#endif
#endif 


#ifndef atomic64_cmpxchg_relaxed
#define  atomic64_cmpxchg_relaxed	atomic64_cmpxchg
#define  atomic64_cmpxchg_acquire	atomic64_cmpxchg
#define  atomic64_cmpxchg_release	atomic64_cmpxchg

#else 

#ifndef atomic64_cmpxchg_acquire
#define  atomic64_cmpxchg_acquire(...)					\
	__atomic_op_acquire(atomic64_cmpxchg, __VA_ARGS__)
#endif

#ifndef atomic64_cmpxchg_release
#define  atomic64_cmpxchg_release(...)					\
	__atomic_op_release(atomic64_cmpxchg, __VA_ARGS__)
#endif

#ifndef atomic64_cmpxchg
#define  atomic64_cmpxchg(...)						\
	__atomic_op_fence(atomic64_cmpxchg, __VA_ARGS__)
#endif
#endif 

#ifndef atomic64_try_cmpxchg

#define __atomic64_try_cmpxchg(type, _p, _po, _n)			\
({									\
	typeof(_po) __po = (_po);					\
	typeof(*(_po)) __r, __o = *__po;				\
	__r = atomic64_cmpxchg##type((_p), __o, (_n));			\
	if (unlikely(__r != __o))					\
		*__po = __r;						\
	likely(__r == __o);						\
})

#define atomic64_try_cmpxchg(_p, _po, _n)		__atomic64_try_cmpxchg(, _p, _po, _n)
#define atomic64_try_cmpxchg_relaxed(_p, _po, _n)	__atomic64_try_cmpxchg(_relaxed, _p, _po, _n)
#define atomic64_try_cmpxchg_acquire(_p, _po, _n)	__atomic64_try_cmpxchg(_acquire, _p, _po, _n)
#define atomic64_try_cmpxchg_release(_p, _po, _n)	__atomic64_try_cmpxchg(_release, _p, _po, _n)

#else 
#define atomic64_try_cmpxchg_relaxed	atomic64_try_cmpxchg
#define atomic64_try_cmpxchg_acquire	atomic64_try_cmpxchg
#define atomic64_try_cmpxchg_release	atomic64_try_cmpxchg
#endif 

#ifndef atomic64_andnot
static inline void atomic64_andnot(long long i, atomic64_t *v)
{
	atomic64_and(~i, v);
}

static inline long long atomic64_fetch_andnot(long long i, atomic64_t *v)
{
	return atomic64_fetch_and(~i, v);
}

static inline long long atomic64_fetch_andnot_relaxed(long long i, atomic64_t *v)
{
	return atomic64_fetch_and_relaxed(~i, v);
}

static inline long long atomic64_fetch_andnot_acquire(long long i, atomic64_t *v)
{
	return atomic64_fetch_and_acquire(~i, v);
}

static inline long long atomic64_fetch_andnot_release(long long i, atomic64_t *v)
{
	return atomic64_fetch_and_release(~i, v);
}
#endif

#include <asm-generic/atomic-long.h>

#endif 
