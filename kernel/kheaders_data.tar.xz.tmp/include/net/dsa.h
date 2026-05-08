

#ifndef __LINUX_NET_DSA_H
#define __LINUX_NET_DSA_H

#include <linux/if_ether.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/of.h>
#include <linux/phy.h>
#include <linux/phy_fixed.h>
#include <linux/ethtool.h>

enum dsa_tag_protocol {
	DSA_TAG_PROTO_NONE = 0,
	DSA_TAG_PROTO_DSA,
	DSA_TAG_PROTO_TRAILER,
	DSA_TAG_PROTO_EDSA,
	DSA_TAG_PROTO_BRCM,
	DSA_TAG_PROTO_QCA,
	DSA_TAG_LAST,		
};

#define DSA_MAX_SWITCHES	4
#define DSA_MAX_PORTS		12

#define DSA_RTABLE_NONE		-1

struct dsa_chip_data {
	
	struct device	*host_dev;
	int		sw_addr;

	
	int		eeprom_len;

	
	struct device_node *of_node;

	
	char		*port_names[DSA_MAX_PORTS];
	struct device_node *port_dn[DSA_MAX_PORTS];

	
	s8		rtable[DSA_MAX_SWITCHES];
};

struct dsa_platform_data {
	
	struct device	*netdev;
	struct net_device *of_netdev;

	
	int		nr_chips;
	struct dsa_chip_data	*chip;
};

struct packet_type;

struct dsa_switch_tree {
	struct list_head	list;

	
	u32 tree;

	
	struct kref refcount;

	
	bool applied;

	
	struct dsa_platform_data	*pd;

	
	struct net_device	*master_netdev;
	int			(*rcv)(struct sk_buff *skb,
				       struct net_device *dev,
				       struct packet_type *pt,
				       struct net_device *orig_dev);

	
	struct ethtool_ops	master_ethtool_ops;
	const struct ethtool_ops *master_orig_ethtool_ops;

	
	s8			cpu_switch;
	s8			cpu_port;

	
	struct dsa_switch	*ds[DSA_MAX_SWITCHES];

	
	const struct dsa_device_ops *tag_ops;
};

struct dsa_port {
	struct net_device	*netdev;
	struct device_node	*dn;
	unsigned int		ageing_time;
	u8			stp_state;
};

struct dsa_switch {
	struct device *dev;

	
	struct dsa_switch_tree	*dst;
	int			index;

	
	void *priv;

	
	struct dsa_chip_data	*cd;

	
	struct dsa_switch_ops	*ops;

	
	s8		rtable[DSA_MAX_SWITCHES];

#ifdef CONFIG_NET_DSA_HWMON
	
	char			hwmon_name[IFNAMSIZ + 8];
	struct device		*hwmon_dev;
#endif

	
	struct net_device *master_netdev;

	
	u32			dsa_port_mask;
	u32			cpu_port_mask;
	u32			enabled_port_mask;
	u32			phys_mii_mask;
	struct dsa_port		ports[DSA_MAX_PORTS];
	struct mii_bus		*slave_mii_bus;
};

static inline bool dsa_is_cpu_port(struct dsa_switch *ds, int p)
{
	return !!(ds->index == ds->dst->cpu_switch && p == ds->dst->cpu_port);
}

static inline bool dsa_is_dsa_port(struct dsa_switch *ds, int p)
{
	return !!((ds->dsa_port_mask) & (1 << p));
}

static inline bool dsa_is_port_initialized(struct dsa_switch *ds, int p)
{
	return ds->enabled_port_mask & (1 << p) && ds->ports[p].netdev;
}

static inline u8 dsa_upstream_port(struct dsa_switch *ds)
{
	struct dsa_switch_tree *dst = ds->dst;

	
	if (dst->cpu_switch == ds->index)
		return dst->cpu_port;
	else
		return ds->rtable[dst->cpu_switch];
}

struct switchdev_trans;
struct switchdev_obj;
struct switchdev_obj_port_fdb;
struct switchdev_obj_port_mdb;
struct switchdev_obj_port_vlan;

struct dsa_switch_ops {
	struct list_head	list;

	
	const char	*(*probe)(struct device *dsa_dev,
				  struct device *host_dev, int sw_addr,
				  void **priv);

	enum dsa_tag_protocol (*get_tag_protocol)(struct dsa_switch *ds);

	int	(*setup)(struct dsa_switch *ds);
	int	(*set_addr)(struct dsa_switch *ds, u8 *addr);
	u32	(*get_phy_flags)(struct dsa_switch *ds, int port);

	
	int	(*phy_read)(struct dsa_switch *ds, int port, int regnum);
	int	(*phy_write)(struct dsa_switch *ds, int port,
			     int regnum, u16 val);

	
	void	(*adjust_link)(struct dsa_switch *ds, int port,
				struct phy_device *phydev);
	void	(*fixed_link_update)(struct dsa_switch *ds, int port,
				struct fixed_phy_status *st);

	
	void	(*get_strings)(struct dsa_switch *ds, int port, uint8_t *data);
	void	(*get_ethtool_stats)(struct dsa_switch *ds,
				     int port, uint64_t *data);
	int	(*get_sset_count)(struct dsa_switch *ds);

	
	void	(*get_wol)(struct dsa_switch *ds, int port,
			   struct ethtool_wolinfo *w);
	int	(*set_wol)(struct dsa_switch *ds, int port,
			   struct ethtool_wolinfo *w);

	
	int	(*suspend)(struct dsa_switch *ds);
	int	(*resume)(struct dsa_switch *ds);

	
	int	(*port_enable)(struct dsa_switch *ds, int port,
			       struct phy_device *phy);
	void	(*port_disable)(struct dsa_switch *ds, int port,
				struct phy_device *phy);

	
	int	(*set_eee)(struct dsa_switch *ds, int port,
			   struct phy_device *phydev,
			   struct ethtool_eee *e);
	int	(*get_eee)(struct dsa_switch *ds, int port,
			   struct ethtool_eee *e);

#ifdef CONFIG_NET_DSA_HWMON
	
	int	(*get_temp)(struct dsa_switch *ds, int *temp);
	int	(*get_temp_limit)(struct dsa_switch *ds, int *temp);
	int	(*set_temp_limit)(struct dsa_switch *ds, int temp);
	int	(*get_temp_alarm)(struct dsa_switch *ds, bool *alarm);
#endif

	
	int	(*get_eeprom_len)(struct dsa_switch *ds);
	int	(*get_eeprom)(struct dsa_switch *ds,
			      struct ethtool_eeprom *eeprom, u8 *data);
	int	(*set_eeprom)(struct dsa_switch *ds,
			      struct ethtool_eeprom *eeprom, u8 *data);

	
	int	(*get_regs_len)(struct dsa_switch *ds, int port);
	void	(*get_regs)(struct dsa_switch *ds, int port,
			    struct ethtool_regs *regs, void *p);

	
	int	(*set_ageing_time)(struct dsa_switch *ds, unsigned int msecs);
	int	(*port_bridge_join)(struct dsa_switch *ds, int port,
				    struct net_device *bridge);
	void	(*port_bridge_leave)(struct dsa_switch *ds, int port);
	void	(*port_stp_state_set)(struct dsa_switch *ds, int port,
				      u8 state);
	void	(*port_fast_age)(struct dsa_switch *ds, int port);

	
	int	(*port_vlan_filtering)(struct dsa_switch *ds, int port,
				       bool vlan_filtering);
	int	(*port_vlan_prepare)(struct dsa_switch *ds, int port,
				     const struct switchdev_obj_port_vlan *vlan,
				     struct switchdev_trans *trans);
	void	(*port_vlan_add)(struct dsa_switch *ds, int port,
				 const struct switchdev_obj_port_vlan *vlan,
				 struct switchdev_trans *trans);
	int	(*port_vlan_del)(struct dsa_switch *ds, int port,
				 const struct switchdev_obj_port_vlan *vlan);
	int	(*port_vlan_dump)(struct dsa_switch *ds, int port,
				  struct switchdev_obj_port_vlan *vlan,
				  int (*cb)(struct switchdev_obj *obj));

	
	int	(*port_fdb_prepare)(struct dsa_switch *ds, int port,
				    const struct switchdev_obj_port_fdb *fdb,
				    struct switchdev_trans *trans);
	void	(*port_fdb_add)(struct dsa_switch *ds, int port,
				const struct switchdev_obj_port_fdb *fdb,
				struct switchdev_trans *trans);
	int	(*port_fdb_del)(struct dsa_switch *ds, int port,
				const struct switchdev_obj_port_fdb *fdb);
	int	(*port_fdb_dump)(struct dsa_switch *ds, int port,
				 struct switchdev_obj_port_fdb *fdb,
				 int (*cb)(struct switchdev_obj *obj));

	
	int	(*port_mdb_prepare)(struct dsa_switch *ds, int port,
				    const struct switchdev_obj_port_mdb *mdb,
				    struct switchdev_trans *trans);
	void	(*port_mdb_add)(struct dsa_switch *ds, int port,
				const struct switchdev_obj_port_mdb *mdb,
				struct switchdev_trans *trans);
	int	(*port_mdb_del)(struct dsa_switch *ds, int port,
				const struct switchdev_obj_port_mdb *mdb);
	int	(*port_mdb_dump)(struct dsa_switch *ds, int port,
				 struct switchdev_obj_port_mdb *mdb,
				 int (*cb)(struct switchdev_obj *obj));
};

void register_switch_driver(struct dsa_switch_ops *type);
void unregister_switch_driver(struct dsa_switch_ops *type);
struct mii_bus *dsa_host_dev_to_mii_bus(struct device *dev);

static inline bool dsa_uses_tagged_protocol(struct dsa_switch_tree *dst)
{
	return dst->rcv != NULL;
}

void dsa_unregister_switch(struct dsa_switch *ds);
int dsa_register_switch(struct dsa_switch *ds, struct device_node *np);
#ifdef CONFIG_PM_SLEEP
int dsa_switch_suspend(struct dsa_switch *ds);
int dsa_switch_resume(struct dsa_switch *ds);
#else
static inline int dsa_switch_suspend(struct dsa_switch *ds)
{
	return 0;
}
static inline int dsa_switch_resume(struct dsa_switch *ds)
{
	return 0;
}
#endif 

#endif
