

#ifndef __TDMB_NOTIFIER_H__
#define __TDMB_NOTIFIER_H__


enum tdmb_notifier_device_t {
	TDMB_NOTIFY_DEV_LCD = 0,
	TDMB_NOTIFY_DEV_TOUCH,
};

enum tdmb_notifier_event_t {
	TDMB_NOTIFY_EVENT_INITIAL = 0,
	TDMB_NOTIFY_EVENT_TUNNER,
};

struct _tdmb_tunner_status {
	int pwr;
	int status_info;
};

struct tdmb_notifier_struct {
	enum tdmb_notifier_event_t event;
	struct _tdmb_tunner_status tdmb_status;
};


extern void tdmb_notifier_call(struct tdmb_notifier_struct *value);
extern int tdmb_notifier_register(struct notifier_block *nb,
		notifier_fn_t notifier, enum tdmb_notifier_device_t listener);
extern int tdmb_notifier_unregister(struct notifier_block *nb);

#endif 
