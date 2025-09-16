

#ifndef _MFD_IPAQ_MICRO_H_
#define _MFD_IPAQ_MICRO_H_

#include <linux/spinlock.h>
#include <linux/completion.h>
#include <linux/list.h>

#define TX_BUF_SIZE	32
#define RX_BUF_SIZE	16
#define CHAR_SOF	0x02


#define MSG_VERSION		0x0
#define MSG_KEYBOARD		0x2
#define MSG_TOUCHSCREEN		0x3
#define MSG_EEPROM_READ		0x4
#define MSG_EEPROM_WRITE	0x5
#define MSG_THERMAL_SENSOR	0x6
#define MSG_NOTIFY_LED		0x8
#define MSG_BATTERY		0x9
#define MSG_SPI_READ		0xb
#define MSG_SPI_WRITE		0xc
#define MSG_BACKLIGHT		0xd 
#define MSG_CODEC_CTRL		0xe 
#define MSG_DISPLAY_CTRL	0xf 


enum rx_state {
	STATE_SOF = 0,     
	STATE_ID,          
	STATE_DATA,        
	STATE_CHKSUM       
};


struct ipaq_micro_txdev {
	u8 len;
	u8 index;
	u8 buf[TX_BUF_SIZE];
};


struct ipaq_micro_rxdev {
	enum rx_state state;
	unsigned char chksum;
	u8            id;
	unsigned int  len;
	unsigned int  index;
	u8            buf[RX_BUF_SIZE];
};


struct ipaq_micro_msg {
	u8 id;
	u8 tx_len;
	u8 tx_data[TX_BUF_SIZE];
	u8 rx_len;
	u8 rx_data[RX_BUF_SIZE];
	struct completion ack;
	struct list_head node;
};


struct ipaq_micro {
	struct device *dev;
	void __iomem *base;
	void __iomem *sdlc;
	char version[5];
	struct ipaq_micro_txdev tx;	
	struct ipaq_micro_rxdev rx;	
	spinlock_t lock;
	struct ipaq_micro_msg *msg;
	struct list_head queue;
	void (*key) (void *data, int len, unsigned char *rxdata);
	void *key_data;
	void (*ts) (void *data, int len, unsigned char *rxdata);
	void *ts_data;
};

extern int
ipaq_micro_tx_msg(struct ipaq_micro *micro, struct ipaq_micro_msg *msg);

static inline int
ipaq_micro_tx_msg_sync(struct ipaq_micro *micro,
		       struct ipaq_micro_msg *msg)
{
	int ret;

	init_completion(&msg->ack);
	ret = ipaq_micro_tx_msg(micro, msg);
	wait_for_completion(&msg->ack);

	return ret;
}

static inline int
ipaq_micro_tx_msg_async(struct ipaq_micro *micro,
			struct ipaq_micro_msg *msg)
{
	init_completion(&msg->ack);
	return ipaq_micro_tx_msg(micro, msg);
}

#endif 
