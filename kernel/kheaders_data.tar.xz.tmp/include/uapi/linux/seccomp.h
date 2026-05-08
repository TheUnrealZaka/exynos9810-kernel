#ifndef _UAPI_LINUX_SECCOMP_H
#define _UAPI_LINUX_SECCOMP_H

#include <linux/compiler.h>
#include <linux/types.h>



#define SECCOMP_MODE_DISABLED	0 
#define SECCOMP_MODE_STRICT	1 
#define SECCOMP_MODE_FILTER	2 


#define SECCOMP_SET_MODE_STRICT	0
#define SECCOMP_SET_MODE_FILTER	1


#define SECCOMP_FILTER_FLAG_TSYNC	(1UL << 0)

#define SECCOMP_FILTER_FLAG_SPEC_ALLOW	(1UL << 2)


#define SECCOMP_RET_KILL	0x00000000U 
#define SECCOMP_RET_TRAP	0x00030000U 
#define SECCOMP_RET_ERRNO	0x00050000U 
#define SECCOMP_RET_TRACE	0x7ff00000U 
#define SECCOMP_RET_ALLOW	0x7fff0000U 


#define SECCOMP_RET_ACTION	0x7fff0000U
#define SECCOMP_RET_DATA	0x0000ffffU


struct seccomp_data {
	int nr;
	__u32 arch;
	__u64 instruction_pointer;
	__u64 args[6];
};

#endif 
