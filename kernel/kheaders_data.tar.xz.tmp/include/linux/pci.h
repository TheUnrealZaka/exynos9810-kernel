
#ifndef LINUX_PCI_H
#define LINUX_PCI_H


#include <linux/mod_devicetable.h>

#include <linux/types.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/list.h>
#include <linux/compiler.h>
#include <linux/errno.h>
#include <linux/kobject.h>
#include <linux/atomic.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/resource_ext.h>
#include <uapi/linux/pci.h>

#include <linux/pci_ids.h>


#define PCI_DEVID(bus, devfn)  ((((u16)(bus)) << 8) | (devfn))

#define PCI_BUS_NUM(x) (((x) >> 8) & 0xff)


struct pci_slot {
	struct pci_bus *bus;		
	struct list_head list;		
	struct hotplug_slot *hotplug;	
	unsigned char number;		
	struct kobject kobj;
};

static inline const char *pci_slot_name(const struct pci_slot *slot)
{
	return kobject_name(&slot->kobj);
}


enum pci_mmap_state {
	pci_mmap_io,
	pci_mmap_mem
};


enum {
	
	PCI_STD_RESOURCES,
	PCI_STD_RESOURCE_END = 5,

	
	PCI_ROM_RESOURCE,

	
#ifdef CONFIG_PCI_IOV
	PCI_IOV_RESOURCES,
	PCI_IOV_RESOURCE_END = PCI_IOV_RESOURCES + PCI_SRIOV_NUM_BARS - 1,
#endif

	
#define PCI_BRIDGE_RESOURCE_NUM 4

	PCI_BRIDGE_RESOURCES,
	PCI_BRIDGE_RESOURCE_END = PCI_BRIDGE_RESOURCES +
				  PCI_BRIDGE_RESOURCE_NUM - 1,

	
	PCI_NUM_RESOURCES,

	
	DEVICE_COUNT_RESOURCE = PCI_NUM_RESOURCES,
};


typedef int __bitwise pci_power_t;

#define PCI_D0		((pci_power_t __force) 0)
#define PCI_D1		((pci_power_t __force) 1)
#define PCI_D2		((pci_power_t __force) 2)
#define PCI_D3hot	((pci_power_t __force) 3)
#define PCI_D3cold	((pci_power_t __force) 4)
#define PCI_UNKNOWN	((pci_power_t __force) 5)
#define PCI_POWER_ERROR	((pci_power_t __force) -1)


extern const char *pci_power_names[];

static inline const char *pci_power_name(pci_power_t state)
{
	return pci_power_names[1 + (__force int) state];
}

#define PCI_PM_D2_DELAY		200
#define PCI_PM_D3_WAIT		10
#define PCI_PM_D3COLD_WAIT	100
#define PCI_PM_BUS_WAIT		50


typedef unsigned int __bitwise pci_channel_state_t;

enum pci_channel_state {
	
	pci_channel_io_normal = (__force pci_channel_state_t) 1,

	
	pci_channel_io_frozen = (__force pci_channel_state_t) 2,

	
	pci_channel_io_perm_failure = (__force pci_channel_state_t) 3,
};

typedef unsigned int __bitwise pcie_reset_state_t;

enum pcie_reset_state {
	
	pcie_deassert_reset = (__force pcie_reset_state_t) 1,

	
	pcie_warm_reset = (__force pcie_reset_state_t) 2,

	
	pcie_hot_reset = (__force pcie_reset_state_t) 3
};

typedef unsigned short __bitwise pci_dev_flags_t;
enum pci_dev_flags {
	
	PCI_DEV_FLAGS_MSI_INTX_DISABLE_BUG = (__force pci_dev_flags_t) (1 << 0),
	
	PCI_DEV_FLAGS_NO_D3 = (__force pci_dev_flags_t) (1 << 1),
	
	PCI_DEV_FLAGS_ASSIGNED = (__force pci_dev_flags_t) (1 << 2),
	
	PCI_DEV_FLAGS_ACS_ENABLED_QUIRK = (__force pci_dev_flags_t) (1 << 3),
	
	PCI_DEV_FLAG_PCIE_BRIDGE_ALIAS = (__force pci_dev_flags_t) (1 << 5),
	
	PCI_DEV_FLAGS_NO_BUS_RESET = (__force pci_dev_flags_t) (1 << 6),
	
	PCI_DEV_FLAGS_NO_PM_RESET = (__force pci_dev_flags_t) (1 << 7),
	
	PCI_DEV_FLAGS_VPD_REF_F0 = (__force pci_dev_flags_t) (1 << 8),
	
	PCI_DEV_FLAGS_NEEDS_RESUME = (__force pci_dev_flags_t) (1 << 11),
};

enum pci_irq_reroute_variant {
	INTEL_IRQ_REROUTE_VARIANT = 1,
	MAX_IRQ_REROUTE_VARIANTS = 3
};

typedef unsigned short __bitwise pci_bus_flags_t;
enum pci_bus_flags {
	PCI_BUS_FLAGS_NO_MSI	= (__force pci_bus_flags_t) 1,
	PCI_BUS_FLAGS_NO_MMRBC	= (__force pci_bus_flags_t) 2,
	PCI_BUS_FLAGS_NO_AERSID	= (__force pci_bus_flags_t) 4,
};


enum pcie_link_width {
	PCIE_LNK_WIDTH_RESRV	= 0x00,
	PCIE_LNK_X1		= 0x01,
	PCIE_LNK_X2		= 0x02,
	PCIE_LNK_X4		= 0x04,
	PCIE_LNK_X8		= 0x08,
	PCIE_LNK_X12		= 0x0C,
	PCIE_LNK_X16		= 0x10,
	PCIE_LNK_X32		= 0x20,
	PCIE_LNK_WIDTH_UNKNOWN  = 0xFF,
};


enum pci_bus_speed {
	PCI_SPEED_33MHz			= 0x00,
	PCI_SPEED_66MHz			= 0x01,
	PCI_SPEED_66MHz_PCIX		= 0x02,
	PCI_SPEED_100MHz_PCIX		= 0x03,
	PCI_SPEED_133MHz_PCIX		= 0x04,
	PCI_SPEED_66MHz_PCIX_ECC	= 0x05,
	PCI_SPEED_100MHz_PCIX_ECC	= 0x06,
	PCI_SPEED_133MHz_PCIX_ECC	= 0x07,
	PCI_SPEED_66MHz_PCIX_266	= 0x09,
	PCI_SPEED_100MHz_PCIX_266	= 0x0a,
	PCI_SPEED_133MHz_PCIX_266	= 0x0b,
	AGP_UNKNOWN			= 0x0c,
	AGP_1X				= 0x0d,
	AGP_2X				= 0x0e,
	AGP_4X				= 0x0f,
	AGP_8X				= 0x10,
	PCI_SPEED_66MHz_PCIX_533	= 0x11,
	PCI_SPEED_100MHz_PCIX_533	= 0x12,
	PCI_SPEED_133MHz_PCIX_533	= 0x13,
	PCIE_SPEED_2_5GT		= 0x14,
	PCIE_SPEED_5_0GT		= 0x15,
	PCIE_SPEED_8_0GT		= 0x16,
	PCI_SPEED_UNKNOWN		= 0xff,
};

struct pci_cap_saved_data {
	u16 cap_nr;
	bool cap_extended;
	unsigned int size;
	u32 data[0];
};

struct pci_cap_saved_state {
	struct hlist_node next;
	struct pci_cap_saved_data cap;
};

struct pcie_link_state;
struct pci_vpd;
struct pci_sriov;
struct pci_ats;


struct pci_dev {
	struct list_head bus_list;	
	struct pci_bus	*bus;		
	struct pci_bus	*subordinate;	

	void		*sysdata;	
	struct proc_dir_entry *procent;	
	struct pci_slot	*slot;		

	unsigned int	devfn;		
	unsigned short	vendor;
	unsigned short	device;
	unsigned short	subsystem_vendor;
	unsigned short	subsystem_device;
	unsigned int	class;		
	u8		revision;	
	u8		hdr_type;	
#ifdef CONFIG_PCIEAER
	u16		aer_cap;	
#endif
	u8		pcie_cap;	
	u8		msi_cap;	
	u8		msix_cap;	
	u8		pcie_mpss:3;	
	u8		rom_base_reg;	
	u8		pin;		
	u16		pcie_flags_reg;	
	unsigned long	*dma_alias_mask;

	struct pci_driver *driver;	
	u64		dma_mask;	

	struct device_dma_parameters dma_parms;

	pci_power_t     current_state;  
	u8		pm_cap;		
	unsigned int	pme_support:5;	
	unsigned int	pme_interrupt:1;
	unsigned int	pme_poll:1;	
	unsigned int	d1_support:1;	
	unsigned int	d2_support:1;	
	unsigned int	no_d1d2:1;	
	unsigned int	no_d3cold:1;	
	unsigned int	bridge_d3:1;	
	unsigned int	d3cold_allowed:1;	
	unsigned int	mmio_always_on:1;	
	unsigned int	wakeup_prepared:1;
	unsigned int	runtime_d3cold:1;	
	unsigned int	ignore_hotplug:1;	
	unsigned int	hotplug_user_indicators:1; 
	unsigned int	clear_retrain_link:1;	
	unsigned int	d3_delay;	
	unsigned int	d3cold_delay;	

#ifdef CONFIG_PCIEASPM
	struct pcie_link_state	*link_state;	
#endif

	pci_channel_state_t error_state;	
	struct	device	dev;		

	int		cfg_size;	

	
	unsigned int	irq;
	struct cpumask	*irq_affinity;
	struct resource resource[DEVICE_COUNT_RESOURCE]; 

	bool match_driver;		
	
	unsigned int	transparent:1;	
	unsigned int	multifunction:1;
	
	unsigned int	is_added:1;
	unsigned int	is_busmaster:1; 
	unsigned int	no_msi:1;	
	unsigned int	no_64bit_msi:1; 
	unsigned int	block_cfg_access:1;	
	unsigned int	broken_parity_status:1;	
	unsigned int	irq_reroute_variant:2;	
	unsigned int	msi_enabled:1;
	unsigned int	msix_enabled:1;
	unsigned int	ari_enabled:1;	
	unsigned int	ats_enabled:1;	
	unsigned int	is_managed:1;
	unsigned int    needs_freset:1; 
	unsigned int	state_saved:1;
	unsigned int	is_physfn:1;
	unsigned int	is_virtfn:1;
	unsigned int	reset_fn:1;
	unsigned int    is_hotplug_bridge:1;
	unsigned int    __aer_firmware_first_valid:1;
	unsigned int	__aer_firmware_first:1;
	unsigned int	broken_intx_masking:1;
	unsigned int	io_window_1k:1;	
	unsigned int	irq_managed:1;
	unsigned int	has_secondary_link:1;
	unsigned int	non_compliant_bars:1;	
	pci_dev_flags_t dev_flags;
	atomic_t	enable_cnt;	

	u32		saved_config_space[16]; 
	struct hlist_head saved_cap_space;
	struct bin_attribute *rom_attr; 
	int rom_attr_enabled;		
	struct bin_attribute *res_attr[DEVICE_COUNT_RESOURCE]; 
	struct bin_attribute *res_attr_wc[DEVICE_COUNT_RESOURCE]; 

#ifdef CONFIG_PCIE_PTM
	unsigned int	ptm_root:1;
	unsigned int	ptm_enabled:1;
	u8		ptm_granularity;
#endif
#ifdef CONFIG_PCI_MSI
	const struct attribute_group **msi_irq_groups;
#endif
	struct pci_vpd *vpd;
#ifdef CONFIG_PCI_ATS
	union {
		struct pci_sriov *sriov;	
		struct pci_dev *physfn;	
	};
	u16		ats_cap;	
	u8		ats_stu;	
	atomic_t	ats_ref_cnt;	
#endif
	phys_addr_t rom; 
	size_t romlen; 
	char *driver_override; 
};

static inline struct pci_dev *pci_physfn(struct pci_dev *dev)
{
#ifdef CONFIG_PCI_IOV
	if (dev->is_virtfn)
		dev = dev->physfn;
#endif
	return dev;
}

struct pci_dev *pci_alloc_dev(struct pci_bus *bus);

#define	to_pci_dev(n) container_of(n, struct pci_dev, dev)
#define for_each_pci_dev(d) while ((d = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, d)) != NULL)

static inline int pci_channel_offline(struct pci_dev *pdev)
{
	return (pdev->error_state != pci_channel_io_normal);
}

struct pci_host_bridge {
	struct device dev;
	struct pci_bus *bus;		
	struct list_head windows;	
	void (*release_fn)(struct pci_host_bridge *);
	void *release_data;
	unsigned int ignore_reset_delay:1;	
	
	resource_size_t (*align_resource)(struct pci_dev *dev,
			const struct resource *res,
			resource_size_t start,
			resource_size_t size,
			resource_size_t align);
};

#define	to_pci_host_bridge(n) container_of(n, struct pci_host_bridge, dev)

struct pci_host_bridge *pci_find_host_bridge(struct pci_bus *bus);

void pci_set_host_bridge_release(struct pci_host_bridge *bridge,
		     void (*release_fn)(struct pci_host_bridge *),
		     void *release_data);

int pcibios_root_bridge_prepare(struct pci_host_bridge *bridge);




#define PCI_SUBTRACTIVE_DECODE	0x1

struct pci_bus_resource {
	struct list_head list;
	struct resource *res;
	unsigned int flags;
};

#define PCI_REGION_FLAG_MASK	0x0fU	

struct pci_bus {
	struct list_head node;		
	struct pci_bus	*parent;	
	struct list_head children;	
	struct list_head devices;	
	struct pci_dev	*self;		
	struct list_head slots;		
	struct resource *resource[PCI_BRIDGE_RESOURCE_NUM];
	struct list_head resources;	
	struct resource busn_res;	

	struct pci_ops	*ops;		
	struct msi_controller *msi;	
	void		*sysdata;	
	struct proc_dir_entry *procdir;	

	unsigned char	number;		
	unsigned char	primary;	
	unsigned char	max_bus_speed;	
	unsigned char	cur_bus_speed;	
#ifdef CONFIG_PCI_DOMAINS_GENERIC
	int		domain_nr;
#endif

	char		name[48];

	unsigned short  bridge_ctl;	
	pci_bus_flags_t bus_flags;	
	struct device		*bridge;
	struct device		dev;
	struct bin_attribute	*legacy_io; 
	struct bin_attribute	*legacy_mem; 
	unsigned int		is_added:1;
};

#define to_pci_bus(n)	container_of(n, struct pci_bus, dev)


static inline bool pci_is_root_bus(struct pci_bus *pbus)
{
	return !(pbus->parent);
}


static inline bool pci_is_bridge(struct pci_dev *dev)
{
	return dev->hdr_type == PCI_HEADER_TYPE_BRIDGE ||
		dev->hdr_type == PCI_HEADER_TYPE_CARDBUS;
}

static inline struct pci_dev *pci_upstream_bridge(struct pci_dev *dev)
{
	dev = pci_physfn(dev);
	if (pci_is_root_bus(dev->bus))
		return NULL;

	return dev->bus->self;
}

struct device *pci_get_host_bridge_device(struct pci_dev *dev);
void pci_put_host_bridge_device(struct device *dev);

#ifdef CONFIG_PCI_MSI
static inline bool pci_dev_msi_enabled(struct pci_dev *pci_dev)
{
	return pci_dev->msi_enabled || pci_dev->msix_enabled;
}
#else
static inline bool pci_dev_msi_enabled(struct pci_dev *pci_dev) { return false; }
#endif


#define PCIBIOS_SUCCESSFUL		0x00
#define PCIBIOS_FUNC_NOT_SUPPORTED	0x81
#define PCIBIOS_BAD_VENDOR_ID		0x83
#define PCIBIOS_DEVICE_NOT_FOUND	0x86
#define PCIBIOS_BAD_REGISTER_NUMBER	0x87
#define PCIBIOS_SET_FAILED		0x88
#define PCIBIOS_BUFFER_TOO_SMALL	0x89


static inline int pcibios_err_to_errno(int err)
{
	if (err <= PCIBIOS_SUCCESSFUL)
		return err; 

	switch (err) {
	case PCIBIOS_FUNC_NOT_SUPPORTED:
		return -ENOENT;
	case PCIBIOS_BAD_VENDOR_ID:
		return -ENOTTY;
	case PCIBIOS_DEVICE_NOT_FOUND:
		return -ENODEV;
	case PCIBIOS_BAD_REGISTER_NUMBER:
		return -EFAULT;
	case PCIBIOS_SET_FAILED:
		return -EIO;
	case PCIBIOS_BUFFER_TOO_SMALL:
		return -ENOSPC;
	}

	return -ERANGE;
}



struct pci_ops {
	int (*add_bus)(struct pci_bus *bus);
	void (*remove_bus)(struct pci_bus *bus);
	void __iomem *(*map_bus)(struct pci_bus *bus, unsigned int devfn, int where);
	int (*read)(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 *val);
	int (*write)(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 val);
};


int raw_pci_read(unsigned int domain, unsigned int bus, unsigned int devfn,
		 int reg, int len, u32 *val);
int raw_pci_write(unsigned int domain, unsigned int bus, unsigned int devfn,
		  int reg, int len, u32 val);

#ifdef CONFIG_PCI_BUS_ADDR_T_64BIT
typedef u64 pci_bus_addr_t;
#else
typedef u32 pci_bus_addr_t;
#endif

struct pci_bus_region {
	pci_bus_addr_t start;
	pci_bus_addr_t end;
};

struct pci_dynids {
	spinlock_t lock;            
	struct list_head list;      
};




typedef unsigned int __bitwise pci_ers_result_t;

enum pci_ers_result {
	
	PCI_ERS_RESULT_NONE = (__force pci_ers_result_t) 1,

	
	PCI_ERS_RESULT_CAN_RECOVER = (__force pci_ers_result_t) 2,

	
	PCI_ERS_RESULT_NEED_RESET = (__force pci_ers_result_t) 3,

	
	PCI_ERS_RESULT_DISCONNECT = (__force pci_ers_result_t) 4,

	
	PCI_ERS_RESULT_RECOVERED = (__force pci_ers_result_t) 5,

	
	PCI_ERS_RESULT_NO_AER_DRIVER = (__force pci_ers_result_t) 6,
};


struct pci_error_handlers {
	
	pci_ers_result_t (*error_detected)(struct pci_dev *dev,
					   enum pci_channel_state error);

	
	pci_ers_result_t (*mmio_enabled)(struct pci_dev *dev);

	
	pci_ers_result_t (*link_reset)(struct pci_dev *dev);

	
	pci_ers_result_t (*slot_reset)(struct pci_dev *dev);

	
	void (*reset_notify)(struct pci_dev *dev, bool prepare);

	
	void (*resume)(struct pci_dev *dev);
};


struct module;
struct pci_driver {
	struct list_head node;
	const char *name;
	const struct pci_device_id *id_table;	
	int  (*probe)  (struct pci_dev *dev, const struct pci_device_id *id);	
	void (*remove) (struct pci_dev *dev);	
	int  (*suspend) (struct pci_dev *dev, pm_message_t state);	
	int  (*suspend_late) (struct pci_dev *dev, pm_message_t state);
	int  (*resume_early) (struct pci_dev *dev);
	int  (*resume) (struct pci_dev *dev);	                
	void (*shutdown) (struct pci_dev *dev);
	int (*sriov_configure) (struct pci_dev *dev, int num_vfs); 
	const struct pci_error_handlers *err_handler;
	struct device_driver	driver;
	struct pci_dynids dynids;
};

#define	to_pci_driver(drv) container_of(drv, struct pci_driver, driver)


#define PCI_DEVICE(vend,dev) \
	.vendor = (vend), .device = (dev), \
	.subvendor = PCI_ANY_ID, .subdevice = PCI_ANY_ID


#define PCI_DEVICE_SUB(vend, dev, subvend, subdev) \
	.vendor = (vend), .device = (dev), \
	.subvendor = (subvend), .subdevice = (subdev)


#define PCI_DEVICE_CLASS(dev_class,dev_class_mask) \
	.class = (dev_class), .class_mask = (dev_class_mask), \
	.vendor = PCI_ANY_ID, .device = PCI_ANY_ID, \
	.subvendor = PCI_ANY_ID, .subdevice = PCI_ANY_ID



#define PCI_VDEVICE(vend, dev) \
	.vendor = PCI_VENDOR_ID_##vend, .device = (dev), \
	.subvendor = PCI_ANY_ID, .subdevice = PCI_ANY_ID, 0, 0

enum {
	PCI_REASSIGN_ALL_RSRC	= 0x00000001,	
	PCI_REASSIGN_ALL_BUS	= 0x00000002,	
	PCI_PROBE_ONLY		= 0x00000004,	
	PCI_CAN_SKIP_ISA_ALIGN	= 0x00000008,	
	PCI_ENABLE_PROC_DOMAINS	= 0x00000010,	
	PCI_COMPAT_DOMAIN_0	= 0x00000020,	
	PCI_SCAN_ALL_PCIE_DEVS	= 0x00000040,	
};


#ifdef CONFIG_PCI

extern unsigned int pci_flags;

static inline void pci_set_flags(int flags) { pci_flags = flags; }
static inline void pci_add_flags(int flags) { pci_flags |= flags; }
static inline void pci_clear_flags(int flags) { pci_flags &= ~flags; }
static inline int pci_has_flag(int flag) { return pci_flags & flag; }

void pcie_bus_configure_settings(struct pci_bus *bus);

enum pcie_bus_config_types {
	PCIE_BUS_TUNE_OFF,	
	PCIE_BUS_DEFAULT,	
	PCIE_BUS_SAFE,		
	PCIE_BUS_PERFORMANCE,	
	PCIE_BUS_PEER2PEER,	
};

extern enum pcie_bus_config_types pcie_bus_config;

extern struct bus_type pci_bus_type;


extern struct list_head pci_root_buses;	

int no_pci_devices(void);

void pcibios_resource_survey_bus(struct pci_bus *bus);
void pcibios_bus_add_device(struct pci_dev *pdev);
void pcibios_add_bus(struct pci_bus *bus);
void pcibios_remove_bus(struct pci_bus *bus);
void pcibios_fixup_bus(struct pci_bus *);
int __must_check pcibios_enable_device(struct pci_dev *, int mask);

char *pcibios_setup(char *str);


resource_size_t pcibios_align_resource(void *, const struct resource *,
				resource_size_t,
				resource_size_t);
void pcibios_update_irq(struct pci_dev *, int irq);


void pci_fixup_cardbus(struct pci_bus *);



void pcibios_resource_to_bus(struct pci_bus *bus, struct pci_bus_region *region,
			     struct resource *res);
void pcibios_bus_to_resource(struct pci_bus *bus, struct resource *res,
			     struct pci_bus_region *region);
void pcibios_scan_specific_bus(int busn);
struct pci_bus *pci_find_bus(int domain, int busnr);
void pci_bus_add_devices(const struct pci_bus *bus);
struct pci_bus *pci_scan_bus(int bus, struct pci_ops *ops, void *sysdata);
struct pci_bus *pci_create_root_bus(struct device *parent, int bus,
				    struct pci_ops *ops, void *sysdata,
				    struct list_head *resources);
int pci_bus_insert_busn_res(struct pci_bus *b, int bus, int busmax);
int pci_bus_update_busn_res_end(struct pci_bus *b, int busmax);
void pci_bus_release_busn_res(struct pci_bus *b);
struct pci_bus *pci_scan_root_bus_msi(struct device *parent, int bus,
				      struct pci_ops *ops, void *sysdata,
				      struct list_head *resources,
				      struct msi_controller *msi);
struct pci_bus *pci_scan_root_bus(struct device *parent, int bus,
					     struct pci_ops *ops, void *sysdata,
					     struct list_head *resources);
struct pci_bus *pci_add_new_bus(struct pci_bus *parent, struct pci_dev *dev,
				int busnr);
void pcie_update_link_speed(struct pci_bus *bus, u16 link_status);
struct pci_slot *pci_create_slot(struct pci_bus *parent, int slot_nr,
				 const char *name,
				 struct hotplug_slot *hotplug);
void pci_destroy_slot(struct pci_slot *slot);
#ifdef CONFIG_SYSFS
void pci_dev_assign_slot(struct pci_dev *dev);
#else
static inline void pci_dev_assign_slot(struct pci_dev *dev) { }
#endif
int pci_scan_slot(struct pci_bus *bus, int devfn);
struct pci_dev *pci_scan_single_device(struct pci_bus *bus, int devfn);
void pci_device_add(struct pci_dev *dev, struct pci_bus *bus);
unsigned int pci_scan_child_bus(struct pci_bus *bus);
void pci_bus_add_device(struct pci_dev *dev);
void pci_read_bridge_bases(struct pci_bus *child);
struct resource *pci_find_parent_resource(const struct pci_dev *dev,
					  struct resource *res);
struct pci_dev *pci_find_pcie_root_port(struct pci_dev *dev);
u8 pci_swizzle_interrupt_pin(const struct pci_dev *dev, u8 pin);
int pci_get_interrupt_pin(struct pci_dev *dev, struct pci_dev **bridge);
u8 pci_common_swizzle(struct pci_dev *dev, u8 *pinp);
struct pci_dev *pci_dev_get(struct pci_dev *dev);
void pci_dev_put(struct pci_dev *dev);
void pci_remove_bus(struct pci_bus *b);
void pci_stop_and_remove_bus_device(struct pci_dev *dev);
void pci_stop_and_remove_bus_device_locked(struct pci_dev *dev);
void pci_stop_root_bus(struct pci_bus *bus);
void pci_remove_root_bus(struct pci_bus *bus);
void pci_setup_cardbus(struct pci_bus *bus);
void pcibios_setup_bridge(struct pci_bus *bus, unsigned long type);
void pci_sort_breadthfirst(void);
#define dev_is_pci(d) ((d)->bus == &pci_bus_type)
#define dev_is_pf(d) ((dev_is_pci(d) ? to_pci_dev(d)->is_physfn : false))
#define dev_num_vf(d) ((dev_is_pci(d) ? pci_num_vf(to_pci_dev(d)) : 0))



enum pci_lost_interrupt_reason {
	PCI_LOST_IRQ_NO_INFORMATION = 0,
	PCI_LOST_IRQ_DISABLE_MSI,
	PCI_LOST_IRQ_DISABLE_MSIX,
	PCI_LOST_IRQ_DISABLE_ACPI,
};
enum pci_lost_interrupt_reason pci_lost_interrupt(struct pci_dev *dev);
int pci_find_capability(struct pci_dev *dev, int cap);
int pci_find_next_capability(struct pci_dev *dev, u8 pos, int cap);
int pci_find_ext_capability(struct pci_dev *dev, int cap);
int pci_find_next_ext_capability(struct pci_dev *dev, int pos, int cap);
int pci_find_ht_capability(struct pci_dev *dev, int ht_cap);
int pci_find_next_ht_capability(struct pci_dev *dev, int pos, int ht_cap);
struct pci_bus *pci_find_next_bus(const struct pci_bus *from);

struct pci_dev *pci_get_device(unsigned int vendor, unsigned int device,
				struct pci_dev *from);
struct pci_dev *pci_get_subsys(unsigned int vendor, unsigned int device,
				unsigned int ss_vendor, unsigned int ss_device,
				struct pci_dev *from);
struct pci_dev *pci_get_slot(struct pci_bus *bus, unsigned int devfn);
struct pci_dev *pci_get_domain_bus_and_slot(int domain, unsigned int bus,
					    unsigned int devfn);
static inline struct pci_dev *pci_get_bus_and_slot(unsigned int bus,
						   unsigned int devfn)
{
	return pci_get_domain_bus_and_slot(0, bus, devfn);
}
struct pci_dev *pci_get_class(unsigned int class, struct pci_dev *from);
int pci_dev_present(const struct pci_device_id *ids);

int pci_bus_read_config_byte(struct pci_bus *bus, unsigned int devfn,
			     int where, u8 *val);
int pci_bus_read_config_word(struct pci_bus *bus, unsigned int devfn,
			     int where, u16 *val);
int pci_bus_read_config_dword(struct pci_bus *bus, unsigned int devfn,
			      int where, u32 *val);
int pci_bus_write_config_byte(struct pci_bus *bus, unsigned int devfn,
			      int where, u8 val);
int pci_bus_write_config_word(struct pci_bus *bus, unsigned int devfn,
			      int where, u16 val);
int pci_bus_write_config_dword(struct pci_bus *bus, unsigned int devfn,
			       int where, u32 val);

int pci_generic_config_read(struct pci_bus *bus, unsigned int devfn,
			    int where, int size, u32 *val);
int pci_generic_config_write(struct pci_bus *bus, unsigned int devfn,
			    int where, int size, u32 val);
int pci_generic_config_read32(struct pci_bus *bus, unsigned int devfn,
			      int where, int size, u32 *val);
int pci_generic_config_write32(struct pci_bus *bus, unsigned int devfn,
			       int where, int size, u32 val);

struct pci_ops *pci_bus_set_ops(struct pci_bus *bus, struct pci_ops *ops);

static inline int pci_read_config_byte(const struct pci_dev *dev, int where, u8 *val)
{
	return pci_bus_read_config_byte(dev->bus, dev->devfn, where, val);
}
static inline int pci_read_config_word(const struct pci_dev *dev, int where, u16 *val)
{
	return pci_bus_read_config_word(dev->bus, dev->devfn, where, val);
}
static inline int pci_read_config_dword(const struct pci_dev *dev, int where,
					u32 *val)
{
	return pci_bus_read_config_dword(dev->bus, dev->devfn, where, val);
}
static inline int pci_write_config_byte(const struct pci_dev *dev, int where, u8 val)
{
	return pci_bus_write_config_byte(dev->bus, dev->devfn, where, val);
}
static inline int pci_write_config_word(const struct pci_dev *dev, int where, u16 val)
{
	return pci_bus_write_config_word(dev->bus, dev->devfn, where, val);
}
static inline int pci_write_config_dword(const struct pci_dev *dev, int where,
					 u32 val)
{
	return pci_bus_write_config_dword(dev->bus, dev->devfn, where, val);
}

int pcie_capability_read_word(struct pci_dev *dev, int pos, u16 *val);
int pcie_capability_read_dword(struct pci_dev *dev, int pos, u32 *val);
int pcie_capability_write_word(struct pci_dev *dev, int pos, u16 val);
int pcie_capability_write_dword(struct pci_dev *dev, int pos, u32 val);
int pcie_capability_clear_and_set_word(struct pci_dev *dev, int pos,
				       u16 clear, u16 set);
int pcie_capability_clear_and_set_dword(struct pci_dev *dev, int pos,
					u32 clear, u32 set);

static inline int pcie_capability_set_word(struct pci_dev *dev, int pos,
					   u16 set)
{
	return pcie_capability_clear_and_set_word(dev, pos, 0, set);
}

static inline int pcie_capability_set_dword(struct pci_dev *dev, int pos,
					    u32 set)
{
	return pcie_capability_clear_and_set_dword(dev, pos, 0, set);
}

static inline int pcie_capability_clear_word(struct pci_dev *dev, int pos,
					     u16 clear)
{
	return pcie_capability_clear_and_set_word(dev, pos, clear, 0);
}

static inline int pcie_capability_clear_dword(struct pci_dev *dev, int pos,
					      u32 clear)
{
	return pcie_capability_clear_and_set_dword(dev, pos, clear, 0);
}


int pci_user_read_config_byte(struct pci_dev *dev, int where, u8 *val);
int pci_user_read_config_word(struct pci_dev *dev, int where, u16 *val);
int pci_user_read_config_dword(struct pci_dev *dev, int where, u32 *val);
int pci_user_write_config_byte(struct pci_dev *dev, int where, u8 val);
int pci_user_write_config_word(struct pci_dev *dev, int where, u16 val);
int pci_user_write_config_dword(struct pci_dev *dev, int where, u32 val);

int __must_check pci_enable_device(struct pci_dev *dev);
int __must_check pci_enable_device_io(struct pci_dev *dev);
int __must_check pci_enable_device_mem(struct pci_dev *dev);
int __must_check pci_reenable_device(struct pci_dev *);
int __must_check pcim_enable_device(struct pci_dev *pdev);
void pcim_pin_device(struct pci_dev *pdev);

static inline int pci_is_enabled(struct pci_dev *pdev)
{
	return (atomic_read(&pdev->enable_cnt) > 0);
}

static inline int pci_is_managed(struct pci_dev *pdev)
{
	return pdev->is_managed;
}

void pci_disable_device(struct pci_dev *dev);

extern unsigned int pcibios_max_latency;
void pci_set_master(struct pci_dev *dev);
void pci_clear_master(struct pci_dev *dev);

int pci_set_pcie_reset_state(struct pci_dev *dev, enum pcie_reset_state state);
int pci_set_cacheline_size(struct pci_dev *dev);
#define HAVE_PCI_SET_MWI
int __must_check pci_set_mwi(struct pci_dev *dev);
int pci_try_set_mwi(struct pci_dev *dev);
void pci_clear_mwi(struct pci_dev *dev);
void pci_intx(struct pci_dev *dev, int enable);
bool pci_intx_mask_supported(struct pci_dev *dev);
bool pci_check_and_mask_intx(struct pci_dev *dev);
bool pci_check_and_unmask_intx(struct pci_dev *dev);
int pci_wait_for_pending(struct pci_dev *dev, int pos, u16 mask);
int pci_wait_for_pending_transaction(struct pci_dev *dev);
int pcix_get_max_mmrbc(struct pci_dev *dev);
int pcix_get_mmrbc(struct pci_dev *dev);
int pcix_set_mmrbc(struct pci_dev *dev, int mmrbc);
int pcie_get_readrq(struct pci_dev *dev);
int pcie_set_readrq(struct pci_dev *dev, int rq);
int pcie_get_mps(struct pci_dev *dev);
int pcie_set_mps(struct pci_dev *dev, int mps);
int pcie_get_minimum_link(struct pci_dev *dev, enum pci_bus_speed *speed,
			  enum pcie_link_width *width);
int __pci_reset_function(struct pci_dev *dev);
int __pci_reset_function_locked(struct pci_dev *dev);
int pci_reset_function(struct pci_dev *dev);
int pci_try_reset_function(struct pci_dev *dev);
int pci_probe_reset_slot(struct pci_slot *slot);
int pci_reset_slot(struct pci_slot *slot);
int pci_try_reset_slot(struct pci_slot *slot);
int pci_probe_reset_bus(struct pci_bus *bus);
int pci_reset_bus(struct pci_bus *bus);
int pci_try_reset_bus(struct pci_bus *bus);
void pci_reset_secondary_bus(struct pci_dev *dev);
void pcibios_reset_secondary_bus(struct pci_dev *dev);
void pci_reset_bridge_secondary_bus(struct pci_dev *dev);
void pci_update_resource(struct pci_dev *dev, int resno);
int __must_check pci_assign_resource(struct pci_dev *dev, int i);
int __must_check pci_reassign_resource(struct pci_dev *dev, int i, resource_size_t add_size, resource_size_t align);
int pci_select_bars(struct pci_dev *dev, unsigned long flags);
bool pci_device_is_present(struct pci_dev *pdev);
void pci_ignore_hotplug(struct pci_dev *dev);


int pci_enable_rom(struct pci_dev *pdev);
void pci_disable_rom(struct pci_dev *pdev);
void __iomem __must_check *pci_map_rom(struct pci_dev *pdev, size_t *size);
void pci_unmap_rom(struct pci_dev *pdev, void __iomem *rom);
size_t pci_get_rom_size(struct pci_dev *pdev, void __iomem *rom, size_t size);
void __iomem __must_check *pci_platform_rom(struct pci_dev *pdev, size_t *size);


int pci_save_state(struct pci_dev *dev);
void pci_restore_state(struct pci_dev *dev);
struct pci_saved_state *pci_store_saved_state(struct pci_dev *dev);
int pci_load_saved_state(struct pci_dev *dev,
			 struct pci_saved_state *state);
int pci_load_and_free_saved_state(struct pci_dev *dev,
				  struct pci_saved_state **state);
struct pci_cap_saved_state *pci_find_saved_cap(struct pci_dev *dev, char cap);
struct pci_cap_saved_state *pci_find_saved_ext_cap(struct pci_dev *dev,
						   u16 cap);
int pci_add_cap_save_buffer(struct pci_dev *dev, char cap, unsigned int size);
int pci_add_ext_cap_save_buffer(struct pci_dev *dev,
				u16 cap, unsigned int size);
int __pci_complete_power_transition(struct pci_dev *dev, pci_power_t state);
int pci_set_power_state(struct pci_dev *dev, pci_power_t state);
pci_power_t pci_choose_state(struct pci_dev *dev, pm_message_t state);
bool pci_pme_capable(struct pci_dev *dev, pci_power_t state);
void pci_pme_active(struct pci_dev *dev, bool enable);
int __pci_enable_wake(struct pci_dev *dev, pci_power_t state,
		      bool runtime, bool enable);
int pci_wake_from_d3(struct pci_dev *dev, bool enable);
int pci_prepare_to_sleep(struct pci_dev *dev);
int pci_back_from_sleep(struct pci_dev *dev);
bool pci_dev_run_wake(struct pci_dev *dev);
bool pci_check_pme_status(struct pci_dev *dev);
void pci_pme_wakeup_bus(struct pci_bus *bus);
void pci_d3cold_enable(struct pci_dev *dev);
void pci_d3cold_disable(struct pci_dev *dev);

static inline int pci_enable_wake(struct pci_dev *dev, pci_power_t state,
				  bool enable)
{
	return __pci_enable_wake(dev, state, false, enable);
}


int pci_save_vc_state(struct pci_dev *dev);
void pci_restore_vc_state(struct pci_dev *dev);
void pci_allocate_vc_save_buffers(struct pci_dev *dev);


void set_pcie_port_type(struct pci_dev *pdev);
void set_pcie_hotplug_bridge(struct pci_dev *pdev);


int pci_bus_find_capability(struct pci_bus *bus, unsigned int devfn, int cap);
unsigned int pci_rescan_bus_bridge_resize(struct pci_dev *bridge);
unsigned int pci_rescan_bus(struct pci_bus *bus);
void pci_lock_rescan_remove(void);
void pci_unlock_rescan_remove(void);


ssize_t pci_read_vpd(struct pci_dev *dev, loff_t pos, size_t count, void *buf);
ssize_t pci_write_vpd(struct pci_dev *dev, loff_t pos, size_t count, const void *buf);
int pci_set_vpd_size(struct pci_dev *dev, size_t len);


resource_size_t pcibios_retrieve_fw_addr(struct pci_dev *dev, int idx);
void pci_bus_assign_resources(const struct pci_bus *bus);
void pci_bus_claim_resources(struct pci_bus *bus);
void pci_bus_size_bridges(struct pci_bus *bus);
int pci_claim_resource(struct pci_dev *, int);
int pci_claim_bridge_resource(struct pci_dev *bridge, int i);
void pci_assign_unassigned_resources(void);
void pci_assign_unassigned_bridge_resources(struct pci_dev *bridge);
void pci_assign_unassigned_bus_resources(struct pci_bus *bus);
void pci_assign_unassigned_root_bus_resources(struct pci_bus *bus);
void pdev_enable_device(struct pci_dev *);
int pci_enable_resources(struct pci_dev *, int mask);
void pci_fixup_irqs(u8 (*)(struct pci_dev *, u8 *),
		    int (*)(const struct pci_dev *, u8, u8));
struct resource *pci_find_resource(struct pci_dev *dev, struct resource *res);
#define HAVE_PCI_REQ_REGIONS	2
int __must_check pci_request_regions(struct pci_dev *, const char *);
int __must_check pci_request_regions_exclusive(struct pci_dev *, const char *);
void pci_release_regions(struct pci_dev *);
int __must_check pci_request_region(struct pci_dev *, int, const char *);
int __must_check pci_request_region_exclusive(struct pci_dev *, int, const char *);
void pci_release_region(struct pci_dev *, int);
int pci_request_selected_regions(struct pci_dev *, int, const char *);
int pci_request_selected_regions_exclusive(struct pci_dev *, int, const char *);
void pci_release_selected_regions(struct pci_dev *, int);


struct pci_bus *pci_bus_get(struct pci_bus *bus);
void pci_bus_put(struct pci_bus *bus);
void pci_add_resource(struct list_head *resources, struct resource *res);
void pci_add_resource_offset(struct list_head *resources, struct resource *res,
			     resource_size_t offset);
void pci_free_resource_list(struct list_head *resources);
void pci_bus_add_resource(struct pci_bus *bus, struct resource *res,
			  unsigned int flags);
struct resource *pci_bus_resource_n(const struct pci_bus *bus, int n);
void pci_bus_remove_resources(struct pci_bus *bus);
int devm_request_pci_bus_resources(struct device *dev,
				   struct list_head *resources);

#define pci_bus_for_each_resource(bus, res, i)				\
	for (i = 0;							\
	    (res = pci_bus_resource_n(bus, i)) || i < PCI_BRIDGE_RESOURCE_NUM; \
	     i++)

int __must_check pci_bus_alloc_resource(struct pci_bus *bus,
			struct resource *res, resource_size_t size,
			resource_size_t align, resource_size_t min,
			unsigned long type_mask,
			resource_size_t (*alignf)(void *,
						  const struct resource *,
						  resource_size_t,
						  resource_size_t),
			void *alignf_data);


int pci_register_io_range(phys_addr_t addr, resource_size_t size);
unsigned long pci_address_to_pio(phys_addr_t addr);
phys_addr_t pci_pio_to_address(unsigned long pio);
int pci_remap_iospace(const struct resource *res, phys_addr_t phys_addr);
int devm_pci_remap_iospace(struct device *dev, const struct resource *res,
			   phys_addr_t phys_addr);
void pci_unmap_iospace(struct resource *res);

static inline pci_bus_addr_t pci_bus_address(struct pci_dev *pdev, int bar)
{
	struct pci_bus_region region;

	pcibios_resource_to_bus(pdev->bus, &region, &pdev->resource[bar]);
	return region.start;
}


int __must_check __pci_register_driver(struct pci_driver *, struct module *,
				       const char *mod_name);


#define pci_register_driver(driver)		\
	__pci_register_driver(driver, THIS_MODULE, KBUILD_MODNAME)

void pci_unregister_driver(struct pci_driver *dev);


#define module_pci_driver(__pci_driver) \
	module_driver(__pci_driver, pci_register_driver, \
		       pci_unregister_driver)


#define builtin_pci_driver(__pci_driver) \
	builtin_driver(__pci_driver, pci_register_driver)

struct pci_driver *pci_dev_driver(const struct pci_dev *dev);
int pci_add_dynid(struct pci_driver *drv,
		  unsigned int vendor, unsigned int device,
		  unsigned int subvendor, unsigned int subdevice,
		  unsigned int class, unsigned int class_mask,
		  unsigned long driver_data);
const struct pci_device_id *pci_match_id(const struct pci_device_id *ids,
					 struct pci_dev *dev);
int pci_scan_bridge(struct pci_bus *bus, struct pci_dev *dev, int max,
		    int pass);

void pci_walk_bus(struct pci_bus *top, int (*cb)(struct pci_dev *, void *),
		  void *userdata);
int pci_cfg_space_size(struct pci_dev *dev);
unsigned char pci_bus_max_busnr(struct pci_bus *bus);
void pci_setup_bridge(struct pci_bus *bus);
resource_size_t pcibios_window_alignment(struct pci_bus *bus,
					 unsigned long type);
resource_size_t pcibios_iov_resource_alignment(struct pci_dev *dev, int resno);

#define PCI_VGA_STATE_CHANGE_BRIDGE (1 << 0)
#define PCI_VGA_STATE_CHANGE_DECODES (1 << 1)

int pci_set_vga_state(struct pci_dev *pdev, bool decode,
		      unsigned int command_bits, u32 flags);

#define PCI_IRQ_LEGACY		(1 << 0) 
#define PCI_IRQ_MSI		(1 << 1) 
#define PCI_IRQ_MSIX		(1 << 2) 
#define PCI_IRQ_AFFINITY	(1 << 3) 
#define PCI_IRQ_ALL_TYPES \
	(PCI_IRQ_LEGACY | PCI_IRQ_MSI | PCI_IRQ_MSIX)



#include <linux/pci-dma.h>
#include <linux/dmapool.h>

#define	pci_pool dma_pool
#define pci_pool_create(name, pdev, size, align, allocation) \
		dma_pool_create(name, &pdev->dev, size, align, allocation)
#define	pci_pool_destroy(pool) dma_pool_destroy(pool)
#define	pci_pool_alloc(pool, flags, handle) dma_pool_alloc(pool, flags, handle)
#define	pci_pool_zalloc(pool, flags, handle) \
		dma_pool_zalloc(pool, flags, handle)
#define	pci_pool_free(pool, vaddr, addr) dma_pool_free(pool, vaddr, addr)

struct msix_entry {
	u32	vector;	
	u16	entry;	
};

#ifdef CONFIG_PCI_MSI
int pci_msi_vec_count(struct pci_dev *dev);
void pci_msi_shutdown(struct pci_dev *dev);
void pci_disable_msi(struct pci_dev *dev);
int pci_msix_vec_count(struct pci_dev *dev);
int pci_enable_msix(struct pci_dev *dev, struct msix_entry *entries, int nvec);
void pci_msix_shutdown(struct pci_dev *dev);
void pci_disable_msix(struct pci_dev *dev);
void pci_restore_msi_state(struct pci_dev *dev);
int pci_msi_enabled(void);
int pci_enable_msi_range(struct pci_dev *dev, int minvec, int maxvec);
static inline int pci_enable_msi_exact(struct pci_dev *dev, int nvec)
{
	int rc = pci_enable_msi_range(dev, nvec, nvec);
	if (rc < 0)
		return rc;
	return 0;
}
int pci_enable_msix_range(struct pci_dev *dev, struct msix_entry *entries,
			  int minvec, int maxvec);
static inline int pci_enable_msix_exact(struct pci_dev *dev,
					struct msix_entry *entries, int nvec)
{
	int rc = pci_enable_msix_range(dev, entries, nvec, nvec);
	if (rc < 0)
		return rc;
	return 0;
}
int pci_alloc_irq_vectors(struct pci_dev *dev, unsigned int min_vecs,
		unsigned int max_vecs, unsigned int flags);
void pci_free_irq_vectors(struct pci_dev *dev);
int pci_irq_vector(struct pci_dev *dev, unsigned int nr);
const struct cpumask *pci_irq_get_affinity(struct pci_dev *pdev, int vec);

#else
static inline int pci_msi_vec_count(struct pci_dev *dev) { return -ENOSYS; }
static inline void pci_msi_shutdown(struct pci_dev *dev) { }
static inline void pci_disable_msi(struct pci_dev *dev) { }
static inline int pci_msix_vec_count(struct pci_dev *dev) { return -ENOSYS; }
static inline int pci_enable_msix(struct pci_dev *dev,
				  struct msix_entry *entries, int nvec)
{ return -ENOSYS; }
static inline void pci_msix_shutdown(struct pci_dev *dev) { }
static inline void pci_disable_msix(struct pci_dev *dev) { }
static inline void pci_restore_msi_state(struct pci_dev *dev) { }
static inline int pci_msi_enabled(void) { return 0; }
static inline int pci_enable_msi_range(struct pci_dev *dev, int minvec,
				       int maxvec)
{ return -ENOSYS; }
static inline int pci_enable_msi_exact(struct pci_dev *dev, int nvec)
{ return -ENOSYS; }
static inline int pci_enable_msix_range(struct pci_dev *dev,
		      struct msix_entry *entries, int minvec, int maxvec)
{ return -ENOSYS; }
static inline int pci_enable_msix_exact(struct pci_dev *dev,
		      struct msix_entry *entries, int nvec)
{ return -ENOSYS; }
static inline int pci_alloc_irq_vectors(struct pci_dev *dev,
		unsigned int min_vecs, unsigned int max_vecs,
		unsigned int flags)
{
	if ((flags & PCI_IRQ_LEGACY) && min_vecs == 1 && dev->irq)
		return 1;
	return -ENOSPC;
}
static inline void pci_free_irq_vectors(struct pci_dev *dev)
{
}

static inline int pci_irq_vector(struct pci_dev *dev, unsigned int nr)
{
	if (WARN_ON_ONCE(nr > 0))
		return -EINVAL;
	return dev->irq;
}
static inline const struct cpumask *pci_irq_get_affinity(struct pci_dev *pdev,
		int vec)
{
	return cpu_possible_mask;
}
#endif

#ifdef CONFIG_PCIEPORTBUS
extern bool pcie_ports_disabled;
extern bool pcie_ports_auto;
#else
#define pcie_ports_disabled	true
#define pcie_ports_auto		false
#endif

#ifdef CONFIG_PCIEASPM
bool pcie_aspm_support_enabled(void);
#else
static inline bool pcie_aspm_support_enabled(void) { return false; }
#endif

#ifdef CONFIG_PCIEAER
void pci_no_aer(void);
bool pci_aer_available(void);
int pci_aer_init(struct pci_dev *dev);
#else
static inline void pci_no_aer(void) { }
static inline bool pci_aer_available(void) { return false; }
static inline int pci_aer_init(struct pci_dev *d) { return -ENODEV; }
#endif

#ifdef CONFIG_PCIE_ECRC
void pcie_set_ecrc_checking(struct pci_dev *dev);
void pcie_ecrc_get_policy(char *str);
#else
static inline void pcie_set_ecrc_checking(struct pci_dev *dev) { }
static inline void pcie_ecrc_get_policy(char *str) { }
#endif

#define pci_enable_msi(pdev)	pci_enable_msi_exact(pdev, 1)

#ifdef CONFIG_HT_IRQ

int  ht_create_irq(struct pci_dev *dev, int idx);
void ht_destroy_irq(unsigned int irq);
#endif 

#ifdef CONFIG_PCI_ATS

void pci_ats_init(struct pci_dev *dev);
int pci_enable_ats(struct pci_dev *dev, int ps);
void pci_disable_ats(struct pci_dev *dev);
int pci_ats_queue_depth(struct pci_dev *dev);
#else
static inline void pci_ats_init(struct pci_dev *d) { }
static inline int pci_enable_ats(struct pci_dev *d, int ps) { return -ENODEV; }
static inline void pci_disable_ats(struct pci_dev *d) { }
static inline int pci_ats_queue_depth(struct pci_dev *d) { return -ENODEV; }
#endif

#ifdef CONFIG_PCIE_PTM
int pci_enable_ptm(struct pci_dev *dev, u8 *granularity);
#else
static inline int pci_enable_ptm(struct pci_dev *dev, u8 *granularity)
{ return -EINVAL; }
#endif

void pci_cfg_access_lock(struct pci_dev *dev);
bool pci_cfg_access_trylock(struct pci_dev *dev);
void pci_cfg_access_unlock(struct pci_dev *dev);


#ifdef CONFIG_PCI_DOMAINS
extern int pci_domains_supported;
int pci_get_new_domain_nr(void);
#else
enum { pci_domains_supported = 0 };
static inline int pci_domain_nr(struct pci_bus *bus) { return 0; }
static inline int pci_proc_domain(struct pci_bus *bus) { return 0; }
static inline int pci_get_new_domain_nr(void) { return -ENOSYS; }
#endif 


#ifdef CONFIG_PCI_DOMAINS_GENERIC
static inline int pci_domain_nr(struct pci_bus *bus)
{
	return bus->domain_nr;
}
#ifdef CONFIG_ACPI
int acpi_pci_bus_find_domain_nr(struct pci_bus *bus);
#else
static inline int acpi_pci_bus_find_domain_nr(struct pci_bus *bus)
{ return 0; }
#endif
int pci_bus_find_domain_nr(struct pci_bus *bus, struct device *parent);
#endif


typedef int (*arch_set_vga_state_t)(struct pci_dev *pdev, bool decode,
		      unsigned int command_bits, u32 flags);
void pci_register_set_vga_state(arch_set_vga_state_t func);

static inline int
pci_request_io_regions(struct pci_dev *pdev, const char *name)
{
	return pci_request_selected_regions(pdev,
			    pci_select_bars(pdev, IORESOURCE_IO), name);
}

static inline void
pci_release_io_regions(struct pci_dev *pdev)
{
	return pci_release_selected_regions(pdev,
			    pci_select_bars(pdev, IORESOURCE_IO));
}

static inline int
pci_request_mem_regions(struct pci_dev *pdev, const char *name)
{
	return pci_request_selected_regions(pdev,
			    pci_select_bars(pdev, IORESOURCE_MEM), name);
}

static inline void
pci_release_mem_regions(struct pci_dev *pdev)
{
	return pci_release_selected_regions(pdev,
			    pci_select_bars(pdev, IORESOURCE_MEM));
}

#else 

static inline void pci_set_flags(int flags) { }
static inline void pci_add_flags(int flags) { }
static inline void pci_clear_flags(int flags) { }
static inline int pci_has_flag(int flag) { return 0; }



#define _PCI_NOP(o, s, t) \
	static inline int pci_##o##_config_##s(struct pci_dev *dev, \
						int where, t val) \
		{ return PCIBIOS_FUNC_NOT_SUPPORTED; }

#define _PCI_NOP_ALL(o, x)	_PCI_NOP(o, byte, u8 x) \
				_PCI_NOP(o, word, u16 x) \
				_PCI_NOP(o, dword, u32 x)
_PCI_NOP_ALL(read, *)
_PCI_NOP_ALL(write,)

static inline struct pci_dev *pci_get_device(unsigned int vendor,
					     unsigned int device,
					     struct pci_dev *from)
{ return NULL; }

static inline struct pci_dev *pci_get_subsys(unsigned int vendor,
					     unsigned int device,
					     unsigned int ss_vendor,
					     unsigned int ss_device,
					     struct pci_dev *from)
{ return NULL; }

static inline struct pci_dev *pci_get_class(unsigned int class,
					    struct pci_dev *from)
{ return NULL; }

#define pci_dev_present(ids)	(0)
#define no_pci_devices()	(1)
#define pci_dev_put(dev)	do { } while (0)

static inline void pci_set_master(struct pci_dev *dev) { }
static inline int pci_enable_device(struct pci_dev *dev) { return -EIO; }
static inline void pci_disable_device(struct pci_dev *dev) { }
static inline int pci_assign_resource(struct pci_dev *dev, int i)
{ return -EBUSY; }
static inline int __must_check __pci_register_driver(struct pci_driver *drv,
						     struct module *owner,
						     const char *mod_name)
{ return 0; }
static inline int pci_register_driver(struct pci_driver *drv)
{ return 0; }
static inline void pci_unregister_driver(struct pci_driver *drv) { }
static inline int pci_find_capability(struct pci_dev *dev, int cap)
{ return 0; }
static inline int pci_find_next_capability(struct pci_dev *dev, u8 post,
					   int cap)
{ return 0; }
static inline int pci_find_ext_capability(struct pci_dev *dev, int cap)
{ return 0; }


static inline int pci_save_state(struct pci_dev *dev) { return 0; }
static inline void pci_restore_state(struct pci_dev *dev) { }
static inline int pci_set_power_state(struct pci_dev *dev, pci_power_t state)
{ return 0; }
static inline int pci_wake_from_d3(struct pci_dev *dev, bool enable)
{ return 0; }
static inline pci_power_t pci_choose_state(struct pci_dev *dev,
					   pm_message_t state)
{ return PCI_D0; }
static inline int pci_enable_wake(struct pci_dev *dev, pci_power_t state,
				  int enable)
{ return 0; }

static inline struct resource *pci_find_resource(struct pci_dev *dev,
						 struct resource *res)
{ return NULL; }
static inline int pci_request_regions(struct pci_dev *dev, const char *res_name)
{ return -EIO; }
static inline void pci_release_regions(struct pci_dev *dev) { }

static inline unsigned long pci_address_to_pio(phys_addr_t addr) { return -1; }

static inline void pci_block_cfg_access(struct pci_dev *dev) { }
static inline int pci_block_cfg_access_in_atomic(struct pci_dev *dev)
{ return 0; }
static inline void pci_unblock_cfg_access(struct pci_dev *dev) { }

static inline struct pci_bus *pci_find_next_bus(const struct pci_bus *from)
{ return NULL; }
static inline struct pci_dev *pci_get_slot(struct pci_bus *bus,
						unsigned int devfn)
{ return NULL; }
static inline struct pci_dev *pci_get_bus_and_slot(unsigned int bus,
						unsigned int devfn)
{ return NULL; }

static inline int pci_domain_nr(struct pci_bus *bus) { return 0; }
static inline struct pci_dev *pci_dev_get(struct pci_dev *dev) { return NULL; }
static inline int pci_get_new_domain_nr(void) { return -ENOSYS; }

#define dev_is_pci(d) (false)
#define dev_is_pf(d) (false)
#define dev_num_vf(d) (0)
#endif 



#include <asm/pci.h>

#ifndef pci_root_bus_fwnode
#define pci_root_bus_fwnode(bus)	NULL
#endif


#define pci_resource_start(dev, bar)	((dev)->resource[(bar)].start)
#define pci_resource_end(dev, bar)	((dev)->resource[(bar)].end)
#define pci_resource_flags(dev, bar)	((dev)->resource[(bar)].flags)
#define pci_resource_len(dev,bar) \
	((pci_resource_start((dev), (bar)) == 0 &&	\
	  pci_resource_end((dev), (bar)) ==		\
	  pci_resource_start((dev), (bar))) ? 0 :	\
							\
	 (pci_resource_end((dev), (bar)) -		\
	  pci_resource_start((dev), (bar)) + 1))


static inline void *pci_get_drvdata(struct pci_dev *pdev)
{
	return dev_get_drvdata(&pdev->dev);
}

static inline void pci_set_drvdata(struct pci_dev *pdev, void *data)
{
	dev_set_drvdata(&pdev->dev, data);
}


static inline const char *pci_name(const struct pci_dev *pdev)
{
	return dev_name(&pdev->dev);
}



#ifdef HAVE_ARCH_PCI_RESOURCE_TO_USER
void pci_resource_to_user(const struct pci_dev *dev, int bar,
			  const struct resource *rsrc,
			  resource_size_t *start, resource_size_t *end);
#else
static inline void pci_resource_to_user(const struct pci_dev *dev, int bar,
		const struct resource *rsrc, resource_size_t *start,
		resource_size_t *end)
{
	*start = rsrc->start;
	*end = rsrc->end;
}
#endif 




struct pci_fixup {
	u16 vendor;		
	u16 device;		
	u32 class;		
	unsigned int class_shift;	
	void (*hook)(struct pci_dev *dev);
};

enum pci_fixup_pass {
	pci_fixup_early,	
	pci_fixup_header,	
	pci_fixup_final,	
	pci_fixup_enable,	
	pci_fixup_resume,	
	pci_fixup_suspend,	
	pci_fixup_resume_early, 
	pci_fixup_suspend_late,	
};


#define DECLARE_PCI_FIXUP_SECTION(section, name, vendor, device, class,	\
				  class_shift, hook)			\
	static const struct pci_fixup __PASTE(__pci_fixup_##name,__LINE__) __used	\
	__attribute__((__section__(#section), aligned((sizeof(void *)))))    \
		= { vendor, device, class, class_shift, hook };

#define DECLARE_PCI_FIXUP_CLASS_EARLY(vendor, device, class,		\
					 class_shift, hook)		\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_early,			\
		hook, vendor, device, class, class_shift, hook)
#define DECLARE_PCI_FIXUP_CLASS_HEADER(vendor, device, class,		\
					 class_shift, hook)		\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_header,			\
		hook, vendor, device, class, class_shift, hook)
#define DECLARE_PCI_FIXUP_CLASS_FINAL(vendor, device, class,		\
					 class_shift, hook)		\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_final,			\
		hook, vendor, device, class, class_shift, hook)
#define DECLARE_PCI_FIXUP_CLASS_ENABLE(vendor, device, class,		\
					 class_shift, hook)		\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_enable,			\
		hook, vendor, device, class, class_shift, hook)
#define DECLARE_PCI_FIXUP_CLASS_RESUME(vendor, device, class,		\
					 class_shift, hook)		\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_resume,			\
		resume##hook, vendor, device, class,	\
		class_shift, hook)
#define DECLARE_PCI_FIXUP_CLASS_RESUME_EARLY(vendor, device, class,	\
					 class_shift, hook)		\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_resume_early,		\
		resume_early##hook, vendor, device,	\
		class, class_shift, hook)
#define DECLARE_PCI_FIXUP_CLASS_SUSPEND(vendor, device, class,		\
					 class_shift, hook)		\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_suspend,			\
		suspend##hook, vendor, device, class,	\
		class_shift, hook)
#define DECLARE_PCI_FIXUP_CLASS_SUSPEND_LATE(vendor, device, class,	\
					 class_shift, hook)		\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_suspend_late,		\
		suspend_late##hook, vendor, device,	\
		class, class_shift, hook)

#define DECLARE_PCI_FIXUP_EARLY(vendor, device, hook)			\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_early,			\
		hook, vendor, device, PCI_ANY_ID, 0, hook)
#define DECLARE_PCI_FIXUP_HEADER(vendor, device, hook)			\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_header,			\
		hook, vendor, device, PCI_ANY_ID, 0, hook)
#define DECLARE_PCI_FIXUP_FINAL(vendor, device, hook)			\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_final,			\
		hook, vendor, device, PCI_ANY_ID, 0, hook)
#define DECLARE_PCI_FIXUP_ENABLE(vendor, device, hook)			\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_enable,			\
		hook, vendor, device, PCI_ANY_ID, 0, hook)
#define DECLARE_PCI_FIXUP_RESUME(vendor, device, hook)			\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_resume,			\
		resume##hook, vendor, device,		\
		PCI_ANY_ID, 0, hook)
#define DECLARE_PCI_FIXUP_RESUME_EARLY(vendor, device, hook)		\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_resume_early,		\
		resume_early##hook, vendor, device,	\
		PCI_ANY_ID, 0, hook)
#define DECLARE_PCI_FIXUP_SUSPEND(vendor, device, hook)			\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_suspend,			\
		suspend##hook, vendor, device,		\
		PCI_ANY_ID, 0, hook)
#define DECLARE_PCI_FIXUP_SUSPEND_LATE(vendor, device, hook)		\
	DECLARE_PCI_FIXUP_SECTION(.pci_fixup_suspend_late,		\
		suspend_late##hook, vendor, device,	\
		PCI_ANY_ID, 0, hook)

#ifdef CONFIG_PCI_QUIRKS
void pci_fixup_device(enum pci_fixup_pass pass, struct pci_dev *dev);
int pci_dev_specific_acs_enabled(struct pci_dev *dev, u16 acs_flags);
int pci_dev_specific_enable_acs(struct pci_dev *dev);
#else
static inline void pci_fixup_device(enum pci_fixup_pass pass,
				    struct pci_dev *dev) { }
static inline int pci_dev_specific_acs_enabled(struct pci_dev *dev,
					       u16 acs_flags)
{
	return -ENOTTY;
}
static inline int pci_dev_specific_enable_acs(struct pci_dev *dev)
{
	return -ENOTTY;
}
#endif

void __iomem *pcim_iomap(struct pci_dev *pdev, int bar, unsigned long maxlen);
void pcim_iounmap(struct pci_dev *pdev, void __iomem *addr);
void __iomem * const *pcim_iomap_table(struct pci_dev *pdev);
int pcim_iomap_regions(struct pci_dev *pdev, int mask, const char *name);
int pcim_iomap_regions_request_all(struct pci_dev *pdev, int mask,
				   const char *name);
void pcim_iounmap_regions(struct pci_dev *pdev, int mask);

extern int pci_pci_problems;
#define PCIPCI_FAIL		1	
#define PCIPCI_TRITON		2
#define PCIPCI_NATOMA		4
#define PCIPCI_VIAETBF		8
#define PCIPCI_VSFX		16
#define PCIPCI_ALIMAGIK		32	
#define PCIAGP_FAIL		64	

extern unsigned long pci_cardbus_io_size;
extern unsigned long pci_cardbus_mem_size;
extern u8 pci_dfl_cache_line_size;
extern u8 pci_cache_line_size;

extern unsigned long pci_hotplug_io_size;
extern unsigned long pci_hotplug_mem_size;
extern unsigned long pci_hotplug_bus_size;


void pcibios_disable_device(struct pci_dev *dev);
void pcibios_set_master(struct pci_dev *dev);
int pcibios_set_pcie_reset_state(struct pci_dev *dev,
				 enum pcie_reset_state state);
int pcibios_add_device(struct pci_dev *dev);
void pcibios_release_device(struct pci_dev *dev);
void pcibios_penalize_isa_irq(int irq, int active);
int pcibios_alloc_irq(struct pci_dev *dev);
void pcibios_free_irq(struct pci_dev *dev);

#ifdef CONFIG_HIBERNATE_CALLBACKS
extern struct dev_pm_ops pcibios_pm_ops;
#endif

#if defined(CONFIG_PCI_MMCONFIG) || defined(CONFIG_ACPI_MCFG)
void __init pci_mmcfg_early_init(void);
void __init pci_mmcfg_late_init(void);
#else
static inline void pci_mmcfg_early_init(void) { }
static inline void pci_mmcfg_late_init(void) { }
#endif

int pci_ext_cfg_avail(void);

void __iomem *pci_ioremap_bar(struct pci_dev *pdev, int bar);
void __iomem *pci_ioremap_wc_bar(struct pci_dev *pdev, int bar);

#ifdef CONFIG_PCI_IOV
int pci_iov_virtfn_bus(struct pci_dev *dev, int id);
int pci_iov_virtfn_devfn(struct pci_dev *dev, int id);

int pci_enable_sriov(struct pci_dev *dev, int nr_virtfn);
void pci_disable_sriov(struct pci_dev *dev);
int pci_iov_add_virtfn(struct pci_dev *dev, int id, int reset);
void pci_iov_remove_virtfn(struct pci_dev *dev, int id, int reset);
int pci_num_vf(struct pci_dev *dev);
int pci_vfs_assigned(struct pci_dev *dev);
int pci_sriov_set_totalvfs(struct pci_dev *dev, u16 numvfs);
int pci_sriov_get_totalvfs(struct pci_dev *dev);
resource_size_t pci_iov_resource_size(struct pci_dev *dev, int resno);
#else
static inline int pci_iov_virtfn_bus(struct pci_dev *dev, int id)
{
	return -ENOSYS;
}
static inline int pci_iov_virtfn_devfn(struct pci_dev *dev, int id)
{
	return -ENOSYS;
}
static inline int pci_enable_sriov(struct pci_dev *dev, int nr_virtfn)
{ return -ENODEV; }
static inline int pci_iov_add_virtfn(struct pci_dev *dev, int id, int reset)
{
	return -ENOSYS;
}
static inline void pci_iov_remove_virtfn(struct pci_dev *dev,
					 int id, int reset) { }
static inline void pci_disable_sriov(struct pci_dev *dev) { }
static inline int pci_num_vf(struct pci_dev *dev) { return 0; }
static inline int pci_vfs_assigned(struct pci_dev *dev)
{ return 0; }
static inline int pci_sriov_set_totalvfs(struct pci_dev *dev, u16 numvfs)
{ return 0; }
static inline int pci_sriov_get_totalvfs(struct pci_dev *dev)
{ return 0; }
static inline resource_size_t pci_iov_resource_size(struct pci_dev *dev, int resno)
{ return 0; }
#endif

#if defined(CONFIG_HOTPLUG_PCI) || defined(CONFIG_HOTPLUG_PCI_MODULE)
void pci_hp_create_module_link(struct pci_slot *pci_slot);
void pci_hp_remove_module_link(struct pci_slot *pci_slot);
#endif


static inline int pci_pcie_cap(struct pci_dev *dev)
{
	return dev->pcie_cap;
}


static inline bool pci_is_pcie(struct pci_dev *dev)
{
	return pci_pcie_cap(dev);
}


static inline u16 pcie_caps_reg(const struct pci_dev *dev)
{
	return dev->pcie_flags_reg;
}


static inline int pci_pcie_type(const struct pci_dev *dev)
{
	return (pcie_caps_reg(dev) & PCI_EXP_FLAGS_TYPE) >> 4;
}

static inline struct pci_dev *pcie_find_root_port(struct pci_dev *dev)
{
	while (1) {
		if (!pci_is_pcie(dev))
			break;
		if (pci_pcie_type(dev) == PCI_EXP_TYPE_ROOT_PORT)
			return dev;
		if (!dev->bus->self)
			break;
		dev = dev->bus->self;
	}
	return NULL;
}

void pci_request_acs(void);
bool pci_acs_enabled(struct pci_dev *pdev, u16 acs_flags);
bool pci_acs_path_enabled(struct pci_dev *start,
			  struct pci_dev *end, u16 acs_flags);

#define PCI_VPD_LRDT			0x80	
#define PCI_VPD_LRDT_ID(x)		((x) | PCI_VPD_LRDT)


#define PCI_VPD_LTIN_ID_STRING		0x02	
#define PCI_VPD_LTIN_RO_DATA		0x10	
#define PCI_VPD_LTIN_RW_DATA		0x11	

#define PCI_VPD_LRDT_ID_STRING		PCI_VPD_LRDT_ID(PCI_VPD_LTIN_ID_STRING)
#define PCI_VPD_LRDT_RO_DATA		PCI_VPD_LRDT_ID(PCI_VPD_LTIN_RO_DATA)
#define PCI_VPD_LRDT_RW_DATA		PCI_VPD_LRDT_ID(PCI_VPD_LTIN_RW_DATA)


#define PCI_VPD_STIN_END		0x0f	

#define PCI_VPD_SRDT_END		(PCI_VPD_STIN_END << 3)

#define PCI_VPD_SRDT_TIN_MASK		0x78
#define PCI_VPD_SRDT_LEN_MASK		0x07
#define PCI_VPD_LRDT_TIN_MASK		0x7f

#define PCI_VPD_LRDT_TAG_SIZE		3
#define PCI_VPD_SRDT_TAG_SIZE		1

#define PCI_VPD_INFO_FLD_HDR_SIZE	3

#define PCI_VPD_RO_KEYWORD_PARTNO	"PN"
#define PCI_VPD_RO_KEYWORD_MFR_ID	"MN"
#define PCI_VPD_RO_KEYWORD_VENDOR0	"V0"
#define PCI_VPD_RO_KEYWORD_CHKSUM	"RV"


static inline u16 pci_vpd_lrdt_size(const u8 *lrdt)
{
	return (u16)lrdt[1] + ((u16)lrdt[2] << 8);
}


static inline u16 pci_vpd_lrdt_tag(const u8 *lrdt)
{
    return (u16)(lrdt[0] & PCI_VPD_LRDT_TIN_MASK);
}


static inline u8 pci_vpd_srdt_size(const u8 *srdt)
{
	return (*srdt) & PCI_VPD_SRDT_LEN_MASK;
}


static inline u8 pci_vpd_srdt_tag(const u8 *srdt)
{
	return ((*srdt) & PCI_VPD_SRDT_TIN_MASK) >> 3;
}


static inline u8 pci_vpd_info_field_size(const u8 *info_field)
{
	return info_field[2];
}


int pci_vpd_find_tag(const u8 *buf, unsigned int off, unsigned int len, u8 rdt);


int pci_vpd_find_info_keyword(const u8 *buf, unsigned int off,
			      unsigned int len, const char *kw);


#ifdef CONFIG_OF
struct device_node;
struct irq_domain;
void pci_set_of_node(struct pci_dev *dev);
void pci_release_of_node(struct pci_dev *dev);
void pci_set_bus_of_node(struct pci_bus *bus);
void pci_release_bus_of_node(struct pci_bus *bus);
struct irq_domain *pci_host_bridge_of_msi_domain(struct pci_bus *bus);


struct device_node *pcibios_get_phb_of_node(struct pci_bus *bus);

static inline struct device_node *
pci_device_to_OF_node(const struct pci_dev *pdev)
{
	return pdev ? pdev->dev.of_node : NULL;
}

static inline struct device_node *pci_bus_to_OF_node(struct pci_bus *bus)
{
	return bus ? bus->dev.of_node : NULL;
}

#else 
static inline void pci_set_of_node(struct pci_dev *dev) { }
static inline void pci_release_of_node(struct pci_dev *dev) { }
static inline void pci_set_bus_of_node(struct pci_bus *bus) { }
static inline void pci_release_bus_of_node(struct pci_bus *bus) { }
static inline struct device_node *
pci_device_to_OF_node(const struct pci_dev *pdev) { return NULL; }
static inline struct irq_domain *
pci_host_bridge_of_msi_domain(struct pci_bus *bus) { return NULL; }
#endif  

#ifdef CONFIG_ACPI
struct irq_domain *pci_host_bridge_acpi_msi_domain(struct pci_bus *bus);

void
pci_msi_register_fwnode_provider(struct fwnode_handle *(*fn)(struct device *));
#else
static inline struct irq_domain *
pci_host_bridge_acpi_msi_domain(struct pci_bus *bus) { return NULL; }
#endif

#ifdef CONFIG_EEH
static inline struct eeh_dev *pci_dev_to_eeh_dev(struct pci_dev *pdev)
{
	return pdev->dev.archdata.edev;
}
#endif

void pci_add_dma_alias(struct pci_dev *dev, u8 devfn);
bool pci_devs_are_dma_aliases(struct pci_dev *dev1, struct pci_dev *dev2);
int pci_for_each_dma_alias(struct pci_dev *pdev,
			   int (*fn)(struct pci_dev *pdev,
				     u16 alias, void *data), void *data);


static inline void pci_set_dev_assigned(struct pci_dev *pdev)
{
	pdev->dev_flags |= PCI_DEV_FLAGS_ASSIGNED;
}
static inline void pci_clear_dev_assigned(struct pci_dev *pdev)
{
	pdev->dev_flags &= ~PCI_DEV_FLAGS_ASSIGNED;
}
static inline bool pci_is_dev_assigned(struct pci_dev *pdev)
{
	return (pdev->dev_flags & PCI_DEV_FLAGS_ASSIGNED) == PCI_DEV_FLAGS_ASSIGNED;
}


static inline bool pci_ari_enabled(struct pci_bus *bus)
{
	return bus->self && bus->self->ari_enabled;
}


#include <linux/pci-dma-compat.h>

#define pci_printk(level, pdev, fmt, arg...) \
	dev_printk(level, &(pdev)->dev, fmt, ##arg)

#define pci_emerg(pdev, fmt, arg...)	dev_emerg(&(pdev)->dev, fmt, ##arg)
#define pci_alert(pdev, fmt, arg...)	dev_alert(&(pdev)->dev, fmt, ##arg)
#define pci_crit(pdev, fmt, arg...)	dev_crit(&(pdev)->dev, fmt, ##arg)
#define pci_err(pdev, fmt, arg...)	dev_err(&(pdev)->dev, fmt, ##arg)
#define pci_warn(pdev, fmt, arg...)	dev_warn(&(pdev)->dev, fmt, ##arg)
#define pci_notice(pdev, fmt, arg...)	dev_notice(&(pdev)->dev, fmt, ##arg)
#define pci_info(pdev, fmt, arg...)	dev_info(&(pdev)->dev, fmt, ##arg)
#define pci_dbg(pdev, fmt, arg...)	dev_dbg(&(pdev)->dev, fmt, ##arg)

#endif 
