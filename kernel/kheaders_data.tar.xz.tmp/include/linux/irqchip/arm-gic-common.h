
#ifndef __LINUX_IRQCHIP_ARM_GIC_COMMON_H
#define __LINUX_IRQCHIP_ARM_GIC_COMMON_H

#include <linux/types.h>
#include <linux/ioport.h>

enum gic_type {
	GIC_V2,
	GIC_V3,
};

struct gic_kvm_info {
	
	enum gic_type	type;
	
	struct resource vcpu;
	
	unsigned int	maint_irq;
	
	struct resource vctrl;
};

const struct gic_kvm_info *gic_get_kvm_info(void);

#endif 
