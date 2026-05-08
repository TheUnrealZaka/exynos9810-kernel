

#ifndef __PLATFORM_DATA_DMTIMER_OMAP_H__
#define __PLATFORM_DATA_DMTIMER_OMAP_H__

struct dmtimer_platform_data {
	
	int (*set_timer_src)(struct platform_device *pdev, int source);
	u32 timer_capability;
	u32 timer_errata;
	int (*get_context_loss_count)(struct device *);
};

#endif 
