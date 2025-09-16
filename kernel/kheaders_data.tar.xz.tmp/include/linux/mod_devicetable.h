

#ifndef LINUX_MOD_DEVICETABLE_H
#define LINUX_MOD_DEVICETABLE_H

#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/uuid.h>
typedef unsigned long kernel_ulong_t;
#endif

#define PCI_ANY_ID (~0)

struct pci_device_id {
	__u32 vendor, device;		
	__u32 subvendor, subdevice;	
	__u32 class, class_mask;	
	kernel_ulong_t driver_data;	
};


#define IEEE1394_MATCH_VENDOR_ID	0x0001
#define IEEE1394_MATCH_MODEL_ID		0x0002
#define IEEE1394_MATCH_SPECIFIER_ID	0x0004
#define IEEE1394_MATCH_VERSION		0x0008

struct ieee1394_device_id {
	__u32 match_flags;
	__u32 vendor_id;
	__u32 model_id;
	__u32 specifier_id;
	__u32 version;
	kernel_ulong_t driver_data;
};





struct usb_device_id {
	
	__u16		match_flags;

	
	__u16		idVendor;
	__u16		idProduct;
	__u16		bcdDevice_lo;
	__u16		bcdDevice_hi;

	
	__u8		bDeviceClass;
	__u8		bDeviceSubClass;
	__u8		bDeviceProtocol;

	
	__u8		bInterfaceClass;
	__u8		bInterfaceSubClass;
	__u8		bInterfaceProtocol;

	
	__u8		bInterfaceNumber;

	
	kernel_ulong_t	driver_info
		__attribute__((aligned(sizeof(kernel_ulong_t))));
};


#define USB_DEVICE_ID_MATCH_VENDOR		0x0001
#define USB_DEVICE_ID_MATCH_PRODUCT		0x0002
#define USB_DEVICE_ID_MATCH_DEV_LO		0x0004
#define USB_DEVICE_ID_MATCH_DEV_HI		0x0008
#define USB_DEVICE_ID_MATCH_DEV_CLASS		0x0010
#define USB_DEVICE_ID_MATCH_DEV_SUBCLASS	0x0020
#define USB_DEVICE_ID_MATCH_DEV_PROTOCOL	0x0040
#define USB_DEVICE_ID_MATCH_INT_CLASS		0x0080
#define USB_DEVICE_ID_MATCH_INT_SUBCLASS	0x0100
#define USB_DEVICE_ID_MATCH_INT_PROTOCOL	0x0200
#define USB_DEVICE_ID_MATCH_INT_NUMBER		0x0400

#define HID_ANY_ID				(~0)
#define HID_BUS_ANY				0xffff
#define HID_GROUP_ANY				0x0000

struct hid_device_id {
	__u16 bus;
	__u16 group;
	__u32 vendor;
	__u32 product;
	kernel_ulong_t driver_data;
};


struct ccw_device_id {
	__u16	match_flags;	

	__u16	cu_type;	
	__u16	dev_type;	
	__u8	cu_model;	
	__u8	dev_model;	

	kernel_ulong_t driver_info;
};

#define CCW_DEVICE_ID_MATCH_CU_TYPE		0x01
#define CCW_DEVICE_ID_MATCH_CU_MODEL		0x02
#define CCW_DEVICE_ID_MATCH_DEVICE_TYPE		0x04
#define CCW_DEVICE_ID_MATCH_DEVICE_MODEL	0x08


struct ap_device_id {
	__u16 match_flags;	
	__u8 dev_type;		
	kernel_ulong_t driver_info;
};

#define AP_DEVICE_ID_MATCH_DEVICE_TYPE		0x01


struct css_device_id {
	__u8 match_flags;
	__u8 type; 
	kernel_ulong_t driver_data;
};

#define ACPI_ID_LEN	9

struct acpi_device_id {
	__u8 id[ACPI_ID_LEN];
	kernel_ulong_t driver_data;
	__u32 cls;
	__u32 cls_msk;
};

#define PNP_ID_LEN	8
#define PNP_MAX_DEVICES	8

struct pnp_device_id {
	__u8 id[PNP_ID_LEN];
	kernel_ulong_t driver_data;
};

struct pnp_card_device_id {
	__u8 id[PNP_ID_LEN];
	kernel_ulong_t driver_data;
	struct {
		__u8 id[PNP_ID_LEN];
	} devs[PNP_MAX_DEVICES];
};


#define SERIO_ANY	0xff

struct serio_device_id {
	__u8 type;
	__u8 extra;
	__u8 id;
	__u8 proto;
};

struct hda_device_id {
	__u32 vendor_id;
	__u32 rev_id;
	__u8 api_version;
	const char *name;
	unsigned long driver_data;
};


struct of_device_id {
	char	name[32];
	char	type[32];
	char	compatible[128];
	const void *data;
};


struct vio_device_id {
	char type[32];
	char compat[32];
};



struct pcmcia_device_id {
	__u16		match_flags;

	__u16		manf_id;
	__u16 		card_id;

	__u8  		func_id;

	
	__u8  		function;

	
	__u8  		device_no;

	__u32 		prod_id_hash[4];

	
	const char *	prod_id[4];

	
	kernel_ulong_t	driver_info;
	char *		cisfile;
};

#define PCMCIA_DEV_ID_MATCH_MANF_ID	0x0001
#define PCMCIA_DEV_ID_MATCH_CARD_ID	0x0002
#define PCMCIA_DEV_ID_MATCH_FUNC_ID	0x0004
#define PCMCIA_DEV_ID_MATCH_FUNCTION	0x0008
#define PCMCIA_DEV_ID_MATCH_PROD_ID1	0x0010
#define PCMCIA_DEV_ID_MATCH_PROD_ID2	0x0020
#define PCMCIA_DEV_ID_MATCH_PROD_ID3	0x0040
#define PCMCIA_DEV_ID_MATCH_PROD_ID4	0x0080
#define PCMCIA_DEV_ID_MATCH_DEVICE_NO	0x0100
#define PCMCIA_DEV_ID_MATCH_FAKE_CIS	0x0200
#define PCMCIA_DEV_ID_MATCH_ANONYMOUS	0x0400


#define INPUT_DEVICE_ID_EV_MAX		0x1f
#define INPUT_DEVICE_ID_KEY_MIN_INTERESTING	0x71
#define INPUT_DEVICE_ID_KEY_MAX		0x2ff
#define INPUT_DEVICE_ID_REL_MAX		0x0f
#define INPUT_DEVICE_ID_ABS_MAX		0x3f
#define INPUT_DEVICE_ID_MSC_MAX		0x07
#define INPUT_DEVICE_ID_LED_MAX		0x0f
#define INPUT_DEVICE_ID_SND_MAX		0x07
#define INPUT_DEVICE_ID_FF_MAX		0x7f
#define INPUT_DEVICE_ID_SW_MAX		0x20

#define INPUT_DEVICE_ID_MATCH_BUS	1
#define INPUT_DEVICE_ID_MATCH_VENDOR	2
#define INPUT_DEVICE_ID_MATCH_PRODUCT	4
#define INPUT_DEVICE_ID_MATCH_VERSION	8

#define INPUT_DEVICE_ID_MATCH_EVBIT	0x0010
#define INPUT_DEVICE_ID_MATCH_KEYBIT	0x0020
#define INPUT_DEVICE_ID_MATCH_RELBIT	0x0040
#define INPUT_DEVICE_ID_MATCH_ABSBIT	0x0080
#define INPUT_DEVICE_ID_MATCH_MSCIT	0x0100
#define INPUT_DEVICE_ID_MATCH_LEDBIT	0x0200
#define INPUT_DEVICE_ID_MATCH_SNDBIT	0x0400
#define INPUT_DEVICE_ID_MATCH_FFBIT	0x0800
#define INPUT_DEVICE_ID_MATCH_SWBIT	0x1000

struct input_device_id {

	kernel_ulong_t flags;

	__u16 bustype;
	__u16 vendor;
	__u16 product;
	__u16 version;

	kernel_ulong_t evbit[INPUT_DEVICE_ID_EV_MAX / BITS_PER_LONG + 1];
	kernel_ulong_t keybit[INPUT_DEVICE_ID_KEY_MAX / BITS_PER_LONG + 1];
	kernel_ulong_t relbit[INPUT_DEVICE_ID_REL_MAX / BITS_PER_LONG + 1];
	kernel_ulong_t absbit[INPUT_DEVICE_ID_ABS_MAX / BITS_PER_LONG + 1];
	kernel_ulong_t mscbit[INPUT_DEVICE_ID_MSC_MAX / BITS_PER_LONG + 1];
	kernel_ulong_t ledbit[INPUT_DEVICE_ID_LED_MAX / BITS_PER_LONG + 1];
	kernel_ulong_t sndbit[INPUT_DEVICE_ID_SND_MAX / BITS_PER_LONG + 1];
	kernel_ulong_t ffbit[INPUT_DEVICE_ID_FF_MAX / BITS_PER_LONG + 1];
	kernel_ulong_t swbit[INPUT_DEVICE_ID_SW_MAX / BITS_PER_LONG + 1];

	kernel_ulong_t driver_info;
};



#define EISA_SIG_LEN   8


struct eisa_device_id {
	char          sig[EISA_SIG_LEN];
	kernel_ulong_t driver_data;
};

#define EISA_DEVICE_MODALIAS_FMT "eisa:s%s"

struct parisc_device_id {
	__u8	hw_type;	
	__u8	hversion_rev;	
	__u16	hversion;	
	__u32	sversion;	
};

#define PA_HWTYPE_ANY_ID	0xff
#define PA_HVERSION_REV_ANY_ID	0xff
#define PA_HVERSION_ANY_ID	0xffff
#define PA_SVERSION_ANY_ID	0xffffffff



#define SDIO_ANY_ID (~0)

struct sdio_device_id {
	__u8	class;			
	__u16	vendor;			
	__u16	device;			
	kernel_ulong_t driver_data;	
};


struct ssb_device_id {
	__u16	vendor;
	__u16	coreid;
	__u8	revision;
	__u8	__pad;
} __attribute__((packed, aligned(2)));
#define SSB_DEVICE(_vendor, _coreid, _revision)  \
	{ .vendor = _vendor, .coreid = _coreid, .revision = _revision, }

#define SSB_ANY_VENDOR		0xFFFF
#define SSB_ANY_ID		0xFFFF
#define SSB_ANY_REV		0xFF


struct bcma_device_id {
	__u16	manuf;
	__u16	id;
	__u8	rev;
	__u8	class;
} __attribute__((packed,aligned(2)));
#define BCMA_CORE(_manuf, _id, _rev, _class)  \
	{ .manuf = _manuf, .id = _id, .rev = _rev, .class = _class, }

#define BCMA_ANY_MANUF		0xFFFF
#define BCMA_ANY_ID		0xFFFF
#define BCMA_ANY_REV		0xFF
#define BCMA_ANY_CLASS		0xFF

struct virtio_device_id {
	__u32 device;
	__u32 vendor;
};
#define VIRTIO_DEV_ANY_ID	0xffffffff


struct hv_vmbus_device_id {
	uuid_le guid;
	kernel_ulong_t driver_data;	
};



#define RPMSG_NAME_SIZE			32
#define RPMSG_DEVICE_MODALIAS_FMT	"rpmsg:%s"

struct rpmsg_device_id {
	char name[RPMSG_NAME_SIZE];
};



#define I2C_NAME_SIZE	20
#define I2C_MODULE_PREFIX "i2c:"

struct i2c_device_id {
	char name[I2C_NAME_SIZE];
	kernel_ulong_t driver_data;	
};



#define SPI_NAME_SIZE	32
#define SPI_MODULE_PREFIX "spi:"

struct spi_device_id {
	char name[SPI_NAME_SIZE];
	kernel_ulong_t driver_data;	
};

#define SPMI_NAME_SIZE	32
#define SPMI_MODULE_PREFIX "spmi:"

struct spmi_device_id {
	char name[SPMI_NAME_SIZE];
	kernel_ulong_t driver_data;	
};


enum dmi_field {
	DMI_NONE,
	DMI_BIOS_VENDOR,
	DMI_BIOS_VERSION,
	DMI_BIOS_DATE,
	DMI_SYS_VENDOR,
	DMI_PRODUCT_NAME,
	DMI_PRODUCT_VERSION,
	DMI_PRODUCT_SERIAL,
	DMI_PRODUCT_UUID,
	DMI_BOARD_VENDOR,
	DMI_BOARD_NAME,
	DMI_BOARD_VERSION,
	DMI_BOARD_SERIAL,
	DMI_BOARD_ASSET_TAG,
	DMI_CHASSIS_VENDOR,
	DMI_CHASSIS_TYPE,
	DMI_CHASSIS_VERSION,
	DMI_CHASSIS_SERIAL,
	DMI_CHASSIS_ASSET_TAG,
	DMI_STRING_MAX,
};

struct dmi_strmatch {
	unsigned char slot:7;
	unsigned char exact_match:1;
	char substr[79];
};

struct dmi_system_id {
	int (*callback)(const struct dmi_system_id *);
	const char *ident;
	struct dmi_strmatch matches[4];
	void *driver_data;
};

#define dmi_device_id dmi_system_id

#define DMI_MATCH(a, b)	{ .slot = a, .substr = b }
#define DMI_EXACT_MATCH(a, b)	{ .slot = a, .substr = b, .exact_match = 1 }

#define PLATFORM_NAME_SIZE	20
#define PLATFORM_MODULE_PREFIX	"platform:"

struct platform_device_id {
	char name[PLATFORM_NAME_SIZE];
	kernel_ulong_t driver_data;
};

#define MDIO_MODULE_PREFIX	"mdio:"

#define MDIO_ID_FMT "%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u%u"
#define MDIO_ID_ARGS(_id) \
	((_id)>>31) & 1, ((_id)>>30) & 1, ((_id)>>29) & 1, ((_id)>>28) & 1, \
	((_id)>>27) & 1, ((_id)>>26) & 1, ((_id)>>25) & 1, ((_id)>>24) & 1, \
	((_id)>>23) & 1, ((_id)>>22) & 1, ((_id)>>21) & 1, ((_id)>>20) & 1, \
	((_id)>>19) & 1, ((_id)>>18) & 1, ((_id)>>17) & 1, ((_id)>>16) & 1, \
	((_id)>>15) & 1, ((_id)>>14) & 1, ((_id)>>13) & 1, ((_id)>>12) & 1, \
	((_id)>>11) & 1, ((_id)>>10) & 1, ((_id)>>9) & 1, ((_id)>>8) & 1, \
	((_id)>>7) & 1, ((_id)>>6) & 1, ((_id)>>5) & 1, ((_id)>>4) & 1, \
	((_id)>>3) & 1, ((_id)>>2) & 1, ((_id)>>1) & 1, (_id) & 1


struct mdio_device_id {
	__u32 phy_id;
	__u32 phy_id_mask;
};

struct zorro_device_id {
	__u32 id;			
	kernel_ulong_t driver_data;	
};

#define ZORRO_WILDCARD			(0xffffffff)	

#define ZORRO_DEVICE_MODALIAS_FMT	"zorro:i%08X"

#define ISAPNP_ANY_ID		0xffff
struct isapnp_device_id {
	unsigned short card_vendor, card_device;
	unsigned short vendor, function;
	kernel_ulong_t driver_data;	
};


struct amba_id {
	unsigned int		id;
	unsigned int		mask;
	void			*data;
};


struct mips_cdmm_device_id {
	__u8	type;
};




#define x86cpu_device_id x86_cpu_id
struct x86_cpu_id {
	__u16 vendor;
	__u16 family;
	__u16 model;
	__u16 feature;	
	kernel_ulong_t driver_data;
	__u16 steppings;
};

#define X86_FEATURE_MATCH(x) \
	{ X86_VENDOR_ANY, X86_FAMILY_ANY, X86_MODEL_ANY, x }

#define X86_VENDOR_ANY 0xffff
#define X86_FAMILY_ANY 0
#define X86_MODEL_ANY  0
#define X86_STEPPING_ANY 0
#define X86_FEATURE_ANY 0	



struct cpu_feature {
	__u16	feature;
};

#define IPACK_ANY_FORMAT 0xff
#define IPACK_ANY_ID (~0)
struct ipack_device_id {
	__u8  format;			
	__u32 vendor;			
	__u32 device;			
};

#define MEI_CL_MODULE_PREFIX "mei:"
#define MEI_CL_NAME_SIZE 32
#define MEI_CL_VERSION_ANY 0xff


struct mei_cl_device_id {
	char name[MEI_CL_NAME_SIZE];
	uuid_le uuid;
	__u8    version;
	kernel_ulong_t driver_info;
};



#define RIO_ANY_ID	0xffff


struct rio_device_id {
	__u16 did, vid;
	__u16 asm_did, asm_vid;
};

struct mcb_device_id {
	__u16 device;
	kernel_ulong_t driver_data;
};

struct ulpi_device_id {
	__u16 vendor;
	__u16 product;
	kernel_ulong_t driver_data;
};


struct fsl_mc_device_id {
	__u16 vendor;
	const char obj_type[16];
};


#endif 
