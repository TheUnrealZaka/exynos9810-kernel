
#ifndef _LINUX_QUOTA_
#define _LINUX_QUOTA_

#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/rwsem.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/percpu_counter.h>

#include <linux/dqblk_xfs.h>
#include <linux/dqblk_v1.h>
#include <linux/dqblk_v2.h>

#include <linux/atomic.h>
#include <linux/uidgid.h>
#include <linux/projid.h>
#include <uapi/linux/quota.h>

#undef USRQUOTA
#undef GRPQUOTA
#undef PRJQUOTA
enum quota_type {
	USRQUOTA = 0,		
	GRPQUOTA = 1,		
	PRJQUOTA = 2,		
};


#define QTYPE_MASK_USR (1 << USRQUOTA)
#define QTYPE_MASK_GRP (1 << GRPQUOTA)
#define QTYPE_MASK_PRJ (1 << PRJQUOTA)

typedef __kernel_uid32_t qid_t; 
typedef long long qsize_t;	

struct kqid {			
	union {
		kuid_t uid;
		kgid_t gid;
		kprojid_t projid;
	};
	enum quota_type type;  
};

extern bool qid_eq(struct kqid left, struct kqid right);
extern bool qid_lt(struct kqid left, struct kqid right);
extern qid_t from_kqid(struct user_namespace *to, struct kqid qid);
extern qid_t from_kqid_munged(struct user_namespace *to, struct kqid qid);
extern bool qid_valid(struct kqid qid);


static inline struct kqid make_kqid(struct user_namespace *from,
				    enum quota_type type, qid_t qid)
{
	struct kqid kqid;

	kqid.type = type;
	switch (type) {
	case USRQUOTA:
		kqid.uid = make_kuid(from, qid);
		break;
	case GRPQUOTA:
		kqid.gid = make_kgid(from, qid);
		break;
	case PRJQUOTA:
		kqid.projid = make_kprojid(from, qid);
		break;
	default:
		BUG();
	}
	return kqid;
}


static inline struct kqid make_kqid_invalid(enum quota_type type)
{
	struct kqid kqid;

	kqid.type = type;
	switch (type) {
	case USRQUOTA:
		kqid.uid = INVALID_UID;
		break;
	case GRPQUOTA:
		kqid.gid = INVALID_GID;
		break;
	case PRJQUOTA:
		kqid.projid = INVALID_PROJID;
		break;
	default:
		BUG();
	}
	return kqid;
}


static inline struct kqid make_kqid_uid(kuid_t uid)
{
	struct kqid kqid;
	kqid.type = USRQUOTA;
	kqid.uid = uid;
	return kqid;
}


static inline struct kqid make_kqid_gid(kgid_t gid)
{
	struct kqid kqid;
	kqid.type = GRPQUOTA;
	kqid.gid = gid;
	return kqid;
}


static inline struct kqid make_kqid_projid(kprojid_t projid)
{
	struct kqid kqid;
	kqid.type = PRJQUOTA;
	kqid.projid = projid;
	return kqid;
}


static inline bool qid_has_mapping(struct user_namespace *ns, struct kqid qid)
{
	return from_kqid(ns, qid) != (qid_t) -1;
}


extern spinlock_t dq_data_lock;


#define DQUOT_INIT_ALLOC max(V1_INIT_ALLOC, V2_INIT_ALLOC)
#define DQUOT_INIT_REWRITE max(V1_INIT_REWRITE, V2_INIT_REWRITE)
#define DQUOT_DEL_ALLOC max(V1_DEL_ALLOC, V2_DEL_ALLOC)
#define DQUOT_DEL_REWRITE max(V1_DEL_REWRITE, V2_DEL_REWRITE)


struct mem_dqblk {
	qsize_t dqb_bhardlimit;	
	qsize_t dqb_bsoftlimit;	
	qsize_t dqb_curspace;	
	qsize_t dqb_rsvspace;   
	qsize_t dqb_ihardlimit;	
	qsize_t dqb_isoftlimit;	
	qsize_t dqb_curinodes;	
	time64_t dqb_btime;	
	time64_t dqb_itime;	
};


struct quota_format_type;

struct mem_dqinfo {
	struct quota_format_type *dqi_format;
	int dqi_fmt_id;		
	struct list_head dqi_dirty_list;	
	unsigned long dqi_flags;
	unsigned int dqi_bgrace;
	unsigned int dqi_igrace;
	qsize_t dqi_max_spc_limit;
	qsize_t dqi_max_ino_limit;
	void *dqi_priv;
};

struct super_block;


#define DQF_GETINFO_MASK (DQF_ROOT_SQUASH | DQF_SYS_FILE)

#define DQF_SETINFO_MASK DQF_ROOT_SQUASH

enum {
	DQF_INFO_DIRTY_B = DQF_PRIVATE,
};
#define DQF_INFO_DIRTY (1 << DQF_INFO_DIRTY_B)	

extern void mark_info_dirty(struct super_block *sb, int type);
static inline int info_dirty(struct mem_dqinfo *info)
{
	return test_bit(DQF_INFO_DIRTY_B, &info->dqi_flags);
}

enum {
	DQST_LOOKUPS,
	DQST_DROPS,
	DQST_READS,
	DQST_WRITES,
	DQST_CACHE_HITS,
	DQST_ALLOC_DQUOTS,
	DQST_FREE_DQUOTS,
	DQST_SYNCS,
	_DQST_DQSTAT_LAST
};

struct dqstats {
	unsigned long stat[_DQST_DQSTAT_LAST];
	struct percpu_counter counter[_DQST_DQSTAT_LAST];
};

extern struct dqstats *dqstats_pcpu;
extern struct dqstats dqstats;

static inline void dqstats_inc(unsigned int type)
{
	percpu_counter_inc(&dqstats.counter[type]);
}

static inline void dqstats_dec(unsigned int type)
{
	percpu_counter_dec(&dqstats.counter[type]);
}

#define DQ_MOD_B	0	
#define DQ_BLKS_B	1	
#define DQ_INODES_B	2	
#define DQ_FAKE_B	3	
#define DQ_READ_B	4	
#define DQ_ACTIVE_B	5	
#define DQ_LASTSET_B	6	

struct dquot {
	struct hlist_node dq_hash;	
	struct list_head dq_inuse;	
	struct list_head dq_free;	
	struct list_head dq_dirty;	
	struct mutex dq_lock;		
	atomic_t dq_count;		
	wait_queue_head_t dq_wait_unused;	
	struct super_block *dq_sb;	
	struct kqid dq_id;		
	loff_t dq_off;			
	unsigned long dq_flags;		
	struct mem_dqblk dq_dqb;	
};


struct quota_format_ops {
	int (*check_quota_file)(struct super_block *sb, int type);	
	int (*read_file_info)(struct super_block *sb, int type);	
	int (*write_file_info)(struct super_block *sb, int type);	
	int (*free_file_info)(struct super_block *sb, int type);	
	int (*read_dqblk)(struct dquot *dquot);		
	int (*commit_dqblk)(struct dquot *dquot);	
	int (*release_dqblk)(struct dquot *dquot);	
	int (*get_next_id)(struct super_block *sb, struct kqid *qid);	
};


struct dquot_operations {
	int (*write_dquot) (struct dquot *);		
	struct dquot *(*alloc_dquot)(struct super_block *, int);	
	void (*destroy_dquot)(struct dquot *);		
	int (*acquire_dquot) (struct dquot *);		
	int (*release_dquot) (struct dquot *);		
	int (*mark_dirty) (struct dquot *);		
	int (*write_info) (struct super_block *, int);	
	
	qsize_t *(*get_reserved_space) (struct inode *);
	int (*get_projid) (struct inode *, kprojid_t *);
	
	int (*get_next_id) (struct super_block *sb, struct kqid *qid);
};

struct path;


struct qc_dqblk {
	int d_fieldmask;	
	u64 d_spc_hardlimit;	
	u64 d_spc_softlimit;	
	u64 d_ino_hardlimit;	
	u64 d_ino_softlimit;	
	u64 d_space;		
	u64 d_ino_count;	
	s64 d_ino_timer;	
				
	s64 d_spc_timer;	
	int d_ino_warns;	
	int d_spc_warns;	
	u64 d_rt_spc_hardlimit;	
	u64 d_rt_spc_softlimit;	
	u64 d_rt_space;		
	s64 d_rt_spc_timer;	
	int d_rt_spc_warns;	
};


#define	QC_INO_SOFT	(1<<0)
#define	QC_INO_HARD	(1<<1)
#define	QC_SPC_SOFT	(1<<2)
#define	QC_SPC_HARD	(1<<3)
#define	QC_RT_SPC_SOFT	(1<<4)
#define	QC_RT_SPC_HARD	(1<<5)
#define QC_LIMIT_MASK (QC_INO_SOFT | QC_INO_HARD | QC_SPC_SOFT | QC_SPC_HARD | \
		       QC_RT_SPC_SOFT | QC_RT_SPC_HARD)
#define	QC_SPC_TIMER	(1<<6)
#define	QC_INO_TIMER	(1<<7)
#define	QC_RT_SPC_TIMER	(1<<8)
#define QC_TIMER_MASK (QC_SPC_TIMER | QC_INO_TIMER | QC_RT_SPC_TIMER)
#define	QC_SPC_WARNS	(1<<9)
#define	QC_INO_WARNS	(1<<10)
#define	QC_RT_SPC_WARNS	(1<<11)
#define QC_WARNS_MASK (QC_SPC_WARNS | QC_INO_WARNS | QC_RT_SPC_WARNS)
#define	QC_SPACE	(1<<12)
#define	QC_INO_COUNT	(1<<13)
#define	QC_RT_SPACE	(1<<14)
#define QC_ACCT_MASK (QC_SPACE | QC_INO_COUNT | QC_RT_SPACE)
#define QC_FLAGS	(1<<15)

#define QCI_SYSFILE		(1 << 0)	
#define QCI_ROOT_SQUASH		(1 << 1)	
#define QCI_ACCT_ENABLED	(1 << 2)	
#define QCI_LIMITS_ENFORCED	(1 << 3)	


struct qc_type_state {
	unsigned int flags;		
	unsigned int spc_timelimit;	
	unsigned int ino_timelimit;	
	unsigned int rt_spc_timelimit;	
	unsigned int spc_warnlimit;	
	unsigned int ino_warnlimit;	
	unsigned int rt_spc_warnlimit;	
	unsigned long long ino;		
	blkcnt_t blocks;		
	blkcnt_t nextents;		
};

struct qc_state {
	unsigned int s_incoredqs;	
	
	struct qc_type_state s_state[XQM_MAXQUOTAS];
};


struct qc_info {
	int i_fieldmask;	
	unsigned int i_flags;		
	unsigned int i_spc_timelimit;	
	unsigned int i_ino_timelimit;	
	unsigned int i_rt_spc_timelimit;
	unsigned int i_spc_warnlimit;	
	unsigned int i_ino_warnlimit;	
	unsigned int i_rt_spc_warnlimit;	
};


struct quotactl_ops {
	int (*quota_on)(struct super_block *, int, int, struct path *);
	int (*quota_off)(struct super_block *, int);
	int (*quota_enable)(struct super_block *, unsigned int);
	int (*quota_disable)(struct super_block *, unsigned int);
	int (*quota_sync)(struct super_block *, int);
	int (*set_info)(struct super_block *, int, struct qc_info *);
	int (*get_dqblk)(struct super_block *, struct kqid, struct qc_dqblk *);
	int (*get_nextdqblk)(struct super_block *, struct kqid *,
			     struct qc_dqblk *);
	int (*set_dqblk)(struct super_block *, struct kqid, struct qc_dqblk *);
	int (*get_state)(struct super_block *, struct qc_state *);
	int (*rm_xquota)(struct super_block *, unsigned int);
};

struct quota_format_type {
	int qf_fmt_id;	
	const struct quota_format_ops *qf_ops;	
	struct module *qf_owner;		
	struct quota_format_type *qf_next;
};


enum {
	_DQUOT_USAGE_ENABLED = 0,		
	_DQUOT_LIMITS_ENABLED,			
	_DQUOT_SUSPENDED,			
	_DQUOT_STATE_FLAGS
};
#define DQUOT_USAGE_ENABLED	(1 << _DQUOT_USAGE_ENABLED * MAXQUOTAS)
#define DQUOT_LIMITS_ENABLED	(1 << _DQUOT_LIMITS_ENABLED * MAXQUOTAS)
#define DQUOT_SUSPENDED		(1 << _DQUOT_SUSPENDED * MAXQUOTAS)
#define DQUOT_STATE_FLAGS	(DQUOT_USAGE_ENABLED | DQUOT_LIMITS_ENABLED | \
				 DQUOT_SUSPENDED)

#define DQUOT_STATE_LAST	(_DQUOT_STATE_FLAGS * MAXQUOTAS)
#define DQUOT_QUOTA_SYS_FILE	(1 << DQUOT_STATE_LAST)
						
#define DQUOT_NEGATIVE_USAGE	(1 << (DQUOT_STATE_LAST + 1))
					       
static inline unsigned int dquot_state_flag(unsigned int flags, int type)
{
	return flags << type;
}

static inline unsigned int dquot_generic_flag(unsigned int flags, int type)
{
	return (flags >> type) & DQUOT_STATE_FLAGS;
}


static __always_inline unsigned dquot_state_types(unsigned flags, unsigned flag)
{
	BUILD_BUG_ON_NOT_POWER_OF_2(flag);
	return (flags / flag) & ((1 << MAXQUOTAS) - 1);
}

#ifdef CONFIG_QUOTA_NETLINK_INTERFACE
extern void quota_send_warning(struct kqid qid, dev_t dev,
			       const char warntype);
#else
static inline void quota_send_warning(struct kqid qid, dev_t dev,
				      const char warntype)
{
	return;
}
#endif 

struct quota_info {
	unsigned int flags;			
	struct mutex dqio_mutex;		
	struct mutex dqonoff_mutex;		
	struct inode *files[MAXQUOTAS];		
	struct mem_dqinfo info[MAXQUOTAS];	
	const struct quota_format_ops *ops[MAXQUOTAS];	
};

int register_quota_format(struct quota_format_type *fmt);
void unregister_quota_format(struct quota_format_type *fmt);

struct quota_module_name {
	int qm_fmt_id;
	char *qm_mod_name;
};

#define INIT_QUOTA_MODULE_NAMES {\
	{QFMT_VFS_OLD, "quota_v1"},\
	{QFMT_VFS_V0, "quota_v2"},\
	{QFMT_VFS_V1, "quota_v2"},\
	{0, NULL}}

#endif 
