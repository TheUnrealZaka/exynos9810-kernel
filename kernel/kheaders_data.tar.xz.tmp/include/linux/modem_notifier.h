

#ifndef __MODEM_NOTIFIER_H__
#define __MODEM_NOTIFIER_H__


enum modem_event {
	MODEM_EVENT_RESET	= 1,
	MODEM_EVENT_EXIT,
	MODEM_EVENT_ONLINE	= 4,
	MODEM_EVENT_WATCHDOG	= 9,
};

extern int register_modem_event_notifier(struct notifier_block *nb);
extern void modem_notify_event(enum modem_event evt);

#endif
