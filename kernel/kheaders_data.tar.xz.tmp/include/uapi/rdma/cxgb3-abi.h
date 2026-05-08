
#ifndef CXGB3_ABI_USER_H
#define CXGB3_ABI_USER_H

#include <linux/types.h>

#define IWCH_UVERBS_ABI_VERSION	1


struct iwch_create_cq_req {
	__u64 user_rptr_addr;
};

struct iwch_create_cq_resp_v0 {
	__u64 key;
	__u32 cqid;
	__u32 size_log2;
};

struct iwch_create_cq_resp {
	__u64 key;
	__u32 cqid;
	__u32 size_log2;
	__u32 memsize;
	__u32 reserved;
};

struct iwch_create_qp_resp {
	__u64 key;
	__u64 db_key;
	__u32 qpid;
	__u32 size_log2;
	__u32 sq_size_log2;
	__u32 rq_size_log2;
};

struct iwch_reg_user_mr_resp {
	__u32 pbl_addr;
};
#endif 
