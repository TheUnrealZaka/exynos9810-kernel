

#ifndef __ASM_DMI_H
#define __ASM_DMI_H

#include <linux/io.h>
#include <linux/slab.h>


#define dmi_early_remap(x, l)		ioremap_cache(x, l)
#define dmi_early_unmap(x, l)		iounmap(x)
#define dmi_remap(x, l)			ioremap_cache(x, l)
#define dmi_unmap(x)			iounmap(x)
#define dmi_alloc(l)			kzalloc(l, GFP_KERNEL)

#endif
