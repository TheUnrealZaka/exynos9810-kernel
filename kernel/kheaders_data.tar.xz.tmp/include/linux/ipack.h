

#include <linux/mod_devicetable.h>
#include <linux/device.h>
#include <linux/interrupt.h>

#define IPACK_IDPROM_OFFSET_I			0x01
#define IPACK_IDPROM_OFFSET_P			0x03
#define IPACK_IDPROM_OFFSET_A			0x05
#define IPACK_IDPROM_OFFSET_C			0x07
#define IPACK_IDPROM_OFFSET_MANUFACTURER_ID	0x09
#define IPACK_IDPROM_OFFSET_MODEL		0x0B
#define IPACK_IDPROM_OFFSET_REVISION		0x0D
#define IPACK_IDPROM_OFFSET_RESERVED		0x0F
#define IPACK_IDPROM_OFFSET_DRIVER_ID_L		0x11
#define IPACK_IDPROM_OFFSET_DRIVER_ID_H		0x13
#define IPACK_IDPROM_OFFSET_NUM_BYTES		0x15
#define IPACK_IDPROM_OFFSET_CRC			0x17




#define IPACK_ID_VERSION_INVALID	0x00
#define IPACK_ID_VERSION_1		0x01
#define IPACK_ID_VERSION_2		0x02


#define IPACK1_VENDOR_ID_RESERVED1	0x00
#define IPACK1_VENDOR_ID_RESERVED2	0xFF
#define IPACK1_VENDOR_ID_UNREGISTRED01	0x01
#define IPACK1_VENDOR_ID_UNREGISTRED02	0x02
#define IPACK1_VENDOR_ID_UNREGISTRED03	0x03
#define IPACK1_VENDOR_ID_UNREGISTRED04	0x04
#define IPACK1_VENDOR_ID_UNREGISTRED05	0x05
#define IPACK1_VENDOR_ID_UNREGISTRED06	0x06
#define IPACK1_VENDOR_ID_UNREGISTRED07	0x07
#define IPACK1_VENDOR_ID_UNREGISTRED08	0x08
#define IPACK1_VENDOR_ID_UNREGISTRED09	0x09
#define IPACK1_VENDOR_ID_UNREGISTRED10	0x0A
#define IPACK1_VENDOR_ID_UNREGISTRED11	0x0B
#define IPACK1_VENDOR_ID_UNREGISTRED12	0x0C
#define IPACK1_VENDOR_ID_UNREGISTRED13	0x0D
#define IPACK1_VENDOR_ID_UNREGISTRED14	0x0E
#define IPACK1_VENDOR_ID_UNREGISTRED15	0x0F

#define IPACK1_VENDOR_ID_SBS            0xF0
#define IPACK1_DEVICE_ID_SBS_OCTAL_232  0x22
#define IPACK1_DEVICE_ID_SBS_OCTAL_422  0x2A
#define IPACK1_DEVICE_ID_SBS_OCTAL_485  0x48

struct ipack_bus_ops;
struct ipack_driver;

enum ipack_space {
	IPACK_IO_SPACE    = 0,
	IPACK_ID_SPACE,
	IPACK_INT_SPACE,
	IPACK_MEM8_SPACE,
	IPACK_MEM16_SPACE,
	
	IPACK_SPACE_COUNT,
};


struct ipack_region {
	phys_addr_t start;
	size_t      size;
};


struct ipack_device {
	unsigned int slot;
	struct ipack_bus_device *bus;
	struct device dev;
	void (*release) (struct ipack_device *dev);
	struct ipack_region      region[IPACK_SPACE_COUNT];
	u8                      *id;
	size_t			 id_avail;
	u32			 id_vendor;
	u32			 id_device;
	u8			 id_format;
	unsigned int		 id_crc_correct:1;
	unsigned int		 speed_8mhz:1;
	unsigned int		 speed_32mhz:1;
};



struct ipack_driver_ops {
	int (*probe) (struct ipack_device *dev);
	void (*remove) (struct ipack_device *dev);
};


struct ipack_driver {
	struct device_driver driver;
	const struct ipack_device_id *id_table;
	const struct ipack_driver_ops *ops;
};


struct ipack_bus_ops {
	int (*request_irq) (struct ipack_device *dev,
			    irqreturn_t (*handler)(void *), void *arg);
	int (*free_irq) (struct ipack_device *dev);
	int (*get_clockrate) (struct ipack_device *dev);
	int (*set_clockrate) (struct ipack_device *dev, int mherz);
	int (*get_error) (struct ipack_device *dev);
	int (*get_timeout) (struct ipack_device *dev);
	int (*reset_timeout) (struct ipack_device *dev);
};


struct ipack_bus_device {
	struct module *owner;
	struct device *parent;
	int slots;
	int bus_nr;
	const struct ipack_bus_ops *ops;
};


struct ipack_bus_device *ipack_bus_register(struct device *parent, int slots,
					    const struct ipack_bus_ops *ops,
					    struct module *owner);


int ipack_bus_unregister(struct ipack_bus_device *bus);


int ipack_driver_register(struct ipack_driver *edrv, struct module *owner,
			  const char *name);
void ipack_driver_unregister(struct ipack_driver *edrv);


int ipack_device_init(struct ipack_device *dev);


int ipack_device_add(struct ipack_device *dev);
void ipack_device_del(struct ipack_device *dev);

void ipack_get_device(struct ipack_device *dev);
void ipack_put_device(struct ipack_device *dev);


#define DEFINE_IPACK_DEVICE_TABLE(_table) \
	const struct ipack_device_id _table[]

#define IPACK_DEVICE(_format, vend, dev) \
	 .format = (_format), \
	 .vendor = (vend), \
	 .device = (dev)


static inline int ipack_get_carrier(struct ipack_device *dev)
{
	return try_module_get(dev->bus->owner);
}


static inline void ipack_put_carrier(struct ipack_device *dev)
{
	module_put(dev->bus->owner);
}
