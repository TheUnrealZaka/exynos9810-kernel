



#ifndef __ACENV_H__
#define __ACENV_H__





#define ACPI_BINARY_SEMAPHORE       0
#define ACPI_OSL_MUTEX              1



#define DEBUGGER_SINGLE_THREADED    0
#define DEBUGGER_MULTI_THREADED     1





#if (defined ACPI_ASL_COMPILER) || \
	(defined ACPI_BIN_APP)      || \
	(defined ACPI_DUMP_APP)     || \
	(defined ACPI_HELP_APP)     || \
	(defined ACPI_NAMES_APP)    || \
	(defined ACPI_SRC_APP)      || \
	(defined ACPI_XTRACT_APP)   || \
	(defined ACPI_EXAMPLE_APP)
#define ACPI_APPLICATION
#define ACPI_SINGLE_THREADED
#define USE_NATIVE_ALLOCATE_ZEROED
#endif



#ifdef ACPI_ASL_COMPILER
#define ACPI_DEBUG_OUTPUT
#define ACPI_CONSTANT_EVAL_ONLY
#define ACPI_LARGE_NAMESPACE_NODE
#define ACPI_DATA_TABLE_DISASSEMBLY
#define ACPI_32BIT_PHYSICAL_ADDRESS
#define ACPI_DISASSEMBLER 1
#endif



#ifdef ACPI_EXEC_APP
#define ACPI_APPLICATION
#define ACPI_FULL_DEBUG
#define ACPI_MUTEX_DEBUG
#define ACPI_DBG_TRACK_ALLOCATIONS
#endif



#ifdef ACPI_HELP_APP
#define ACPI_NO_ERROR_MESSAGES
#endif



#ifdef ACPI_NAMES_APP
#define ACPI_DEBUG_OUTPUT
#endif



#if (defined ACPI_EXEC_APP)     || \
	(defined ACPI_EXAMPLE_APP)  || \
	(defined ACPI_NAMES_APP)
#define ACPI_USE_NATIVE_RSDP_POINTER
#endif



#ifdef ACPI_DUMP_APP
#define ACPI_USE_NATIVE_MEMORY_MAPPING
#endif



#if (defined ACPI_EXAMPLE_APP)  || \
	(defined ACPI_NAMES_APP)
#define ACPI_REDUCED_HARDWARE 1
#endif



#ifdef ACPI_LIBRARY
#define ACPI_USE_LOCAL_CACHE
#define ACPI_DEBUGGER 1
#define ACPI_DISASSEMBLER 1

#ifdef _DEBUG
#define ACPI_DEBUG_OUTPUT
#endif
#endif



#ifdef ACPI_APPLICATION
#define ACPI_USE_LOCAL_CACHE
#endif



#ifdef ACPI_FULL_DEBUG
#define ACPI_DEBUG_OUTPUT
#define ACPI_DEBUGGER 1
#define ACPI_DISASSEMBLER 1
#endif






#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
#include <acpi/platform/acgcc.h>

#elif defined(_MSC_VER)
#include "acmsvc.h"

#elif defined(__INTEL_COMPILER)
#include <acpi/platform/acintel.h>

#endif

#if defined(_LINUX) || defined(__linux__)
#include <acpi/platform/aclinux.h>

#elif defined(_APPLE) || defined(__APPLE__)
#include "acmacosx.h"

#elif defined(__DragonFly__)
#include "acdragonfly.h"

#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#include "acfreebsd.h"

#elif defined(__NetBSD__)
#include "acnetbsd.h"

#elif defined(__sun)
#include "acsolaris.h"

#elif defined(MODESTO)
#include "acmodesto.h"

#elif defined(NETWARE)
#include "acnetware.h"

#elif defined(_CYGWIN)
#include "accygwin.h"

#elif defined(WIN32)
#include "acwin.h"

#elif defined(WIN64)
#include "acwin64.h"

#elif defined(_WRS_LIB_BUILD)
#include "acvxworks.h"

#elif defined(__OS2__)
#include "acos2.h"

#elif defined(__HAIKU__)
#include "achaiku.h"

#elif defined(__QNX__)
#include "acqnx.h"


#elif defined(_AED_EFI) || defined(_GNU_EFI) || defined(_EDK2_EFI)
#include "acefi.h"

#else



#error Unknown target environment
#endif







#ifndef COMPILER_DEPENDENT_INT64
#define COMPILER_DEPENDENT_INT64   long long
#endif

#ifndef COMPILER_DEPENDENT_UINT64
#define COMPILER_DEPENDENT_UINT64  unsigned long long
#endif


#ifndef ACPI_MUTEX_TYPE
#define ACPI_MUTEX_TYPE             ACPI_BINARY_SEMAPHORE
#endif



#ifndef ACPI_ACQUIRE_GLOBAL_LOCK
#define ACPI_ACQUIRE_GLOBAL_LOCK(Glptr, acquired) acquired = 1
#endif

#ifndef ACPI_RELEASE_GLOBAL_LOCK
#define ACPI_RELEASE_GLOBAL_LOCK(Glptr, pending) pending = 0
#endif



#ifndef ACPI_FLUSH_CPU_CACHE
#define ACPI_FLUSH_CPU_CACHE()
#endif



#ifndef ACPI_INLINE
#define ACPI_INLINE
#endif


#ifndef ACPI_SYSTEM_XFACE
#define ACPI_SYSTEM_XFACE
#endif

#ifndef ACPI_EXTERNAL_XFACE
#define ACPI_EXTERNAL_XFACE
#endif

#ifndef ACPI_INTERNAL_XFACE
#define ACPI_INTERNAL_XFACE
#endif

#ifndef ACPI_INTERNAL_VAR_XFACE
#define ACPI_INTERNAL_VAR_XFACE
#endif


#ifndef DEBUGGER_THREADING
#if !defined (ACPI_APPLICATION) || defined (ACPI_EXEC_APP)
#define DEBUGGER_THREADING          DEBUGGER_MULTI_THREADED

#else
#define DEBUGGER_THREADING          DEBUGGER_SINGLE_THREADED
#endif
#endif				







#ifdef ACPI_USE_STANDARD_HEADERS



#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef ACPI_APPLICATION
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#endif

#endif				

#ifdef ACPI_APPLICATION
#define ACPI_FILE              FILE *
#define ACPI_FILE_OUT          stdout
#define ACPI_FILE_ERR          stderr
#else
#define ACPI_FILE              void *
#define ACPI_FILE_OUT          NULL
#define ACPI_FILE_ERR          NULL
#endif				

#ifndef ACPI_INIT_FUNCTION
#define ACPI_INIT_FUNCTION
#endif

#endif				
