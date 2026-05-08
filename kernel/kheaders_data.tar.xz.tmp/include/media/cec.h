

#ifndef _MEDIA_CEC_H
#define _MEDIA_CEC_H

#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kthread.h>
#include <linux/timer.h>
#include <linux/cec-funcs.h>
#include <media/rc-core.h>
#include <media/cec-edid.h>


struct cec_devnode {
	
	struct device dev;
	struct cdev cdev;
	struct device *parent;

	
	int minor;
	bool registered;
	bool unregistered;
	struct list_head fhs;
	struct mutex lock;
};

struct cec_adapter;
struct cec_data;

struct cec_data {
	struct list_head list;
	struct list_head xfer_list;
	struct cec_adapter *adap;
	struct cec_msg msg;
	struct cec_fh *fh;
	struct delayed_work work;
	struct completion c;
	u8 attempts;
	bool new_initiator;
	bool blocking;
	bool completed;
};

struct cec_msg_entry {
	struct list_head	list;
	struct cec_msg		msg;
};

#define CEC_NUM_EVENTS		CEC_EVENT_LOST_MSGS

struct cec_fh {
	struct list_head	list;
	struct list_head	xfer_list;
	struct cec_adapter	*adap;
	u8			mode_initiator;
	u8			mode_follower;

	
	wait_queue_head_t	wait;
	unsigned int		pending_events;
	struct cec_event	events[CEC_NUM_EVENTS];
	struct mutex		lock;
	struct list_head	msgs; 
	unsigned int		queued_msgs;
};

#define CEC_SIGNAL_FREE_TIME_RETRY		3
#define CEC_SIGNAL_FREE_TIME_NEW_INITIATOR	5
#define CEC_SIGNAL_FREE_TIME_NEXT_XFER		7


#define CEC_FREE_TIME_TO_USEC(ft)		((ft) * 2400)

struct cec_adap_ops {
	
	int (*adap_enable)(struct cec_adapter *adap, bool enable);
	int (*adap_monitor_all_enable)(struct cec_adapter *adap, bool enable);
	int (*adap_log_addr)(struct cec_adapter *adap, u8 logical_addr);
	int (*adap_transmit)(struct cec_adapter *adap, u8 attempts,
			     u32 signal_free_time, struct cec_msg *msg);
	void (*adap_status)(struct cec_adapter *adap, struct seq_file *file);

	
	int (*received)(struct cec_adapter *adap, struct cec_msg *msg);
};


#define CEC_MAX_MSG_RX_QUEUE_SZ		(18 * 3)


#define CEC_MAX_MSG_TX_QUEUE_SZ		(18 * 1)

struct cec_adapter {
	struct module *owner;
	char name[32];
	struct cec_devnode devnode;
	struct mutex lock;
	struct rc_dev *rc;

	struct list_head transmit_queue;
	unsigned int transmit_queue_sz;
	struct list_head wait_queue;
	struct cec_data *transmitting;

	struct task_struct *kthread_config;
	struct completion config_completion;

	struct task_struct *kthread;
	wait_queue_head_t kthread_waitq;
	wait_queue_head_t waitq;

	const struct cec_adap_ops *ops;
	void *priv;
	u32 capabilities;
	u8 available_log_addrs;

	u16 phys_addr;
	bool is_configuring;
	bool is_configured;
	u32 monitor_all_cnt;
	u32 follower_cnt;
	struct cec_fh *cec_follower;
	struct cec_fh *cec_initiator;
	bool passthrough;
	struct cec_log_addrs log_addrs;

	struct dentry *cec_dir;
	struct dentry *status_file;

	u16 phys_addrs[15];
	u32 sequence;

	char input_name[32];
	char input_phys[32];
	char input_drv[32];
};

static inline bool cec_has_log_addr(const struct cec_adapter *adap, u8 log_addr)
{
	return adap->log_addrs.log_addr_mask & (1 << log_addr);
}

static inline bool cec_is_sink(const struct cec_adapter *adap)
{
	return adap->phys_addr == 0;
}

#if IS_ENABLED(CONFIG_MEDIA_CEC)
struct cec_adapter *cec_allocate_adapter(const struct cec_adap_ops *ops,
		void *priv, const char *name, u32 caps, u8 available_las,
		struct device *parent);
int cec_register_adapter(struct cec_adapter *adap);
void cec_unregister_adapter(struct cec_adapter *adap);
void cec_delete_adapter(struct cec_adapter *adap);

int cec_s_log_addrs(struct cec_adapter *adap, struct cec_log_addrs *log_addrs,
		    bool block);
void cec_s_phys_addr(struct cec_adapter *adap, u16 phys_addr,
		     bool block);
int cec_transmit_msg(struct cec_adapter *adap, struct cec_msg *msg,
		     bool block);


void cec_transmit_done(struct cec_adapter *adap, u8 status, u8 arb_lost_cnt,
		       u8 nack_cnt, u8 low_drive_cnt, u8 error_cnt);
void cec_received_msg(struct cec_adapter *adap, struct cec_msg *msg);

#else

static inline int cec_register_adapter(struct cec_adapter *adap)
{
	return 0;
}

static inline void cec_unregister_adapter(struct cec_adapter *adap)
{
}

static inline void cec_delete_adapter(struct cec_adapter *adap)
{
}

static inline void cec_s_phys_addr(struct cec_adapter *adap, u16 phys_addr,
				   bool block)
{
}

#endif

#endif 
