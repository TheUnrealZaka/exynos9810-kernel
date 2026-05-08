

#ifndef __PHY_H
#define __PHY_H

#include <linux/compiler.h>
#include <linux/spinlock.h>
#include <linux/ethtool.h>
#include <linux/mdio.h>
#include <linux/mii.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/mod_devicetable.h>

#include <linux/atomic.h>

#define PHY_DEFAULT_FEATURES	(SUPPORTED_Autoneg | \
				 SUPPORTED_TP | \
				 SUPPORTED_MII)

#define PHY_10BT_FEATURES	(SUPPORTED_10baseT_Half | \
				 SUPPORTED_10baseT_Full)

#define PHY_100BT_FEATURES	(SUPPORTED_100baseT_Half | \
				 SUPPORTED_100baseT_Full)

#define PHY_1000BT_FEATURES	(SUPPORTED_1000baseT_Half | \
				 SUPPORTED_1000baseT_Full)

#define PHY_BASIC_FEATURES	(PHY_10BT_FEATURES | \
				 PHY_100BT_FEATURES | \
				 PHY_DEFAULT_FEATURES)

#define PHY_GBIT_FEATURES	(PHY_BASIC_FEATURES | \
				 PHY_1000BT_FEATURES)



#define PHY_POLL		-1
#define PHY_IGNORE_INTERRUPT	-2

#define PHY_HAS_INTERRUPT	0x00000001
#define PHY_HAS_MAGICANEG	0x00000002
#define PHY_IS_INTERNAL		0x00000004
#define MDIO_DEVICE_IS_PHY	0x80000000


typedef enum {
	PHY_INTERFACE_MODE_NA,
	PHY_INTERFACE_MODE_MII,
	PHY_INTERFACE_MODE_GMII,
	PHY_INTERFACE_MODE_SGMII,
	PHY_INTERFACE_MODE_TBI,
	PHY_INTERFACE_MODE_REVMII,
	PHY_INTERFACE_MODE_RMII,
	PHY_INTERFACE_MODE_RGMII,
	PHY_INTERFACE_MODE_RGMII_ID,
	PHY_INTERFACE_MODE_RGMII_RXID,
	PHY_INTERFACE_MODE_RGMII_TXID,
	PHY_INTERFACE_MODE_RTBI,
	PHY_INTERFACE_MODE_SMII,
	PHY_INTERFACE_MODE_XGMII,
	PHY_INTERFACE_MODE_MOCA,
	PHY_INTERFACE_MODE_QSGMII,
	PHY_INTERFACE_MODE_TRGMII,
	PHY_INTERFACE_MODE_MAX,
} phy_interface_t;


static inline const char *phy_modes(phy_interface_t interface)
{
	switch (interface) {
	case PHY_INTERFACE_MODE_NA:
		return "";
	case PHY_INTERFACE_MODE_MII:
		return "mii";
	case PHY_INTERFACE_MODE_GMII:
		return "gmii";
	case PHY_INTERFACE_MODE_SGMII:
		return "sgmii";
	case PHY_INTERFACE_MODE_TBI:
		return "tbi";
	case PHY_INTERFACE_MODE_REVMII:
		return "rev-mii";
	case PHY_INTERFACE_MODE_RMII:
		return "rmii";
	case PHY_INTERFACE_MODE_RGMII:
		return "rgmii";
	case PHY_INTERFACE_MODE_RGMII_ID:
		return "rgmii-id";
	case PHY_INTERFACE_MODE_RGMII_RXID:
		return "rgmii-rxid";
	case PHY_INTERFACE_MODE_RGMII_TXID:
		return "rgmii-txid";
	case PHY_INTERFACE_MODE_RTBI:
		return "rtbi";
	case PHY_INTERFACE_MODE_SMII:
		return "smii";
	case PHY_INTERFACE_MODE_XGMII:
		return "xgmii";
	case PHY_INTERFACE_MODE_MOCA:
		return "moca";
	case PHY_INTERFACE_MODE_QSGMII:
		return "qsgmii";
	case PHY_INTERFACE_MODE_TRGMII:
		return "trgmii";
	default:
		return "unknown";
	}
}


#define PHY_INIT_TIMEOUT	100000
#define PHY_STATE_TIME		1
#define PHY_FORCE_TIMEOUT	10
#define PHY_AN_TIMEOUT		10

#define PHY_MAX_ADDR	32


#define PHY_ID_FMT "%s:%02x"

#define MII_BUS_ID_SIZE	61


#define MII_ADDR_C45 (1<<30)

struct device;
struct sk_buff;


struct mii_bus {
	struct module *owner;
	const char *name;
	char id[MII_BUS_ID_SIZE];
	void *priv;
	int (*read)(struct mii_bus *bus, int addr, int regnum);
	int (*write)(struct mii_bus *bus, int addr, int regnum, u16 val);
	int (*reset)(struct mii_bus *bus);

	
	struct mutex mdio_lock;

	struct device *parent;
	enum {
		MDIOBUS_ALLOCATED = 1,
		MDIOBUS_REGISTERED,
		MDIOBUS_UNREGISTERED,
		MDIOBUS_RELEASED,
	} state;
	struct device dev;

	
	struct mdio_device *mdio_map[PHY_MAX_ADDR];

	
	u32 phy_mask;

	
	u32 phy_ignore_ta_mask;

	
	int irq[PHY_MAX_ADDR];
};
#define to_mii_bus(d) container_of(d, struct mii_bus, dev)

struct mii_bus *mdiobus_alloc_size(size_t);
static inline struct mii_bus *mdiobus_alloc(void)
{
	return mdiobus_alloc_size(0);
}

int __mdiobus_register(struct mii_bus *bus, struct module *owner);
#define mdiobus_register(bus) __mdiobus_register(bus, THIS_MODULE)
void mdiobus_unregister(struct mii_bus *bus);
void mdiobus_free(struct mii_bus *bus);
struct mii_bus *devm_mdiobus_alloc_size(struct device *dev, int sizeof_priv);
static inline struct mii_bus *devm_mdiobus_alloc(struct device *dev)
{
	return devm_mdiobus_alloc_size(dev, 0);
}

void devm_mdiobus_free(struct device *dev, struct mii_bus *bus);
struct phy_device *mdiobus_scan(struct mii_bus *bus, int addr);

#define PHY_INTERRUPT_DISABLED	0x0
#define PHY_INTERRUPT_ENABLED	0x80000000


enum phy_state {
	PHY_DOWN = 0,
	PHY_STARTING,
	PHY_READY,
	PHY_PENDING,
	PHY_UP,
	PHY_AN,
	PHY_RUNNING,
	PHY_NOLINK,
	PHY_FORCING,
	PHY_CHANGELINK,
	PHY_HALTED,
	PHY_RESUMING
};


struct phy_c45_device_ids {
	u32 devices_in_package;
	u32 device_ids[8];
};


struct phy_device {
	struct mdio_device mdio;

	
	
	struct phy_driver *drv;

	u32 phy_id;

	struct phy_c45_device_ids c45_ids;
	bool is_c45;
	bool is_internal;
	bool is_pseudo_fixed_link;
	bool has_fixups;
	bool suspended;
	bool suspended_by_mdio_bus;

	enum phy_state state;

	u32 dev_flags;

	phy_interface_t interface;

	
	int speed;
	int duplex;
	int pause;
	int asym_pause;

	
	int link;

	
	u32 interrupts;

	
	
	u32 supported;
	u32 advertising;
	u32 lp_advertising;

	
	u32 eee_broken_modes;

	int autoneg;

	int link_timeout;

	
	int irq;

	
	
	void *priv;

	
	struct work_struct phy_queue;
	struct delayed_work state_queue;
	atomic_t irq_disable;

	struct mutex lock;

	struct net_device *attached_dev;

	u8 mdix;

	void (*adjust_link)(struct net_device *dev);
};
#define to_phy_device(d) container_of(to_mdio_device(d), \
				      struct phy_device, mdio)


struct phy_driver {
	struct mdio_driver_common mdiodrv;
	u32 phy_id;
	char *name;
	unsigned int phy_id_mask;
	u32 features;
	u32 flags;
	const void *driver_data;

	
	int (*soft_reset)(struct phy_device *phydev);

	
	int (*config_init)(struct phy_device *phydev);

	
	int (*probe)(struct phy_device *phydev);

	
	int (*suspend)(struct phy_device *phydev);
	int (*resume)(struct phy_device *phydev);

	
	int (*config_aneg)(struct phy_device *phydev);

	
	int (*aneg_done)(struct phy_device *phydev);

	
	int (*read_status)(struct phy_device *phydev);

	
	int (*ack_interrupt)(struct phy_device *phydev);

	
	int (*config_intr)(struct phy_device *phydev);

	
	int (*did_interrupt)(struct phy_device *phydev);

	
	void (*remove)(struct phy_device *phydev);

	
	int (*match_phy_device)(struct phy_device *phydev);

	
	int (*ts_info)(struct phy_device *phydev, struct ethtool_ts_info *ti);

	
	int  (*hwtstamp)(struct phy_device *phydev, struct ifreq *ifr);

	
	bool (*rxtstamp)(struct phy_device *dev, struct sk_buff *skb, int type);

	
	void (*txtstamp)(struct phy_device *dev, struct sk_buff *skb, int type);

	
	int (*set_wol)(struct phy_device *dev, struct ethtool_wolinfo *wol);

	
	void (*get_wol)(struct phy_device *dev, struct ethtool_wolinfo *wol);

	
	void (*link_change_notify)(struct phy_device *dev);

	
	int (*read_mmd_indirect)(struct phy_device *dev, int ptrad,
				 int devnum, int regnum);

	
	void (*write_mmd_indirect)(struct phy_device *dev, int ptrad,
				   int devnum, int regnum, u32 val);

	
	int (*module_info)(struct phy_device *dev,
			   struct ethtool_modinfo *modinfo);

	
	int (*module_eeprom)(struct phy_device *dev,
			     struct ethtool_eeprom *ee, u8 *data);

	
	int (*get_sset_count)(struct phy_device *dev);
	void (*get_strings)(struct phy_device *dev, u8 *data);
	void (*get_stats)(struct phy_device *dev,
			  struct ethtool_stats *stats, u64 *data);
};
#define to_phy_driver(d) container_of(to_mdio_common_driver(d),		\
				      struct phy_driver, mdiodrv)

#define PHY_ANY_ID "MATCH ANY PHY"
#define PHY_ANY_UID 0xffffffff


struct phy_fixup {
	struct list_head list;
	char bus_id[MII_BUS_ID_SIZE + 3];
	u32 phy_uid;
	u32 phy_uid_mask;
	int (*run)(struct phy_device *phydev);
};


static inline int phy_read_mmd(struct phy_device *phydev, int devad, u32 regnum)
{
	if (!phydev->is_c45)
		return -EOPNOTSUPP;

	return mdiobus_read(phydev->mdio.bus, phydev->mdio.addr,
			    MII_ADDR_C45 | (devad << 16) | (regnum & 0xffff));
}


int phy_read_mmd_indirect(struct phy_device *phydev, int prtad, int devad);


static inline int phy_read(struct phy_device *phydev, u32 regnum)
{
	return mdiobus_read(phydev->mdio.bus, phydev->mdio.addr, regnum);
}


static inline int phy_write(struct phy_device *phydev, u32 regnum, u16 val)
{
	return mdiobus_write(phydev->mdio.bus, phydev->mdio.addr, regnum, val);
}


static inline bool phy_interrupt_is_valid(struct phy_device *phydev)
{
	return phydev->irq != PHY_POLL && phydev->irq != PHY_IGNORE_INTERRUPT;
}


static inline bool phy_is_internal(struct phy_device *phydev)
{
	return phydev->is_internal;
}


static inline bool phy_interface_mode_is_rgmii(phy_interface_t mode)
{
	return mode >= PHY_INTERFACE_MODE_RGMII &&
		mode <= PHY_INTERFACE_MODE_RGMII_TXID;
};


static inline bool phy_interface_is_rgmii(struct phy_device *phydev)
{
	return phydev->interface >= PHY_INTERFACE_MODE_RGMII &&
		phydev->interface <= PHY_INTERFACE_MODE_RGMII_TXID;
};


static inline bool phy_is_pseudo_fixed_link(struct phy_device *phydev)
{
	return phydev->is_pseudo_fixed_link;
}


static inline int phy_write_mmd(struct phy_device *phydev, int devad,
				u32 regnum, u16 val)
{
	if (!phydev->is_c45)
		return -EOPNOTSUPP;

	regnum = MII_ADDR_C45 | ((devad & 0x1f) << 16) | (regnum & 0xffff);

	return mdiobus_write(phydev->mdio.bus, phydev->mdio.addr, regnum, val);
}


void phy_write_mmd_indirect(struct phy_device *phydev, int prtad,
			    int devad, u32 data);

struct phy_device *phy_device_create(struct mii_bus *bus, int addr, int phy_id,
				     bool is_c45,
				     struct phy_c45_device_ids *c45_ids);
struct phy_device *get_phy_device(struct mii_bus *bus, int addr, bool is_c45);
int phy_device_register(struct phy_device *phy);
void phy_device_remove(struct phy_device *phydev);
int phy_init_hw(struct phy_device *phydev);
int phy_suspend(struct phy_device *phydev);
int phy_resume(struct phy_device *phydev);
struct phy_device *phy_attach(struct net_device *dev, const char *bus_id,
			      phy_interface_t interface);
struct phy_device *phy_find_first(struct mii_bus *bus);
int phy_attach_direct(struct net_device *dev, struct phy_device *phydev,
		      u32 flags, phy_interface_t interface);
int phy_connect_direct(struct net_device *dev, struct phy_device *phydev,
		       void (*handler)(struct net_device *),
		       phy_interface_t interface);
struct phy_device *phy_connect(struct net_device *dev, const char *bus_id,
			       void (*handler)(struct net_device *),
			       phy_interface_t interface);
void phy_disconnect(struct phy_device *phydev);
void phy_detach(struct phy_device *phydev);
void phy_start(struct phy_device *phydev);
void phy_stop(struct phy_device *phydev);
int phy_start_aneg(struct phy_device *phydev);

int phy_stop_interrupts(struct phy_device *phydev);

static inline int phy_read_status(struct phy_device *phydev)
{
	return phydev->drv->read_status(phydev);
}

#define phydev_err(_phydev, format, args...)	\
	dev_err(&_phydev->mdio.dev, format, ##args)

#define phydev_dbg(_phydev, format, args...)	\
	dev_dbg(&_phydev->mdio.dev, format, ##args);

static inline const char *phydev_name(const struct phy_device *phydev)
{
	return dev_name(&phydev->mdio.dev);
}

void phy_attached_print(struct phy_device *phydev, const char *fmt, ...)
	__printf(2, 3);
void phy_attached_info(struct phy_device *phydev);
int genphy_config_init(struct phy_device *phydev);
int genphy_setup_forced(struct phy_device *phydev);
int genphy_restart_aneg(struct phy_device *phydev);
int genphy_config_aneg(struct phy_device *phydev);
int genphy_aneg_done(struct phy_device *phydev);
int genphy_update_link(struct phy_device *phydev);
int genphy_read_status(struct phy_device *phydev);
int genphy_suspend(struct phy_device *phydev);
int genphy_resume(struct phy_device *phydev);
int genphy_soft_reset(struct phy_device *phydev);
static inline int genphy_no_soft_reset(struct phy_device *phydev)
{
	return 0;
}
void phy_driver_unregister(struct phy_driver *drv);
void phy_drivers_unregister(struct phy_driver *drv, int n);
int phy_driver_register(struct phy_driver *new_driver, struct module *owner);
int phy_drivers_register(struct phy_driver *new_driver, int n,
			 struct module *owner);
void phy_state_machine(struct work_struct *work);
void phy_change(struct work_struct *work);
void phy_mac_interrupt(struct phy_device *phydev, int new_link);
void phy_start_machine(struct phy_device *phydev);
void phy_stop_machine(struct phy_device *phydev);
void phy_trigger_machine(struct phy_device *phydev, bool sync);
int phy_ethtool_sset(struct phy_device *phydev, struct ethtool_cmd *cmd);
int phy_ethtool_gset(struct phy_device *phydev, struct ethtool_cmd *cmd);
int phy_ethtool_ksettings_get(struct phy_device *phydev,
			      struct ethtool_link_ksettings *cmd);
int phy_ethtool_ksettings_set(struct phy_device *phydev,
			      const struct ethtool_link_ksettings *cmd);
int phy_mii_ioctl(struct phy_device *phydev, struct ifreq *ifr, int cmd);
int phy_start_interrupts(struct phy_device *phydev);
void phy_print_status(struct phy_device *phydev);
void phy_device_free(struct phy_device *phydev);
int phy_set_max_speed(struct phy_device *phydev, u32 max_speed);

int phy_register_fixup(const char *bus_id, u32 phy_uid, u32 phy_uid_mask,
		       int (*run)(struct phy_device *));
int phy_register_fixup_for_id(const char *bus_id,
			      int (*run)(struct phy_device *));
int phy_register_fixup_for_uid(u32 phy_uid, u32 phy_uid_mask,
			       int (*run)(struct phy_device *));

int phy_init_eee(struct phy_device *phydev, bool clk_stop_enable);
int phy_get_eee_err(struct phy_device *phydev);
int phy_ethtool_set_eee(struct phy_device *phydev, struct ethtool_eee *data);
int phy_ethtool_get_eee(struct phy_device *phydev, struct ethtool_eee *data);
int phy_ethtool_set_wol(struct phy_device *phydev, struct ethtool_wolinfo *wol);
void phy_ethtool_get_wol(struct phy_device *phydev,
			 struct ethtool_wolinfo *wol);
int phy_ethtool_get_link_ksettings(struct net_device *ndev,
				   struct ethtool_link_ksettings *cmd);
int phy_ethtool_set_link_ksettings(struct net_device *ndev,
				   const struct ethtool_link_ksettings *cmd);

int __init mdio_bus_init(void);
void mdio_bus_exit(void);

extern struct bus_type mdio_bus_type;


#define phy_module_driver(__phy_drivers, __count)			\
static int __init phy_module_init(void)					\
{									\
	return phy_drivers_register(__phy_drivers, __count, THIS_MODULE); \
}									\
module_init(phy_module_init);						\
static void __exit phy_module_exit(void)				\
{									\
	phy_drivers_unregister(__phy_drivers, __count);			\
}									\
module_exit(phy_module_exit)

#define module_phy_driver(__phy_drivers)				\
	phy_module_driver(__phy_drivers, ARRAY_SIZE(__phy_drivers))

#endif 
