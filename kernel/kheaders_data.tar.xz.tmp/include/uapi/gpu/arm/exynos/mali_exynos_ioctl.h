/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_MALI_EXYNOS_IOCTL_H_
#define _UAPI_MALI_EXYNOS_IOCTL_H_

#ifdef __cpluscplus
extern "C" {
#endif

#include <asm-generic/ioctl.h>
#include <linux/types.h>

#define KBASE_IOCTL_TYPE 0x80

#define KBASE_IOCTL_EXTRA_TYPE (KBASE_IOCTL_TYPE + 2)


struct mali_exynos_ioctl_amigo_flags {
	__u32 flags;
};

#define MALI_EXYNOS_IOCTL_AMIGO_FLAGS \
	_IOR(KBASE_IOCTL_EXTRA_TYPE, 0, struct mali_exynos_ioctl_amigo_flags)


struct mali_exynos_ioctl_gts_info {
	__u32 util_avg;
	__u32 hcm_mode;
	__u64 out_data[4];
	__u64 input;
	__u64 input2;
	__u64 freq;
	__u64 flimit;
};

#define MALI_EXYNOS_IOCTL_READ_GTS_INFO \
	_IOR(KBASE_IOCTL_EXTRA_TYPE, 1, struct mali_exynos_ioctl_gts_info)

struct mali_exynos_ioctl_hcm_pmqos {
	__u32 mode;
};

#define MALI_EXYNOS_IOCTL_HCM_PMQOS \
	_IOW(KBASE_IOCTL_EXTRA_TYPE, 2, struct mali_exynos_ioctl_hcm_pmqos)


struct mali_exynos_ioctl_mem_usage_add {
	__u64 gl_mem_usage;
};

#define MALI_EXYNOS_IOCTL_MEM_USAGE_ADD \
	_IOW(KBASE_IOCTL_EXTRA_TYPE, 3, struct mali_exynos_ioctl_mem_usage_add)


typedef enum cmar_boost_flag {
	CMAR_BOOST_DEFAULT = 1u << 0,
	CMAR_BOOST_USER_RT = 1u << 1,
	CMAR_BOOST_SET_RT = 1u << 2,
	CMAR_BOOST_SET_DEFAULT = 1u << 3,
} cmar_boost_flag;


struct mali_exynos_ioctl_cmar_boost {
	__u32 flags;
};

#define MALI_EXYNOS_IOCTL_CMAR_BOOST \
	_IOW(KBASE_IOCTL_EXTRA_TYPE, 4, struct mali_exynos_ioctl_cmar_boost)


struct kbase_ioctl_slsi_egp {
	__u64 start_timestamp;
	__u64 end_timestamp;
};

#define KBASE_IOCTL_SLSI_EGP \
       _IOW(KBASE_IOCTL_EXTRA_TYPE, 5, struct kbase_ioctl_slsi_egp)


struct mali_exynos_ioctl_interactive_boost {
	__u64 duration;
};

#define MALI_EXYNOS_IOCTL_INTERACTIVE_BOOST \
       _IOW(KBASE_IOCTL_EXTRA_TYPE, 6, struct mali_exynos_ioctl_interactive_boost)




struct kbase_ioctl_slsi_singlebuffer_boost_flags {
	__u32 flags;
};

#define KBASE_IOCTL_SLSI_SINGLEBUFFER_BOOST_FLAGS \
	_IOW(KBASE_IOCTL_TYPE, 46, struct kbase_ioctl_slsi_singlebuffer_boost_flags)


struct mali_exynos_ioctl_ehmp_flags {
	__u32 flags;
};

#define EHMP_SET (512 + 41)
#define EHMP_UNSET (512 + 42)

#define MALI_EXYNOS_IOCTL_EHMP \
	_IOW(KBASE_IOCTL_TYPE, 42, struct mali_exynos_ioctl_ehmp_flags)

#ifdef __cpluscplus
}
#endif

#endif 
