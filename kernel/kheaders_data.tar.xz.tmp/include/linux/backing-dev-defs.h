#ifndef __LINUX_BACKING_DEV_DEFS_H
#define __LINUX_BACKING_DEV_DEFS_H

#include <linux/list.h>
#include <linux/radix-tree.h>
#include <linux/rbtree.h>
#include <linux/spinlock.h>
#include <linux/percpu_counter.h>
#include <linux/percpu-refcount.h>
#include <linux/flex_proportions.h>
#include <linux/timer.h>
#include <linux/workqueue.h>

struct page;
struct device;
struct dentry;


enum wb_state {
	WB_registered,		
	WB_writeback_running,	
	WB_has_dirty_io,	
};

enum wb_congested_state {
	WB_async_congested,	
	WB_sync_congested,	
};

typedef int (congested_fn)(void *, int);

enum wb_stat_item {
	WB_RECLAIMABLE,
	WB_WRITEBACK,
	WB_DIRTIED,
	WB_WRITTEN,
	NR_WB_STAT_ITEMS
};

#define WB_STAT_BATCH (8*(1+ilog2(nr_cpu_ids)))


struct bdi_writeback_congested {
	unsigned long state;		
	atomic_t refcnt;		

#ifdef CONFIG_CGROUP_WRITEBACK
	struct backing_dev_info *bdi;	
	int blkcg_id;			
	struct rb_node rb_node;		
#endif
};


struct bdi_writeback {
	struct backing_dev_info *bdi;	

	unsigned long state;		
	unsigned long last_old_flush;	

	struct list_head b_dirty;	
	struct list_head b_io;		
	struct list_head b_more_io;	
	struct list_head b_dirty_time;	
	spinlock_t list_lock;		

	struct percpu_counter stat[NR_WB_STAT_ITEMS];

	struct bdi_writeback_congested *congested;

	unsigned long bw_time_stamp;	
	unsigned long dirtied_stamp;
	unsigned long written_stamp;	
	unsigned long write_bandwidth;	
	unsigned long avg_write_bandwidth; 

	
	unsigned long dirty_ratelimit;
	unsigned long balanced_dirty_ratelimit;

	struct fprop_local_percpu completions;
	int dirty_exceeded;

	spinlock_t work_lock;		
	struct list_head work_list;
	struct delayed_work dwork;	

	struct list_head bdi_node;	

#ifdef CONFIG_CGROUP_WRITEBACK
	struct percpu_ref refcnt;	
	struct fprop_local_percpu memcg_completions;
	struct cgroup_subsys_state *memcg_css; 
	struct cgroup_subsys_state *blkcg_css; 
	struct list_head memcg_node;	
	struct list_head blkcg_node;	

	union {
		struct work_struct release_work;
		struct rcu_head rcu;
	};
#endif
};

struct backing_dev_info {
	struct list_head bdi_list;
	unsigned long ra_pages;	
	unsigned long io_pages;	
	unsigned int capabilities; 
	congested_fn *congested_fn; 
	void *congested_data;	

	char *name;

	unsigned int min_ratio;
	unsigned int max_ratio, max_prop_frac;

	
	unsigned long last_thresh;  
	unsigned long last_nr_dirty; 
	unsigned long paused_total; 

	
	atomic_long_t tot_write_bandwidth;

	struct bdi_writeback wb;  
	struct list_head wb_list; 
#ifdef CONFIG_CGROUP_WRITEBACK
	struct radix_tree_root cgwb_tree; 
	struct rb_root cgwb_congested_tree; 
	atomic_t usage_cnt; 
	struct rw_semaphore wb_switch_rwsem; 
#else
	struct bdi_writeback_congested *wb_congested;
#endif
	wait_queue_head_t wb_waitq;

	struct device *dev;
	struct device *owner;

	struct timer_list laptop_mode_wb_timer;

#ifdef CONFIG_DEBUG_FS
	struct dentry *debug_dir;
	struct dentry *debug_stats;
#endif
};

enum {
	BLK_RW_ASYNC	= 0,
	BLK_RW_SYNC	= 1,
};

void clear_wb_congested(struct bdi_writeback_congested *congested, int sync);
void set_wb_congested(struct bdi_writeback_congested *congested, int sync);

static inline void clear_bdi_congested(struct backing_dev_info *bdi, int sync)
{
	clear_wb_congested(bdi->wb.congested, sync);
}

static inline void set_bdi_congested(struct backing_dev_info *bdi, int sync)
{
	set_wb_congested(bdi->wb.congested, sync);
}

struct wb_lock_cookie {
	bool locked;
	unsigned long flags;
};

#ifdef CONFIG_CGROUP_WRITEBACK


static inline bool wb_tryget(struct bdi_writeback *wb)
{
	if (wb != &wb->bdi->wb)
		return percpu_ref_tryget(&wb->refcnt);
	return true;
}


static inline void wb_get(struct bdi_writeback *wb)
{
	if (wb != &wb->bdi->wb)
		percpu_ref_get(&wb->refcnt);
}


static inline void wb_put(struct bdi_writeback *wb)
{
	if (WARN_ON_ONCE(!wb->bdi)) {
		
		return;
	}

	if (wb != &wb->bdi->wb)
		percpu_ref_put(&wb->refcnt);
}


static inline bool wb_dying(struct bdi_writeback *wb)
{
	return percpu_ref_is_dying(&wb->refcnt);
}

#else	

static inline bool wb_tryget(struct bdi_writeback *wb)
{
	return true;
}

static inline void wb_get(struct bdi_writeback *wb)
{
}

static inline void wb_put(struct bdi_writeback *wb)
{
}

static inline bool wb_dying(struct bdi_writeback *wb)
{
	return false;
}

#endif	

#endif	
