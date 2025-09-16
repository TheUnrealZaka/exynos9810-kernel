

#ifndef __MAILBOX_CONTROLLER_H
#define __MAILBOX_CONTROLLER_H

#include <linux/of.h>
#include <linux/types.h>
#include <linux/hrtimer.h>
#include <linux/device.h>
#include <linux/completion.h>

struct mbox_chan;


struct mbox_chan_ops {
	int (*send_data)(struct mbox_chan *chan, void *data);
	int (*startup)(struct mbox_chan *chan);
	void (*shutdown)(struct mbox_chan *chan);
	bool (*last_tx_done)(struct mbox_chan *chan);
	bool (*peek_data)(struct mbox_chan *chan);
};


struct mbox_controller {
	struct device *dev;
	const struct mbox_chan_ops *ops;
	struct mbox_chan *chans;
	int num_chans;
	bool txdone_irq;
	bool txdone_poll;
	unsigned txpoll_period;
	struct mbox_chan *(*of_xlate)(struct mbox_controller *mbox,
				      const struct of_phandle_args *sp);
	
	struct hrtimer poll_hrt;
	struct list_head node;
};


#define MBOX_TX_QUEUE_LEN	20


struct mbox_chan {
	struct mbox_controller *mbox;
	unsigned txdone_method;
	struct mbox_client *cl;
	struct completion tx_complete;
	void *active_req;
	unsigned msg_count, msg_free;
	void *msg_data[MBOX_TX_QUEUE_LEN];
	spinlock_t lock; 
	void *con_priv;
};

int mbox_controller_register(struct mbox_controller *mbox); 
void mbox_controller_unregister(struct mbox_controller *mbox); 
void mbox_chan_received_data(struct mbox_chan *chan, void *data); 
void mbox_chan_txdone(struct mbox_chan *chan, int r); 

#endif 
