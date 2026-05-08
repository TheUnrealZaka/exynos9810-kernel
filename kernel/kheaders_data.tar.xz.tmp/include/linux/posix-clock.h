
#ifndef _LINUX_POSIX_CLOCK_H_
#define _LINUX_POSIX_CLOCK_H_

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/posix-timers.h>
#include <linux/rwsem.h>

struct posix_clock;


struct posix_clock_operations {
	struct module *owner;

	int  (*clock_adjtime)(struct posix_clock *pc, struct timex *tx);

	int  (*clock_gettime)(struct posix_clock *pc, struct timespec64 *ts);

	int  (*clock_getres) (struct posix_clock *pc, struct timespec64 *ts);

	int  (*clock_settime)(struct posix_clock *pc,
			      const struct timespec64 *ts);

	int  (*timer_create) (struct posix_clock *pc, struct k_itimer *kit);

	int  (*timer_delete) (struct posix_clock *pc, struct k_itimer *kit);

	void (*timer_gettime)(struct posix_clock *pc,
			      struct k_itimer *kit, struct itimerspec64 *tsp);

	int  (*timer_settime)(struct posix_clock *pc,
			      struct k_itimer *kit, int flags,
			      struct itimerspec64 *tsp, struct itimerspec64 *old);
	
	int     (*fasync)  (struct posix_clock *pc,
			    int fd, struct file *file, int on);

	long    (*ioctl)   (struct posix_clock *pc,
			    unsigned int cmd, unsigned long arg);

	int     (*mmap)    (struct posix_clock *pc,
			    struct vm_area_struct *vma);

	int     (*open)    (struct posix_clock *pc, fmode_t f_mode);

	uint    (*poll)    (struct posix_clock *pc,
			    struct file *file, poll_table *wait);

	int     (*release) (struct posix_clock *pc);

	ssize_t (*read)    (struct posix_clock *pc,
			    uint flags, char __user *buf, size_t cnt);
};


struct posix_clock {
	struct posix_clock_operations ops;
	struct cdev cdev;
	struct device *dev;
	struct rw_semaphore rwsem;
	bool zombie;
};


int posix_clock_register(struct posix_clock *clk, struct device *dev);


void posix_clock_unregister(struct posix_clock *clk);

#endif
