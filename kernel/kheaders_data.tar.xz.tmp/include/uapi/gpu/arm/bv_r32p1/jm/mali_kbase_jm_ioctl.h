/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _UAPI_KBASE_JM_IOCTL_H_
#define _UAPI_KBASE_JM_IOCTL_H_

#include <asm-generic/ioctl.h>
#include <linux/types.h>


#define BASE_UK_VERSION_MAJOR 11
#define BASE_UK_VERSION_MINOR 31


struct kbase_ioctl_version_check {
	__u16 major;
	__u16 minor;
};

#define KBASE_IOCTL_VERSION_CHECK \
	_IOWR(KBASE_IOCTL_TYPE, 0, struct kbase_ioctl_version_check)



struct kbase_ioctl_job_submit {
	__u64 addr;
	__u32 nr_atoms;
	__u32 stride;
};

#define KBASE_IOCTL_JOB_SUBMIT \
	_IOW(KBASE_IOCTL_TYPE, 2, struct kbase_ioctl_job_submit)

#define KBASE_IOCTL_POST_TERM \
	_IO(KBASE_IOCTL_TYPE, 4)


struct kbase_ioctl_soft_event_update {
	__u64 event;
	__u32 new_status;
	__u32 flags;
};

#define KBASE_IOCTL_SOFT_EVENT_UPDATE \
	_IOW(KBASE_IOCTL_TYPE, 28, struct kbase_ioctl_soft_event_update)


struct kbase_kinstr_jm_fd_out {
	__u16 size;
	__u8 version;
	__u8 padding[5];
};


struct kbase_kinstr_jm_fd_in {
	__u16 count;
	__u8 padding[6];
};

union kbase_kinstr_jm_fd {
	struct kbase_kinstr_jm_fd_in in;
	struct kbase_kinstr_jm_fd_out out;
};

#define KBASE_IOCTL_KINSTR_JM_FD \
	_IOWR(KBASE_IOCTL_TYPE, 51, union kbase_kinstr_jm_fd)


#define KBASE_IOCTL_VERSION_CHECK_RESERVED \
	_IOWR(KBASE_IOCTL_TYPE, 52, struct kbase_ioctl_version_check)

#endif 
