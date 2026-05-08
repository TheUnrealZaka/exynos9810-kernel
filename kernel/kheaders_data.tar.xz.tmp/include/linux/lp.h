
#ifndef _LINUX_LP_H
#define _LINUX_LP_H


#include <linux/wait.h>
#include <linux/mutex.h>
#include <uapi/linux/lp.h>


#define LP_PARPORT_UNSPEC -4
#define LP_PARPORT_AUTO -3
#define LP_PARPORT_OFF -2
#define LP_PARPORT_NONE -1

#define LP_F(minor)	lp_table[(minor)].flags		
#define LP_CHAR(minor)	lp_table[(minor)].chars		
#define LP_TIME(minor)	lp_table[(minor)].time		
#define LP_WAIT(minor)	lp_table[(minor)].wait		
#define LP_IRQ(minor)	lp_table[(minor)].dev->port->irq 
					
#ifdef LP_STATS
#define LP_STAT(minor)	lp_table[(minor)].stats		
#endif
#define LP_BUFFER_SIZE PAGE_SIZE

#define LP_BASE(x)	lp_table[(x)].dev->port->base

#ifdef LP_STATS
struct lp_stats {
	unsigned long chars;
	unsigned long sleeps;
	unsigned int maxrun;
	unsigned int maxwait;
	unsigned int meanwait;
	unsigned int mdev;
};
#endif

struct lp_struct {
	struct pardevice *dev;
	unsigned long flags;
	unsigned int chars;
	unsigned int time;
	unsigned int wait;
	char *lp_buffer;
#ifdef LP_STATS
	unsigned int lastcall;
	unsigned int runchars;
	struct lp_stats stats;
#endif
	wait_queue_head_t waitq;
	unsigned int last_error;
	struct mutex port_mutex;
	wait_queue_head_t dataq;
	long timeout;
	unsigned int best_mode;
	unsigned int current_mode;
	unsigned long bits;
};





#define LP_PINTEN	0x10  
#define LP_PSELECP	0x08  
#define LP_PINITP	0x04  
#define LP_PAUTOLF	0x02  
#define LP_PSTROBE	0x01  


#define LP_DUMMY	0x00


#define LP_DELAY 	50

#endif
