#ifndef _LINUX_PSI_TYPES_H
#define _LINUX_PSI_TYPES_H

#include <linux/kthread.h>
#include <linux/seqlock.h>
#include <linux/types.h>
#include <linux/kref.h>
#include <linux/wait.h>

#ifdef CONFIG_PSI


enum psi_task_count {
	NR_IOWAIT,
	NR_MEMSTALL,
	NR_RUNNING,
	NR_PSI_TASK_COUNTS = 3,
};


#define TSK_IOWAIT	(1 << NR_IOWAIT)
#define TSK_MEMSTALL	(1 << NR_MEMSTALL)
#define TSK_RUNNING	(1 << NR_RUNNING)


enum psi_res {
	PSI_IO,
	PSI_MEM,
	PSI_CPU,
	NR_PSI_RESOURCES = 3,
};


enum psi_states {
	PSI_IO_SOME,
	PSI_IO_FULL,
	PSI_MEM_SOME,
	PSI_MEM_FULL,
	PSI_CPU_SOME,
	
	PSI_NONIDLE,
	NR_PSI_STATES = 6,
};

enum psi_aggregators {
	PSI_AVGS = 0,
	PSI_POLL,
	NR_PSI_AGGREGATORS,
};

struct psi_group_cpu {
	

	
	seqcount_t seq ____cacheline_aligned_in_smp;

	
	unsigned int tasks[NR_PSI_TASK_COUNTS];

	
	u32 state_mask;

	
	u32 times[NR_PSI_STATES];

	
	u64 state_start;

	

	
	u32 times_prev[NR_PSI_AGGREGATORS][NR_PSI_STATES]
			____cacheline_aligned_in_smp;
};


struct psi_window {
	
	u64 size;

	
	u64 start_time;

	
	u64 start_value;

	
	u64 prev_growth;
};

struct psi_trigger {
	
	enum psi_states state;

	
	u64 threshold;

	
	struct list_head node;

	
	struct psi_group *group;

	
	wait_queue_head_t event_wait;

	
	int event;

	
	struct psi_window win;

	
	u64 last_event_time;
};

struct psi_group {
	
	struct mutex avgs_lock;

	
	struct psi_group_cpu __percpu *pcpu;

	
	u64 avg_total[NR_PSI_STATES - 1];
	u64 avg_last_update;
	u64 avg_next_update;

	
	struct delayed_work avgs_work;

	
	u64 total[NR_PSI_AGGREGATORS][NR_PSI_STATES - 1];
	unsigned long avg[NR_PSI_STATES - 1][3];

	
	atomic_t poll_scheduled;
	struct kthread_worker __rcu *poll_kworker;
	struct kthread_delayed_work poll_work;

	
	struct mutex trigger_lock;

	
	struct list_head triggers;
	u32 nr_triggers[NR_PSI_STATES - 1];
	u32 poll_states;
	u64 poll_min_period;

	
	u64 polling_total[NR_PSI_STATES - 1];
	u64 polling_next_update;
	u64 polling_until;
};

#else 

struct psi_group { };

#endif 

#endif 
