

#ifndef __WIL6210_UAPI_H__
#define __WIL6210_UAPI_H__

#if !defined(__KERNEL__)
#define __user
#endif

#include <linux/sockios.h>




#define WIL_IOCTL_MEMIO (SIOCDEVPRIVATE + 2)


#define WIL_IOCTL_MEMIO_BLOCK (SIOCDEVPRIVATE + 3)


enum wil_memio_op {
	wil_mmio_read = 0,
	wil_mmio_write = 1,
	wil_mmio_op_mask = 0xff,
	wil_mmio_addr_linker = 0 << 8,
	wil_mmio_addr_ahb = 1 << 8,
	wil_mmio_addr_bar = 2 << 8,
	wil_mmio_addr_mask = 0xff00,
};

struct wil_memio {
	uint32_t op; 
	uint32_t addr; 
	uint32_t val;
};

struct wil_memio_block {
	uint32_t op; 
	uint32_t addr; 
	uint32_t size; 
	void __user *block; 
};

#endif 
