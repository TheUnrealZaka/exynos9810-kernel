
#ifndef __CLKSOURCE_ARM_ARCH_TIMER_H
#define __CLKSOURCE_ARM_ARCH_TIMER_H

#include <linux/timecounter.h>
#include <linux/types.h>

#define ARCH_TIMER_CTRL_ENABLE		(1 << 0)
#define ARCH_TIMER_CTRL_IT_MASK		(1 << 1)
#define ARCH_TIMER_CTRL_IT_STAT		(1 << 2)

#define CNTHCTL_EL1PCTEN		(1 << 0)
#define CNTHCTL_EL1PCEN			(1 << 1)
#define CNTHCTL_EVNTEN			(1 << 2)
#define CNTHCTL_EVNTDIR			(1 << 3)
#define CNTHCTL_EVNTI			(0xF << 4)

enum arch_timer_reg {
	ARCH_TIMER_REG_CTRL,
	ARCH_TIMER_REG_TVAL,
};

#define ARCH_TIMER_PHYS_ACCESS		0
#define ARCH_TIMER_VIRT_ACCESS		1
#define ARCH_TIMER_MEM_PHYS_ACCESS	2
#define ARCH_TIMER_MEM_VIRT_ACCESS	3

#define ARCH_TIMER_USR_PCT_ACCESS_EN	(1 << 0) 
#define ARCH_TIMER_USR_VCT_ACCESS_EN	(1 << 1) 
#define ARCH_TIMER_VIRT_EVT_EN		(1 << 2)
#define ARCH_TIMER_EVT_TRIGGER_SHIFT	(4)
#define ARCH_TIMER_EVT_TRIGGER_MASK	(0xF << ARCH_TIMER_EVT_TRIGGER_SHIFT)
#define ARCH_TIMER_USR_VT_ACCESS_EN	(1 << 8) 
#define ARCH_TIMER_USR_PT_ACCESS_EN	(1 << 9) 

#define ARCH_TIMER_EVT_STREAM_FREQ	10000	

struct arch_timer_kvm_info {
	struct timecounter timecounter;
	int virtual_irq;
};

#ifdef CONFIG_ARM_ARCH_TIMER

extern u32 arch_timer_get_rate(void);
extern u64 (*arch_timer_read_counter)(void);
extern struct arch_timer_kvm_info *arch_timer_get_kvm_info(void);

#else

static inline u32 arch_timer_get_rate(void)
{
	return 0;
}

static inline u64 arch_timer_read_counter(void)
{
	return 0;
}

#endif

#endif
