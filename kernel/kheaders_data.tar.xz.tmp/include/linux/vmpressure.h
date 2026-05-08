#ifndef __LINUX_VMPRESSURE_H
#define __LINUX_VMPRESSURE_H

#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/gfp.h>
#include <linux/types.h>
#include <linux/cgroup.h>
#include <linux/eventfd.h>

struct vmpressure {
	unsigned long scanned;
	unsigned long reclaimed;
	unsigned long pressure;

	unsigned long tree_scanned;
	unsigned long tree_reclaimed;
	
	struct spinlock sr_lock;

	
	struct list_head events;
	
	struct mutex events_lock;

	struct work_struct work;
};

struct mem_cgroup;

#ifdef CONFIG_MEMCG
extern void vmpressure(gfp_t gfp, struct mem_cgroup *memcg, bool tree,
		       unsigned long scanned, unsigned long reclaimed);
extern void vmpressure_prio(gfp_t gfp, struct mem_cgroup *memcg, int prio);

extern void vmpressure_init(struct vmpressure *vmpr);
extern void vmpressure_cleanup(struct vmpressure *vmpr);
extern struct vmpressure *memcg_to_vmpressure(struct mem_cgroup *memcg);
extern struct cgroup_subsys_state *vmpressure_to_css(struct vmpressure *vmpr);
extern int vmpressure_register_event(struct mem_cgroup *memcg,
				     struct eventfd_ctx *eventfd,
				     const char *args);
extern void vmpressure_unregister_event(struct mem_cgroup *memcg,
					struct eventfd_ctx *eventfd);
#else
static inline void vmpressure(gfp_t gfp, struct mem_cgroup *memcg, bool tree,
			      unsigned long scanned, unsigned long reclaimed) {}
static inline void vmpressure_prio(gfp_t gfp, struct mem_cgroup *memcg,
				   int prio) {}
#endif 
#endif 
