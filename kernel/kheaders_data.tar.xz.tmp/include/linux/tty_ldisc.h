#ifndef _LINUX_TTY_LDISC_H
#define _LINUX_TTY_LDISC_H



#include <linux/fs.h>
#include <linux/wait.h>



struct ld_semaphore {
	long			count;
	raw_spinlock_t		wait_lock;
	unsigned int		wait_readers;
	struct list_head	read_wait;
	struct list_head	write_wait;
#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lockdep_map	dep_map;
#endif
};

extern void __init_ldsem(struct ld_semaphore *sem, const char *name,
			 struct lock_class_key *key);

#define init_ldsem(sem)						\
do {								\
	static struct lock_class_key __key;			\
								\
	__init_ldsem((sem), #sem, &__key);			\
} while (0)


extern int ldsem_down_read(struct ld_semaphore *sem, long timeout);
extern int ldsem_down_read_trylock(struct ld_semaphore *sem);
extern int ldsem_down_write(struct ld_semaphore *sem, long timeout);
extern int ldsem_down_write_trylock(struct ld_semaphore *sem);
extern void ldsem_up_read(struct ld_semaphore *sem);
extern void ldsem_up_write(struct ld_semaphore *sem);

#ifdef CONFIG_DEBUG_LOCK_ALLOC
extern int ldsem_down_read_nested(struct ld_semaphore *sem, int subclass,
				  long timeout);
extern int ldsem_down_write_nested(struct ld_semaphore *sem, int subclass,
				   long timeout);
#else
# define ldsem_down_read_nested(sem, subclass, timeout)		\
		ldsem_down_read(sem, timeout)
# define ldsem_down_write_nested(sem, subclass, timeout)	\
		ldsem_down_write(sem, timeout)
#endif


struct tty_ldisc_ops {
	int	magic;
	char	*name;
	int	num;
	int	flags;

	
	int	(*open)(struct tty_struct *);
	void	(*close)(struct tty_struct *);
	void	(*flush_buffer)(struct tty_struct *tty);
	ssize_t	(*read)(struct tty_struct *tty, struct file *file,
			unsigned char __user *buf, size_t nr);
	ssize_t	(*write)(struct tty_struct *tty, struct file *file,
			 const unsigned char *buf, size_t nr);
	int	(*ioctl)(struct tty_struct *tty, struct file *file,
			 unsigned int cmd, unsigned long arg);
	long	(*compat_ioctl)(struct tty_struct *tty, struct file *file,
				unsigned int cmd, unsigned long arg);
	void	(*set_termios)(struct tty_struct *tty, struct ktermios *old);
	unsigned int (*poll)(struct tty_struct *, struct file *,
			     struct poll_table_struct *);
	int	(*hangup)(struct tty_struct *tty);

	
	void	(*receive_buf)(struct tty_struct *, const unsigned char *cp,
			       char *fp, int count);
	void	(*write_wakeup)(struct tty_struct *);
	void	(*dcd_change)(struct tty_struct *, unsigned int);
	int	(*receive_buf2)(struct tty_struct *, const unsigned char *cp,
				char *fp, int count);

	struct  module *owner;

	int refcount;
};

struct tty_ldisc {
	struct tty_ldisc_ops *ops;
	struct tty_struct *tty;
};

#define TTY_LDISC_MAGIC	0x5403

#define LDISC_FLAG_DEFINED	0x00000001

#define MODULE_ALIAS_LDISC(ldisc) \
	MODULE_ALIAS("tty-ldisc-" __stringify(ldisc))

#endif 
