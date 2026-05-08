



#ifndef __ACGCC_H__
#define __ACGCC_H__


#ifndef va_arg
#ifdef ACPI_USE_BUILTIN_STDARG
typedef __builtin_va_list va_list;
#define va_start(v, l)          __builtin_va_start(v, l)
#define va_end(v)               __builtin_va_end(v)
#define va_arg(v, l)            __builtin_va_arg(v, l)
#define va_copy(d, s)           __builtin_va_copy(d, s)
#else
#include <stdarg.h>
#endif
#endif

#define ACPI_INLINE             __inline__



#define ACPI_GET_FUNCTION_NAME          __func__


#define ACPI_PRINTF_LIKE(c) __attribute__ ((__format__ (__printf__, c, c+1)))


#define ACPI_UNUSED_VAR __attribute__ ((unused))



#define COMPILER_VA_MACRO               1

#endif				
