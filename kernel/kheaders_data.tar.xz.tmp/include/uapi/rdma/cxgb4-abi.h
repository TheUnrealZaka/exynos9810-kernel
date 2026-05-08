
#ifndef CXGB4_ABI_USER_H
#define CXGB4_ABI_USER_H

#include <linux/types.h>

#define C4IW_UVERBS_ABI_VERSION	3


struct c4iw_create_cq_resp {
	__u64 key;
	__u64 gts_key;
	__u64 memsize;
	__u32 cqid;
	__u32 size;
	__u32 qid_mask;
	__u32 reserved; 
};

enum {
	C4IW_QPF_ONCHIP = (1 << 0)
};

struct c4iw_create_qp_resp {
	__u64 ma_sync_key;
	__u64 sq_key;
	__u64 rq_key;
	__u64 sq_db_gts_key;
	__u64 rq_db_gts_key;
	__u64 sq_memsize;
	__u64 rq_memsize;
	__u32 sqid;
	__u32 rqid;
	__u32 sq_size;
	__u32 rq_size;
	__u32 qid_mask;
	__u32 flags;
};

struct c4iw_alloc_ucontext_resp {
	__u64 status_page_key;
	__u32 status_page_size;
	__u32 reserved; 
};
#endif 
