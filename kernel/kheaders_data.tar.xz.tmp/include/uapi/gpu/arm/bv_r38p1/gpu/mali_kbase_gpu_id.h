/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_KBASE_GPU_ID_H_
#define _UAPI_KBASE_GPU_ID_H_

#include <linux/types.h>


#define KBASE_GPU_ID_VERSION_STATUS_SHIFT 0
#define KBASE_GPU_ID_VERSION_MINOR_SHIFT 4
#define KBASE_GPU_ID_VERSION_MAJOR_SHIFT 12
#define KBASE_GPU_ID_VERSION_PRODUCT_ID_SHIFT 16

#define GPU_ID_VERSION_STATUS (0xFu << KBASE_GPU_ID_VERSION_STATUS_SHIFT)
#define GPU_ID_VERSION_MINOR (0xFFu << KBASE_GPU_ID_VERSION_MINOR_SHIFT)
#define GPU_ID_VERSION_MAJOR (0xFu << KBASE_GPU_ID_VERSION_MAJOR_SHIFT)
#define GPU_ID_VERSION_PRODUCT_ID (0xFFFFu << KBASE_GPU_ID_VERSION_PRODUCT_ID_SHIFT)

#define GPU_ID2_VERSION_STATUS_SHIFT      0
#define GPU_ID2_VERSION_MINOR_SHIFT       4
#define GPU_ID2_VERSION_MAJOR_SHIFT       12
#define GPU_ID2_PRODUCT_MAJOR_SHIFT       16
#define GPU_ID2_ARCH_REV_SHIFT            20
#define GPU_ID2_ARCH_MINOR_SHIFT          24
#define GPU_ID2_ARCH_MAJOR_SHIFT          28
#define GPU_ID2_VERSION_STATUS            (0xFu << GPU_ID2_VERSION_STATUS_SHIFT)
#define GPU_ID2_VERSION_MINOR             (0xFFu << GPU_ID2_VERSION_MINOR_SHIFT)
#define GPU_ID2_VERSION_MAJOR             (0xFu << GPU_ID2_VERSION_MAJOR_SHIFT)
#define GPU_ID2_PRODUCT_MAJOR             (0xFu << GPU_ID2_PRODUCT_MAJOR_SHIFT)
#define GPU_ID2_ARCH_REV                  (0xFu << GPU_ID2_ARCH_REV_SHIFT)
#define GPU_ID2_ARCH_MINOR                (0xFu << GPU_ID2_ARCH_MINOR_SHIFT)
#define GPU_ID2_ARCH_MAJOR                (0xFu << GPU_ID2_ARCH_MAJOR_SHIFT)
#define GPU_ID2_PRODUCT_MODEL  (GPU_ID2_ARCH_MAJOR | GPU_ID2_PRODUCT_MAJOR)
#define GPU_ID2_VERSION        (GPU_ID2_VERSION_MAJOR | \
								GPU_ID2_VERSION_MINOR | \
								GPU_ID2_VERSION_STATUS)


#define ARCH_MAJOR_REV_REG(gpu_id)                                             \
	((((__u32)gpu_id) & GPU_ID2_ARCH_MAJOR) |                              \
	 (((__u32)gpu_id) & GPU_ID2_ARCH_REV))


#define GPU_ID2_ARCH_MAJOR_REV_MAKE(arch_major, arch_rev)                      \
	((((__u32)arch_major) << GPU_ID2_ARCH_MAJOR_SHIFT) |                   \
	 (((__u32)arch_rev) << GPU_ID2_ARCH_REV_SHIFT))


#define GPU_ID2_PRODUCT_MAKE(arch_major, arch_minor, arch_rev, product_major) \
		((((__u32)arch_major) << GPU_ID2_ARCH_MAJOR_SHIFT)  | \
		 (((__u32)arch_minor) << GPU_ID2_ARCH_MINOR_SHIFT)  | \
		 (((__u32)arch_rev) << GPU_ID2_ARCH_REV_SHIFT)      | \
		 (((__u32)product_major) << GPU_ID2_PRODUCT_MAJOR_SHIFT))


#define GPU_ID2_VERSION_MAKE(version_major, version_minor, version_status) \
		((((__u32)version_major) << GPU_ID2_VERSION_MAJOR_SHIFT)  | \
		 (((__u32)version_minor) << GPU_ID2_VERSION_MINOR_SHIFT)  | \
		 (((__u32)version_status) << GPU_ID2_VERSION_STATUS_SHIFT))


#define GPU_ID2_MAKE(arch_major, arch_minor, arch_rev, product_major, \
	version_major, version_minor, version_status) \
		(GPU_ID2_PRODUCT_MAKE(arch_major, arch_minor, arch_rev, \
			product_major) | \
		 GPU_ID2_VERSION_MAKE(version_major, version_minor,     \
			version_status))


#define GPU_ID2_MODEL_MAKE(arch_major, product_major) \
		((((__u32)arch_major) << GPU_ID2_ARCH_MAJOR_SHIFT)  | \
		(((__u32)product_major) << GPU_ID2_PRODUCT_MAJOR_SHIFT))


#define GPU_ID2_MODEL_MATCH_VALUE(product_id) \
		((((__u32)product_id) << GPU_ID2_PRODUCT_MAJOR_SHIFT) & \
		    GPU_ID2_PRODUCT_MODEL)

#define GPU_ID2_PRODUCT_TMIX              GPU_ID2_MODEL_MAKE(6, 0)
#define GPU_ID2_PRODUCT_THEX              GPU_ID2_MODEL_MAKE(6, 1)
#define GPU_ID2_PRODUCT_TSIX              GPU_ID2_MODEL_MAKE(7, 0)
#define GPU_ID2_PRODUCT_TDVX              GPU_ID2_MODEL_MAKE(7, 3)
#define GPU_ID2_PRODUCT_TNOX              GPU_ID2_MODEL_MAKE(7, 1)
#define GPU_ID2_PRODUCT_TGOX              GPU_ID2_MODEL_MAKE(7, 2)
#define GPU_ID2_PRODUCT_TTRX              GPU_ID2_MODEL_MAKE(9, 0)
#define GPU_ID2_PRODUCT_TNAX              GPU_ID2_MODEL_MAKE(9, 1)
#define GPU_ID2_PRODUCT_TBEX              GPU_ID2_MODEL_MAKE(9, 2)
#define GPU_ID2_PRODUCT_LBEX              GPU_ID2_MODEL_MAKE(9, 4)
#define GPU_ID2_PRODUCT_TBAX              GPU_ID2_MODEL_MAKE(9, 5)
#define GPU_ID2_PRODUCT_TDUX              GPU_ID2_MODEL_MAKE(10, 1)
#define GPU_ID2_PRODUCT_TODX              GPU_ID2_MODEL_MAKE(10, 2)
#define GPU_ID2_PRODUCT_TGRX              GPU_ID2_MODEL_MAKE(10, 3)
#define GPU_ID2_PRODUCT_TVAX              GPU_ID2_MODEL_MAKE(10, 4)
#define GPU_ID2_PRODUCT_LODX              GPU_ID2_MODEL_MAKE(10, 7)
#define GPU_ID2_PRODUCT_TTUX              GPU_ID2_MODEL_MAKE(11, 2)
#define GPU_ID2_PRODUCT_LTUX              GPU_ID2_MODEL_MAKE(11, 3)


#define GPU_ID_MAKE(id, major, minor, status)                                                      \
	((((__u32)id) << KBASE_GPU_ID_VERSION_PRODUCT_ID_SHIFT) |                                  \
	 (((__u32)major) << KBASE_GPU_ID_VERSION_MAJOR_SHIFT) |                                    \
	 (((__u32)minor) << KBASE_GPU_ID_VERSION_MINOR_SHIFT) |                                    \
	 (((__u32)status) << KBASE_GPU_ID_VERSION_STATUS_SHIFT))

#endif 
