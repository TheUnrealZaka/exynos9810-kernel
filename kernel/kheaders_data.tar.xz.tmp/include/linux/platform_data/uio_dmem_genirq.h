

#ifndef _UIO_DMEM_GENIRQ_H
#define _UIO_DMEM_GENIRQ_H

#include <linux/uio_driver.h>

struct uio_dmem_genirq_pdata {
	struct uio_info	uioinfo;
	unsigned int *dynamic_region_sizes;
	unsigned int num_dynamic_regions;
};
#endif 
