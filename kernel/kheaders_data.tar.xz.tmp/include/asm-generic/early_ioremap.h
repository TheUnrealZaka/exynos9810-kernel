#ifndef _ASM_EARLY_IOREMAP_H_
#define _ASM_EARLY_IOREMAP_H_

#include <linux/types.h>


extern void __iomem *early_ioremap(resource_size_t phys_addr,
				   unsigned long size);
extern void *early_memremap(resource_size_t phys_addr,
			    unsigned long size);
extern void *early_memremap_ro(resource_size_t phys_addr,
			       unsigned long size);
extern void early_iounmap(void __iomem *addr, unsigned long size);
extern void early_memunmap(void *addr, unsigned long size);


extern void early_ioremap_shutdown(void);

#if defined(CONFIG_GENERIC_EARLY_IOREMAP) && defined(CONFIG_MMU)

extern void early_ioremap_init(void);


extern void early_ioremap_setup(void);


extern void early_ioremap_reset(void);


extern void copy_from_early_mem(void *dest, phys_addr_t src,
				unsigned long size);

#else
static inline void early_ioremap_init(void) { }
static inline void early_ioremap_setup(void) { }
static inline void early_ioremap_reset(void) { }
#endif

#endif 
