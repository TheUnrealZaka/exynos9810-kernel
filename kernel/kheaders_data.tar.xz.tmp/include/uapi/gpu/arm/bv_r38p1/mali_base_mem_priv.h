/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_BASE_MEM_PRIV_H_
#define _UAPI_BASE_MEM_PRIV_H_

#include <linux/types.h>
#include "mali_base_common_kernel.h"

#define BASE_SYNCSET_OP_MSYNC	(1U << 0)
#define BASE_SYNCSET_OP_CSYNC	(1U << 1)


struct basep_syncset {
	struct base_mem_handle mem_handle;
	__u64 user_addr;
	__u64 size;
	__u8 type;
	__u8 padding[7];
};

#endif 
