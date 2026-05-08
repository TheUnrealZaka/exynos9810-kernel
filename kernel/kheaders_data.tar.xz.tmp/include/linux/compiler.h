#ifndef __LINUX_COMPILER_H
#define __LINUX_COMPILER_H

#ifndef __ASSEMBLY__

#ifdef __CHECKER__
# define __user		__attribute__((noderef, address_space(1)))
# define __kernel	__attribute__((address_space(0)))
# define __safe		__attribute__((safe))
# define __force	__attribute__((force))
# define __nocast	__attribute__((nocast))
# define __iomem	__attribute__((noderef, address_space(2)))
# define __must_hold(x)	__attribute__((context(x,1,1)))
# define __acquires(x)	__attribute__((context(x,0,1)))
# define __releases(x)	__attribute__((context(x,1,0)))
# define __acquire(x)	__context__(x,1)
# define __release(x)	__context__(x,-1)
# define __cond_lock(x,c)	((c) ? ({ __acquire(x); 1; }) : 0)
# define __percpu	__attribute__((noderef, address_space(3)))
#ifdef CONFIG_SPARSE_RCU_POINTER
# define __rcu		__attribute__((noderef, address_space(4)))
#else 
# define __rcu
#endif 
# define __private	__attribute__((noderef))
extern void __chk_user_ptr(const volatile void __user *);
extern void __chk_io_ptr(const volatile void __iomem *);
# define ACCESS_PRIVATE(p, member) (*((typeof((p)->member) __force *) &(p)->member))
#else 
# define __user
# define __kernel
# define __safe
# define __force
# define __nocast
# define __iomem
# define __chk_user_ptr(x) (void)0
# define __chk_io_ptr(x) (void)0
# define __builtin_warning(x, y...) (1)
# define __must_hold(x)
# define __acquires(x)
# define __releases(x)
# define __acquire(x) (void)0
# define __release(x) (void)0
# define __cond_lock(x,c) (c)
# define __percpu
# define __rcu
# define __private
# define ACCESS_PRIVATE(p, member) ((p)->member)
#endif 


#define ___PASTE(a,b) a##b
#define __PASTE(a,b) ___PASTE(a,b)

#ifdef __KERNEL__


#ifndef __has_attribute
# define __has_attribute(x) __GCC4_has_attribute_##x
# define __GCC4_has_attribute___copy__                0
#endif

#if __has_attribute(__copy__)
# define __copy(symbol)                 __attribute__((__copy__(symbol)))
#else
# define __copy(symbol)
#endif

#ifdef __GNUC__
#include <linux/compiler-gcc.h>
#endif

#if defined(CC_USING_HOTPATCH) && !defined(__CHECKER__)
#define notrace __attribute__((hotpatch(0,0)))
#else
#define notrace __attribute__((no_instrument_function))
#endif


#ifdef __INTEL_COMPILER
# include <linux/compiler-intel.h>
#endif


#ifdef __clang__
#include <linux/compiler-clang.h>
#endif



struct ftrace_branch_data {
	const char *func;
	const char *file;
	unsigned line;
	union {
		struct {
			unsigned long correct;
			unsigned long incorrect;
		};
		struct {
			unsigned long miss;
			unsigned long hit;
		};
		unsigned long miss_hit[2];
	};
};


#if defined(CONFIG_TRACE_BRANCH_PROFILING) \
    && !defined(DISABLE_BRANCH_PROFILING) && !defined(__CHECKER__)
void ftrace_likely_update(struct ftrace_branch_data *f, int val, int expect);

#define likely_notrace(x)	__builtin_expect(!!(x), 1)
#define unlikely_notrace(x)	__builtin_expect(!!(x), 0)

#define __branch_check__(x, expect) ({					\
			long ______r;					\
			static struct ftrace_branch_data		\
				__attribute__((__aligned__(4)))		\
				__attribute__((section("_ftrace_annotated_branch"))) \
				______f = {				\
				.func = __func__,			\
				.file = __FILE__,			\
				.line = __LINE__,			\
			};						\
			______r = likely_notrace(x);			\
			ftrace_likely_update(&______f, ______r, expect); \
			______r;					\
		})


# ifndef likely
#  define likely(x)	(__builtin_constant_p(x) ? !!(x) : __branch_check__(x, 1))
# endif
# ifndef unlikely
#  define unlikely(x)	(__builtin_constant_p(x) ? !!(x) : __branch_check__(x, 0))
# endif

#ifdef CONFIG_PROFILE_ALL_BRANCHES

#define if(cond, ...) __trace_if( (cond , ## __VA_ARGS__) )
#define __trace_if(cond) \
	if (__builtin_constant_p(!!(cond)) ? !!(cond) :			\
	({								\
		int ______r;						\
		static struct ftrace_branch_data			\
			__attribute__((__aligned__(4)))			\
			__attribute__((section("_ftrace_branch")))	\
			______f = {					\
				.func = __func__,			\
				.file = __FILE__,			\
				.line = __LINE__,			\
			};						\
		______r = !!(cond);					\
		______f.miss_hit[______r]++;					\
		______r;						\
	}))
#endif 

#else
# define likely(x)	__builtin_expect(!!(x), 1)
# define unlikely(x)	__builtin_expect(!!(x), 0)
#endif


#ifndef barrier
# define barrier() __memory_barrier()
#endif

#ifndef barrier_data
# define barrier_data(ptr) barrier()
#endif


#ifndef barrier_before_unreachable
# define barrier_before_unreachable() do { } while (0)
#endif


#ifndef unreachable
# define unreachable() do { } while (1)
#endif


#ifndef KENTRY
# define KENTRY(sym)						\
	extern typeof(sym) sym;					\
	static const unsigned long __kentry_##sym		\
	__used							\
	__attribute__((section("___kentry" "+" #sym ), used))	\
	= (unsigned long)&sym;
#endif

#ifndef RELOC_HIDE
# define RELOC_HIDE(ptr, off)					\
  ({ unsigned long __ptr;					\
     __ptr = (unsigned long) (ptr);				\
    (typeof(ptr)) (__ptr + (off)); })
#endif

#define absolute_pointer(val)	RELOC_HIDE((void *)(val), 0)

#ifndef OPTIMIZER_HIDE_VAR
#define OPTIMIZER_HIDE_VAR(var) barrier()
#endif


#ifndef __UNIQUE_ID
# define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __LINE__)
#endif

#include <uapi/linux/types.h>

#define __READ_ONCE_SIZE						\
({									\
	switch (size) {							\
	case 1: *(__u8 *)res = *(volatile __u8 *)p; break;		\
	case 2: *(__u16 *)res = *(volatile __u16 *)p; break;		\
	case 4: *(__u32 *)res = *(volatile __u32 *)p; break;		\
	case 8: *(__u64 *)res = *(volatile __u64 *)p; break;		\
	default:							\
		barrier();						\
		__builtin_memcpy((void *)res, (const void *)p, size);	\
		barrier();						\
	}								\
})

static __always_inline
void __read_once_size(const volatile void *p, void *res, int size)
{
	__READ_ONCE_SIZE;
}

#ifdef CONFIG_KASAN

# define __no_kasan_or_inline __no_sanitize_address __maybe_unused
#else
# define __no_kasan_or_inline __always_inline
#endif

static __no_kasan_or_inline
void __read_once_size_nocheck(const volatile void *p, void *res, int size)
{
	__READ_ONCE_SIZE;
}

static __always_inline void __write_once_size(volatile void *p, void *res, int size)
{
	switch (size) {
	case 1: *(volatile __u8 *)p = *(__u8 *)res; break;
	case 2: *(volatile __u16 *)p = *(__u16 *)res; break;
	case 4: *(volatile __u32 *)p = *(__u32 *)res; break;
	case 8: *(volatile __u64 *)p = *(__u64 *)res; break;
	default:
		barrier();
		__builtin_memcpy((void *)p, (const void *)res, size);
		barrier();
	}
}


#include <linux/kasan-checks.h>

#define __READ_ONCE(x, check)						\
({									\
	union { typeof(x) __val; char __c[1]; } __u;			\
	if (check)							\
		__read_once_size(&(x), __u.__c, sizeof(x));		\
	else								\
		__read_once_size_nocheck(&(x), __u.__c, sizeof(x));	\
	__u.__val;							\
})
#define READ_ONCE(x) __READ_ONCE(x, 1)


#define READ_ONCE_NOCHECK(x) __READ_ONCE(x, 0)

static __no_kasan_or_inline
unsigned long read_word_at_a_time(const void *addr)
{
	kasan_check_read(addr, 1);
	return *(unsigned long *)addr;
}

#define WRITE_ONCE(x, val) \
({							\
	union { typeof(x) __val; char __c[1]; } __u =	\
		{ .__val = (__force typeof(x)) (val) }; \
	__write_once_size(&(x), __u.__c, sizeof(x));	\
	__u.__val;					\
})

#endif 

#endif 

#ifdef __KERNEL__

#ifndef __deprecated
# define __deprecated		
#endif

#ifdef MODULE
#define __deprecated_for_modules __deprecated
#else
#define __deprecated_for_modules
#endif

#ifndef __must_check
#define __must_check
#endif

#ifndef CONFIG_ENABLE_MUST_CHECK
#undef __must_check
#define __must_check
#endif
#ifndef CONFIG_ENABLE_WARN_DEPRECATED
#undef __deprecated
#undef __deprecated_for_modules
#define __deprecated
#define __deprecated_for_modules
#endif

#ifndef __malloc
#define __malloc
#endif


#ifndef __used
# define __used			
#endif

#ifndef __maybe_unused
# define __maybe_unused		
#endif

#ifndef __always_unused
# define __always_unused	
#endif

#ifndef noinline
#define noinline
#endif


#define noinline_for_stack noinline

#ifndef __always_inline
#define __always_inline inline
#endif

#endif 


#ifndef __attribute_const__
# define __attribute_const__	
#endif

#ifndef __latent_entropy
# define __latent_entropy
#endif



#ifndef __cold
#define __cold
#endif


#ifndef __section
# define __section(S) __attribute__ ((__section__(#S)))
#endif

#ifndef __visible
#define __visible
#endif

#ifndef __norecordmcount
#define __norecordmcount
#endif

#ifndef __nocfi
#define __nocfi
#endif


#ifndef __assume_aligned
#define __assume_aligned(a, ...)
#endif



#ifndef __same_type
# define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#endif


#ifndef __native_word
# define __native_word(t) (sizeof(t) == sizeof(char) || sizeof(t) == sizeof(short) || sizeof(t) == sizeof(int) || sizeof(t) == sizeof(long))
#endif

#ifndef __optimize
# define __optimize(level)
#endif


#ifndef __compiletime_object_size
# define __compiletime_object_size(obj) -1
#endif
#ifndef __compiletime_warning
# define __compiletime_warning(message)
#endif
#ifndef __compiletime_error
# define __compiletime_error(message)

# ifndef __CHECKER__
#  define __compiletime_error_fallback(condition) \
	do { ((void)sizeof(char[1 - 2 * condition])); } while (0)
# endif
#endif
#ifndef __compiletime_error_fallback
# define __compiletime_error_fallback(condition) do { } while (0)
#endif

#define __compiletime_assert(condition, msg, prefix, suffix)		\
	do {								\
		bool __cond = !(condition);				\
		extern void prefix ## suffix(void) __compiletime_error(msg); \
		if (__cond)						\
			prefix ## suffix();				\
		__compiletime_error_fallback(__cond);			\
	} while (0)

#define _compiletime_assert(condition, msg, prefix, suffix) \
	__compiletime_assert(condition, msg, prefix, suffix)


#define compiletime_assert(condition, msg) \
	_compiletime_assert(condition, msg, __compiletime_assert_, __COUNTER__)

#define compiletime_assert_atomic_type(t)				\
	compiletime_assert(__native_word(t),				\
		"Need native word sized stores/loads for atomicity.")


#define __ACCESS_ONCE(x) ({ \
	 __maybe_unused typeof(x) __var = (__force typeof(x)) 0; \
	(volatile typeof(x) *)&(x); })
#define ACCESS_ONCE(x) (*__ACCESS_ONCE(x))


#define lockless_dereference(p) \
({ \
	typeof(p) _________p1 = READ_ONCE(p); \
	typeof(*(p)) *___typecheck_p __maybe_unused; \
	smp_read_barrier_depends();  \
	(_________p1); \
})


#ifdef CONFIG_KPROBES
# define __kprobes	__attribute__((__section__(".kprobes.text")))
# define nokprobe_inline	__always_inline
#else
# define __kprobes
# define nokprobe_inline	inline
#endif


#define prevent_tail_call_optimization()	mb()

#endif 
