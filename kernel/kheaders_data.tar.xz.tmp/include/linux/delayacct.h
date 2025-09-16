

#ifndef _LINUX_DELAYACCT_H
#define _LINUX_DELAYACCT_H

#include <linux/sched.h>
#include <linux/slab.h>


#define DELAYACCT_PF_SWAPIN	0x00000001	
#define DELAYACCT_PF_BLKIO	0x00000002	

#ifdef CONFIG_TASK_DELAY_ACCT
struct task_delay_info {
	spinlock_t	lock;
	unsigned int	flags;	

	

	
	u64 blkio_start;	
	u64 blkio_delay;	
	u64 swapin_delay;	
	u32 blkio_count;	
				
	u32 swapin_count;	
				

	u64 freepages_start;
	u64 freepages_delay;	

	u64 thrashing_start;
	u64 thrashing_delay;	

	u32 freepages_count;	
	u32 thrashing_count;	
};
#endif

#include <linux/sched.h>
#include <linux/slab.h>

#ifdef CONFIG_TASK_DELAY_ACCT
extern int delayacct_on;	
extern struct kmem_cache *delayacct_cache;
extern void delayacct_init(void);
extern void __delayacct_tsk_init(struct task_struct *);
extern void __delayacct_tsk_exit(struct task_struct *);
extern void __delayacct_blkio_start(void);
extern void __delayacct_blkio_end(void);
extern int __delayacct_add_tsk(struct taskstats *, struct task_struct *);
extern __u64 __delayacct_blkio_ticks(struct task_struct *);
#ifdef CONFIG_PAGE_BOOST
extern __u64 __delayacct_blkio_nsecs(struct task_struct *);
#endif
extern void __delayacct_freepages_start(void);
extern void __delayacct_freepages_end(void);
extern void __delayacct_thrashing_start(void);
extern void __delayacct_thrashing_end(void);

static inline int delayacct_is_task_waiting_on_io(struct task_struct *p)
{
	if (p->delays)
		return (p->delays->flags & DELAYACCT_PF_BLKIO);
	else
		return 0;
}

static inline void delayacct_set_flag(int flag)
{
	if (current->delays)
		current->delays->flags |= flag;
}

static inline void delayacct_clear_flag(int flag)
{
	if (current->delays)
		current->delays->flags &= ~flag;
}

static inline void delayacct_tsk_init(struct task_struct *tsk)
{
	
	tsk->delays = NULL;
	if (delayacct_on)
		__delayacct_tsk_init(tsk);
}


static inline void delayacct_tsk_free(struct task_struct *tsk)
{
	if (tsk->delays)
		kmem_cache_free(delayacct_cache, tsk->delays);
	tsk->delays = NULL;
}

static inline void delayacct_blkio_start(void)
{
	delayacct_set_flag(DELAYACCT_PF_BLKIO);
	if (current->delays)
		__delayacct_blkio_start();
}

static inline void delayacct_blkio_end(void)
{
	if (current->delays)
		__delayacct_blkio_end();
	delayacct_clear_flag(DELAYACCT_PF_BLKIO);
}

static inline int delayacct_add_tsk(struct taskstats *d,
					struct task_struct *tsk)
{
	if (!delayacct_on || !tsk->delays)
		return 0;
	return __delayacct_add_tsk(d, tsk);
}

static inline __u64 delayacct_blkio_ticks(struct task_struct *tsk)
{
	if (tsk->delays)
		return __delayacct_blkio_ticks(tsk);
	return 0;
}

#ifdef CONFIG_PAGE_BOOST
static inline __u64 delayacct_blkio_nsecs(struct task_struct *tsk)
{
	if (tsk->delays)
		return __delayacct_blkio_nsecs(tsk);
	return 0;
}
#endif

static inline void delayacct_freepages_start(void)
{
	if (current->delays)
		__delayacct_freepages_start();
}

static inline void delayacct_freepages_end(void)
{
	if (current->delays)
		__delayacct_freepages_end();
}

static inline void delayacct_thrashing_start(void)
{
	if (current->delays)
		__delayacct_thrashing_start();
}

static inline void delayacct_thrashing_end(void)
{
	if (current->delays)
		__delayacct_thrashing_end();
}

#else
static inline void delayacct_set_flag(int flag)
{}
static inline void delayacct_clear_flag(int flag)
{}
static inline void delayacct_init(void)
{}
static inline void delayacct_tsk_init(struct task_struct *tsk)
{}
static inline void delayacct_tsk_free(struct task_struct *tsk)
{}
static inline void delayacct_blkio_start(void)
{}
static inline void delayacct_blkio_end(void)
{}
static inline int delayacct_add_tsk(struct taskstats *d,
					struct task_struct *tsk)
{ return 0; }
static inline __u64 delayacct_blkio_ticks(struct task_struct *tsk)
{ return 0; }
#ifdef CONFIG_PAGE_BOOST
static inline __u64 delayacct_blkio_nsecs(struct task_struct *tsk)
{ return 0; }
#endif
static inline int delayacct_is_task_waiting_on_io(struct task_struct *p)
{ return 0; }
static inline void delayacct_freepages_start(void)
{}
static inline void delayacct_freepages_end(void)
{}
static inline void delayacct_thrashing_start(void)
{}
static inline void delayacct_thrashing_end(void)
{}

#endif 

#endif
