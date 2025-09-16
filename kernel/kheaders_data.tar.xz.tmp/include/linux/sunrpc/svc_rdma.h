

#ifndef SVC_RDMA_H
#define SVC_RDMA_H
#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/svcsock.h>
#include <linux/sunrpc/rpc_rdma.h>
#include <rdma/ib_verbs.h>
#include <rdma/rdma_cm.h>
#define SVCRDMA_DEBUG


extern unsigned int svcrdma_ord;
extern unsigned int svcrdma_max_requests;
extern unsigned int svcrdma_max_bc_requests;
extern unsigned int svcrdma_max_req_size;

extern atomic_t rdma_stat_recv;
extern atomic_t rdma_stat_read;
extern atomic_t rdma_stat_write;
extern atomic_t rdma_stat_sq_starve;
extern atomic_t rdma_stat_rq_starve;
extern atomic_t rdma_stat_rq_poll;
extern atomic_t rdma_stat_rq_prod;
extern atomic_t rdma_stat_sq_poll;
extern atomic_t rdma_stat_sq_prod;


struct svc_rdma_op_ctxt {
	struct list_head free;
	struct svc_rdma_op_ctxt *read_hdr;
	struct svc_rdma_fastreg_mr *frmr;
	int hdr_count;
	struct xdr_buf arg;
	struct ib_cqe cqe;
	struct ib_cqe reg_cqe;
	struct ib_cqe inv_cqe;
	struct list_head dto_q;
	enum ib_wc_status wc_status;
	u32 byte_len;
	u32 position;
	struct svcxprt_rdma *xprt;
	unsigned long flags;
	enum dma_data_direction direction;
	int count;
	unsigned int mapped_sges;
	struct ib_sge sge[RPCSVC_MAXPAGES];
	struct page *pages[RPCSVC_MAXPAGES];
};


struct svc_rdma_chunk_sge {
	int start;		
	int count;		
};
struct svc_rdma_fastreg_mr {
	struct ib_mr *mr;
	struct scatterlist *sg;
	int sg_nents;
	unsigned long access_flags;
	enum dma_data_direction direction;
	struct list_head frmr_list;
};
struct svc_rdma_req_map {
	struct list_head free;
	unsigned long count;
	union {
		struct kvec sge[RPCSVC_MAXPAGES];
		struct svc_rdma_chunk_sge ch[RPCSVC_MAXPAGES];
		unsigned long lkey[RPCSVC_MAXPAGES];
	};
};
#define RDMACTXT_F_LAST_CTXT	2

#define	SVCRDMA_DEVCAP_FAST_REG		1	
#define	SVCRDMA_DEVCAP_READ_W_INV	2	

struct svcxprt_rdma {
	struct svc_xprt      sc_xprt;		
	struct rdma_cm_id    *sc_cm_id;		
	struct list_head     sc_accept_q;	
	int		     sc_ord;		
	int                  sc_max_sge;
	int                  sc_max_sge_rd;	
	bool		     sc_snd_w_inv;	

	atomic_t             sc_sq_count;	
	unsigned int	     sc_sq_depth;	
	unsigned int	     sc_rq_depth;	
	u32		     sc_max_requests;	
	u32		     sc_max_bc_requests;
	int                  sc_max_req_size;	

	struct ib_pd         *sc_pd;

	atomic_t	     sc_dma_used;
	spinlock_t	     sc_ctxt_lock;
	struct list_head     sc_ctxts;
	int		     sc_ctxt_used;
	spinlock_t	     sc_map_lock;
	struct list_head     sc_maps;

	struct list_head     sc_rq_dto_q;
	spinlock_t	     sc_rq_dto_lock;
	struct ib_qp         *sc_qp;
	struct ib_cq         *sc_rq_cq;
	struct ib_cq         *sc_sq_cq;
	int		     (*sc_reader)(struct svcxprt_rdma *,
					  struct svc_rqst *,
					  struct svc_rdma_op_ctxt *,
					  int *, u32 *, u32, u32, u64, bool);
	u32		     sc_dev_caps;	
	unsigned int	     sc_frmr_pg_list_len;
	struct list_head     sc_frmr_q;
	spinlock_t	     sc_frmr_q_lock;

	spinlock_t	     sc_lock;		

	wait_queue_head_t    sc_send_wait;	
	unsigned long	     sc_flags;
	struct list_head     sc_dto_q;		
	struct list_head     sc_read_complete_q;
	struct work_struct   sc_work;
};

#define RDMAXPRT_CONN_PENDING	3

#define RPCRDMA_LISTEN_BACKLOG  10

#define RPCRDMA_ORD             (64/4)
#define RPCRDMA_SQ_DEPTH_MULT   8
#define RPCRDMA_MAX_REQUESTS    32
#define RPCRDMA_MAX_REQ_SIZE    4096


#define RPCRDMA_MAX_BC_REQUESTS	2

#define RPCSVC_MAXPAYLOAD_RDMA	RPCSVC_MAXPAYLOAD


static inline void svc_rdma_count_mappings(struct svcxprt_rdma *rdma,
					   struct svc_rdma_op_ctxt *ctxt)
{
	ctxt->mapped_sges++;
	atomic_inc(&rdma->sc_dma_used);
}


extern int svc_rdma_handle_bc_reply(struct rpc_xprt *xprt,
				    struct rpcrdma_msg *rmsgp,
				    struct xdr_buf *rcvbuf);


extern int svc_rdma_xdr_decode_req(struct xdr_buf *);
extern int svc_rdma_xdr_encode_error(struct svcxprt_rdma *,
				     struct rpcrdma_msg *,
				     enum rpcrdma_errcode, __be32 *);
extern void svc_rdma_xdr_encode_write_list(struct rpcrdma_msg *, int);
extern void svc_rdma_xdr_encode_reply_array(struct rpcrdma_write_array *, int);
extern void svc_rdma_xdr_encode_array_chunk(struct rpcrdma_write_array *, int,
					    __be32, __be64, u32);
extern void svc_rdma_xdr_encode_reply_header(struct svcxprt_rdma *,
					     struct rpcrdma_msg *,
					     struct rpcrdma_msg *,
					     enum rpcrdma_proc);
extern int svc_rdma_xdr_get_reply_hdr_len(struct rpcrdma_msg *);


extern int svc_rdma_recvfrom(struct svc_rqst *);
extern int rdma_read_chunk_lcl(struct svcxprt_rdma *, struct svc_rqst *,
			       struct svc_rdma_op_ctxt *, int *, u32 *,
			       u32, u32, u64, bool);
extern int rdma_read_chunk_frmr(struct svcxprt_rdma *, struct svc_rqst *,
				struct svc_rdma_op_ctxt *, int *, u32 *,
				u32, u32, u64, bool);


extern int svc_rdma_map_xdr(struct svcxprt_rdma *, struct xdr_buf *,
			    struct svc_rdma_req_map *, bool);
extern int svc_rdma_sendto(struct svc_rqst *);
extern struct rpcrdma_read_chunk *
	svc_rdma_get_read_chunk(struct rpcrdma_msg *);
extern void svc_rdma_send_error(struct svcxprt_rdma *, struct rpcrdma_msg *,
				int);


extern void svc_rdma_wc_send(struct ib_cq *, struct ib_wc *);
extern void svc_rdma_wc_write(struct ib_cq *, struct ib_wc *);
extern void svc_rdma_wc_reg(struct ib_cq *, struct ib_wc *);
extern void svc_rdma_wc_read(struct ib_cq *, struct ib_wc *);
extern void svc_rdma_wc_inv(struct ib_cq *, struct ib_wc *);
extern int svc_rdma_send(struct svcxprt_rdma *, struct ib_send_wr *);
extern int svc_rdma_post_recv(struct svcxprt_rdma *, gfp_t);
extern int svc_rdma_repost_recv(struct svcxprt_rdma *, gfp_t);
extern int svc_rdma_create_listen(struct svc_serv *, int, struct sockaddr *);
extern struct svc_rdma_op_ctxt *svc_rdma_get_context(struct svcxprt_rdma *);
extern void svc_rdma_put_context(struct svc_rdma_op_ctxt *, int);
extern void svc_rdma_unmap_dma(struct svc_rdma_op_ctxt *ctxt);
extern struct svc_rdma_req_map *svc_rdma_get_req_map(struct svcxprt_rdma *);
extern void svc_rdma_put_req_map(struct svcxprt_rdma *,
				 struct svc_rdma_req_map *);
extern struct svc_rdma_fastreg_mr *svc_rdma_get_frmr(struct svcxprt_rdma *);
extern void svc_rdma_put_frmr(struct svcxprt_rdma *,
			      struct svc_rdma_fastreg_mr *);
extern void svc_sq_reap(struct svcxprt_rdma *);
extern void svc_rq_reap(struct svcxprt_rdma *);
extern void svc_rdma_prep_reply_hdr(struct svc_rqst *);

extern struct svc_xprt_class svc_rdma_class;
#ifdef CONFIG_SUNRPC_BACKCHANNEL
extern struct svc_xprt_class svc_rdma_bc_class;
#endif


extern struct workqueue_struct *svc_rdma_wq;
extern int svc_rdma_init(void);
extern void svc_rdma_cleanup(void);

#endif
