/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_KBASE_GPU_REGMAP_H_
#define _UAPI_KBASE_GPU_REGMAP_H_

#if !MALI_USE_CSF
#include "backend/mali_kbase_gpu_regmap_jm.h"
#endif 


#define MEMORY_MANAGEMENT_BASE  0x2000
#define MMU_REG(r)              (MEMORY_MANAGEMENT_BASE + (r))
#define MMU_IRQ_RAWSTAT         0x000   

#endif 
