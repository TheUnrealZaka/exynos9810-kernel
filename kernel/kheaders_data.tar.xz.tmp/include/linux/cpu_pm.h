

#ifndef _LINUX_CPU_PM_H
#define _LINUX_CPU_PM_H

#include <linux/kernel.h>
#include <linux/notifier.h>




enum cpu_pm_event {
	
	CPU_PM_ENTER_PREPARE,

	
	CPU_PM_ENTER,

	
	CPU_PM_ENTER_FAILED,

	
	CPU_PM_EXIT,

	
	CPU_PM_EXIT_POST,

	
	CPU_CLUSTER_PM_ENTER,

	
	CPU_CLUSTER_PM_ENTER_FAILED,

	
	CPU_CLUSTER_PM_EXIT,
};

#ifdef CONFIG_CPU_PM
int cpu_pm_register_notifier(struct notifier_block *nb);
int cpu_pm_unregister_notifier(struct notifier_block *nb);
int cpu_pm_enter(void);
int cpu_pm_exit(void);
int cpu_cluster_pm_enter(void);
int cpu_cluster_pm_exit(void);
int cpu_pm_enter_pre(void);
int cpu_pm_exit_post(void);

#else

static inline int cpu_pm_register_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int cpu_pm_unregister_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int cpu_pm_enter(void)
{
	return 0;
}

static inline int cpu_pm_exit(void)
{
	return 0;
}

static inline int cpu_cluster_pm_enter(void)
{
	return 0;
}

static inline int cpu_cluster_pm_exit(void)
{
	return 0;
}

static inline int cpu_pm_enter_pre(void)
{
	return 0;
}

static inline int cpu_pm_exit_post(void)
{
	return 0;
}

#endif
#endif
