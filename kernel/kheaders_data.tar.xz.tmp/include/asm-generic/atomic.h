
#ifndef __ASM_GENERIC_ATOMIC_H
#define __ASM_GENERIC_ATOMIC_H

#include <asm/cmpxchg.h>
#include <asm/barrier.h>





#ifdef CONFIG_SMP



#define ATOMIC_OP(op, c_op)						\
static inline void atomic_##op(int i, atomic_t *v)			\
{									\
	int c, old;							\
									\
	c = v->counter;							\
	while ((old = cmpxchg(&v->counter, c, c c_op i)) != c)		\
		c = old;						\
}

#define ATOMIC_OP_RETURN(op, c_op)					\
static inline int atomic_##op##_return(int i, atomic_t *v)		\
{									\
	int c, old;							\
									\
	c = v->counter;							\
	while ((old = cmpxchg(&v->counter, c, c c_op i)) != c)		\
		c = old;						\
									\
	return c c_op i;						\
}

#define ATOMIC_FETCH_OP(op, c_op)					\
static inline int atomic_fetch_##op(int i, atomic_t *v)			\
{									\
	int c, old;							\
									\
	c = v->counter;							\
	while ((old = cmpxchg(&v->counter, c, c c_op i)) != c)		\
		c = old;						\
									\
	return c;							\
}

#else

#include <linux/irqflags.h>

#define ATOMIC_OP(op, c_op)						\
static inline void atomic_##op(int i, atomic_t *v)			\
{									\
	unsigned long flags;						\
									\
	raw_local_irq_save(flags);					\
	v->counter = v->counter c_op i;					\
	raw_local_irq_restore(flags);					\
}

#define ATOMIC_OP_RETURN(op, c_op)					\
static inline int atomic_##op##_return(int i, atomic_t *v)		\
{									\
	unsigned long flags;						\
	int ret;							\
									\
	raw_local_irq_save(flags);					\
	ret = (v->counter = v->counter c_op i);				\
	raw_local_irq_restore(flags);					\
									\
	return ret;							\
}

#define ATOMIC_FETCH_OP(op, c_op)					\
static inline int atomic_fetch_##op(int i, atomic_t *v)			\
{									\
	unsigned long flags;						\
	int ret;							\
									\
	raw_local_irq_save(flags);					\
	ret = v->counter;						\
	v->counter = v->counter c_op i;					\
	raw_local_irq_restore(flags);					\
									\
	return ret;							\
}

#endif 

#ifndef atomic_add_return
ATOMIC_OP_RETURN(add, +)
#endif

#ifndef atomic_sub_return
ATOMIC_OP_RETURN(sub, -)
#endif

#ifndef atomic_fetch_add
ATOMIC_FETCH_OP(add, +)
#endif

#ifndef atomic_fetch_sub
ATOMIC_FETCH_OP(sub, -)
#endif

#ifndef atomic_fetch_and
ATOMIC_FETCH_OP(and, &)
#endif

#ifndef atomic_fetch_or
ATOMIC_FETCH_OP(or, |)
#endif

#ifndef atomic_fetch_xor
ATOMIC_FETCH_OP(xor, ^)
#endif

#ifndef atomic_and
ATOMIC_OP(and, &)
#endif

#ifndef atomic_or
ATOMIC_OP(or, |)
#endif

#ifndef atomic_xor
ATOMIC_OP(xor, ^)
#endif

#undef ATOMIC_FETCH_OP
#undef ATOMIC_OP_RETURN
#undef ATOMIC_OP



#define ATOMIC_INIT(i)	{ (i) }


#ifndef atomic_read
#define atomic_read(v)	READ_ONCE((v)->counter)
#endif


#define atomic_set(v, i) WRITE_ONCE(((v)->counter), (i))

#include <linux/irqflags.h>

static inline int atomic_add_negative(int i, atomic_t *v)
{
	return atomic_add_return(i, v) < 0;
}

static inline void atomic_add(int i, atomic_t *v)
{
	atomic_add_return(i, v);
}

static inline void atomic_sub(int i, atomic_t *v)
{
	atomic_sub_return(i, v);
}

static inline void atomic_inc(atomic_t *v)
{
	atomic_add_return(1, v);
}

static inline void atomic_dec(atomic_t *v)
{
	atomic_sub_return(1, v);
}

#define atomic_dec_return(v)		atomic_sub_return(1, (v))
#define atomic_inc_return(v)		atomic_add_return(1, (v))

#define atomic_sub_and_test(i, v)	(atomic_sub_return((i), (v)) == 0)
#define atomic_dec_and_test(v)		(atomic_dec_return(v) == 0)
#define atomic_inc_and_test(v)		(atomic_inc_return(v) == 0)

#define atomic_xchg(ptr, v)		(xchg(&(ptr)->counter, (v)))
#define atomic_cmpxchg(v, old, new)	(cmpxchg(&((v)->counter), (old), (new)))

static inline int __atomic_add_unless(atomic_t *v, int a, int u)
{
	int c, old;
	c = atomic_read(v);
	while (c != u && (old = atomic_cmpxchg(v, c, c + a)) != c)
		c = old;
	return c;
}

#endif 
