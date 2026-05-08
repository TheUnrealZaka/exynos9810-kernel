



#ifndef _LINUX_SUNRPC_BC_XPRT_H
#define _LINUX_SUNRPC_BC_XPRT_H

#include <linux/sunrpc/svcsock.h>
#include <linux/sunrpc/xprt.h>
#include <linux/sunrpc/sched.h>

#ifdef CONFIG_SUNRPC_BACKCHANNEL
struct rpc_rqst *xprt_lookup_bc_request(struct rpc_xprt *xprt, __be32 xid);
void xprt_complete_bc_request(struct rpc_rqst *req, uint32_t copied);
void xprt_free_bc_request(struct rpc_rqst *req);
int xprt_setup_backchannel(struct rpc_xprt *, unsigned int min_reqs);
void xprt_destroy_backchannel(struct rpc_xprt *, unsigned int max_reqs);


int xprt_setup_bc(struct rpc_xprt *xprt, unsigned int min_reqs);
void xprt_destroy_bc(struct rpc_xprt *xprt, unsigned int max_reqs);
void xprt_free_bc_rqst(struct rpc_rqst *req);


static inline int svc_is_backchannel(const struct svc_rqst *rqstp)
{
	if (rqstp->rq_server->sv_bc_xprt)
		return 1;
	return 0;
}
#else 
static inline int xprt_setup_backchannel(struct rpc_xprt *xprt,
					 unsigned int min_reqs)
{
	return 0;
}

static inline int svc_is_backchannel(const struct svc_rqst *rqstp)
{
	return 0;
}

static inline void xprt_free_bc_request(struct rpc_rqst *req)
{
}
#endif 
#endif 

