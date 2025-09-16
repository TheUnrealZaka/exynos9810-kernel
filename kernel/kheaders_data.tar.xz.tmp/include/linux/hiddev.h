

#ifndef _HIDDEV_H
#define _HIDDEV_H

#include <uapi/linux/hiddev.h>




struct hid_device;
struct hid_usage;
struct hid_field;
struct hid_report;

#ifdef CONFIG_USB_HIDDEV
int hiddev_connect(struct hid_device *hid, unsigned int force);
void hiddev_disconnect(struct hid_device *);
void hiddev_hid_event(struct hid_device *hid, struct hid_field *field,
		      struct hid_usage *usage, __s32 value);
void hiddev_report_event(struct hid_device *hid, struct hid_report *report);
#else
static inline int hiddev_connect(struct hid_device *hid,
		unsigned int force)
{ return -1; }
static inline void hiddev_disconnect(struct hid_device *hid) { }
static inline void hiddev_hid_event(struct hid_device *hid, struct hid_field *field,
		      struct hid_usage *usage, __s32 value) { }
static inline void hiddev_report_event(struct hid_device *hid, struct hid_report *report) { }
#endif

#endif
