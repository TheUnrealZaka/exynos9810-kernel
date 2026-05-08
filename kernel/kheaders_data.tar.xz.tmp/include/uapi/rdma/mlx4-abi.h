

#ifndef MLX4_ABI_USER_H
#define MLX4_ABI_USER_H

#include <linux/types.h>



#define MLX4_IB_UVERBS_NO_DEV_CAPS_ABI_VERSION	3
#define MLX4_IB_UVERBS_ABI_VERSION		4



struct mlx4_ib_alloc_ucontext_resp_v3 {
	__u32	qp_tab_size;
	__u16	bf_reg_size;
	__u16	bf_regs_per_page;
};

struct mlx4_ib_alloc_ucontext_resp {
	__u32	dev_caps;
	__u32	qp_tab_size;
	__u16	bf_reg_size;
	__u16	bf_regs_per_page;
	__u32	cqe_size;
};

struct mlx4_ib_alloc_pd_resp {
	__u32	pdn;
	__u32	reserved;
};

struct mlx4_ib_create_cq {
	__u64	buf_addr;
	__u64	db_addr;
};

struct mlx4_ib_create_cq_resp {
	__u32	cqn;
	__u32	reserved;
};

struct mlx4_ib_resize_cq {
	__u64	buf_addr;
};

struct mlx4_ib_create_srq {
	__u64	buf_addr;
	__u64	db_addr;
};

struct mlx4_ib_create_srq_resp {
	__u32	srqn;
	__u32	reserved;
};

struct mlx4_ib_create_qp {
	__u64	buf_addr;
	__u64	db_addr;
	__u8	log_sq_bb_count;
	__u8	log_sq_stride;
	__u8	sq_no_prefetch;
	__u8	reserved[5];
};

#endif 
