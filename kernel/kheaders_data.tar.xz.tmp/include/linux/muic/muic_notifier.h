

#ifndef __MUIC_NOTIFIER_H__
#define __MUIC_NOTIFIER_H__

#include <linux/ccic/ccic_notifier.h>


typedef enum {
	MUIC_NOTIFY_CMD_DETACH	= 0,
	MUIC_NOTIFY_CMD_ATTACH,
	MUIC_NOTIFY_CMD_LOGICALLY_DETACH,
	MUIC_NOTIFY_CMD_LOGICALLY_ATTACH,
} muic_notifier_cmd_t;


typedef enum {
	MUIC_NOTIFY_DEV_DOCK = 0,
	MUIC_NOTIFY_DEV_MHL,
	MUIC_NOTIFY_DEV_USB,
	MUIC_NOTIFY_DEV_TSP,
	MUIC_NOTIFY_DEV_CHARGER,
	MUIC_NOTIFY_DEV_CPUIDLE,
	MUIC_NOTIFY_DEV_CPUFREQ,
#ifdef CONFIG_USB_TYPEC_MANAGER_NOTIFIER
	MUIC_NOTIFY_DEV_MANAGER,
#endif
	MUIC_NOTIFY_DEV_CABLE_DATA,
} muic_notifier_device_t;

struct muic_notifier_struct {
	muic_attached_dev_t attached_dev;
	muic_notifier_cmd_t cmd;
	CC_NOTI_ATTACH_TYPEDEF cxt;
	struct blocking_notifier_head notifier_call_chain;
};

#define MUIC_NOTIFIER_BLOCK(name)	\
	struct notifier_block (name)


extern void muic_notifier_attach_attached_dev(muic_attached_dev_t new_dev);
extern void muic_notifier_detach_attached_dev(muic_attached_dev_t cur_dev);
extern void muic_notifier_logically_attach_attached_dev(muic_attached_dev_t new_dev);
extern void muic_notifier_logically_detach_attached_dev(muic_attached_dev_t cur_dev);


extern int muic_notifier_register(struct notifier_block *nb,
		notifier_fn_t notifier, muic_notifier_device_t listener);
extern int muic_notifier_unregister(struct notifier_block *nb);


extern void muic_notifier_set_new_noti(bool flag);

#endif 
