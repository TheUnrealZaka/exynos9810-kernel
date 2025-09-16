
#ifndef __LINUX_BLK_TYPES_H
#define __LINUX_BLK_TYPES_H

#include <linux/types.h>
#include <linux/bvec.h>

struct bio_set;
struct bio;
struct bio_integrity_payload;
struct page;
struct block_device;
struct io_context;
struct cgroup_subsys_state;
typedef void (bio_end_io_t) (struct bio *);

#ifdef CONFIG_BLOCK

struct bio {
	struct bio		*bi_next;	
	struct block_device	*bi_bdev;
	unsigned short		bi_write_hint;
	int			bi_error;
	unsigned int		bi_opf;		
	unsigned short		bi_flags;	
	unsigned short		bi_ioprio;
	unsigned short		bi_sec_flags;	

	struct bvec_iter	bi_iter;

	
	unsigned int		bi_phys_segments;

	
	unsigned int		bi_seg_front_size;
	unsigned int		bi_seg_back_size;

	atomic_t		__bi_remaining;

	bio_end_io_t		*bi_end_io;

	void			*bi_private;
	void			*bi_aux_private;

#ifdef CONFIG_BLK_CGROUP
	
	struct io_context	*bi_ioc;
	struct cgroup_subsys_state *bi_css;
#endif
	union {
#if defined(CONFIG_BLK_DEV_INTEGRITY)
		struct bio_integrity_payload *bi_integrity; 
#endif
	};

	unsigned short		bi_vcnt;	

	

	unsigned short		bi_max_vecs;	

	atomic_t		__bi_cnt;	

	struct bio_vec		*bi_io_vec;	

	struct bio_set		*bi_pool;

#ifdef CONFIG_DDAR
	struct inode		*bi_dio_inode;
#endif

	
	struct bio_vec		bi_inline_vecs[0];
};

#define BIO_OP_SHIFT	(8 * FIELD_SIZEOF(struct bio, bi_opf) - REQ_OP_BITS)
#define bio_flags(bio)	((bio)->bi_opf & ((1 << BIO_OP_SHIFT) - 1))
#define bio_op(bio)	((bio)->bi_opf >> BIO_OP_SHIFT)

#define bio_set_op_attrs(bio, op, op_flags) do {			\
	if (__builtin_constant_p(op))					\
		BUILD_BUG_ON((op) + 0U >= (1U << REQ_OP_BITS));		\
	else								\
		WARN_ON_ONCE((op) + 0U >= (1U << REQ_OP_BITS));		\
	if (__builtin_constant_p(op_flags))				\
		BUILD_BUG_ON((op_flags) + 0U >= (1U << BIO_OP_SHIFT));	\
	else								\
		WARN_ON_ONCE((op_flags) + 0U >= (1U << BIO_OP_SHIFT));	\
	(bio)->bi_opf = bio_flags(bio);					\
	(bio)->bi_opf |= (((op) + 0U) << BIO_OP_SHIFT);			\
	(bio)->bi_opf |= (op_flags);					\
} while (0)

#define BIO_RESET_BYTES		offsetof(struct bio, bi_max_vecs)


#define BIO_SEG_VALID	1	
#define BIO_CLONED	2	
#define BIO_BOUNCED	3	
#define BIO_USER_MAPPED 4	
#define BIO_NULL_MAPPED 5	
#define BIO_WORKINGSET	6	
#define BIO_QUIET	7	
#define BIO_CHAIN	8	
#define BIO_REFFED	9	


#define BIO_RESET_BITS	10


#define BVEC_POOL_NR		6
#define BVEC_POOL_MAX		(BVEC_POOL_NR - 1)


#define BVEC_POOL_BITS		(4)
#define BVEC_POOL_OFFSET	(16 - BVEC_POOL_BITS)
#define BVEC_POOL_IDX(bio)	((bio)->bi_flags >> BVEC_POOL_OFFSET)

#endif 

#define __SEC_BYPASS	(0)
#define SEC_BYPASS	(1ULL << __SEC_BYPASS)


enum rq_flag_bits {
	
	__REQ_FAILFAST_DEV,	
	__REQ_FAILFAST_TRANSPORT, 
	__REQ_FAILFAST_DRIVER,	

	__REQ_SYNC,		
	__REQ_META,		
	__REQ_PRIO,		

	__REQ_NOIDLE,		
	__REQ_INTEGRITY,	
	__REQ_FUA,		
	__REQ_PREFLUSH,		
	__REQ_CRYPT,		

	
	__REQ_RAHEAD,		
	__REQ_THROTTLED,	

	
	__REQ_SORTED,		
	__REQ_SOFTBARRIER,	
	__REQ_NOMERGE,		
	__REQ_STARTED,		
	__REQ_DONTPREP,		
	__REQ_QUEUED,		
	__REQ_ELVPRIV,		
	__REQ_FAILED,		
	__REQ_QUIET,		
	__REQ_PREEMPT,		
	__REQ_ALLOCED,		
	__REQ_COPY_USER,	
	__REQ_FLUSH_SEQ,	
	__REQ_IO_STAT,		
	__REQ_MIXED_MERGE,	
	__REQ_PM,		
	__REQ_HASHED,		
	__REQ_MQ_INFLIGHT,	
	__REQ_NR_BITS,		
};

#define REQ_FAILFAST_DEV	(1ULL << __REQ_FAILFAST_DEV)
#define REQ_FAILFAST_TRANSPORT	(1ULL << __REQ_FAILFAST_TRANSPORT)
#define REQ_FAILFAST_DRIVER	(1ULL << __REQ_FAILFAST_DRIVER)
#define REQ_SYNC		(1ULL << __REQ_SYNC)
#define REQ_META		(1ULL << __REQ_META)
#define REQ_PRIO		(1ULL << __REQ_PRIO)
#define REQ_NOIDLE		(1ULL << __REQ_NOIDLE)
#define REQ_INTEGRITY		(1ULL << __REQ_INTEGRITY)

#define REQ_FAILFAST_MASK \
	(REQ_FAILFAST_DEV | REQ_FAILFAST_TRANSPORT | REQ_FAILFAST_DRIVER)
#define REQ_COMMON_MASK \
	(REQ_FAILFAST_MASK | REQ_SYNC | REQ_META | REQ_PRIO | REQ_NOIDLE | \
	 REQ_PREFLUSH | REQ_FUA | REQ_INTEGRITY | REQ_NOMERGE)
#define REQ_CLONE_MASK		REQ_COMMON_MASK


#define REQ_NOMERGE_FLAGS \
	(REQ_NOMERGE | REQ_STARTED | REQ_SOFTBARRIER | REQ_PREFLUSH | REQ_FUA | REQ_FLUSH_SEQ)

#define REQ_RAHEAD		(1ULL << __REQ_RAHEAD)
#define REQ_THROTTLED		(1ULL << __REQ_THROTTLED)

#define REQ_SORTED		(1ULL << __REQ_SORTED)
#define REQ_SOFTBARRIER		(1ULL << __REQ_SOFTBARRIER)
#define REQ_CRYPT		(1ULL << __REQ_CRYPT)
#define REQ_FUA			(1ULL << __REQ_FUA)
#define REQ_NOMERGE		(1ULL << __REQ_NOMERGE)
#define REQ_STARTED		(1ULL << __REQ_STARTED)
#define REQ_DONTPREP		(1ULL << __REQ_DONTPREP)
#define REQ_QUEUED		(1ULL << __REQ_QUEUED)
#define REQ_ELVPRIV		(1ULL << __REQ_ELVPRIV)
#define REQ_FAILED		(1ULL << __REQ_FAILED)
#define REQ_QUIET		(1ULL << __REQ_QUIET)
#define REQ_PREEMPT		(1ULL << __REQ_PREEMPT)
#define REQ_ALLOCED		(1ULL << __REQ_ALLOCED)
#define REQ_COPY_USER		(1ULL << __REQ_COPY_USER)
#define REQ_PREFLUSH		(1ULL << __REQ_PREFLUSH)
#define REQ_FLUSH_SEQ		(1ULL << __REQ_FLUSH_SEQ)
#define REQ_IO_STAT		(1ULL << __REQ_IO_STAT)
#define REQ_MIXED_MERGE		(1ULL << __REQ_MIXED_MERGE)
#define REQ_PM			(1ULL << __REQ_PM)
#define REQ_HASHED		(1ULL << __REQ_HASHED)
#define REQ_MQ_INFLIGHT		(1ULL << __REQ_MQ_INFLIGHT)

enum req_op {
	REQ_OP_READ,
	REQ_OP_WRITE,
	REQ_OP_DISCARD,		
	REQ_OP_SECURE_ERASE,	
	REQ_OP_WRITE_SAME,	
	REQ_OP_FLUSH,		
};

#define REQ_OP_BITS 3

typedef unsigned int blk_qc_t;
#define BLK_QC_T_NONE	-1U
#define BLK_QC_T_SHIFT	16

static inline bool blk_qc_t_valid(blk_qc_t cookie)
{
	return cookie != BLK_QC_T_NONE;
}

static inline blk_qc_t blk_tag_to_qc_t(unsigned int tag, unsigned int queue_num)
{
	return tag | (queue_num << BLK_QC_T_SHIFT);
}

static inline unsigned int blk_qc_t_to_queue_num(blk_qc_t cookie)
{
	return cookie >> BLK_QC_T_SHIFT;
}

static inline unsigned int blk_qc_t_to_tag(blk_qc_t cookie)
{
	return cookie & ((1u << BLK_QC_T_SHIFT) - 1);
}

#endif 
