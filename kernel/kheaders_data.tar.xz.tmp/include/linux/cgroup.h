#ifndef _LINUX_CGROUP_H
#define _LINUX_CGROUP_H


#include <linux/sched.h>
#include <linux/cpumask.h>
#include <linux/nodemask.h>
#include <linux/rculist.h>
#include <linux/cgroupstats.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/kernfs.h>
#include <linux/jump_label.h>
#include <linux/types.h>
#include <linux/ns_common.h>
#include <linux/nsproxy.h>
#include <linux/user_namespace.h>
#include <linux/refcount.h>

#include <linux/cgroup-defs.h>

#ifdef CONFIG_CGROUPS


#define CGROUP_WEIGHT_MIN		1
#define CGROUP_WEIGHT_DFL		100
#define CGROUP_WEIGHT_MAX		10000


#define CSS_TASK_ITER_PROCS		(1U << 0)

#define CSS_TASK_ITER_THREADED		(1U << 1)


#define CSS_TASK_ITER_SKIPPED		(1U << 16)


struct css_task_iter {
	struct cgroup_subsys		*ss;
	unsigned int			flags;

	struct list_head		*cset_pos;
	struct list_head		*cset_head;

	struct list_head		*tcset_pos;
	struct list_head		*tcset_head;

	struct list_head		*task_pos;
	struct list_head		*tasks_head;
	struct list_head		*mg_tasks_head;
	struct list_head		*dying_tasks_head;

	struct list_head		*cur_tasks_head;
	struct css_set			*cur_cset;
	struct css_set			*cur_dcset;
	struct task_struct		*cur_task;
	struct list_head		iters_node;	
};

extern struct cgroup_root cgrp_dfl_root;
extern struct css_set init_css_set;

#define SUBSYS(_x) extern struct cgroup_subsys _x ## _cgrp_subsys;
#include <linux/cgroup_subsys.h>
#undef SUBSYS

#define SUBSYS(_x)								\
	extern struct static_key_true _x ## _cgrp_subsys_enabled_key;		\
	extern struct static_key_true _x ## _cgrp_subsys_on_dfl_key;
#include <linux/cgroup_subsys.h>
#undef SUBSYS


#define cgroup_subsys_enabled(ss)						\
	static_branch_likely(&ss ## _enabled_key)


#define cgroup_subsys_on_dfl(ss)						\
	static_branch_likely(&ss ## _on_dfl_key)

bool css_has_online_children(struct cgroup_subsys_state *css);
struct cgroup_subsys_state *css_from_id(int id, struct cgroup_subsys *ss);
struct cgroup_subsys_state *cgroup_get_e_css(struct cgroup *cgroup,
					     struct cgroup_subsys *ss);
struct cgroup_subsys_state *css_tryget_online_from_dir(struct dentry *dentry,
						       struct cgroup_subsys *ss);

struct cgroup *cgroup_get_from_path(const char *path);
struct cgroup *cgroup_get_from_fd(int fd);

int cgroup_attach_task_all(struct task_struct *from, struct task_struct *);
int cgroup_transfer_tasks(struct cgroup *to, struct cgroup *from);

int cgroup_add_dfl_cftypes(struct cgroup_subsys *ss, struct cftype *cfts);
int cgroup_add_legacy_cftypes(struct cgroup_subsys *ss, struct cftype *cfts);
int cgroup_rm_cftypes(struct cftype *cfts);
void cgroup_file_notify(struct cgroup_file *cfile);

int task_cgroup_path(struct task_struct *task, char *buf, size_t buflen);
int cgroupstats_build(struct cgroupstats *stats, struct dentry *dentry);
int proc_cgroup_show(struct seq_file *m, struct pid_namespace *ns,
		     struct pid *pid, struct task_struct *tsk);

void cgroup_fork(struct task_struct *p);
extern int cgroup_can_fork(struct task_struct *p);
extern void cgroup_cancel_fork(struct task_struct *p);
extern void cgroup_post_fork(struct task_struct *p);
void cgroup_exit(struct task_struct *p);
void cgroup_release(struct task_struct *p);
void cgroup_free(struct task_struct *p);

int cgroup_init_early(void);
int cgroup_init(void);



struct cgroup_subsys_state *css_next_child(struct cgroup_subsys_state *pos,
					   struct cgroup_subsys_state *parent);
struct cgroup_subsys_state *css_next_descendant_pre(struct cgroup_subsys_state *pos,
						    struct cgroup_subsys_state *css);
struct cgroup_subsys_state *css_rightmost_descendant(struct cgroup_subsys_state *pos);
struct cgroup_subsys_state *css_next_descendant_post(struct cgroup_subsys_state *pos,
						     struct cgroup_subsys_state *css);

struct task_struct *cgroup_taskset_first(struct cgroup_taskset *tset,
					 struct cgroup_subsys_state **dst_cssp);
struct task_struct *cgroup_taskset_next(struct cgroup_taskset *tset,
					struct cgroup_subsys_state **dst_cssp);

void css_task_iter_start(struct cgroup_subsys_state *css, unsigned int flags,
			 struct css_task_iter *it);
struct task_struct *css_task_iter_next(struct css_task_iter *it);
void css_task_iter_end(struct css_task_iter *it);


#define css_for_each_child(pos, parent)					\
	for ((pos) = css_next_child(NULL, (parent)); (pos);		\
	     (pos) = css_next_child((pos), (parent)))


#define css_for_each_descendant_pre(pos, css)				\
	for ((pos) = css_next_descendant_pre(NULL, (css)); (pos);	\
	     (pos) = css_next_descendant_pre((pos), (css)))


#define css_for_each_descendant_post(pos, css)				\
	for ((pos) = css_next_descendant_post(NULL, (css)); (pos);	\
	     (pos) = css_next_descendant_post((pos), (css)))


#define cgroup_taskset_for_each(task, dst_css, tset)			\
	for ((task) = cgroup_taskset_first((tset), &(dst_css));		\
	     (task);							\
	     (task) = cgroup_taskset_next((tset), &(dst_css)))


#define cgroup_taskset_for_each_leader(leader, dst_css, tset)		\
	for ((leader) = cgroup_taskset_first((tset), &(dst_css));	\
	     (leader);							\
	     (leader) = cgroup_taskset_next((tset), &(dst_css)))	\
		if ((leader) != (leader)->group_leader)			\
			;						\
		else




static inline void css_get(struct cgroup_subsys_state *css)
{
	if (!(css->flags & CSS_NO_REF))
		percpu_ref_get(&css->refcnt);
}


static inline void css_get_many(struct cgroup_subsys_state *css, unsigned int n)
{
	if (!(css->flags & CSS_NO_REF))
		percpu_ref_get_many(&css->refcnt, n);
}


static inline bool css_tryget(struct cgroup_subsys_state *css)
{
	if (!(css->flags & CSS_NO_REF))
		return percpu_ref_tryget(&css->refcnt);
	return true;
}


static inline bool css_tryget_online(struct cgroup_subsys_state *css)
{
	if (!(css->flags & CSS_NO_REF))
		return percpu_ref_tryget_live(&css->refcnt);
	return true;
}


static inline bool css_is_dying(struct cgroup_subsys_state *css)
{
	return !(css->flags & CSS_NO_REF) && percpu_ref_is_dying(&css->refcnt);
}


static inline void css_put(struct cgroup_subsys_state *css)
{
	if (!(css->flags & CSS_NO_REF))
		percpu_ref_put(&css->refcnt);
}


static inline void css_put_many(struct cgroup_subsys_state *css, unsigned int n)
{
	if (!(css->flags & CSS_NO_REF))
		percpu_ref_put_many(&css->refcnt, n);
}

static inline void cgroup_get(struct cgroup *cgrp)
{
	css_get(&cgrp->self);
}

static inline bool cgroup_tryget(struct cgroup *cgrp)
{
	return css_tryget(&cgrp->self);
}

static inline void cgroup_put(struct cgroup *cgrp)
{
	css_put(&cgrp->self);
}


#ifdef CONFIG_PROVE_RCU
extern struct mutex cgroup_mutex;
extern spinlock_t css_set_lock;
#define task_css_set_check(task, __c)					\
	rcu_dereference_check((task)->cgroups,				\
		lockdep_is_held(&cgroup_mutex) ||			\
		lockdep_is_held(&css_set_lock) ||			\
		((task)->flags & PF_EXITING) || (__c))
#else
#define task_css_set_check(task, __c)					\
	rcu_dereference((task)->cgroups)
#endif


#define task_css_check(task, subsys_id, __c)				\
	task_css_set_check((task), (__c))->subsys[(subsys_id)]


static inline struct css_set *task_css_set(struct task_struct *task)
{
	return task_css_set_check(task, false);
}


static inline struct cgroup_subsys_state *task_css(struct task_struct *task,
						   int subsys_id)
{
	return task_css_check(task, subsys_id, false);
}


static inline struct cgroup_subsys_state *
task_get_css(struct task_struct *task, int subsys_id)
{
	struct cgroup_subsys_state *css;

	rcu_read_lock();
	while (true) {
		css = task_css(task, subsys_id);
		
		if (likely(css_tryget(css)))
			break;
		cpu_relax();
	}
	rcu_read_unlock();
	return css;
}


static inline bool task_css_is_root(struct task_struct *task, int subsys_id)
{
	return task_css_check(task, subsys_id, true) ==
		init_css_set.subsys[subsys_id];
}

static inline struct cgroup *task_cgroup(struct task_struct *task,
					 int subsys_id)
{
	return task_css(task, subsys_id)->cgroup;
}

static inline struct cgroup *task_dfl_cgroup(struct task_struct *task)
{
	return task_css_set(task)->dfl_cgrp;
}

static inline struct cgroup *cgroup_parent(struct cgroup *cgrp)
{
	struct cgroup_subsys_state *parent_css = cgrp->self.parent;

	if (parent_css)
		return container_of(parent_css, struct cgroup, self);
	return NULL;
}


static inline bool cgroup_is_descendant(struct cgroup *cgrp,
					struct cgroup *ancestor)
{
	if (cgrp->root != ancestor->root || cgrp->level < ancestor->level)
		return false;
	return cgrp->ancestor_ids[ancestor->level] == ancestor->id;
}


static inline struct cgroup *cgroup_ancestor(struct cgroup *cgrp,
					     int ancestor_level)
{
	if (cgrp->level < ancestor_level)
		return NULL;
	while (cgrp && cgrp->level > ancestor_level)
		cgrp = cgroup_parent(cgrp);
	return cgrp;
}


static inline bool task_under_cgroup_hierarchy(struct task_struct *task,
					       struct cgroup *ancestor)
{
	struct css_set *cset = task_css_set(task);

	return cgroup_is_descendant(cset->dfl_cgrp, ancestor);
}


static inline bool cgroup_is_populated(struct cgroup *cgrp)
{
	return cgrp->nr_populated_csets + cgrp->nr_populated_domain_children +
		cgrp->nr_populated_threaded_children;
}


static inline ino_t cgroup_ino(struct cgroup *cgrp)
{
	return cgrp->kn->id.ino;
}


static inline struct cftype *of_cft(struct kernfs_open_file *of)
{
	return of->kn->priv;
}

struct cgroup_subsys_state *of_css(struct kernfs_open_file *of);


static inline struct cftype *seq_cft(struct seq_file *seq)
{
	return of_cft(seq->private);
}

static inline struct cgroup_subsys_state *seq_css(struct seq_file *seq)
{
	return of_css(seq->private);
}



static inline int cgroup_name(struct cgroup *cgrp, char *buf, size_t buflen)
{
	return kernfs_name(cgrp->kn, buf, buflen);
}

static inline int cgroup_path(struct cgroup *cgrp, char *buf, size_t buflen)
{
	return kernfs_path(cgrp->kn, buf, buflen);
}

static inline void pr_cont_cgroup_name(struct cgroup *cgrp)
{
	pr_cont_kernfs_name(cgrp->kn);
}

static inline void pr_cont_cgroup_path(struct cgroup *cgrp)
{
	pr_cont_kernfs_path(cgrp->kn);
}

static inline struct psi_group *cgroup_psi(struct cgroup *cgrp)
{
	return &cgrp->psi;
}

static inline void cgroup_init_kthreadd(void)
{
	
	current->no_cgroup_migration = 1;
}

static inline void cgroup_kthread_ready(void)
{
	
	current->no_cgroup_migration = 0;
}

#else 

struct cgroup_subsys_state;
struct cgroup;

static inline void css_put(struct cgroup_subsys_state *css) {}
static inline int cgroup_attach_task_all(struct task_struct *from,
					 struct task_struct *t) { return 0; }
static inline int cgroupstats_build(struct cgroupstats *stats,
				    struct dentry *dentry) { return -EINVAL; }

static inline void cgroup_fork(struct task_struct *p) {}
static inline int cgroup_can_fork(struct task_struct *p) { return 0; }
static inline void cgroup_cancel_fork(struct task_struct *p) {}
static inline void cgroup_post_fork(struct task_struct *p) {}
static inline void cgroup_exit(struct task_struct *p) {}
static inline void cgroup_release(struct task_struct *p) {}
static inline void cgroup_free(struct task_struct *p) {}

static inline int cgroup_init_early(void) { return 0; }
static inline int cgroup_init(void) { return 0; }
static inline void cgroup_init_kthreadd(void) {}
static inline void cgroup_kthread_ready(void) {}

static inline struct cgroup *cgroup_parent(struct cgroup *cgrp)
{
	return NULL;
}

static inline struct psi_group *cgroup_psi(struct cgroup *cgrp)
{
	return NULL;
}

static inline bool task_under_cgroup_hierarchy(struct task_struct *task,
					       struct cgroup *ancestor)
{
	return true;
}
#endif 


#ifdef CONFIG_SOCK_CGROUP_DATA

#if defined(CONFIG_CGROUP_NET_PRIO) || defined(CONFIG_CGROUP_NET_CLASSID)
extern spinlock_t cgroup_sk_update_lock;
#endif

void cgroup_sk_alloc_disable(void);
void cgroup_sk_alloc(struct sock_cgroup_data *skcd);
void cgroup_sk_clone(struct sock_cgroup_data *skcd);
void cgroup_sk_free(struct sock_cgroup_data *skcd);

static inline struct cgroup *sock_cgroup_ptr(struct sock_cgroup_data *skcd)
{
#if defined(CONFIG_CGROUP_NET_PRIO) || defined(CONFIG_CGROUP_NET_CLASSID)
	unsigned long v;

	
	v = READ_ONCE(skcd->val);

	if (v & 3)
		return &cgrp_dfl_root.cgrp;

	return (struct cgroup *)(unsigned long)v ?: &cgrp_dfl_root.cgrp;
#else
	return (struct cgroup *)(unsigned long)skcd->val;
#endif
}

#else	

static inline void cgroup_sk_alloc(struct sock_cgroup_data *skcd) {}
static inline void cgroup_sk_clone(struct sock_cgroup_data *skcd) {}
static inline void cgroup_sk_free(struct sock_cgroup_data *skcd) {}

#endif	

struct cgroup_namespace {
	refcount_t		count;
	struct ns_common	ns;
	struct user_namespace	*user_ns;
	struct ucounts		*ucounts;
	struct css_set          *root_cset;
};

extern struct cgroup_namespace init_cgroup_ns;

#ifdef CONFIG_CGROUPS

void free_cgroup_ns(struct cgroup_namespace *ns);

struct cgroup_namespace *copy_cgroup_ns(unsigned long flags,
					struct user_namespace *user_ns,
					struct cgroup_namespace *old_ns);

int cgroup_path_ns(struct cgroup *cgrp, char *buf, size_t buflen,
		   struct cgroup_namespace *ns);

#else 

static inline void free_cgroup_ns(struct cgroup_namespace *ns) { }
static inline struct cgroup_namespace *
copy_cgroup_ns(unsigned long flags, struct user_namespace *user_ns,
	       struct cgroup_namespace *old_ns)
{
	return old_ns;
}

#endif 

static inline void get_cgroup_ns(struct cgroup_namespace *ns)
{
	if (ns)
		refcount_inc(&ns->count);
}

static inline void put_cgroup_ns(struct cgroup_namespace *ns)
{
	if (ns && refcount_dec_and_test(&ns->count))
		free_cgroup_ns(ns);
}

#ifdef CONFIG_CGROUPS

void cgroup_enter_frozen(void);
void cgroup_leave_frozen(bool always_leave);
void cgroup_update_frozen(struct cgroup *cgrp);
void cgroup_freeze(struct cgroup *cgrp, bool freeze);
void cgroup_freezer_migrate_task(struct task_struct *task, struct cgroup *src,
				 struct cgroup *dst);
void cgroup_freezer_frozen_exit(struct task_struct *task);
static inline bool cgroup_task_freeze(struct task_struct *task)
{
	bool ret;

	if (task->flags & PF_KTHREAD)
		return false;

	rcu_read_lock();
	ret = test_bit(CGRP_FREEZE, &task_dfl_cgroup(task)->flags);
	rcu_read_unlock();

	return ret;
}

static inline bool cgroup_task_frozen(struct task_struct *task)
{
	return task->frozen;
}

#else 

static inline void cgroup_enter_frozen(void) { }
static inline void cgroup_leave_frozen(bool always_leave) { }
static inline bool cgroup_task_freeze(struct task_struct *task)
{
	return false;
}
static inline bool cgroup_task_frozen(struct task_struct *task)
{
	return false;
}

#endif 

#endif 
