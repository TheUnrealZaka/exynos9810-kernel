#ifndef __LINUX_COMPILER_H
#error "Please don't include <linux/compiler-clang.h> directly, include <linux/compiler.h> instead."
#endif



#ifdef uninitialized_var
#undef uninitialized_var
#define uninitialized_var(x) x = *(&(x))
#endif


#define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)

#undef __no_sanitize_address
#define __no_sanitize_address __attribute__((no_sanitize("address")))


#ifdef __noretpoline
#undef __noretpoline
#endif

#ifdef CONFIG_LTO_CLANG
#ifdef CONFIG_FTRACE_MCOUNT_RECORD
#define __norecordmcount \
	__attribute__((__section__(".text..ftrace")))
#endif

#define __nocfi		__attribute__((no_sanitize("cfi")))
#endif


#define KASAN_ABI_VERSION 5


#if __has_feature(address_sanitizer)
#define __SANITIZE_ADDRESS__
#endif


#undef COMPILER_HAS_GENERIC_BUILTIN_OVERFLOW
#if __has_builtin(__builtin_mul_overflow) && \
    __has_builtin(__builtin_add_overflow) && \
    __has_builtin(__builtin_sub_overflow)
#define COMPILER_HAS_GENERIC_BUILTIN_OVERFLOW 1
#endif
