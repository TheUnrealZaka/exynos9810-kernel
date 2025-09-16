

#ifndef _LINUX_SUNRPC_CLNT_H
#define _LINUX_SUNRPC_CLNT_H

#include <linux/types.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/in6.h>

#include <linux/sunrpc/msg_prot.h>
#include <linux/sunrpc/sched.h>
#include <linux/sunrpc/xprt.h>
#include <linux/sunrpc/auth.h>
#include <linux/sunrpc/stats.h>
#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/timer.h>
#include <linux/sunrpc/rpc_pipe_fs.h>
#include <asm/signal.h>
#include <linux/path.h>
#include <net/ipv6.h>
#include <linux/sunrpc/xprtmultipath.h>

struct rpc_inode;


struct rpc_clnt {
	atomic_t		cl_count;	
	unsigned int		cl_clid;	
	struct list_head	cl_clients;	
	struct list_head	cl_tasks;	
	spinlock_t		cl_lock;	
	struct rpc_xprt __rcu *	cl_xprt;	
	struct rpc_procinfo *	cl_procinfo;	
	u32			cl_prog,	
				cl_vers,	
				cl_maxproc;	

	struct rpc_auth *	cl_auth;	
	struct rpc_stat *	cl_stats;	
	struct rpc_iostats *	cl_metrics;	

	unsigned int		cl_softrtry : 1,
				cl_discrtry : 1,
				cl_noretranstimeo: 1,
				cl_autobind : 1,
				cl_chatty   : 1;

	struct rpc_rtt *	cl_rtt;		
	const struct rpc_timeout *cl_timeout;	

	atomic_t		cl_swapper;	
	int			cl_nodelen;	
	char 			cl_nodename[UNX_MAXNODENAME+1];
	struct rpc_pipe_dir_head cl_pipedir_objects;
	struct rpc_clnt *	cl_parent;	
	struct rpc_rtt		cl_rtt_default;
	struct rpc_timeout	cl_timeout_default;
	const struct rpc_program *cl_program;
#if IS_ENABLED(CONFIG_SUNRPC_DEBUG)
	struct dentry		*cl_debugfs;	
#endif
	struct rpc_xprt_iter	cl_xpi;
};


#define RPC_MAXVERSION		4
struct rpc_program {
	const char *		name;		
	u32			number;		
	unsigned int		nrvers;		
	const struct rpc_version **	version;	
	struct rpc_stat *	stats;		
	const char *		pipe_dir_name;	
};

struct rpc_version {
	u32			number;		
	unsigned int		nrprocs;	
	struct rpc_procinfo *	procs;		
};


struct rpc_procinfo {
	u32			p_proc;		
	kxdreproc_t		p_encode;	
	kxdrdproc_t		p_decode;	
	unsigned int		p_arglen;	
	unsigned int		p_replen;	
	unsigned int		p_count;	
	unsigned int		p_timer;	
	u32			p_statidx;	
	const char *		p_name;		
};

#ifdef __KERNEL__

struct rpc_create_args {
	struct net		*net;
	int			protocol;
	struct sockaddr		*address;
	size_t			addrsize;
	struct sockaddr		*saddress;
	const struct rpc_timeout *timeout;
	const char		*servername;
	const char		*nodename;
	const struct rpc_program *program;
	u32			prognumber;	
	u32			version;
	rpc_authflavor_t	authflavor;
	unsigned long		flags;
	char			*client_name;
	struct svc_xprt		*bc_xprt;	
};

struct rpc_add_xprt_test {
	int (*add_xprt_test)(struct rpc_clnt *,
		struct rpc_xprt *,
		void *calldata);
	void *data;
};


#define RPC_CLNT_CREATE_HARDRTRY	(1UL << 0)
#define RPC_CLNT_CREATE_AUTOBIND	(1UL << 2)
#define RPC_CLNT_CREATE_NONPRIVPORT	(1UL << 3)
#define RPC_CLNT_CREATE_NOPING		(1UL << 4)
#define RPC_CLNT_CREATE_DISCRTRY	(1UL << 5)
#define RPC_CLNT_CREATE_QUIET		(1UL << 6)
#define RPC_CLNT_CREATE_INFINITE_SLOTS	(1UL << 7)
#define RPC_CLNT_CREATE_NO_IDLE_TIMEOUT	(1UL << 8)
#define RPC_CLNT_CREATE_NO_RETRANS_TIMEOUT	(1UL << 9)

struct rpc_clnt *rpc_create(struct rpc_create_args *args);
struct rpc_clnt	*rpc_bind_new_program(struct rpc_clnt *,
				const struct rpc_program *, u32);
struct rpc_clnt *rpc_clone_client(struct rpc_clnt *);
struct rpc_clnt *rpc_clone_client_set_auth(struct rpc_clnt *,
				rpc_authflavor_t);
int		rpc_switch_client_transport(struct rpc_clnt *,
				struct xprt_create *,
				const struct rpc_timeout *);

void		rpc_shutdown_client(struct rpc_clnt *);
void		rpc_release_client(struct rpc_clnt *);
void		rpc_task_release_transport(struct rpc_task *);
void		rpc_task_release_client(struct rpc_task *);

int		rpcb_create_local(struct net *);
void		rpcb_put_local(struct net *);
int		rpcb_register(struct net *, u32, u32, int, unsigned short);
int		rpcb_v4_register(struct net *net, const u32 program,
				 const u32 version,
				 const struct sockaddr *address,
				 const char *netid);
void		rpcb_getport_async(struct rpc_task *);

void		rpc_call_start(struct rpc_task *);
int		rpc_call_async(struct rpc_clnt *clnt,
			       const struct rpc_message *msg, int flags,
			       const struct rpc_call_ops *tk_ops,
			       void *calldata);
int		rpc_call_sync(struct rpc_clnt *clnt,
			      const struct rpc_message *msg, int flags);
struct rpc_task *rpc_call_null(struct rpc_clnt *clnt, struct rpc_cred *cred,
			       int flags);
int		rpc_restart_call_prepare(struct rpc_task *);
int		rpc_restart_call(struct rpc_task *);
void		rpc_setbufsize(struct rpc_clnt *, unsigned int, unsigned int);
int		rpc_protocol(struct rpc_clnt *);
struct net *	rpc_net_ns(struct rpc_clnt *);
size_t		rpc_max_payload(struct rpc_clnt *);
size_t		rpc_max_bc_payload(struct rpc_clnt *);
unsigned long	rpc_get_timeout(struct rpc_clnt *clnt);
void		rpc_force_rebind(struct rpc_clnt *);
size_t		rpc_peeraddr(struct rpc_clnt *, struct sockaddr *, size_t);
const char	*rpc_peeraddr2str(struct rpc_clnt *, enum rpc_display_format_t);
int		rpc_localaddr(struct rpc_clnt *, struct sockaddr *, size_t);

int 		rpc_clnt_iterate_for_each_xprt(struct rpc_clnt *clnt,
			int (*fn)(struct rpc_clnt *, struct rpc_xprt *, void *),
			void *data);

int 		rpc_clnt_test_and_add_xprt(struct rpc_clnt *clnt,
			struct rpc_xprt_switch *xps,
			struct rpc_xprt *xprt,
			void *dummy);
int		rpc_clnt_add_xprt(struct rpc_clnt *, struct xprt_create *,
			int (*setup)(struct rpc_clnt *,
				struct rpc_xprt_switch *,
				struct rpc_xprt *,
				void *),
			void *data);
void		rpc_cap_max_reconnect_timeout(struct rpc_clnt *clnt,
			unsigned long timeo);

int		rpc_clnt_setup_test_and_add_xprt(struct rpc_clnt *,
			struct rpc_xprt_switch *,
			struct rpc_xprt *,
			void *);

const char *rpc_proc_name(const struct rpc_task *task);

void rpc_clnt_xprt_switch_put(struct rpc_clnt *);
void rpc_clnt_xprt_switch_add_xprt(struct rpc_clnt *, struct rpc_xprt *);
bool rpc_clnt_xprt_switch_has_addr(struct rpc_clnt *clnt,
			const struct sockaddr *sap);
void rpc_cleanup_clids(void);
#endif 
#endif 
