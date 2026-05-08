

#ifndef _LINUX_PM_WAKEUP_H
#define _LINUX_PM_WAKEUP_H

#ifndef _DEVICE_H_
# error "please don't include this file directly"
#endif

#include <linux/types.h>

struct wake_irq;


struct wakeup_source {
	const char 		*name;
	struct list_head	entry;
	spinlock_t		lock;
	struct wake_irq		*wakeirq;
	struct timer_list	timer;
	unsigned long		timer_expires;
	ktime_t total_time;
	ktime_t max_time;
	ktime_t last_time;
	ktime_t start_prevent_time;
	ktime_t prevent_sleep_time;
#ifdef CONFIG_SEC_PM_DEBUG
	ktime_t start_screen_off;
	ktime_t time_with_screen_off;
#endif
	unsigned long		event_count;
	unsigned long		active_count;
	unsigned long		relax_count;
	unsigned long		expire_count;
	unsigned long		wakeup_count;
	bool			active:1;
	bool			autosleep_enabled:1;
#ifdef CONFIG_SEC_PM_DEBUG
	bool			is_screen_off:1;
#endif
};

#ifdef CONFIG_PM_SLEEP



static inline bool device_can_wakeup(struct device *dev)
{
	return dev->power.can_wakeup;
}

static inline bool device_may_wakeup(struct device *dev)
{
	return dev->power.can_wakeup && !!dev->power.wakeup;
}


extern void wakeup_source_prepare(struct wakeup_source *ws, const char *name);
extern struct wakeup_source *wakeup_source_create(const char *name);
extern void wakeup_source_drop(struct wakeup_source *ws);
extern void wakeup_source_destroy(struct wakeup_source *ws);
extern void wakeup_source_add(struct wakeup_source *ws);
extern void wakeup_source_remove(struct wakeup_source *ws);
extern struct wakeup_source *wakeup_source_register(const char *name);
extern void wakeup_source_unregister(struct wakeup_source *ws);
extern int device_wakeup_enable(struct device *dev);
extern int device_wakeup_disable(struct device *dev);
extern void device_set_wakeup_capable(struct device *dev, bool capable);
extern int device_init_wakeup(struct device *dev, bool val);
extern int device_set_wakeup_enable(struct device *dev, bool enable);
extern void __pm_stay_awake(struct wakeup_source *ws);
extern void pm_stay_awake(struct device *dev);
extern void __pm_relax(struct wakeup_source *ws);
extern void pm_relax(struct device *dev);
extern void __pm_wakeup_event(struct wakeup_source *ws, unsigned int msec);
extern void pm_wakeup_event(struct device *dev, unsigned int msec);

#else 

static inline void device_set_wakeup_capable(struct device *dev, bool capable)
{
	dev->power.can_wakeup = capable;
}

static inline bool device_can_wakeup(struct device *dev)
{
	return dev->power.can_wakeup;
}

static inline void wakeup_source_prepare(struct wakeup_source *ws,
					 const char *name) {}

static inline struct wakeup_source *wakeup_source_create(const char *name)
{
	return NULL;
}

static inline void wakeup_source_drop(struct wakeup_source *ws) {}

static inline void wakeup_source_destroy(struct wakeup_source *ws) {}

static inline void wakeup_source_add(struct wakeup_source *ws) {}

static inline void wakeup_source_remove(struct wakeup_source *ws) {}

static inline struct wakeup_source *wakeup_source_register(const char *name)
{
	return NULL;
}

static inline void wakeup_source_unregister(struct wakeup_source *ws) {}

static inline int device_wakeup_enable(struct device *dev)
{
	dev->power.should_wakeup = true;
	return 0;
}

static inline int device_wakeup_disable(struct device *dev)
{
	dev->power.should_wakeup = false;
	return 0;
}

static inline int device_set_wakeup_enable(struct device *dev, bool enable)
{
	dev->power.should_wakeup = enable;
	return 0;
}

static inline int device_init_wakeup(struct device *dev, bool val)
{
	device_set_wakeup_capable(dev, val);
	device_set_wakeup_enable(dev, val);
	return 0;
}

static inline bool device_may_wakeup(struct device *dev)
{
	return dev->power.can_wakeup && dev->power.should_wakeup;
}

static inline void __pm_stay_awake(struct wakeup_source *ws) {}

static inline void pm_stay_awake(struct device *dev) {}

static inline void __pm_relax(struct wakeup_source *ws) {}

static inline void pm_relax(struct device *dev) {}

static inline void __pm_wakeup_event(struct wakeup_source *ws, unsigned int msec) {}

static inline void pm_wakeup_event(struct device *dev, unsigned int msec) {}

#endif 

static inline void wakeup_source_init(struct wakeup_source *ws,
				      const char *name)
{
	wakeup_source_prepare(ws, name);
	wakeup_source_add(ws);
}

static inline void wakeup_source_trash(struct wakeup_source *ws)
{
	wakeup_source_remove(ws);
	wakeup_source_drop(ws);
}

#endif 
