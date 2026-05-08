#ifndef __LINUX_COMPILER_H
#error "Please don't include <linux/compiler-gcc.h> directly, include <linux/compiler.h> instead."
#endif


#define GCC_VERSION (__GNUC__ * 10000		\
		     + __GNUC_MINOR__ * 100	\
		     + __GNUC_PATCHLEVEL__)




#define barrier() __asm__ __volatile__("": : :"memory")

#define barrier_data(ptr) __asm__ __volatile__("": :"r"(ptr) :"memory")


#define RELOC_HIDE(ptr, off)						\
({									\
	unsigned long __ptr;						\
	__asm__ ("" : "=r"(__ptr) : "0"(ptr));				\
	(typeof(ptr)) (__ptr + (off));					\
})


#define OPTIMIZER_HIDE_VAR(var)						\
	__asm__ ("" : "=r" (var) : "0" (var))

#ifdef __CHECKER__
#define __must_be_array(a)	0
#else

#define __must_be_array(a)	BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#endif


#ifdef __GNUC_STDC_INLINE__
# define __gnu_inline	__attribute__((gnu_inline))
#else
# define __gnu_inline
#endif


#if !defined(CONFIG_ARCH_SUPPORTS_OPTIMIZED_INLINING) ||		\
    !defined(CONFIG_OPTIMIZE_INLINING) || (__GNUC__ < 4)
#define inline \
	inline __attribute__((always_inline, unused)) notrace __gnu_inline
#else
#define inline inline		__attribute__((unused)) notrace __gnu_inline
#endif

#define __inline__ inline
#define __inline inline
#define __always_inline	inline __attribute__((always_inline))
#define  noinline	__attribute__((noinline))

#define __deprecated	__attribute__((deprecated))
#define __packed	__attribute__((packed))
#define __weak		__attribute__((weak))
#define __alias(symbol)	__attribute__((alias(#symbol)))

#ifdef CONFIG_RETPOLINE
#define __noretpoline __attribute__((indirect_branch("keep")))
#endif


#define __naked		__attribute__((naked)) noinline __noclone notrace

#define __noreturn	__attribute__((noreturn))


#define __pure			__attribute__((pure))
#define __aligned(x)		__attribute__((aligned(x)))
#define __aligned_largest	__attribute__((aligned))
#define __printf(a, b)		__attribute__((format(printf, a, b)))
#define __scanf(a, b)		__attribute__((format(scanf, a, b)))
#define __attribute_const__	__attribute__((__const__))
#define __maybe_unused		__attribute__((unused))
#define __always_unused		__attribute__((unused))



#if GCC_VERSION < 30200
# error Sorry, your compiler is too old - please upgrade it.
#elif defined(CONFIG_ARM64) && GCC_VERSION < 50100 && !defined(__clang__)

# error Sorry, your version of GCC is too old - please use 5.1 or newer.
#endif

#if GCC_VERSION < 30300
# define __used			__attribute__((__unused__))
#else
# define __used			__attribute__((__used__))
#endif

#ifdef CONFIG_GCOV_KERNEL
# if GCC_VERSION < 30400
#   error "GCOV profiling support for gcc versions below 3.4 not included"
# endif 
#endif 

#if GCC_VERSION >= 30400
#define __must_check		__attribute__((warn_unused_result))
#define __malloc		__attribute__((__malloc__))
#endif

#if GCC_VERSION >= 40000


#ifdef __KERNEL__
# if GCC_VERSION >= 40100 &&  GCC_VERSION <= 40101
#  error Your version of gcc miscompiles the __weak directive
# endif
#endif

#define __used			__attribute__((__used__))
#define __compiler_offsetof(a, b)					\
	__builtin_offsetof(a, b)

#if GCC_VERSION >= 40100
# define __compiletime_object_size(obj) __builtin_object_size(obj, 0)
#endif

#if GCC_VERSION >= 40300

#define __cold			__attribute__((__cold__))

#define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)

#ifndef __CHECKER__
# define __compiletime_warning(message) __attribute__((warning(message)))
# define __compiletime_error(message) __attribute__((error(message)))
#endif 
#endif 

#if GCC_VERSION >= 40400
#define __optimize(level)	__attribute__((__optimize__(level)))
#endif 

#if GCC_VERSION >= 40500

#ifndef __CHECKER__
#ifdef LATENT_ENTROPY_PLUGIN
#define __latent_entropy __attribute__((latent_entropy))
#endif
#endif

#ifdef CONFIG_STACK_VALIDATION
#define annotate_unreachable() ({					\
	asm("1:\t\n"							\
	    ".pushsection .discard.unreachable\t\n"			\
	    ".long 1b\t\n"						\
	    ".popsection\t\n");						\
})
#else
#define annotate_unreachable()
#endif


#define barrier_before_unreachable() asm volatile("")


#define unreachable() \
	do {					\
		annotate_unreachable();		\
		barrier_before_unreachable();	\
		__builtin_unreachable();	\
	} while (0)


#define __noclone	__attribute__((__noclone__, __optimize__("no-tracer")))

#endif 

#if GCC_VERSION >= 40600

#define __visible	__attribute__((externally_visible))
#endif


#if GCC_VERSION >= 40900 && !defined(__CHECKER__)

#define __assume_aligned(a, ...) __attribute__((__assume_aligned__(a, ## __VA_ARGS__)))
#endif


#define asm_volatile_goto(x...)	do { asm goto(x); asm (""); } while (0)


#if defined(CONFIG_ARCH_USE_BUILTIN_BSWAP) && !defined(__CHECKER__)
#if GCC_VERSION >= 40400
#define __HAVE_BUILTIN_BSWAP32__
#define __HAVE_BUILTIN_BSWAP64__
#endif
#if GCC_VERSION >= 40800
#define __HAVE_BUILTIN_BSWAP16__
#endif
#endif 

#if GCC_VERSION >= 70000
#define KASAN_ABI_VERSION 5
#elif GCC_VERSION >= 50000
#define KASAN_ABI_VERSION 4
#elif GCC_VERSION >= 40902
#define KASAN_ABI_VERSION 3
#endif

#if GCC_VERSION >= 40902

#define __no_sanitize_address __attribute__((no_sanitize_address))
#endif

#endif	

#if !defined(__noclone)
#define __noclone	
#endif

#if !defined(__no_sanitize_address)
#define __no_sanitize_address
#endif


#define uninitialized_var(x) x = x

#if GCC_VERSION >= 50100
#define COMPILER_HAS_GENERIC_BUILTIN_OVERFLOW 1
#endif
