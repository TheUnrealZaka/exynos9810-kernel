

#ifndef __EXYNOS_PM_H
#define __EXYNOS_PM_H

#include <linux/kernel.h>
#include <linux/notifier.h>


enum exynos_pm_event {
	
	LPA_ENTER,

	
	LPA_ENTER_FAIL,

	
	LPA_EXIT,

	
	SICD_ENTER,
	SICD_AUD_ENTER,

	
	SICD_EXIT,
	SICD_AUD_EXIT,
};

#define EXYNOS_PM_PREFIX	"EXYNOS-PM:"

int register_usb_is_connect(bool (*func)(void));

#ifdef CONFIG_CPU_IDLE
int exynos_pm_register_notifier(struct notifier_block *nb);
int exynos_pm_unregister_notifier(struct notifier_block *nb);
int exynos_pm_notify(enum exynos_pm_event event);
#else
static inline int exynos_pm_register_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int exynos_pm_unregister_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int exynos_pm_notify(enum exynos_pm_event event)
{
	return 0;
}
#endif

#ifdef CONFIG_EXYNOS_FLEXPMU_DBG
extern int acpm_get_mifdn_count(void);
extern int acpm_get_mif_request(void);
#else
static inline int acpm_get_mif_request(void)
{
	return 0;
}
static inline int acpm_get_mifdn_count(void)
{
	return 0;
}
#endif

#if defined(CONFIG_SEC_DEBUG)
enum ids_info {
	tg,
	lg,
	bg,
	g3dg,
	mifg,
	bids,
	gids,
};

extern int asv_ids_information(enum ids_info id);
#endif

#endif 
