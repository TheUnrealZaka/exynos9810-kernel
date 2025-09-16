
#ifndef _LINUX_WATCHDOG_H
#define _LINUX_WATCHDOG_H


#include <linux/bitops.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/notifier.h>
#include <uapi/linux/watchdog.h>

struct watchdog_ops;
struct watchdog_device;
struct watchdog_core_data;
struct watchdog_governor;


struct watchdog_ops {
	struct module *owner;
	
	int (*start)(struct watchdog_device *);
	int (*stop)(struct watchdog_device *);
	
	int (*ping)(struct watchdog_device *);
	unsigned int (*status)(struct watchdog_device *);
	int (*set_timeout)(struct watchdog_device *, unsigned int);
	int (*set_pretimeout)(struct watchdog_device *, unsigned int);
	unsigned int (*get_timeleft)(struct watchdog_device *);
	int (*restart)(struct watchdog_device *, unsigned long, void *);
	long (*ioctl)(struct watchdog_device *, unsigned int, unsigned long);
};


struct watchdog_device {
	int id;
	struct device *parent;
	const struct attribute_group **groups;
	const struct watchdog_info *info;
	const struct watchdog_ops *ops;
	const struct watchdog_governor *gov;
	unsigned int bootstatus;
	unsigned int timeout;
	unsigned int pretimeout;
	unsigned int min_timeout;
	unsigned int max_timeout;
	unsigned int min_hw_heartbeat_ms;
	unsigned int max_hw_heartbeat_ms;
	struct notifier_block reboot_nb;
	struct notifier_block restart_nb;
	void *driver_data;
	struct watchdog_core_data *wd_data;
	unsigned long status;

#define WDOG_ACTIVE		0	
#define WDOG_NO_WAY_OUT		1	
#define WDOG_STOP_ON_REBOOT	2	
#define WDOG_HW_RUNNING		3	
	struct list_head deferred;
};

#define WATCHDOG_NOWAYOUT		IS_BUILTIN(CONFIG_WATCHDOG_NOWAYOUT)
#define WATCHDOG_NOWAYOUT_INIT_STATUS	(WATCHDOG_NOWAYOUT << WDOG_NO_WAY_OUT)


static inline bool watchdog_active(struct watchdog_device *wdd)
{
	return test_bit(WDOG_ACTIVE, &wdd->status);
}


static inline bool watchdog_hw_running(struct watchdog_device *wdd)
{
	return test_bit(WDOG_HW_RUNNING, &wdd->status);
}


static inline void watchdog_set_nowayout(struct watchdog_device *wdd, bool nowayout)
{
	if (nowayout)
		set_bit(WDOG_NO_WAY_OUT, &wdd->status);
}


static inline void watchdog_stop_on_reboot(struct watchdog_device *wdd)
{
	set_bit(WDOG_STOP_ON_REBOOT, &wdd->status);
}


static inline bool watchdog_timeout_invalid(struct watchdog_device *wdd, unsigned int t)
{
	
	return t > UINT_MAX / 1000 || t < wdd->min_timeout ||
		(!wdd->max_hw_heartbeat_ms && wdd->max_timeout &&
		 t > wdd->max_timeout);
}


static inline bool watchdog_pretimeout_invalid(struct watchdog_device *wdd,
					       unsigned int t)
{
	return t && wdd->timeout && t >= wdd->timeout;
}


static inline void watchdog_set_drvdata(struct watchdog_device *wdd, void *data)
{
	wdd->driver_data = data;
}

static inline void *watchdog_get_drvdata(struct watchdog_device *wdd)
{
	return wdd->driver_data;
}


#if IS_ENABLED(CONFIG_WATCHDOG_PRETIMEOUT_GOV)
void watchdog_notify_pretimeout(struct watchdog_device *wdd);
#else
static inline void watchdog_notify_pretimeout(struct watchdog_device *wdd)
{
	pr_alert("watchdog%d: pretimeout event\n", wdd->id);
}
#endif


void watchdog_set_restart_priority(struct watchdog_device *wdd, int priority);
extern int watchdog_init_timeout(struct watchdog_device *wdd,
				  unsigned int timeout_parm, struct device *dev);
extern int watchdog_register_device(struct watchdog_device *);
extern void watchdog_unregister_device(struct watchdog_device *);


int devm_watchdog_register_device(struct device *dev, struct watchdog_device *);

#endif  
