
#ifndef __ASM_PTDUMP_H
#define __ASM_PTDUMP_H

#ifdef CONFIG_ARM64_PTDUMP

#include <linux/mm_types.h>

struct addr_marker {
	unsigned long start_address;
	char *name;
};

struct ptdump_info {
	struct mm_struct		*mm;
	const struct addr_marker	*markers;
	unsigned long			base_addr;
	unsigned long			max_addr;
};

int ptdump_register(struct ptdump_info *info, const char *name);

#else
static inline int ptdump_register(struct ptdump_info *info, const char *name)
{
	return 0;
}
#endif 

#endif 
