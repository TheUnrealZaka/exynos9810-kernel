

#ifndef _LINUX_SUNRPC_METRICS_H
#define _LINUX_SUNRPC_METRICS_H

#include <linux/seq_file.h>
#include <linux/ktime.h>
#include <linux/spinlock.h>

#define RPC_IOSTATS_VERS	"1.0"

struct rpc_iostats {
	spinlock_t		om_lock;

	
	unsigned long		om_ops,		
				om_ntrans,	
				om_timeouts;	

	
	unsigned long long      om_bytes_sent,	
				om_bytes_recv;	

	
	ktime_t			om_queue,	
				om_rtt,		
				om_execute;	
} ____cacheline_aligned;

struct rpc_task;
struct rpc_clnt;



#ifdef CONFIG_PROC_FS

struct rpc_iostats *	rpc_alloc_iostats(struct rpc_clnt *);
void			rpc_count_iostats(const struct rpc_task *,
					  struct rpc_iostats *);
void			rpc_count_iostats_metrics(const struct rpc_task *,
					  struct rpc_iostats *);
void			rpc_print_iostats(struct seq_file *, struct rpc_clnt *);
void			rpc_free_iostats(struct rpc_iostats *);

#else  

static inline struct rpc_iostats *rpc_alloc_iostats(struct rpc_clnt *clnt) { return NULL; }
static inline void rpc_count_iostats(const struct rpc_task *task,
				     struct rpc_iostats *stats) {}
static inline void rpc_count_iostats_metrics(const struct rpc_task *task,
					     struct rpc_iostats *stats)
{
}

static inline void rpc_print_iostats(struct seq_file *seq, struct rpc_clnt *clnt) {}
static inline void rpc_free_iostats(struct rpc_iostats *stats) {}

#endif  

#endif 
