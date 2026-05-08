
#ifndef WRITEBACK_H
#define WRITEBACK_H

#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/fs.h>
#include <linux/flex_proportions.h>
#include <linux/backing-dev-defs.h>

DECLARE_PER_CPU(int, dirty_throttle_leaks);


#define DIRTY_SCOPE		8
#define DIRTY_FULL_SCOPE	(DIRTY_SCOPE / 2)

struct backing_dev_info;


enum writeback_sync_modes {
	WB_SYNC_NONE,	
	WB_SYNC_ALL,	
};


enum wb_reason {
	WB_REASON_BACKGROUND,
	WB_REASON_TRY_TO_FREE_PAGES,
	WB_REASON_SYNC,
	WB_REASON_PERIODIC,
	WB_REASON_LAPTOP_TIMER,
	WB_REASON_FREE_MORE_MEM,
	WB_REASON_FS_FREE_SPACE,
	
	WB_REASON_FORKER_THREAD,

	WB_REASON_MAX,
};


struct writeback_control {
	long nr_to_write;		
	long pages_skipped;		

	
	loff_t range_start;
	loff_t range_end;

	enum writeback_sync_modes sync_mode;

	unsigned for_kupdate:1;		
	unsigned for_background:1;	
	unsigned tagged_writepages:1;	
	unsigned for_reclaim:1;		
	unsigned range_cyclic:1;	
	unsigned for_sync:1;		
#ifdef CONFIG_CGROUP_WRITEBACK
	struct bdi_writeback *wb;	
	struct inode *inode;		

	
	int wb_id;			
	int wb_lcand_id;		
	int wb_tcand_id;		
	size_t wb_bytes;		
	size_t wb_lcand_bytes;		
	size_t wb_tcand_bytes;		
#endif
};


struct wb_domain {
	spinlock_t lock;

	
	struct fprop_global completions;
	struct timer_list period_timer;	
	unsigned long period_time;

	
	unsigned long dirty_limit_tstamp;
	unsigned long dirty_limit;
};


static inline void wb_domain_size_changed(struct wb_domain *dom)
{
	spin_lock(&dom->lock);
	dom->dirty_limit_tstamp = jiffies;
	dom->dirty_limit = 0;
	spin_unlock(&dom->lock);
}

	
struct bdi_writeback;
void writeback_inodes_sb(struct super_block *, enum wb_reason reason);
void writeback_inodes_sb_nr(struct super_block *, unsigned long nr,
							enum wb_reason reason);
bool try_to_writeback_inodes_sb(struct super_block *, enum wb_reason reason);
bool try_to_writeback_inodes_sb_nr(struct super_block *, unsigned long nr,
				   enum wb_reason reason);
void sync_inodes_sb(struct super_block *);
void wakeup_flusher_threads(long nr_pages, enum wb_reason reason);
void inode_wait_for_writeback(struct inode *inode);


static inline void wait_on_inode(struct inode *inode)
{
	might_sleep();
	wait_on_bit(&inode->i_state, __I_NEW, TASK_UNINTERRUPTIBLE);
}

#ifdef CONFIG_CGROUP_WRITEBACK

#include <linux/cgroup.h>
#include <linux/bio.h>

void __inode_attach_wb(struct inode *inode, struct page *page);
void wbc_attach_and_unlock_inode(struct writeback_control *wbc,
				 struct inode *inode)
	__releases(&inode->i_lock);
void wbc_detach_inode(struct writeback_control *wbc);
void wbc_account_io(struct writeback_control *wbc, struct page *page,
		    size_t bytes);
void cgroup_writeback_umount(void);


static inline void inode_attach_wb(struct inode *inode, struct page *page)
{
	if (!inode->i_wb)
		__inode_attach_wb(inode, page);
}


static inline void inode_detach_wb(struct inode *inode)
{
	if (inode->i_wb) {
		wb_put(inode->i_wb);
		inode->i_wb = NULL;
	}
}


static inline void wbc_attach_fdatawrite_inode(struct writeback_control *wbc,
					       struct inode *inode)
{
	spin_lock(&inode->i_lock);
	inode_attach_wb(inode, NULL);
	wbc_attach_and_unlock_inode(wbc, inode);
}


static inline void wbc_init_bio(struct writeback_control *wbc, struct bio *bio)
{
	
	if (wbc->wb)
		bio_associate_blkcg(bio, wbc->wb->blkcg_css);
}

#else	

static inline void inode_attach_wb(struct inode *inode, struct page *page)
{
}

static inline void inode_detach_wb(struct inode *inode)
{
}

static inline void wbc_attach_and_unlock_inode(struct writeback_control *wbc,
					       struct inode *inode)
	__releases(&inode->i_lock)
{
	spin_unlock(&inode->i_lock);
}

static inline void wbc_attach_fdatawrite_inode(struct writeback_control *wbc,
					       struct inode *inode)
{
}

static inline void wbc_detach_inode(struct writeback_control *wbc)
{
}

static inline void wbc_init_bio(struct writeback_control *wbc, struct bio *bio)
{
}

static inline void wbc_account_io(struct writeback_control *wbc,
				  struct page *page, size_t bytes)
{
}

static inline void cgroup_writeback_umount(void)
{
}

#endif	


#ifdef CONFIG_BLOCK
void laptop_io_completion(struct backing_dev_info *info);
void laptop_sync_completion(void);
void laptop_mode_sync(struct work_struct *work);
void laptop_mode_timer_fn(unsigned long data);
#else
static inline void laptop_sync_completion(void) { }
#endif
bool node_dirty_ok(struct pglist_data *pgdat);
int wb_domain_init(struct wb_domain *dom, gfp_t gfp);
#ifdef CONFIG_CGROUP_WRITEBACK
void wb_domain_exit(struct wb_domain *dom);
#endif

extern struct wb_domain global_wb_domain;


extern int dirty_background_ratio;
extern unsigned long dirty_background_bytes;
extern int vm_dirty_ratio;
extern unsigned long vm_dirty_bytes;
extern unsigned int dirty_writeback_interval;
extern unsigned int dirty_expire_interval;
extern unsigned int dirtytime_expire_interval;
extern int vm_highmem_is_dirtyable;
extern int block_dump;
extern int laptop_mode;

extern int dirty_background_ratio_handler(struct ctl_table *table, int write,
		void __user *buffer, size_t *lenp,
		loff_t *ppos);
extern int dirty_background_bytes_handler(struct ctl_table *table, int write,
		void __user *buffer, size_t *lenp,
		loff_t *ppos);
extern int dirty_ratio_handler(struct ctl_table *table, int write,
		void __user *buffer, size_t *lenp,
		loff_t *ppos);
extern int dirty_bytes_handler(struct ctl_table *table, int write,
		void __user *buffer, size_t *lenp,
		loff_t *ppos);
int dirtytime_interval_handler(struct ctl_table *table, int write,
			       void __user *buffer, size_t *lenp, loff_t *ppos);

struct ctl_table;
int dirty_writeback_centisecs_handler(struct ctl_table *, int,
				      void __user *, size_t *, loff_t *);

void global_dirty_limits(unsigned long *pbackground, unsigned long *pdirty);
unsigned long wb_calc_thresh(struct bdi_writeback *wb, unsigned long thresh);

void wb_update_bandwidth(struct bdi_writeback *wb, unsigned long start_time);
void balance_dirty_pages_ratelimited(struct address_space *mapping);
bool wb_over_bg_thresh(struct bdi_writeback *wb);

typedef int (*writepage_t)(struct page *page, struct writeback_control *wbc,
				void *data);

int generic_writepages(struct address_space *mapping,
		       struct writeback_control *wbc);
void tag_pages_for_writeback(struct address_space *mapping,
			     pgoff_t start, pgoff_t end);
int write_cache_pages(struct address_space *mapping,
		      struct writeback_control *wbc, writepage_t writepage,
		      void *data);
int do_writepages(struct address_space *mapping, struct writeback_control *wbc);
void writeback_set_ratelimit(void);
void tag_pages_for_writeback(struct address_space *mapping,
			     pgoff_t start, pgoff_t end);

void account_page_redirty(struct page *page);

void sb_mark_inode_writeback(struct inode *inode);
void sb_clear_inode_writeback(struct inode *inode);

#endif		
