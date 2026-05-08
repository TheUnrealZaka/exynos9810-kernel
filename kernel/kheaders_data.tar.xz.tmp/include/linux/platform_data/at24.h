

#ifndef _LINUX_AT24_H
#define _LINUX_AT24_H

#include <linux/types.h>
#include <linux/nvmem-consumer.h>
#include <linux/bitops.h>



struct at24_platform_data {
	u32		byte_len;		
	u16		page_size;		
	u8		flags;
#define AT24_FLAG_ADDR16	BIT(7)	
#define AT24_FLAG_READONLY	BIT(6)	
#define AT24_FLAG_IRUGO		BIT(5)	
#define AT24_FLAG_TAKE8ADDR	BIT(4)	
#define AT24_FLAG_SERIAL	BIT(3)	
#define AT24_FLAG_MAC		BIT(2)	

	void		(*setup)(struct nvmem_device *nvmem, void *context);
	void		*context;
};

#endif 
