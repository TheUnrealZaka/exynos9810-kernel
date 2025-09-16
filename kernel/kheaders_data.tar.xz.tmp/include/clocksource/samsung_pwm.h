
#ifndef __CLOCKSOURCE_SAMSUNG_PWM_H
#define __CLOCKSOURCE_SAMSUNG_PWM_H

#include <linux/spinlock.h>

#define SAMSUNG_PWM_NUM		5


#ifdef CONFIG_CLKSRC_SAMSUNG_PWM
extern spinlock_t samsung_pwm_lock;
#endif

struct samsung_pwm_variant {
	u8 bits;
	u8 div_base;
	u8 tclk_mask;
	u8 output_mask;
	bool has_tint_cstat;
};

void samsung_pwm_clocksource_init(void __iomem *base,
		unsigned int *irqs, struct samsung_pwm_variant *variant);

#endif 
