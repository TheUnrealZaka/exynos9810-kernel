

#ifndef __FSL_QMAN_H
#define __FSL_QMAN_H

#include <linux/bitops.h>


#define QM_CHANNEL_SWPORTAL0 0
#define QMAN_CHANNEL_POOL1 0x21
#define QMAN_CHANNEL_POOL1_REV3 0x401
extern u16 qm_channel_pool1;


#define QM_PIRQ_CSCI	0x00100000	
#define QM_PIRQ_EQCI	0x00080000	
#define QM_PIRQ_EQRI	0x00040000	
#define QM_PIRQ_DQRI	0x00020000	
#define QM_PIRQ_MRI	0x00010000	

#define QM_PIRQ_SLOW	(QM_PIRQ_CSCI | QM_PIRQ_EQCI | QM_PIRQ_EQRI | \
			 QM_PIRQ_MRI)


#define QM_SDQCR_CHANNELS_POOL_MASK	0x00007fff

#define QM_SDQCR_CHANNELS_POOL(n)	(0x00008000 >> (n))

static inline u32 QM_SDQCR_CHANNELS_POOL_CONV(u16 channel)
{
	return QM_SDQCR_CHANNELS_POOL(channel + 1 - qm_channel_pool1);
}




struct qm_fd {
	union {
		struct {
			u8 cfg8b_w1;
			u8 bpid;	
			u8 cfg8b_w3;
			u8 addr_hi;	
			__be32 addr_lo;	
		} __packed;
		__be64 data;
	};
	__be32 cfg;	
	union {
		__be32 cmd;
		__be32 status;
	};
} __aligned(8);

#define QM_FD_FORMAT_SG		BIT(31)
#define QM_FD_FORMAT_LONG	BIT(30)
#define QM_FD_FORMAT_COMPOUND	BIT(29)
#define QM_FD_FORMAT_MASK	GENMASK(31, 29)
#define QM_FD_OFF_SHIFT		20
#define QM_FD_OFF_MASK		GENMASK(28, 20)
#define QM_FD_LEN_MASK		GENMASK(19, 0)
#define QM_FD_LEN_BIG_MASK	GENMASK(28, 0)

enum qm_fd_format {
	
	qm_fd_contig = 0,
	qm_fd_contig_big = QM_FD_FORMAT_LONG,
	qm_fd_sg = QM_FD_FORMAT_SG,
	qm_fd_sg_big = QM_FD_FORMAT_SG | QM_FD_FORMAT_LONG,
	qm_fd_compound = QM_FD_FORMAT_COMPOUND
};

static inline dma_addr_t qm_fd_addr(const struct qm_fd *fd)
{
	return be64_to_cpu(fd->data) & 0xffffffffffLLU;
}

static inline u64 qm_fd_addr_get64(const struct qm_fd *fd)
{
	return be64_to_cpu(fd->data) & 0xffffffffffLLU;
}

static inline void qm_fd_addr_set64(struct qm_fd *fd, u64 addr)
{
	fd->addr_hi = upper_32_bits(addr);
	fd->addr_lo = cpu_to_be32(lower_32_bits(addr));
}


static inline enum qm_fd_format qm_fd_get_format(const struct qm_fd *fd)
{
	return be32_to_cpu(fd->cfg) & QM_FD_FORMAT_MASK;
}

static inline int qm_fd_get_offset(const struct qm_fd *fd)
{
	return (be32_to_cpu(fd->cfg) & QM_FD_OFF_MASK) >> QM_FD_OFF_SHIFT;
}

static inline int qm_fd_get_length(const struct qm_fd *fd)
{
	return be32_to_cpu(fd->cfg) & QM_FD_LEN_MASK;
}

static inline int qm_fd_get_len_big(const struct qm_fd *fd)
{
	return be32_to_cpu(fd->cfg) & QM_FD_LEN_BIG_MASK;
}

static inline void qm_fd_set_param(struct qm_fd *fd, enum qm_fd_format fmt,
				   int off, int len)
{
	fd->cfg = cpu_to_be32(fmt | (len & QM_FD_LEN_BIG_MASK) |
			      ((off << QM_FD_OFF_SHIFT) & QM_FD_OFF_MASK));
}

#define qm_fd_set_contig(fd, off, len) \
	qm_fd_set_param(fd, qm_fd_contig, off, len)
#define qm_fd_set_sg(fd, off, len) qm_fd_set_param(fd, qm_fd_sg, off, len)
#define qm_fd_set_contig_big(fd, len) \
	qm_fd_set_param(fd, qm_fd_contig_big, 0, len)
#define qm_fd_set_sg_big(fd, len) qm_fd_set_param(fd, qm_fd_sg_big, 0, len)

static inline void qm_fd_clear_fd(struct qm_fd *fd)
{
	fd->data = 0;
	fd->cfg = 0;
	fd->cmd = 0;
}


struct qm_sg_entry {
	union {
		struct {
			u8 __reserved1[3];
			u8 addr_hi;	
			__be32 addr_lo;	
		};
		__be64 data;
	};
	__be32 cfg;	
	u8 __reserved2;
	u8 bpid;
	__be16 offset; 
} __packed;

#define QM_SG_LEN_MASK	GENMASK(29, 0)
#define QM_SG_OFF_MASK	GENMASK(12, 0)
#define QM_SG_FIN	BIT(30)
#define QM_SG_EXT	BIT(31)

static inline dma_addr_t qm_sg_addr(const struct qm_sg_entry *sg)
{
	return be64_to_cpu(sg->data) & 0xffffffffffLLU;
}

static inline u64 qm_sg_entry_get64(const struct qm_sg_entry *sg)
{
	return be64_to_cpu(sg->data) & 0xffffffffffLLU;
}

static inline void qm_sg_entry_set64(struct qm_sg_entry *sg, u64 addr)
{
	sg->addr_hi = upper_32_bits(addr);
	sg->addr_lo = cpu_to_be32(lower_32_bits(addr));
}

static inline bool qm_sg_entry_is_final(const struct qm_sg_entry *sg)
{
	return be32_to_cpu(sg->cfg) & QM_SG_FIN;
}

static inline bool qm_sg_entry_is_ext(const struct qm_sg_entry *sg)
{
	return be32_to_cpu(sg->cfg) & QM_SG_EXT;
}

static inline int qm_sg_entry_get_len(const struct qm_sg_entry *sg)
{
	return be32_to_cpu(sg->cfg) & QM_SG_LEN_MASK;
}

static inline void qm_sg_entry_set_len(struct qm_sg_entry *sg, int len)
{
	sg->cfg = cpu_to_be32(len & QM_SG_LEN_MASK);
}

static inline void qm_sg_entry_set_f(struct qm_sg_entry *sg, int len)
{
	sg->cfg = cpu_to_be32(QM_SG_FIN | (len & QM_SG_LEN_MASK));
}

static inline int qm_sg_entry_get_off(const struct qm_sg_entry *sg)
{
	return be32_to_cpu(sg->offset) & QM_SG_OFF_MASK;
}


struct qm_dqrr_entry {
	u8 verb;
	u8 stat;
	u16 seqnum;	
	u8 tok;
	u8 __reserved2[3];
	u32 fqid;	
	u32 contextB;
	struct qm_fd fd;
	u8 __reserved4[32];
} __packed;
#define QM_DQRR_VERB_VBIT		0x80
#define QM_DQRR_VERB_MASK		0x7f	
#define QM_DQRR_VERB_FRAME_DEQUEUE	0x60	
#define QM_DQRR_STAT_FQ_EMPTY		0x80	
#define QM_DQRR_STAT_FQ_HELDACTIVE	0x40	
#define QM_DQRR_STAT_FQ_FORCEELIGIBLE	0x20	
#define QM_DQRR_STAT_FD_VALID		0x10	
#define QM_DQRR_STAT_UNSCHEDULED	0x02	
#define QM_DQRR_STAT_DQCR_EXPIRED	0x01	



union qm_mr_entry {
	struct {
		u8 verb;
		u8 __reserved[63];
	};
	struct {
		u8 verb;
		u8 dca;
		u16 seqnum;
		u8 rc;		
		u8 orp_hi;	
		u16 orp_lo;
		u32 fqid;	
		u32 tag;
		struct qm_fd fd;
		u8 __reserved1[32];
	} __packed ern;
	struct {
		u8 verb;
		u8 fqs;		
		u8 __reserved1[6];
		u32 fqid;	
		u32 contextB;
		u8 __reserved2[48];
	} __packed fq;		
};
#define QM_MR_VERB_VBIT			0x80

#define QM_MR_VERB_TYPE_MASK		0x27
#define QM_MR_VERB_DC_ERN		0x20
#define QM_MR_VERB_FQRN			0x21
#define QM_MR_VERB_FQRNI		0x22
#define QM_MR_VERB_FQRL			0x23
#define QM_MR_VERB_FQPN			0x24
#define QM_MR_RC_MASK			0xf0	
#define QM_MR_RC_CGR_TAILDROP		0x00
#define QM_MR_RC_WRED			0x10
#define QM_MR_RC_ERROR			0x20
#define QM_MR_RC_ORPWINDOW_EARLY	0x30
#define QM_MR_RC_ORPWINDOW_LATE		0x40
#define QM_MR_RC_FQ_TAILDROP		0x50
#define QM_MR_RC_ORPWINDOW_RETIRED	0x60
#define QM_MR_RC_ORP_ZERO		0x70
#define QM_MR_FQS_ORLPRESENT		0x02	
#define QM_MR_FQS_NOTEMPTY		0x01	


struct qm_fqd_stashing {
	
	u8 exclusive;
	
	u8 cl; 
};

struct qm_fqd_oac {
	
	u8 oac; 
	
	s8 oal; 
};

struct qm_fqd {
	
	u8 orpc;
	u8 cgid;
	__be16 fq_ctrl;	
	__be16 dest_wq;	
	__be16 ics_cred; 
	
	union {
		__be16 td; 
		struct qm_fqd_oac oac_init;
	};
	__be32 context_b;
	union {
		
		__be64 opaque;
		struct {
			__be32 hi;
			__be32 lo;
		};
		
		
		struct {
			struct qm_fqd_stashing stashing;
			
			__be16 context_hi;
			__be32 context_lo;
		} __packed;
	} context_a;
	struct qm_fqd_oac oac_query;
} __packed;

#define QM_FQD_CHAN_OFF		3
#define QM_FQD_WQ_MASK		GENMASK(2, 0)
#define QM_FQD_TD_EXP_MASK	GENMASK(4, 0)
#define QM_FQD_TD_MANT_OFF	5
#define QM_FQD_TD_MANT_MASK	GENMASK(12, 5)
#define QM_FQD_TD_MAX		0xe0000000
#define QM_FQD_TD_MANT_MAX	0xff
#define QM_FQD_OAC_OFF		6
#define QM_FQD_AS_OFF		4
#define QM_FQD_DS_OFF		2
#define QM_FQD_XS_MASK		0x3


static inline u64 qm_fqd_stashing_get64(const struct qm_fqd *fqd)
{
	return be64_to_cpu(fqd->context_a.opaque) & 0xffffffffffffULL;
}

static inline dma_addr_t qm_fqd_stashing_addr(const struct qm_fqd *fqd)
{
	return be64_to_cpu(fqd->context_a.opaque) & 0xffffffffffffULL;
}

static inline u64 qm_fqd_context_a_get64(const struct qm_fqd *fqd)
{
	return qm_fqd_stashing_get64(fqd);
}

static inline void qm_fqd_stashing_set64(struct qm_fqd *fqd, u64 addr)
{
	fqd->context_a.context_hi = upper_32_bits(addr);
	fqd->context_a.context_lo = lower_32_bits(addr);
}

static inline void qm_fqd_context_a_set64(struct qm_fqd *fqd, u64 addr)
{
	fqd->context_a.hi = cpu_to_be16(upper_32_bits(addr));
	fqd->context_a.lo = cpu_to_be32(lower_32_bits(addr));
}


static inline int qm_fqd_set_taildrop(struct qm_fqd *fqd, u32 val,
				      int roundup)
{
	u32 e = 0;
	int td, oddbit = 0;

	if (val > QM_FQD_TD_MAX)
		return -ERANGE;

	while (val > QM_FQD_TD_MANT_MAX) {
		oddbit = val & 1;
		val >>= 1;
		e++;
		if (roundup && oddbit)
			val++;
	}

	td = (val << QM_FQD_TD_MANT_OFF) & QM_FQD_TD_MANT_MASK;
	td |= (e & QM_FQD_TD_EXP_MASK);
	fqd->td = cpu_to_be16(td);
	return 0;
}

static inline int qm_fqd_get_taildrop(const struct qm_fqd *fqd)
{
	int td = be16_to_cpu(fqd->td);

	return ((td & QM_FQD_TD_MANT_MASK) >> QM_FQD_TD_MANT_OFF)
		<< (td & QM_FQD_TD_EXP_MASK);
}

static inline void qm_fqd_set_stashing(struct qm_fqd *fqd, u8 as, u8 ds, u8 cs)
{
	struct qm_fqd_stashing *st = &fqd->context_a.stashing;

	st->cl = ((as & QM_FQD_XS_MASK) << QM_FQD_AS_OFF) |
		 ((ds & QM_FQD_XS_MASK) << QM_FQD_DS_OFF) |
		 (cs & QM_FQD_XS_MASK);
}

static inline u8 qm_fqd_get_stashing(const struct qm_fqd *fqd)
{
	return fqd->context_a.stashing.cl;
}

static inline void qm_fqd_set_oac(struct qm_fqd *fqd, u8 val)
{
	fqd->oac_init.oac = val << QM_FQD_OAC_OFF;
}

static inline void qm_fqd_set_oal(struct qm_fqd *fqd, s8 val)
{
	fqd->oac_init.oal = val;
}

static inline void qm_fqd_set_destwq(struct qm_fqd *fqd, int ch, int wq)
{
	fqd->dest_wq = cpu_to_be16((ch << QM_FQD_CHAN_OFF) |
				   (wq & QM_FQD_WQ_MASK));
}

static inline int qm_fqd_get_chan(const struct qm_fqd *fqd)
{
	return be16_to_cpu(fqd->dest_wq) >> QM_FQD_CHAN_OFF;
}

static inline int qm_fqd_get_wq(const struct qm_fqd *fqd)
{
	return be16_to_cpu(fqd->dest_wq) & QM_FQD_WQ_MASK;
}



#define QM_FQCTRL_MASK		0x07ff	
#define QM_FQCTRL_CGE		0x0400	
#define QM_FQCTRL_TDE		0x0200	
#define QM_FQCTRL_CTXASTASHING	0x0080	
#define QM_FQCTRL_CPCSTASH	0x0040	
#define QM_FQCTRL_FORCESFDR	0x0008	
#define QM_FQCTRL_AVOIDBLOCK	0x0004	
#define QM_FQCTRL_HOLDACTIVE	0x0002	
#define QM_FQCTRL_PREFERINCACHE	0x0001	
#define QM_FQCTRL_LOCKINCACHE	QM_FQCTRL_PREFERINCACHE 



#define QM_STASHING_EXCL_ANNOTATION	0x04
#define QM_STASHING_EXCL_DATA		0x02
#define QM_STASHING_EXCL_CTX		0x01



#define QM_OAC_ICS		0x2 
#define QM_OAC_CG		0x1 


struct qm_cgr_wr_parm {
	
	u32 word;
};

struct qm_cgr_cs_thres {
	
	u16 word;
};

struct __qm_mc_cgr {
	struct qm_cgr_wr_parm wr_parm_g;
	struct qm_cgr_wr_parm wr_parm_y;
	struct qm_cgr_wr_parm wr_parm_r;
	u8 wr_en_g;	
	u8 wr_en_y;	
	u8 wr_en_r;	
	u8 cscn_en;	
	union {
		struct {
			u16 cscn_targ_upd_ctrl; 
			u16 cscn_targ_dcp_low;	
		};
		u32 cscn_targ;	
	};
	u8 cstd_en;	
	u8 cs;		
	struct qm_cgr_cs_thres cs_thres; 
	u8 mode;	
} __packed;
#define QM_CGR_EN		0x01 
#define QM_CGR_TARG_UDP_CTRL_WRITE_BIT	0x8000 
#define QM_CGR_TARG_UDP_CTRL_DCP	0x4000 
#define QM_CGR_TARG_PORTAL(n)	(0x80000000 >> (n)) 
#define QM_CGR_TARG_FMAN0	0x00200000 
#define QM_CGR_TARG_FMAN1	0x00100000 

static inline u64 qm_cgr_cs_thres_get64(const struct qm_cgr_cs_thres *th)
{
	return ((th->word >> 5) & 0xff) << (th->word & 0x1f);
}

static inline int qm_cgr_cs_thres_set64(struct qm_cgr_cs_thres *th, u64 val,
					int roundup)
{
	u32 e = 0;
	int oddbit = 0;

	while (val > 0xff) {
		oddbit = val & 1;
		val >>= 1;
		e++;
		if (roundup && oddbit)
			val++;
	}
	th->word = ((val & 0xff) << 5) | (e & 0x1f);
	return 0;
}


struct qm_mcc_initfq {
	u8 __reserved1[2];
	u16 we_mask;	
	u32 fqid;	
	u16 count;	
	struct qm_fqd fqd; 
	u8 __reserved2[30];
} __packed;

struct qm_mcc_initcgr {
	u8 __reserve1[2];
	u16 we_mask;	
	struct __qm_mc_cgr cgr;	
	u8 __reserved2[2];
	u8 cgid;
	u8 __reserved3[32];
} __packed;


#define QM_INITFQ_WE_MASK		0x01ff	
#define QM_INITFQ_WE_OAC		0x0100
#define QM_INITFQ_WE_ORPC		0x0080
#define QM_INITFQ_WE_CGID		0x0040
#define QM_INITFQ_WE_FQCTRL		0x0020
#define QM_INITFQ_WE_DESTWQ		0x0010
#define QM_INITFQ_WE_ICSCRED		0x0008
#define QM_INITFQ_WE_TDTHRESH		0x0004
#define QM_INITFQ_WE_CONTEXTB		0x0002
#define QM_INITFQ_WE_CONTEXTA		0x0001

#define QM_CGR_WE_MASK			0x07ff	
#define QM_CGR_WE_WR_PARM_G		0x0400
#define QM_CGR_WE_WR_PARM_Y		0x0200
#define QM_CGR_WE_WR_PARM_R		0x0100
#define QM_CGR_WE_WR_EN_G		0x0080
#define QM_CGR_WE_WR_EN_Y		0x0040
#define QM_CGR_WE_WR_EN_R		0x0020
#define QM_CGR_WE_CSCN_EN		0x0010
#define QM_CGR_WE_CSCN_TARG		0x0008
#define QM_CGR_WE_CSTD_EN		0x0004
#define QM_CGR_WE_CS_THRES		0x0002
#define QM_CGR_WE_MODE			0x0001

#define QMAN_CGR_FLAG_USE_INIT	     0x00000001

	

struct qman_portal;


struct qman_fq;


struct qman_cgr;


enum qman_cb_dqrr_result {
	
	qman_cb_dqrr_consume,
	
	qman_cb_dqrr_park,
	
	qman_cb_dqrr_defer,
	
	qman_cb_dqrr_stop,
	
	qman_cb_dqrr_consume_stop
};
typedef enum qman_cb_dqrr_result (*qman_cb_dqrr)(struct qman_portal *qm,
					struct qman_fq *fq,
					const struct qm_dqrr_entry *dqrr);


typedef void (*qman_cb_mr)(struct qman_portal *qm, struct qman_fq *fq,
			   const union qm_mr_entry *msg);


enum qman_fq_state {
	qman_fq_state_oos,
	qman_fq_state_parked,
	qman_fq_state_sched,
	qman_fq_state_retired
};

#define QMAN_FQ_STATE_CHANGING	     0x80000000 
#define QMAN_FQ_STATE_NE	     0x40000000 
#define QMAN_FQ_STATE_ORL	     0x20000000 
#define QMAN_FQ_STATE_BLOCKOOS	     0xe0000000 
#define QMAN_FQ_STATE_CGR_EN	     0x10000000 
#define QMAN_FQ_STATE_VDQCR	     0x08000000 



struct qman_fq_cb {
	qman_cb_dqrr dqrr;	
	qman_cb_mr ern;		
	qman_cb_mr fqs;		
};

struct qman_fq {
	
	struct qman_fq_cb cb;
	
	u32 fqid, idx;
	unsigned long flags;
	enum qman_fq_state state;
	int cgr_groupid;
};


typedef void (*qman_cb_cgr)(struct qman_portal *qm,
			    struct qman_cgr *cgr, int congested);

struct qman_cgr {
	
	u32 cgrid; 
	qman_cb_cgr cb;
	
	u16 chan; 
	struct list_head node;
};


#define QMAN_FQ_FLAG_NO_ENQUEUE	     0x00000001 
#define QMAN_FQ_FLAG_NO_MODIFY	     0x00000002 
#define QMAN_FQ_FLAG_TO_DCPORTAL     0x00000004 
#define QMAN_FQ_FLAG_DYNAMIC_FQID    0x00000020 


#define QMAN_INITFQ_FLAG_SCHED	     0x00000001 
#define QMAN_INITFQ_FLAG_LOCAL	     0x00000004 

	

void qman_p_irqsource_add(struct qman_portal *p, u32 bits);


void qman_p_irqsource_remove(struct qman_portal *p, u32 bits);


const cpumask_t *qman_affine_cpus(void);


u16 qman_affine_channel(int cpu);


struct qman_portal *qman_get_affine_portal(int cpu);


int qman_p_poll_dqrr(struct qman_portal *p, unsigned int limit);


void qman_p_static_dequeue_add(struct qman_portal *p, u32 pools);

	

int qman_create_fq(u32 fqid, u32 flags, struct qman_fq *fq);


void qman_destroy_fq(struct qman_fq *fq);


u32 qman_fq_fqid(struct qman_fq *fq);


int qman_init_fq(struct qman_fq *fq, u32 flags, struct qm_mcc_initfq *opts);


int qman_schedule_fq(struct qman_fq *fq);


int qman_retire_fq(struct qman_fq *fq, u32 *flags);


int qman_oos_fq(struct qman_fq *fq);


int qman_enqueue(struct qman_fq *fq, const struct qm_fd *fd);


int qman_alloc_fqid_range(u32 *result, u32 count);
#define qman_alloc_fqid(result) qman_alloc_fqid_range(result, 1)


int qman_release_fqid(u32 fqid);

	

int qman_alloc_pool_range(u32 *result, u32 count);
#define qman_alloc_pool(result) qman_alloc_pool_range(result, 1)


int qman_release_pool(u32 id);

	

int qman_create_cgr(struct qman_cgr *cgr, u32 flags,
		    struct qm_mcc_initcgr *opts);


int qman_delete_cgr(struct qman_cgr *cgr);


void qman_delete_cgr_safe(struct qman_cgr *cgr);


int qman_query_cgr_congested(struct qman_cgr *cgr, bool *result);


int qman_alloc_cgrid_range(u32 *result, u32 count);
#define qman_alloc_cgrid(result) qman_alloc_cgrid_range(result, 1)


int qman_release_cgrid(u32 id);

#endif	
