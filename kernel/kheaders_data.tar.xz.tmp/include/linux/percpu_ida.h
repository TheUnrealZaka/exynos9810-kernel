#ifndef __PERCPU_IDA_H__
#define __PERCPU_IDA_H__

#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/spinlock_types.h>
#include <linux/wait.h>
#include <linux/cpumask.h>

struct percpu_ida_cpu;

struct percpu_ida {
	
	unsigned			nr_tags;
	unsigned			percpu_max_size;
	unsigned			percpu_batch_size;

	struct percpu_ida_cpu __percpu	*tag_cpu;

	
	cpumask_t			cpus_have_tags;

	struct {
		spinlock_t		lock;
		
		unsigned		cpu_last_stolen;

		
		wait_queue_head_t	wait;

		
		unsigned		nr_free;
		unsigned		*freelist;
	} ____cacheline_aligned_in_smp;
};


#define IDA_DEFAULT_PCPU_BATCH_MOVE	32U

#define IDA_DEFAULT_PCPU_SIZE	((IDA_DEFAULT_PCPU_BATCH_MOVE * 3) / 2)

int percpu_ida_alloc(struct percpu_ida *pool, int state);
void percpu_ida_free(struct percpu_ida *pool, unsigned tag);

void percpu_ida_destroy(struct percpu_ida *pool);
int __percpu_ida_init(struct percpu_ida *pool, unsigned long nr_tags,
	unsigned long max_size, unsigned long batch_size);
static inline int percpu_ida_init(struct percpu_ida *pool, unsigned long nr_tags)
{
	return __percpu_ida_init(pool, nr_tags, IDA_DEFAULT_PCPU_SIZE,
		IDA_DEFAULT_PCPU_BATCH_MOVE);
}

typedef int (*percpu_ida_cb)(unsigned, void *);
int percpu_ida_for_each_free(struct percpu_ida *pool, percpu_ida_cb fn,
	void *data);

unsigned percpu_ida_free_tags(struct percpu_ida *pool, int cpu);
#endif 
