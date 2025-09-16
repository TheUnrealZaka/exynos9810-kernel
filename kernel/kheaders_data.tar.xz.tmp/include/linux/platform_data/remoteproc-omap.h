

#ifndef _PLAT_REMOTEPROC_H
#define _PLAT_REMOTEPROC_H

struct rproc_ops;
struct platform_device;


struct omap_rproc_pdata {
	const char *name;
	const char *oh_name;
	const char *oh_name_opt;
	const char *firmware;
	const char *mbox_name;
	const struct rproc_ops *ops;
	int (*device_enable)(struct platform_device *pdev);
	int (*device_shutdown)(struct platform_device *pdev);
	void (*set_bootaddr)(u32);
};

#if defined(CONFIG_OMAP_REMOTEPROC) || defined(CONFIG_OMAP_REMOTEPROC_MODULE)

void __init omap_rproc_reserve_cma(void);

#else

static inline void __init omap_rproc_reserve_cma(void)
{
}

#endif

#endif 
