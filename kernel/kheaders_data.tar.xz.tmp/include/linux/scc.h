
#ifndef	_SCC_H
#define	_SCC_H

#include <uapi/linux/scc.h>


enum {TX_OFF, TX_ON};	



#define VECTOR_MASK	0x06
#define TXINT		0x00
#define EXINT		0x02
#define RXINT		0x04
#define SPINT		0x06

#ifdef CONFIG_SCC_DELAY
#define Inb(port)	inb_p(port)
#define Outb(port, val)	outb_p(val, port)
#else
#define Inb(port)	inb(port)
#define Outb(port, val)	outb(val, port)
#endif



struct scc_kiss {
	unsigned char txdelay;		
	unsigned char persist;		
	unsigned char slottime;		
	unsigned char tailtime;		
	unsigned char fulldup;		
	unsigned char waittime;		
	unsigned int  maxkeyup;		
	unsigned int  mintime;		
	unsigned int  idletime;		
	unsigned int  maxdefer;		
	unsigned char tx_inhibit;		
	unsigned char group;		
	unsigned char mode;		
	unsigned char softdcd;		
};




struct scc_channel {
	int init;			

	struct net_device *dev;		
	struct net_device_stats dev_stat;

	char brand;			
	long clock;			

	io_port ctrl;			
	io_port	data;			
	io_port special;		
	int irq;			

	char option;
	char enhanced;			

	unsigned char wreg[16]; 	
	unsigned char status;		
	unsigned char dcd;		

        struct scc_kiss kiss;		
        struct scc_stat stat;		
        struct scc_modem modem; 	

        struct sk_buff_head tx_queue;	
        struct sk_buff *rx_buff;	
        struct sk_buff *tx_buff;	

	
	struct timer_list tx_t;		
	struct timer_list tx_wdog;	
	
	
	spinlock_t	lock;		
};

#endif 
