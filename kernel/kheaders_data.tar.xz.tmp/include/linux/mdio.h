
#ifndef __LINUX_MDIO_H__
#define __LINUX_MDIO_H__

#include <uapi/linux/mdio.h>

struct mii_bus;


enum mdio_mutex_lock_class {
	MDIO_MUTEX_NORMAL,
	MDIO_MUTEX_MUX,
	MDIO_MUTEX_NESTED,
};

struct mdio_device {
	struct device dev;

	const struct dev_pm_ops *pm_ops;
	struct mii_bus *bus;

	int (*bus_match)(struct device *dev, struct device_driver *drv);
	void (*device_free)(struct mdio_device *mdiodev);
	void (*device_remove)(struct mdio_device *mdiodev);

	
	int addr;
	int flags;
};
#define to_mdio_device(d) container_of(d, struct mdio_device, dev)


struct mdio_driver_common {
	struct device_driver driver;
	int flags;
};
#define MDIO_DEVICE_FLAG_PHY		1
#define to_mdio_common_driver(d) \
	container_of(d, struct mdio_driver_common, driver)


struct mdio_driver {
	struct mdio_driver_common mdiodrv;

	
	int (*probe)(struct mdio_device *mdiodev);

	
	void (*remove)(struct mdio_device *mdiodev);

	
	void (*shutdown)(struct mdio_device *mdiodev);
};
#define to_mdio_driver(d)						\
	container_of(to_mdio_common_driver(d), struct mdio_driver, mdiodrv)

void mdio_device_free(struct mdio_device *mdiodev);
struct mdio_device *mdio_device_create(struct mii_bus *bus, int addr);
int mdio_device_register(struct mdio_device *mdiodev);
void mdio_device_remove(struct mdio_device *mdiodev);
int mdio_driver_register(struct mdio_driver *drv);
void mdio_driver_unregister(struct mdio_driver *drv);

static inline bool mdio_phy_id_is_c45(int phy_id)
{
	return (phy_id & MDIO_PHY_ID_C45) && !(phy_id & ~MDIO_PHY_ID_C45_MASK);
}

static inline __u16 mdio_phy_id_prtad(int phy_id)
{
	return (phy_id & MDIO_PHY_ID_PRTAD) >> 5;
}

static inline __u16 mdio_phy_id_devad(int phy_id)
{
	return phy_id & MDIO_PHY_ID_DEVAD;
}


struct mdio_if_info {
	int prtad;
	u32 mmds;
	unsigned mode_support;

	struct net_device *dev;
	int (*mdio_read)(struct net_device *dev, int prtad, int devad,
			 u16 addr);
	int (*mdio_write)(struct net_device *dev, int prtad, int devad,
			  u16 addr, u16 val);
};

#define MDIO_PRTAD_NONE			(-1)
#define MDIO_DEVAD_NONE			(-1)
#define MDIO_SUPPORTS_C22		1
#define MDIO_SUPPORTS_C45		2
#define MDIO_EMULATE_C22		4

struct ethtool_cmd;
struct ethtool_pauseparam;
extern int mdio45_probe(struct mdio_if_info *mdio, int prtad);
extern int mdio_set_flag(const struct mdio_if_info *mdio,
			 int prtad, int devad, u16 addr, int mask,
			 bool sense);
extern int mdio45_links_ok(const struct mdio_if_info *mdio, u32 mmds);
extern int mdio45_nway_restart(const struct mdio_if_info *mdio);
extern void mdio45_ethtool_gset_npage(const struct mdio_if_info *mdio,
				      struct ethtool_cmd *ecmd,
				      u32 npage_adv, u32 npage_lpa);


static inline void mdio45_ethtool_gset(const struct mdio_if_info *mdio,
				       struct ethtool_cmd *ecmd)
{
	mdio45_ethtool_gset_npage(mdio, ecmd, 0, 0);
}

extern int mdio_mii_ioctl(const struct mdio_if_info *mdio,
			  struct mii_ioctl_data *mii_data, int cmd);


static inline u32 mmd_eee_cap_to_ethtool_sup_t(u16 eee_cap)
{
	u32 supported = 0;

	if (eee_cap & MDIO_EEE_100TX)
		supported |= SUPPORTED_100baseT_Full;
	if (eee_cap & MDIO_EEE_1000T)
		supported |= SUPPORTED_1000baseT_Full;
	if (eee_cap & MDIO_EEE_10GT)
		supported |= SUPPORTED_10000baseT_Full;
	if (eee_cap & MDIO_EEE_1000KX)
		supported |= SUPPORTED_1000baseKX_Full;
	if (eee_cap & MDIO_EEE_10GKX4)
		supported |= SUPPORTED_10000baseKX4_Full;
	if (eee_cap & MDIO_EEE_10GKR)
		supported |= SUPPORTED_10000baseKR_Full;

	return supported;
}


static inline u32 mmd_eee_adv_to_ethtool_adv_t(u16 eee_adv)
{
	u32 adv = 0;

	if (eee_adv & MDIO_EEE_100TX)
		adv |= ADVERTISED_100baseT_Full;
	if (eee_adv & MDIO_EEE_1000T)
		adv |= ADVERTISED_1000baseT_Full;
	if (eee_adv & MDIO_EEE_10GT)
		adv |= ADVERTISED_10000baseT_Full;
	if (eee_adv & MDIO_EEE_1000KX)
		adv |= ADVERTISED_1000baseKX_Full;
	if (eee_adv & MDIO_EEE_10GKX4)
		adv |= ADVERTISED_10000baseKX4_Full;
	if (eee_adv & MDIO_EEE_10GKR)
		adv |= ADVERTISED_10000baseKR_Full;

	return adv;
}


static inline u16 ethtool_adv_to_mmd_eee_adv_t(u32 adv)
{
	u16 reg = 0;

	if (adv & ADVERTISED_100baseT_Full)
		reg |= MDIO_EEE_100TX;
	if (adv & ADVERTISED_1000baseT_Full)
		reg |= MDIO_EEE_1000T;
	if (adv & ADVERTISED_10000baseT_Full)
		reg |= MDIO_EEE_10GT;
	if (adv & ADVERTISED_1000baseKX_Full)
		reg |= MDIO_EEE_1000KX;
	if (adv & ADVERTISED_10000baseKX4_Full)
		reg |= MDIO_EEE_10GKX4;
	if (adv & ADVERTISED_10000baseKR_Full)
		reg |= MDIO_EEE_10GKR;

	return reg;
}

int mdiobus_read(struct mii_bus *bus, int addr, u32 regnum);
int mdiobus_read_nested(struct mii_bus *bus, int addr, u32 regnum);
int mdiobus_write(struct mii_bus *bus, int addr, u32 regnum, u16 val);
int mdiobus_write_nested(struct mii_bus *bus, int addr, u32 regnum, u16 val);

int mdiobus_register_device(struct mdio_device *mdiodev);
int mdiobus_unregister_device(struct mdio_device *mdiodev);
bool mdiobus_is_registered_device(struct mii_bus *bus, int addr);
struct phy_device *mdiobus_get_phy(struct mii_bus *bus, int addr);


#define mdio_module_driver(_mdio_driver)				\
static int __init mdio_module_init(void)				\
{									\
	return mdio_driver_register(&_mdio_driver);			\
}									\
module_init(mdio_module_init);						\
static void __exit mdio_module_exit(void)				\
{									\
	mdio_driver_unregister(&_mdio_driver);				\
}									\
module_exit(mdio_module_exit)

#endif 
