

#ifndef __MLX5_VPORT_H__
#define __MLX5_VPORT_H__

#include <linux/mlx5/driver.h>
#include <linux/mlx5/device.h>

u8 mlx5_query_vport_state(struct mlx5_core_dev *mdev, u8 opmod, u16 vport);
u8 mlx5_query_vport_admin_state(struct mlx5_core_dev *mdev, u8 opmod,
				u16 vport);
int mlx5_modify_vport_admin_state(struct mlx5_core_dev *mdev, u8 opmod,
				  u16 vport, u8 state);
int mlx5_query_nic_vport_mac_address(struct mlx5_core_dev *mdev,
				     u16 vport, u8 *addr);
void mlx5_query_nic_vport_min_inline(struct mlx5_core_dev *mdev,
				     u8 *min_inline);
int mlx5_modify_nic_vport_min_inline(struct mlx5_core_dev *mdev,
				     u16 vport, u8 min_inline);
int mlx5_modify_nic_vport_mac_address(struct mlx5_core_dev *dev,
				      u16 vport, u8 *addr);
int mlx5_query_nic_vport_mtu(struct mlx5_core_dev *mdev, u16 *mtu);
int mlx5_modify_nic_vport_mtu(struct mlx5_core_dev *mdev, u16 mtu);
int mlx5_query_nic_vport_system_image_guid(struct mlx5_core_dev *mdev,
					   u64 *system_image_guid);
int mlx5_query_nic_vport_node_guid(struct mlx5_core_dev *mdev, u64 *node_guid);
int mlx5_modify_nic_vport_node_guid(struct mlx5_core_dev *mdev,
				    u32 vport, u64 node_guid);
int mlx5_query_nic_vport_qkey_viol_cntr(struct mlx5_core_dev *mdev,
					u16 *qkey_viol_cntr);
int mlx5_query_hca_vport_gid(struct mlx5_core_dev *dev, u8 other_vport,
			     u8 port_num, u16  vf_num, u16 gid_index,
			     union ib_gid *gid);
int mlx5_query_hca_vport_pkey(struct mlx5_core_dev *dev, u8 other_vport,
			      u8 port_num, u16 vf_num, u16 pkey_index,
			      u16 *pkey);
int mlx5_query_hca_vport_context(struct mlx5_core_dev *dev,
				 u8 other_vport, u8 port_num,
				 u16 vf_num,
				 struct mlx5_hca_vport_context *rep);
int mlx5_query_hca_vport_system_image_guid(struct mlx5_core_dev *dev,
					   u64 *sys_image_guid);
int mlx5_query_hca_vport_node_guid(struct mlx5_core_dev *dev,
				   u64 *node_guid);
int mlx5_query_nic_vport_mac_list(struct mlx5_core_dev *dev,
				  u32 vport,
				  enum mlx5_list_type list_type,
				  u8 addr_list[][ETH_ALEN],
				  int *list_size);
int mlx5_modify_nic_vport_mac_list(struct mlx5_core_dev *dev,
				   enum mlx5_list_type list_type,
				   u8 addr_list[][ETH_ALEN],
				   int list_size);
int mlx5_query_nic_vport_promisc(struct mlx5_core_dev *mdev,
				 u32 vport,
				 int *promisc_uc,
				 int *promisc_mc,
				 int *promisc_all);
int mlx5_modify_nic_vport_promisc(struct mlx5_core_dev *mdev,
				  int promisc_uc,
				  int promisc_mc,
				  int promisc_all);
int mlx5_query_nic_vport_vlans(struct mlx5_core_dev *dev,
			       u32 vport,
			       u16 vlans[],
			       int *size);
int mlx5_modify_nic_vport_vlans(struct mlx5_core_dev *dev,
				u16 vlans[],
				int list_size);

int mlx5_nic_vport_enable_roce(struct mlx5_core_dev *mdev);
int mlx5_nic_vport_disable_roce(struct mlx5_core_dev *mdev);
int mlx5_core_query_vport_counter(struct mlx5_core_dev *dev, u8 other_vport,
				  int vf, u8 port_num, void *out,
				  size_t out_sz);
int mlx5_core_modify_hca_vport_context(struct mlx5_core_dev *dev,
				       u8 other_vport, u8 port_num,
				       int vf,
				       struct mlx5_hca_vport_context *req);

#endif 
