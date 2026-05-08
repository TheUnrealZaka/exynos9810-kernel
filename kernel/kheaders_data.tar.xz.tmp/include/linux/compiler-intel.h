#ifndef __LINUX_COMPILER_H
#error "Please don't include <linux/compiler-intel.h> directly, include <linux/compiler.h> instead."
#endif

#ifdef __ECC



#include <asm/intrinsics.h>


#undef barrier
#undef barrier_data
#undef RELOC_HIDE
#undef OPTIMIZER_HIDE_VAR

#define barrier() __memory_barrier()
#define barrier_data(ptr) barrier()

#define RELOC_HIDE(ptr, off)					\
  ({ unsigned long __ptr;					\
     __ptr = (unsigned long) (ptr);				\
    (typeof(ptr)) (__ptr + (off)); })


#define OPTIMIZER_HIDE_VAR(var) barrier()


#define __must_be_array(a) 0

#endif

#ifndef __HAVE_BUILTIN_BSWAP16__

#define __HAVE_BUILTIN_BSWAP16__
#define __builtin_bswap16 _bswap16
#endif


#undef COMPILER_HAS_GENERIC_BUILTIN_OVERFLOW
