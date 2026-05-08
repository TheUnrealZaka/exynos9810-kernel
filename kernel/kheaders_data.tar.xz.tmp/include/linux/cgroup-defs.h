
#ifndef _LINUX_CGROUP_DEFS_H
#define _LINUX_CGROUP_DEFS_H

#include <linux/limits.h>
#include <linux/list.h>
#include <linux/idr.h>
#include <linux/wait.h>
#include <linux/mutex.h>
#include <linux/rcupdate.h>
#include <linux/refcount.h>
#include <linux/percpu-refcount.h>
#include <linux/percpu-rwsem.h>
#include <linux/workqueue.h>
#include <linux/bpf-cgroup.h>
#include <linux/psi_types.h>

#ifdef CONFIG_CGROUPS

struct cgroup;
struct cgroup_root;
struct cgroup_subsys;
struct cgroup_taskset;
struct kernfs_node;
struct kernfs_ops;
struct kernfs_open_file;
struct seq_file;
struct poll_table_struct;

#define MAX_CGROUP_TYPE_NAMELEN 32
#define MAX_CGROUP_ROOT_NAMELEN 64
#define MAX_CFTYPE_NAME		64


#define SUBSYS(_x) _x ## _cgrp_id,
enum cgroup_subsys_id {
#include <linux/cgroup_subsys.h>
	CGROUP_SUBSYS_COUNT,
};
#undef SUBSYS


enum {
	CSS_NO_REF	= (1 << 0), 
	CSS_ONLINE	= (1 << 1), 
	CSS_RELEASED	= (1 << 2), 
	CSS_VISIBLE	= (1 << 3), 
	CSS_DYING	= (1 << 4), 
};


enum {
	
	CGRP_NOTIFY_ON_RELEASE,
	
	CGRP_CPUSET_CLONE_CHILDREN,

	
	CGRP_FREEZE,

	
	CGRP_FROZEN,
};


enum {
	CGRP_ROOT_NOPREFIX	= (1 << 1), 
	CGRP_ROOT_XATTR		= (1 << 2), 

	
	CGRP_ROOT_NS_DELEGATE	= (1 << 3),

	
	CGRP_ROOT_CPUSET_V2_MODE = (1 << 4),
};


enum {
	CFTYPE_ONLY_ON_ROOT	= (1 << 0),	
	CFTYPE_NOT_ON_ROOT	= (1 << 1),	
	CFTYPE_NS_DELEGATABLE	= (1 << 2),	

	CFTYPE_NO_PREFIX	= (1 << 3),	
	CFTYPE_WORLD_WRITABLE	= (1 << 4),	

	
	__CFTYPE_ONLY_ON_DFL	= (1 << 16),	
	__CFTYPE_NOT_ON_DFL	= (1 << 17),	
};


struct cgroup_file {
	
	struct kernfs_node *kn;
};


struct cgroup_subsys_state {
	
	struct cgroup *cgroup;

	
	struct cgroup_subsys *ss;

	
	struct percpu_ref refcnt;

	
	struct list_head sibling;
	struct list_head children;

	
	int id;

	unsigned int flags;

	
	u64 serial_nr;

	
	atomic_t online_cnt;

	
	struct rcu_head rcu_head;
	struct work_struct destroy_work;

	
	struct cgroup_subsys_state *parent;
};


struct css_set {
	
	struct cgroup_subsys_state *subsys[CGROUP_SUBSYS_COUNT];

	
	refcount_t refcount;

	
	struct css_set *dom_cset;

	
	struct cgroup *dfl_cgrp;

	
	int nr_tasks;

	
	struct list_head tasks;
	struct list_head mg_tasks;
	struct list_head dying_tasks;

	
	struct list_head task_iters;

	
	struct list_head e_cset_node[CGROUP_SUBSYS_COUNT];

	
	struct list_head threaded_csets;
	struct list_head threaded_csets_node;

	
	struct hlist_node hlist;

	
	struct list_head cgrp_links;

	
	struct list_head mg_src_preload_node;
	struct list_head mg_dst_preload_node;
	struct list_head mg_node;

	
	struct cgroup *mg_src_cgrp;
	struct cgroup *mg_dst_cgrp;
	struct css_set *mg_dst_cset;

	
	bool dead;

	
	struct rcu_head rcu_head;
};

struct cgroup_freezer_state {
	
	bool freeze;

	
	int e_freeze;

	

	
	int nr_frozen_descendants;

	
	int nr_frozen_tasks;
};

struct cgroup {
	
	struct cgroup_subsys_state self;

	unsigned long flags;		

	
	int id;

	
	int level;

	
	int max_depth;

	
	int nr_descendants;
	int nr_dying_descendants;
	int max_descendants;

	
	int nr_populated_csets;
	int nr_populated_domain_children;
	int nr_populated_threaded_children;

	int nr_threaded_children;	

	struct kernfs_node *kn;		
	struct cgroup_file procs_file;	
	struct cgroup_file events_file;	

	
	u16 subtree_control;
	u16 subtree_ss_mask;
	u16 old_subtree_control;
	u16 old_subtree_ss_mask;

	
	struct cgroup_subsys_state __rcu *subsys[CGROUP_SUBSYS_COUNT];

	struct cgroup_root *root;

	
	struct list_head cset_links;

	
	struct list_head e_csets[CGROUP_SUBSYS_COUNT];

	
	struct cgroup *dom_cgrp;
	struct cgroup *old_dom_cgrp;		

	
	struct list_head pidlists;
	struct mutex pidlist_mutex;

	
	wait_queue_head_t offline_waitq;

	
	struct work_struct release_agent_work;

	
	struct psi_group psi;

	
	struct cgroup_bpf bpf;

	
	struct cgroup_freezer_state freezer;

	
	int ancestor_ids[];
};


struct cgroup_root {
	struct kernfs_root *kf_root;

	
	unsigned int subsys_mask;

	
	int hierarchy_id;

	
	struct cgroup cgrp;

	
	int cgrp_ancestor_id_storage;

	
	atomic_t nr_cgrps;

	
	struct list_head root_list;

	
	unsigned int flags;

	
	struct idr cgroup_idr;

	
	char release_agent_path[PATH_MAX];

	
	char name[MAX_CGROUP_ROOT_NAMELEN];
};


struct cftype {
	
	char name[MAX_CFTYPE_NAME];
	unsigned long private;

	
	size_t max_write_len;

	
	unsigned int flags;

	
	unsigned int file_offset;

	
	struct cgroup_subsys *ss;	
	struct list_head node;		
	struct kernfs_ops *kf_ops;

	int (*open)(struct kernfs_open_file *of);
	void (*release)(struct kernfs_open_file *of);

	
	u64 (*read_u64)(struct cgroup_subsys_state *css, struct cftype *cft);
	
	s64 (*read_s64)(struct cgroup_subsys_state *css, struct cftype *cft);

	
	int (*seq_show)(struct seq_file *sf, void *v);

	
	void *(*seq_start)(struct seq_file *sf, loff_t *ppos);
	void *(*seq_next)(struct seq_file *sf, void *v, loff_t *ppos);
	void (*seq_stop)(struct seq_file *sf, void *v);

	
	int (*write_u64)(struct cgroup_subsys_state *css, struct cftype *cft,
			 u64 val);
	
	int (*write_s64)(struct cgroup_subsys_state *css, struct cftype *cft,
			 s64 val);

	
	ssize_t (*write)(struct kernfs_open_file *of,
			 char *buf, size_t nbytes, loff_t off);

	unsigned int (*poll)(struct kernfs_open_file *of,
			 struct poll_table_struct *pt);

#ifdef CONFIG_DEBUG_LOCK_ALLOC
	struct lock_class_key	lockdep_key;
#endif
};


struct cgroup_subsys {
	struct cgroup_subsys_state *(*css_alloc)(struct cgroup_subsys_state *parent_css);
	int (*css_online)(struct cgroup_subsys_state *css);
	void (*css_offline)(struct cgroup_subsys_state *css);
	void (*css_released)(struct cgroup_subsys_state *css);
	void (*css_free)(struct cgroup_subsys_state *css);
	void (*css_reset)(struct cgroup_subsys_state *css);

	int (*can_attach)(struct cgroup_taskset *tset);
	void (*cancel_attach)(struct cgroup_taskset *tset);
	void (*attach)(struct cgroup_taskset *tset);
	void (*post_attach)(void);
	int (*can_fork)(struct task_struct *task);
	void (*cancel_fork)(struct task_struct *task);
	void (*fork)(struct task_struct *task);
	void (*exit)(struct task_struct *task);
	void (*release)(struct task_struct *task);
	void (*bind)(struct cgroup_subsys_state *root_css);

	bool early_init:1;

	
	bool implicit_on_dfl:1;

	
	bool threaded:1;

	
	bool broken_hierarchy:1;
	bool warned_broken_hierarchy:1;

	
	int id;
	const char *name;

	
	const char *legacy_name;

	
	struct cgroup_root *root;

	
	struct idr css_idr;

	
	struct list_head cfts;

	
	struct cftype *dfl_cftypes;	
	struct cftype *legacy_cftypes;	

	
	unsigned int depends_on;
};

extern struct percpu_rw_semaphore cgroup_threadgroup_rwsem;


static inline void cgroup_threadgroup_change_begin(struct task_struct *tsk)
{
	percpu_down_read(&cgroup_threadgroup_rwsem);
}


static inline void cgroup_threadgroup_change_end(struct task_struct *tsk)
{
	percpu_up_read(&cgroup_threadgroup_rwsem);
}

#else	

#define CGROUP_SUBSYS_COUNT 0

static inline void cgroup_threadgroup_change_begin(struct task_struct *tsk) {}
static inline void cgroup_threadgroup_change_end(struct task_struct *tsk) {}

#endif	

#ifdef CONFIG_SOCK_CGROUP_DATA


struct sock_cgroup_data {
	union {
#ifdef __LITTLE_ENDIAN
		struct {
			u8	is_data : 1;
			u8	no_refcnt : 1;
			u8	unused : 6;
			u8	padding;
			u16	prioidx;
			u32	classid;
		} __packed;
#else
		struct {
			u32	classid;
			u16	prioidx;
			u8	padding;
			u8	unused : 6;
			u8	no_refcnt : 1;
			u8	is_data : 1;
		} __packed;
#endif
		u64		val;
	};
};


static inline u16 sock_cgroup_prioidx(const struct sock_cgroup_data *skcd)
{
	
	return (skcd->is_data & 1) ? skcd->prioidx : 1;
}

static inline u32 sock_cgroup_classid(const struct sock_cgroup_data *skcd)
{
	
	return (skcd->is_data & 1) ? skcd->classid : 0;
}


static inline void sock_cgroup_set_prioidx(struct sock_cgroup_data *skcd,
					   u16 prioidx)
{
	struct sock_cgroup_data skcd_buf = {{ .val = READ_ONCE(skcd->val) }};

	if (sock_cgroup_prioidx(&skcd_buf) == prioidx)
		return;

	if (!(skcd_buf.is_data & 1)) {
		skcd_buf.val = 0;
		skcd_buf.is_data = 1;
	}

	skcd_buf.prioidx = prioidx;
	WRITE_ONCE(skcd->val, skcd_buf.val);	
}

static inline void sock_cgroup_set_classid(struct sock_cgroup_data *skcd,
					   u32 classid)
{
	struct sock_cgroup_data skcd_buf = {{ .val = READ_ONCE(skcd->val) }};

	if (sock_cgroup_classid(&skcd_buf) == classid)
		return;

	if (!(skcd_buf.is_data & 1)) {
		skcd_buf.val = 0;
		skcd_buf.is_data = 1;
	}

	skcd_buf.classid = classid;
	WRITE_ONCE(skcd->val, skcd_buf.val);	
}

#else	

struct sock_cgroup_data {
};

#endif	

#endif	
