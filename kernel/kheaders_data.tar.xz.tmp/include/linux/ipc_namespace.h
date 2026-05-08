#ifndef __IPC_NAMESPACE_H__
#define __IPC_NAMESPACE_H__

#include <linux/err.h>
#include <linux/idr.h>
#include <linux/rwsem.h>
#include <linux/notifier.h>
#include <linux/nsproxy.h>
#include <linux/ns_common.h>

struct user_namespace;

struct ipc_ids {
	int in_use;
	unsigned short seq;
	struct rw_semaphore rwsem;
	struct idr ipcs_idr;
	int next_id;
};

struct ipc_namespace {
	atomic_t	count;
	struct ipc_ids	ids[3];

	int		sem_ctls[4];
	int		used_sems;

	unsigned int	msg_ctlmax;
	unsigned int	msg_ctlmnb;
	unsigned int	msg_ctlmni;
	atomic_t	msg_bytes;
	atomic_t	msg_hdrs;

	size_t		shm_ctlmax;
	size_t		shm_ctlall;
	unsigned long	shm_tot;
	int		shm_ctlmni;
	
	int		shm_rmid_forced;

	struct notifier_block ipcns_nb;

	
	struct vfsmount	*mq_mnt;

	
	unsigned int    mq_queues_count;

	
	unsigned int    mq_queues_max;   
	unsigned int    mq_msg_max;      
	unsigned int    mq_msgsize_max;  
	unsigned int    mq_msg_default;
	unsigned int    mq_msgsize_default;

	
	struct user_namespace *user_ns;
	struct ucounts *ucounts;

	struct ns_common ns;
};

extern struct ipc_namespace init_ipc_ns;
extern spinlock_t mq_lock;

#ifdef CONFIG_SYSVIPC
extern void shm_destroy_orphaned(struct ipc_namespace *ns);
#else 
static inline void shm_destroy_orphaned(struct ipc_namespace *ns) {}
#endif 

#ifdef CONFIG_POSIX_MQUEUE
extern int mq_init_ns(struct ipc_namespace *ns);

#define DFLT_QUEUESMAX		      256
#define MIN_MSGMAX			1
#define DFLT_MSG		       10U
#define DFLT_MSGMAX		       10
#define HARD_MSGMAX		    65536
#define MIN_MSGSIZEMAX		      128
#define DFLT_MSGSIZE		     8192U
#define DFLT_MSGSIZEMAX		     8192
#define HARD_MSGSIZEMAX	    (16*1024*1024)
#else
static inline int mq_init_ns(struct ipc_namespace *ns) { return 0; }
#endif

#if defined(CONFIG_IPC_NS)
extern struct ipc_namespace *copy_ipcs(unsigned long flags,
	struct user_namespace *user_ns, struct ipc_namespace *ns);

static inline struct ipc_namespace *get_ipc_ns(struct ipc_namespace *ns)
{
	if (ns)
		atomic_inc(&ns->count);
	return ns;
}

static inline struct ipc_namespace *get_ipc_ns_not_zero(struct ipc_namespace *ns)
{
	if (ns) {
		if (atomic_inc_not_zero(&ns->count))
			return ns;
	}

	return NULL;
}

extern void put_ipc_ns(struct ipc_namespace *ns);
#else
static inline struct ipc_namespace *copy_ipcs(unsigned long flags,
	struct user_namespace *user_ns, struct ipc_namespace *ns)
{
	if (flags & CLONE_NEWIPC)
		return ERR_PTR(-EINVAL);

	return ns;
}

static inline struct ipc_namespace *get_ipc_ns(struct ipc_namespace *ns)
{
	return ns;
}

static inline struct ipc_namespace *get_ipc_ns_not_zero(struct ipc_namespace *ns)
{
	return ns;
}

static inline void put_ipc_ns(struct ipc_namespace *ns)
{
}
#endif

#ifdef CONFIG_POSIX_MQUEUE_SYSCTL

struct ctl_table_header;
extern struct ctl_table_header *mq_register_sysctl_table(void);

#else 

static inline struct ctl_table_header *mq_register_sysctl_table(void)
{
	return NULL;
}

#endif 
#endif
