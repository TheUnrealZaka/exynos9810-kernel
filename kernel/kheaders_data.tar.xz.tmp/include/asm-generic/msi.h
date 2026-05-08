#ifndef __ASM_GENERIC_MSI_H
#define __ASM_GENERIC_MSI_H

#include <linux/types.h>

#ifndef NUM_MSI_ALLOC_SCRATCHPAD_REGS
# define NUM_MSI_ALLOC_SCRATCHPAD_REGS	2
#endif

struct msi_desc;


typedef struct msi_alloc_info {
	struct msi_desc			*desc;
	irq_hw_number_t			hwirq;
	union {
		unsigned long		ul;
		void			*ptr;
	} scratchpad[NUM_MSI_ALLOC_SCRATCHPAD_REGS];
} msi_alloc_info_t;

#define GENERIC_MSI_DOMAIN_OPS		1

#endif
