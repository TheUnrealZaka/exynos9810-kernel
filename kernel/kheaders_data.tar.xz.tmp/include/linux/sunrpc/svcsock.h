

#ifndef SUNRPC_SVCSOCK_H
#define SUNRPC_SVCSOCK_H

#include <linux/sunrpc/svc.h>
#include <linux/sunrpc/svc_xprt.h>


struct svc_sock {
	struct svc_xprt		sk_xprt;
	struct socket *		sk_sock;	
	struct sock *		sk_sk;		

	
	void			(*sk_ostate)(struct sock *);
	void			(*sk_odata)(struct sock *);
	void			(*sk_owspace)(struct sock *);

	
	
	__be32			sk_reclen;
	
	u32			sk_tcplen;
	
	u32			sk_datalen;

	struct page *		sk_pages[RPCSVC_MAXPAGES];	
};

static inline u32 svc_sock_reclen(struct svc_sock *svsk)
{
	return ntohl(svsk->sk_reclen) & RPC_FRAGMENT_SIZE_MASK;
}

static inline u32 svc_sock_final_rec(struct svc_sock *svsk)
{
	return ntohl(svsk->sk_reclen) & RPC_LAST_STREAM_FRAGMENT;
}


void		svc_close_net(struct svc_serv *, struct net *);
int		svc_recv(struct svc_rqst *, long);
int		svc_send(struct svc_rqst *);
void		svc_drop(struct svc_rqst *);
void		svc_sock_update_bufs(struct svc_serv *serv);
bool		svc_alien_sock(struct net *net, int fd);
int		svc_addsock(struct svc_serv *serv, const int fd,
					char *name_return, const size_t len);
void		svc_init_xprt_sock(void);
void		svc_cleanup_xprt_sock(void);
struct svc_xprt *svc_sock_create(struct svc_serv *serv, int prot);
void		svc_sock_destroy(struct svc_xprt *);


#define SVC_SOCK_DEFAULTS	(0U)
#define SVC_SOCK_ANONYMOUS	(1U << 0)	
#define SVC_SOCK_TEMPORARY	(1U << 1)	

#endif 
