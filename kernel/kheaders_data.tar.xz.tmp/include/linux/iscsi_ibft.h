

#ifndef ISCSI_IBFT_H
#define ISCSI_IBFT_H

#include <linux/acpi.h>


extern struct acpi_table_ibft *ibft_addr;


#ifdef CONFIG_ISCSI_IBFT_FIND
unsigned long find_ibft_region(unsigned long *sizep);
#else
static inline unsigned long find_ibft_region(unsigned long *sizep)
{
	*sizep = 0;
	return 0;
}
#endif

#endif 
