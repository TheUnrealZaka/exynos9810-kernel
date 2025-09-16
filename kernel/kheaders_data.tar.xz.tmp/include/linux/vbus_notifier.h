

#ifndef __VBUS_NOTIFIER_H__
#define __VBUS_NOTIFIER_H__


typedef enum {
	VBUS_NOTIFY_CMD_NONE = 0,
	VBUS_NOTIFY_CMD_FALLING,
	VBUS_NOTIFY_CMD_RISING,
} vbus_notifier_cmd_t;


typedef enum {
	VBUS_NOTIFY_DEV_USB,
	VBUS_NOTIFY_DEV_CHARGER,
	VBUS_NOTIFY_DEV_MANAGER,	
} vbus_notifier_device_t;

typedef enum {
	STATUS_VBUS_UNKNOWN = 0,
	STATUS_VBUS_LOW,
	STATUS_VBUS_HIGH,
} vbus_status_t;

typedef enum {
	VBUS_NOTIFIER_NOT_READY = 0,
	VBUS_NOTIFIER_NOT_READY_DETECT,
	VBUS_NOTIFIER_READY,
} vbus_notifier_stat_t;

struct vbus_notifier_struct {
	struct mutex		vbus_mutex;
	vbus_notifier_stat_t	status;
	vbus_status_t		vbus_type;
	vbus_notifier_cmd_t	cmd;
	struct blocking_notifier_head notifier_call_chain;
};

#define VBUS_NOTIFIER_BLOCK(name)	\
	struct notifier_block (name)


extern void vbus_notifier_handle(vbus_status_t new_dev);


extern int vbus_notifier_register(struct notifier_block *nb,
		notifier_fn_t notifier, vbus_notifier_device_t listener);
extern int vbus_notifier_unregister(struct notifier_block *nb);

#endif 
