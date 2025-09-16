

#ifndef SEC_ABC_H
#define SEC_ABC_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sec_sysfs.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/err.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/suspend.h>
#include <linux/workqueue.h>
#include <linux/rtc.h>

#define ABC_UEVENT_MAX		20
#define ABC_BUFFER_MAX		256
#define ABC_LOG_STR_LEN		50
#define ABC_LOG_MAX		80

#define ABC_WAIT_ENABLE_TIMEOUT	10000

enum {
	ABC_DISABLED,
	
	ABC_TYPE1_ENABLED,
	
	ABC_TYPE2_ENABLED,
};

enum {
	ABC_EVENT_I2C = 1,
	ABC_EVENT_UNDERRUN,
	ABC_EVENT_GPU_FAULT,
};

struct abc_fault_info {
	unsigned long cur_time;
	int cur_cnt;
};

struct abc_buffer {
	int size;
	int rear;
	int front;

	struct abc_fault_info *abc_element;
};

struct abc_qdata {
	const char *desc;
	int queue_size;
	int threshold_cnt;
	int threshold_time;
	int fail_cnt;

	struct abc_buffer buffer;
};

struct abc_platform_data {
	struct abc_qdata *gpu_items;
	struct abc_qdata *aicl_items;

	unsigned int nItem;
	unsigned int nGpu;
	unsigned int nAicl;
};

struct abc_log_entry {
	struct list_head node;
	char abc_log_str[ABC_LOG_STR_LEN];
};

struct abc_info {
	struct device *dev;
	struct workqueue_struct *workqueue;
	struct work_struct work;
	struct list_head log_list;
	struct completion enable_done;
	int log_list_cnt;
	char abc_str[ABC_BUFFER_MAX];
	struct abc_platform_data *pdata;
	struct mutex log_mutex;
};

extern void sec_abc_send_event(char *str);
extern int sec_abc_get_enabled(void);
extern int sec_abc_wait_enabled(void);
#endif
