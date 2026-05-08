

#ifndef __MAILBOX_CLIENT_H
#define __MAILBOX_CLIENT_H

#include <linux/of.h>
#include <linux/device.h>

struct mbox_chan;


struct mbox_client {
	struct device *dev;
	bool tx_block;
	unsigned long tx_tout;
	bool knows_txdone;

	void (*rx_callback)(struct mbox_client *cl, void *mssg);
	void (*tx_prepare)(struct mbox_client *cl, void *mssg);
	void (*tx_done)(struct mbox_client *cl, void *mssg, int r);
};

struct mbox_chan *mbox_request_channel_byname(struct mbox_client *cl,
					      const char *name);
struct mbox_chan *mbox_request_channel(struct mbox_client *cl, int index);
int mbox_send_message(struct mbox_chan *chan, void *mssg);
void mbox_client_txdone(struct mbox_chan *chan, int r); 
bool mbox_client_peek_data(struct mbox_chan *chan); 
void mbox_free_channel(struct mbox_chan *chan); 

#endif 
