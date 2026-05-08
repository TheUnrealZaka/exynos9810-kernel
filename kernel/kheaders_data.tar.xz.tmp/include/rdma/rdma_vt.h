#ifndef DEF_RDMA_VT_H
#define DEF_RDMA_VT_H





#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/hash.h>
#include <rdma/ib_verbs.h>
#include <rdma/rdmavt_mr.h>
#include <rdma/rdmavt_qp.h>

#define RVT_MAX_PKEY_VALUES 16

struct rvt_ibport {
	struct rvt_qp __rcu *qp[2];
	struct ib_mad_agent *send_agent;	
	struct rb_root mcast_tree;
	spinlock_t lock;		

	
	unsigned long mkey_lease_timeout;
	unsigned long trap_timeout;
	__be64 gid_prefix;      
	__be64 mkey;
	u64 tid;
	u32 port_cap_flags;
	u32 pma_sample_start;
	u32 pma_sample_interval;
	__be16 pma_counter_select[5];
	u16 pma_tag;
	u16 mkey_lease_period;
	u16 sm_lid;
	u8 sm_sl;
	u8 mkeyprot;
	u8 subnet_timeout;
	u8 vl_high_limit;

	
	u64 n_rc_resends;
	u64 n_seq_naks;
	u64 n_rdma_seq;
	u64 n_rnr_naks;
	u64 n_other_naks;
	u64 n_loop_pkts;
	u64 n_pkt_drops;
	u64 n_vl15_dropped;
	u64 n_rc_timeouts;
	u64 n_dmawait;
	u64 n_unaligned;
	u64 n_rc_dupreq;
	u64 n_rc_seqnak;
	u16 pkey_violations;
	u16 qkey_violations;
	u16 mkey_violations;

	
	u64 z_rc_acks;
	u64 z_rc_qacks;
	u64 z_rc_delayed_comp;
	u64 __percpu *rc_acks;
	u64 __percpu *rc_qacks;
	u64 __percpu *rc_delayed_comp;

	void *priv; 

	
	u16 *pkey_table;

	struct rvt_ah *sm_ah;
};

#define RVT_CQN_MAX 16 


struct rvt_driver_params {
	struct ib_device_attr props;

	
	unsigned int lkey_table_size;
	unsigned int qp_table_size;
	int qpn_start;
	int qpn_inc;
	int qpn_res_start;
	int qpn_res_end;
	int nports;
	int npkeys;
	char cq_name[RVT_CQN_MAX];
	int node;
	int psn_mask;
	int psn_shift;
	int psn_modify_mask;
	u32 core_cap_flags;
	u32 max_mad_size;
	u8 qos_shift;
	u8 max_rdma_atomic;
	u8 reserved_operations;
};


struct rvt_pd {
	struct ib_pd ibpd;
	int user;               
};


struct rvt_ah {
	struct ib_ah ibah;
	struct ib_ah_attr attr;
	atomic_t refcount;
	u8 vl;
	u8 log_pmtu;
};

struct rvt_dev_info;
struct rvt_swqe;
struct rvt_driver_provided {
	

	
	int (*port_callback)(struct ib_device *, u8, struct kobject *);

	
	const char * (*get_card_name)(struct rvt_dev_info *rdi);

	
	struct pci_dev * (*get_pci_dev)(struct rvt_dev_info *rdi);

	
	void * (*qp_priv_alloc)(struct rvt_dev_info *rdi, struct rvt_qp *qp,
				gfp_t gfp);

	
	void (*qp_priv_free)(struct rvt_dev_info *rdi, struct rvt_qp *qp);

	
	void (*notify_qp_reset)(struct rvt_qp *qp);

	
	void (*schedule_send)(struct rvt_qp *qp);
	void (*schedule_send_no_lock)(struct rvt_qp *qp);

	
	void (*do_send)(struct rvt_qp *qp);

	
	int (*get_pmtu_from_attr)(struct rvt_dev_info *rdi, struct rvt_qp *qp,
				  struct ib_qp_attr *attr);

	
	void (*flush_qp_waiters)(struct rvt_qp *qp);

	
	void (*stop_send_queue)(struct rvt_qp *qp);

	
	void (*quiesce_qp)(struct rvt_qp *qp);

	
	void (*notify_error_qp)(struct rvt_qp *qp);

	
	u32 (*mtu_from_qp)(struct rvt_dev_info *rdi, struct rvt_qp *qp,
			   u32 pmtu);
	
	int (*mtu_to_path_mtu)(u32 mtu);

	
	int (*get_guid_be)(struct rvt_dev_info *rdi, struct rvt_ibport *rvp,
			   int guid_index, __be64 *guid);

	
	int (*query_port_state)(struct rvt_dev_info *rdi, u8 port_num,
				struct ib_port_attr *props);

	
	int (*shut_down_port)(struct rvt_dev_info *rdi, u8 port_num);

	
	void (*cap_mask_chg)(struct rvt_dev_info *rdi, u8 port_num);

	

	
	unsigned (*free_all_qps)(struct rvt_dev_info *rdi);

	
	int (*check_ah)(struct ib_device *, struct ib_ah_attr *);

	
	void (*notify_new_ah)(struct ib_device *, struct ib_ah_attr *,
			      struct rvt_ah *);

	
	int (*alloc_qpn)(struct rvt_dev_info *rdi, struct rvt_qpn_table *qpt,
			 enum ib_qp_type type, u8 port_num, gfp_t gfp);

	
	int (*check_modify_qp)(struct rvt_qp *qp, struct ib_qp_attr *attr,
			       int attr_mask, struct ib_udata *udata);

	
	void (*modify_qp)(struct rvt_qp *qp, struct ib_qp_attr *attr,
			  int attr_mask, struct ib_udata *udata);

	
	int (*check_send_wqe)(struct rvt_qp *qp, struct rvt_swqe *wqe);

	
	void (*notify_create_mad_agent)(struct rvt_dev_info *rdi, int port_idx);

	
	void (*notify_free_mad_agent)(struct rvt_dev_info *rdi, int port_idx);

};

struct rvt_dev_info {
	struct ib_device ibdev; 

	

	
	struct rvt_driver_params dparms;

	
	const struct rvt_operation_params *post_parms;

	struct rvt_mregion __rcu *dma_mr;
	struct rvt_lkey_table lkey_table;

	
	struct rvt_driver_provided driver_f;

	
	int n_pds_allocated;
	spinlock_t n_pds_lock; 

	int n_ahs_allocated;
	spinlock_t n_ahs_lock; 

	u32 n_srqs_allocated;
	spinlock_t n_srqs_lock; 

	int flags;
	struct rvt_ibport **ports;

	
	struct rvt_qp_ibdev *qp_dev;
	u32 n_qps_allocated;    
	u32 n_rc_qps;		
	u32 busy_jiffies;	
	spinlock_t n_qps_lock;	

	
	struct list_head pending_mmaps;
	spinlock_t mmap_offset_lock; 
	u32 mmap_offset;
	spinlock_t pending_lock; 

	
	struct kthread_worker *worker; 
	u32 n_cqs_allocated;    
	spinlock_t n_cqs_lock; 

	
	u32 n_mcast_grps_allocated; 
	spinlock_t n_mcast_grps_lock;

};

static inline struct rvt_pd *ibpd_to_rvtpd(struct ib_pd *ibpd)
{
	return container_of(ibpd, struct rvt_pd, ibpd);
}

static inline struct rvt_ah *ibah_to_rvtah(struct ib_ah *ibah)
{
	return container_of(ibah, struct rvt_ah, ibah);
}

static inline struct rvt_dev_info *ib_to_rvt(struct ib_device *ibdev)
{
	return  container_of(ibdev, struct rvt_dev_info, ibdev);
}

static inline struct rvt_srq *ibsrq_to_rvtsrq(struct ib_srq *ibsrq)
{
	return container_of(ibsrq, struct rvt_srq, ibsrq);
}

static inline struct rvt_qp *ibqp_to_rvtqp(struct ib_qp *ibqp)
{
	return container_of(ibqp, struct rvt_qp, ibqp);
}

static inline unsigned rvt_get_npkeys(struct rvt_dev_info *rdi)
{
	
	return rdi->dparms.npkeys;
}


static inline unsigned int rvt_max_atomic(struct rvt_dev_info *rdi)
{
	return rdi->dparms.max_rdma_atomic + 1;
}


static inline u16 rvt_get_pkey(struct rvt_dev_info *rdi,
			       int port_index,
			       unsigned index)
{
	if (index >= rvt_get_npkeys(rdi))
		return 0;
	else
		return rdi->ports[port_index]->pkey_table[index];
}



static inline struct rvt_qp *rvt_lookup_qpn(struct rvt_dev_info *rdi,
					    struct rvt_ibport *rvp,
					    u32 qpn) __must_hold(RCU)
{
	struct rvt_qp *qp = NULL;

	if (unlikely(qpn <= 1)) {
		qp = rcu_dereference(rvp->qp[qpn]);
	} else {
		u32 n = hash_32(qpn, rdi->qp_dev->qp_table_bits);

		for (qp = rcu_dereference(rdi->qp_dev->qp_table[n]); qp;
			qp = rcu_dereference(qp->next))
			if (qp->ibqp.qp_num == qpn)
				break;
	}
	return qp;
}

struct rvt_dev_info *rvt_alloc_device(size_t size, int nports);
void rvt_dealloc_device(struct rvt_dev_info *rdi);
int rvt_register_device(struct rvt_dev_info *rvd);
void rvt_unregister_device(struct rvt_dev_info *rvd);
int rvt_check_ah(struct ib_device *ibdev, struct ib_ah_attr *ah_attr);
int rvt_init_port(struct rvt_dev_info *rdi, struct rvt_ibport *port,
		  int port_index, u16 *pkey_table);
int rvt_fast_reg_mr(struct rvt_qp *qp, struct ib_mr *ibmr, u32 key,
		    int access);
int rvt_invalidate_rkey(struct rvt_qp *qp, u32 rkey);
int rvt_rkey_ok(struct rvt_qp *qp, struct rvt_sge *sge,
		u32 len, u64 vaddr, u32 rkey, int acc);
int rvt_lkey_ok(struct rvt_lkey_table *rkt, struct rvt_pd *pd,
		struct rvt_sge *isge, struct ib_sge *sge, int acc);
struct rvt_mcast *rvt_mcast_find(struct rvt_ibport *ibp, union ib_gid *mgid);

#endif          
