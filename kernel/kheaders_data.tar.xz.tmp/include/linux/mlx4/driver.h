

#ifndef MLX4_DRIVER_H
#define MLX4_DRIVER_H

#include <net/devlink.h>
#include <linux/mlx4/device.h>

struct mlx4_dev;

#define MLX4_MAC_MASK	   0xffffffffffffULL

enum mlx4_dev_event {
	MLX4_DEV_EVENT_CATASTROPHIC_ERROR,
	MLX4_DEV_EVENT_PORT_UP,
	MLX4_DEV_EVENT_PORT_DOWN,
	MLX4_DEV_EVENT_PORT_REINIT,
	MLX4_DEV_EVENT_PORT_MGMT_CHANGE,
	MLX4_DEV_EVENT_SLAVE_INIT,
	MLX4_DEV_EVENT_SLAVE_SHUTDOWN,
};

enum {
	MLX4_INTFF_BONDING	= 1 << 0
};

struct mlx4_interface {
	void *			(*add)	 (struct mlx4_dev *dev);
	void			(*remove)(struct mlx4_dev *dev, void *context);
	void			(*event) (struct mlx4_dev *dev, void *context,
					  enum mlx4_dev_event event, unsigned long param);
	void *			(*get_dev)(struct mlx4_dev *dev, void *context, u8 port);
	void			(*activate)(struct mlx4_dev *dev, void *context);
	struct list_head	list;
	enum mlx4_protocol	protocol;
	int			flags;
};

int mlx4_register_interface(struct mlx4_interface *intf);
void mlx4_unregister_interface(struct mlx4_interface *intf);

int mlx4_bond(struct mlx4_dev *dev);
int mlx4_unbond(struct mlx4_dev *dev);
static inline int mlx4_is_bonded(struct mlx4_dev *dev)
{
	return !!(dev->flags & MLX4_FLAG_BONDED);
}

static inline int mlx4_is_mf_bonded(struct mlx4_dev *dev)
{
	return (mlx4_is_bonded(dev) && mlx4_is_mfunc(dev));
}

struct mlx4_port_map {
	u8	port1;
	u8	port2;
};

int mlx4_port_map_set(struct mlx4_dev *dev, struct mlx4_port_map *v2p);

void *mlx4_get_protocol_dev(struct mlx4_dev *dev, enum mlx4_protocol proto, int port);

struct devlink_port *mlx4_get_devlink_port(struct mlx4_dev *dev, int port);

static inline u64 mlx4_mac_to_u64(u8 *addr)
{
	u64 mac = 0;
	int i;

	for (i = 0; i < ETH_ALEN; i++) {
		mac <<= 8;
		mac |= addr[i];
	}
	return mac;
}

#endif 
