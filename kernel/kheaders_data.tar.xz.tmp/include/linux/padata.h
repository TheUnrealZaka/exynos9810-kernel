

#ifndef PADATA_H
#define PADATA_H

#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/notifier.h>
#include <linux/kobject.h>

#define PADATA_CPU_SERIAL   0x01
#define PADATA_CPU_PARALLEL 0x02


struct padata_priv {
	struct list_head	list;
	struct parallel_data	*pd;
	int			cb_cpu;
	int			cpu;
	int			info;
	void                    (*parallel)(struct padata_priv *padata);
	void                    (*serial)(struct padata_priv *padata);
};


struct padata_list {
	struct list_head        list;
	spinlock_t              lock;
};


struct padata_serial_queue {
       struct padata_list    serial;
       struct work_struct    work;
       struct parallel_data *pd;
};


struct padata_parallel_queue {
       struct padata_list    parallel;
       struct padata_list    reorder;
       struct work_struct    work;
       atomic_t              num_obj;
       int                   cpu_index;
};


struct padata_cpumask {
	cpumask_var_t	pcpu;
	cpumask_var_t	cbcpu;
};


struct parallel_data {
	struct padata_instance		*pinst;
	struct padata_parallel_queue	__percpu *pqueue;
	struct padata_serial_queue	__percpu *squeue;
	atomic_t			reorder_objects;
	atomic_t			refcnt;
	atomic_t			seq_nr;
	int				cpu;
	struct padata_cpumask		cpumask;
	struct work_struct		reorder_work;
	spinlock_t                      lock ____cacheline_aligned;
};


struct padata_instance {
	struct hlist_node		 node;
	struct workqueue_struct		*wq;
	struct parallel_data		*pd;
	struct padata_cpumask		cpumask;
	struct blocking_notifier_head	 cpumask_change_notifier;
	struct kobject                   kobj;
	struct mutex			 lock;
	u8				 flags;
#define	PADATA_INIT	1
#define	PADATA_RESET	2
#define	PADATA_INVALID	4
};

extern struct padata_instance *padata_alloc_possible(
					struct workqueue_struct *wq);
extern struct padata_instance *padata_alloc(struct workqueue_struct *wq,
					    const struct cpumask *pcpumask,
					    const struct cpumask *cbcpumask);
extern void padata_free(struct padata_instance *pinst);
extern int padata_do_parallel(struct padata_instance *pinst,
			      struct padata_priv *padata, int cb_cpu);
extern void padata_do_serial(struct padata_priv *padata);
extern int padata_set_cpumask(struct padata_instance *pinst, int cpumask_type,
			      cpumask_var_t cpumask);
extern int padata_start(struct padata_instance *pinst);
extern void padata_stop(struct padata_instance *pinst);
extern int padata_register_cpumask_notifier(struct padata_instance *pinst,
					    struct notifier_block *nblock);
extern int padata_unregister_cpumask_notifier(struct padata_instance *pinst,
					      struct notifier_block *nblock);
#endif
