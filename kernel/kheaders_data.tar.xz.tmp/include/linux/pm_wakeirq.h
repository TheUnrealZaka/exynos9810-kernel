

#ifndef _LINUX_PM_WAKEIRQ_H
#define _LINUX_PM_WAKEIRQ_H

#ifdef CONFIG_PM

extern int dev_pm_set_wake_irq(struct device *dev, int irq);
extern int dev_pm_set_dedicated_wake_irq(struct device *dev,
					 int irq);
extern void dev_pm_clear_wake_irq(struct device *dev);
extern void dev_pm_enable_wake_irq(struct device *dev);
extern void dev_pm_disable_wake_irq(struct device *dev);

#else	

static inline int dev_pm_set_wake_irq(struct device *dev, int irq)
{
	return 0;
}

static inline int dev_pm_set_dedicated_wake_irq(struct device *dev, int irq)
{
	return 0;
}

static inline void dev_pm_clear_wake_irq(struct device *dev)
{
}

static inline void dev_pm_enable_wake_irq(struct device *dev)
{
}

static inline void dev_pm_disable_wake_irq(struct device *dev)
{
}

#endif	
#endif	
