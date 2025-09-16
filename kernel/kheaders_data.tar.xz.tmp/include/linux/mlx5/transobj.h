

#ifndef __TRANSOBJ_H__
#define __TRANSOBJ_H__

#include <linux/mlx5/driver.h>

int mlx5_core_alloc_transport_domain(struct mlx5_core_dev *dev, u32 *tdn);
void mlx5_core_dealloc_transport_domain(struct mlx5_core_dev *dev, u32 tdn);
int mlx5_core_create_rq(struct mlx5_core_dev *dev, u32 *in, int inlen,
			u32 *rqn);
int mlx5_core_modify_rq(struct mlx5_core_dev *dev, u32 rqn, u32 *in, int inlen);
void mlx5_core_destroy_rq(struct mlx5_core_dev *dev, u32 rqn);
int mlx5_core_query_rq(struct mlx5_core_dev *dev, u32 rqn, u32 *out);
int mlx5_core_create_sq(struct mlx5_core_dev *dev, u32 *in, int inlen,
			u32 *sqn);
int mlx5_core_modify_sq(struct mlx5_core_dev *dev, u32 sqn, u32 *in, int inlen);
void mlx5_core_destroy_sq(struct mlx5_core_dev *dev, u32 sqn);
int mlx5_core_query_sq(struct mlx5_core_dev *dev, u32 sqn, u32 *out);
int mlx5_core_create_tir(struct mlx5_core_dev *dev, u32 *in, int inlen,
			 u32 *tirn);
int mlx5_core_modify_tir(struct mlx5_core_dev *dev, u32 tirn, u32 *in,
			 int inlen);
void mlx5_core_destroy_tir(struct mlx5_core_dev *dev, u32 tirn);
int mlx5_core_create_tis(struct mlx5_core_dev *dev, u32 *in, int inlen,
			 u32 *tisn);
int mlx5_core_modify_tis(struct mlx5_core_dev *dev, u32 tisn, u32 *in,
			 int inlen);
void mlx5_core_destroy_tis(struct mlx5_core_dev *dev, u32 tisn);
int mlx5_core_create_rmp(struct mlx5_core_dev *dev, u32 *in, int inlen,
			 u32 *rmpn);
int mlx5_core_modify_rmp(struct mlx5_core_dev *dev, u32 *in, int inlen);
int mlx5_core_destroy_rmp(struct mlx5_core_dev *dev, u32 rmpn);
int mlx5_core_query_rmp(struct mlx5_core_dev *dev, u32 rmpn, u32 *out);
int mlx5_core_arm_rmp(struct mlx5_core_dev *dev, u32 rmpn, u16 lwm);
int mlx5_core_create_xsrq(struct mlx5_core_dev *dev, u32 *in, int inlen,
			  u32 *rmpn);
int mlx5_core_destroy_xsrq(struct mlx5_core_dev *dev, u32 rmpn);
int mlx5_core_query_xsrq(struct mlx5_core_dev *dev, u32 rmpn, u32 *out);
int mlx5_core_arm_xsrq(struct mlx5_core_dev *dev, u32 rmpn, u16 lwm);

int mlx5_core_create_rqt(struct mlx5_core_dev *dev, u32 *in, int inlen,
			 u32 *rqtn);
int mlx5_core_modify_rqt(struct mlx5_core_dev *dev, u32 rqtn, u32 *in,
			 int inlen);
void mlx5_core_destroy_rqt(struct mlx5_core_dev *dev, u32 rqtn);

#endif 
