

#ifndef __LINUX_PUBLIC_PRIVCMD_H__
#define __LINUX_PUBLIC_PRIVCMD_H__

#include <linux/types.h>
#include <linux/compiler.h>
#include <xen/interface/xen.h>

struct privcmd_hypercall {
	__u64 op;
	__u64 arg[5];
};

struct privcmd_mmap_entry {
	__u64 va;
	
	__u64 mfn;
	__u64 npages;
};

struct privcmd_mmap {
	int num;
	domid_t dom; 
	struct privcmd_mmap_entry __user *entry;
};

struct privcmd_mmapbatch {
	int num;     
	domid_t dom; 
	__u64 addr;  
	xen_pfn_t __user *arr; 
};

#define PRIVCMD_MMAPBATCH_MFN_ERROR     0xf0000000U
#define PRIVCMD_MMAPBATCH_PAGED_ERROR   0x80000000U

struct privcmd_mmapbatch_v2 {
	unsigned int num; 
	domid_t dom;      
	__u64 addr;       
	const xen_pfn_t __user *arr; 
	int __user *err;  
};


#define IOCTL_PRIVCMD_HYPERCALL					\
	_IOC(_IOC_NONE, 'P', 0, sizeof(struct privcmd_hypercall))
#define IOCTL_PRIVCMD_MMAP					\
	_IOC(_IOC_NONE, 'P', 2, sizeof(struct privcmd_mmap))
#define IOCTL_PRIVCMD_MMAPBATCH					\
	_IOC(_IOC_NONE, 'P', 3, sizeof(struct privcmd_mmapbatch))
#define IOCTL_PRIVCMD_MMAPBATCH_V2				\
	_IOC(_IOC_NONE, 'P', 4, sizeof(struct privcmd_mmapbatch_v2))

#endif 
