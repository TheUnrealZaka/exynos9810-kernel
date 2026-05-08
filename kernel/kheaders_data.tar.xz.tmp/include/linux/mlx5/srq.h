

#ifndef MLX5_SRQ_H
#define MLX5_SRQ_H

#include <linux/mlx5/driver.h>

enum {
	MLX5_SRQ_FLAG_ERR    = (1 << 0),
	MLX5_SRQ_FLAG_WQ_SIG = (1 << 1),
};

struct mlx5_srq_attr {
	u32 type;
	u32 flags;
	u32 log_size;
	u32 wqe_shift;
	u32 log_page_size;
	u32 wqe_cnt;
	u32 srqn;
	u32 xrcd;
	u32 page_offset;
	u32 cqn;
	u32 pd;
	u32 lwm;
	u32 user_index;
	u64 db_record;
	u64 *pas;
};

struct mlx5_core_dev;

void mlx5_init_srq_table(struct mlx5_core_dev *dev);
void mlx5_cleanup_srq_table(struct mlx5_core_dev *dev);

#endif 
