

#ifndef __MTD_FLASHCHIP_H__
#define __MTD_FLASHCHIP_H__


#include <linux/sched.h>
#include <linux/mutex.h>

typedef enum {
	FL_READY,
	FL_STATUS,
	FL_CFI_QUERY,
	FL_JEDEC_QUERY,
	FL_ERASING,
	FL_ERASE_SUSPENDING,
	FL_ERASE_SUSPENDED,
	FL_WRITING,
	FL_WRITING_TO_BUFFER,
	FL_OTP_WRITE,
	FL_WRITE_SUSPENDING,
	FL_WRITE_SUSPENDED,
	FL_PM_SUSPENDED,
	FL_SYNCING,
	FL_UNLOADING,
	FL_LOCKING,
	FL_UNLOCKING,
	FL_POINT,
	FL_XIP_WHILE_ERASING,
	FL_XIP_WHILE_WRITING,
	FL_SHUTDOWN,
	
	FL_READING,
	FL_CACHEDPRG,
	
	FL_RESETING,
	FL_OTPING,
	FL_PREPARING_ERASE,
	FL_VERIFYING_ERASE,

	FL_UNKNOWN
} flstate_t;





struct flchip {
	unsigned long start; 
	//	unsigned long len;
	
	int ref_point_counter;
	flstate_t state;
	flstate_t oldstate;

	unsigned int write_suspended:1;
	unsigned int erase_suspended:1;
	unsigned long in_progress_block_addr;
	unsigned long in_progress_block_mask;

	struct mutex mutex;
	wait_queue_head_t wq; 
	int word_write_time;
	int buffer_write_time;
	int erase_time;

	int word_write_time_max;
	int buffer_write_time_max;
	int erase_time_max;

	void *priv;
};


struct flchip_shared {
	struct mutex lock;
	struct flchip *writing;
	struct flchip *erasing;
};


#endif 
