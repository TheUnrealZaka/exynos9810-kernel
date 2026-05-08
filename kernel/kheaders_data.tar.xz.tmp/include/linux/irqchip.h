

#ifndef _LINUX_IRQCHIP_H
#define _LINUX_IRQCHIP_H

#include <linux/acpi.h>
#include <linux/of.h>


#define IRQCHIP_DECLARE(name, compat, fn) OF_DECLARE_2(irqchip, name, compat, fn)


#define IRQCHIP_ACPI_DECLARE(name, subtable, validate, data, fn)	\
	ACPI_DECLARE_PROBE_ENTRY(irqchip, name, ACPI_SIG_MADT, 		\
				 subtable, validate, data, fn)

#ifdef CONFIG_IRQCHIP
void irqchip_init(void);
#else
static inline void irqchip_init(void) {}
#endif

#endif
