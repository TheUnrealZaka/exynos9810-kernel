

#ifndef LINUX_PPS_KERNEL_H
#define LINUX_PPS_KERNEL_H

#include <linux/pps.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/time.h>



struct pps_device;


struct pps_source_info {
	char name[PPS_MAX_NAME_LEN];		
	char path[PPS_MAX_NAME_LEN];		
	int mode;				

	void (*echo)(struct pps_device *pps,
			int event, void *data);	

	struct module *owner;
	struct device *dev;		
};

struct pps_event_time {
#ifdef CONFIG_NTP_PPS
	struct timespec64 ts_raw;
#endif 
	struct timespec64 ts_real;
};


struct pps_device {
	struct pps_source_info info;		

	struct pps_kparams params;		

	__u32 assert_sequence;			
	__u32 clear_sequence;			
	struct pps_ktime assert_tu;
	struct pps_ktime clear_tu;
	int current_mode;			

	unsigned int last_ev;			
	wait_queue_head_t queue;		

	unsigned int id;			
	void const *lookup_cookie;		
	struct cdev cdev;
	struct device *dev;
	struct fasync_struct *async_queue;	
	spinlock_t lock;
};



extern const struct attribute_group *pps_groups[];



extern int pps_register_cdev(struct pps_device *pps);
extern void pps_unregister_cdev(struct pps_device *pps);



extern struct pps_device *pps_register_source(
		struct pps_source_info *info, int default_params);
extern void pps_unregister_source(struct pps_device *pps);
extern void pps_event(struct pps_device *pps,
		struct pps_event_time *ts, int event, void *data);

struct pps_device *pps_lookup_dev(void const *cookie);

static inline void timespec_to_pps_ktime(struct pps_ktime *kt,
		struct timespec64 ts)
{
	kt->sec = ts.tv_sec;
	kt->nsec = ts.tv_nsec;
}

static inline void pps_get_ts(struct pps_event_time *ts)
{
	struct system_time_snapshot snap;

	ktime_get_snapshot(&snap);
	ts->ts_real = ktime_to_timespec64(snap.real);
#ifdef CONFIG_NTP_PPS
	ts->ts_raw = ktime_to_timespec64(snap.raw);
#endif
}


static inline void pps_sub_ts(struct pps_event_time *ts, struct timespec64 delta)
{
	ts->ts_real = timespec64_sub(ts->ts_real, delta);
#ifdef CONFIG_NTP_PPS
	ts->ts_raw = timespec64_sub(ts->ts_raw, delta);
#endif
}

#endif 

