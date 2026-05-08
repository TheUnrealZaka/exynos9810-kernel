
#ifndef __LINUX_FMC_H__
#define __LINUX_FMC_H__
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/io.h>

struct fmc_device;
struct fmc_driver;



#define FMC_MAJOR	3
#define FMC_MINOR	0
#define FMC_VERSION	((FMC_MAJOR << 16) | FMC_MINOR)
#define __FMC_MAJOR(x)	((x) >> 16)
#define __FMC_MINOR(x)	((x) & 0xffff)


struct fmc_fru_id {
	char *manufacturer;
	char *product_name;
};


struct fmc_sdb_one_id {
	uint64_t vendor;
	uint32_t device;
};
struct fmc_sdb_id {
	struct fmc_sdb_one_id *cores;
	int cores_nr;
};

struct fmc_device_id {
	struct fmc_fru_id *fru_id;
	int fru_id_nr;
	struct fmc_sdb_id *sdb_id;
	int sdb_id_nr;
};


#define FMC_MAX_CARDS 32


struct fmc_driver {
	unsigned long version;
	struct device_driver driver;
	int (*probe)(struct fmc_device *);
	int (*remove)(struct fmc_device *);
	const struct fmc_device_id id_table;
	
	int busid_n;
	int busid_val[FMC_MAX_CARDS];
	int gw_n;
	char *gw_val[FMC_MAX_CARDS];
};
#define to_fmc_driver(x) container_of((x), struct fmc_driver, driver)


#define FMC_PARAM_BUSID(_d) \
    module_param_array_named(busid, _d.busid_val, int, &_d.busid_n, 0444)
#define FMC_PARAM_GATEWARE(_d) \
    module_param_array_named(gateware, _d.gw_val, charp, &_d.gw_n, 0444)


struct fmc_gpio {
	char *carrier_name; 
	int gpio;
	int _gpio;	
	int mode;	
	int irqmode;	
};


#define FMC_GPIO_RAW(x)		(x)		
#define __FMC_GPIO_IS_RAW(x)	((x) < 0x1000)
#define FMC_GPIO_IRQ(x)		((x) + 0x1000)	
#define FMC_GPIO_LED(x)		((x) + 0x1100)	
#define FMC_GPIO_KEY(x)		((x) + 0x1200)	
#define FMC_GPIO_TP(x)		((x) + 0x1300)	
#define FMC_GPIO_USER(x)	((x) + 0x1400)	



#ifndef GPIOF_DIR_IN
#  define GPIOF_DIR_OUT   (0 << 0)
#  define GPIOF_DIR_IN    (1 << 0)
#  define GPIOF_INIT_LOW  (0 << 1)
#  define GPIOF_INIT_HIGH (1 << 1)
#endif


struct fmc_operations {
	uint32_t (*read32)(struct fmc_device *fmc, int offset);
	void (*write32)(struct fmc_device *fmc, uint32_t value, int offset);
	int (*validate)(struct fmc_device *fmc, struct fmc_driver *drv);
	int (*reprogram)(struct fmc_device *f, struct fmc_driver *d, char *gw);
	int (*irq_request)(struct fmc_device *fmc, irq_handler_t h,
			   char *name, int flags);
	void (*irq_ack)(struct fmc_device *fmc);
	int (*irq_free)(struct fmc_device *fmc);
	int (*gpio_config)(struct fmc_device *fmc, struct fmc_gpio *gpio,
			   int ngpio);
	int (*read_ee)(struct fmc_device *fmc, int pos, void *d, int l);
	int (*write_ee)(struct fmc_device *fmc, int pos, const void *d, int l);
};


extern int fmc_reprogram(struct fmc_device *f, struct fmc_driver *d, char *gw,
		     int sdb_entry);


struct fmc_device {
	unsigned long version;
	unsigned long flags;
	struct module *owner;		
	struct fmc_fru_id id;		
	struct fmc_operations *op;	
	int irq;			
	int eeprom_len;			
	int eeprom_addr;		
	uint8_t *eeprom;		
	char *carrier_name;		
	void *carrier_data;		
	__iomem void *fpga_base;	
	__iomem void *slot_base;	
	struct fmc_device **devarray;	
	int slot_id;			
	int nr_slots;			
	unsigned long memlen;		
	struct device dev;		
	struct device *hwdev;		
	unsigned long sdbfs_entry;
	struct sdb_array *sdb;
	uint32_t device_id;		
	char *mezzanine_name;		
	void *mezzanine_data;
};
#define to_fmc_device(x) container_of((x), struct fmc_device, dev)

#define FMC_DEVICE_HAS_GOLDEN		1
#define FMC_DEVICE_HAS_CUSTOM		2
#define FMC_DEVICE_NO_MEZZANINE		4
#define FMC_DEVICE_MATCH_SDB		8 


static inline uint32_t fmc_readl(struct fmc_device *fmc, int offset)
{
	if (unlikely(fmc->op->read32))
		return fmc->op->read32(fmc, offset);
	return readl(fmc->fpga_base + offset);
}
static inline void fmc_writel(struct fmc_device *fmc, uint32_t val, int off)
{
	if (unlikely(fmc->op->write32))
		fmc->op->write32(fmc, val, off);
	else
		writel(val, fmc->fpga_base + off);
}


static inline void *fmc_get_drvdata(const struct fmc_device *fmc)
{
	return dev_get_drvdata(&fmc->dev);
}

static inline void fmc_set_drvdata(struct fmc_device *fmc, void *data)
{
	dev_set_drvdata(&fmc->dev, data);
}


extern int fmc_driver_register(struct fmc_driver *drv);
extern void fmc_driver_unregister(struct fmc_driver *drv);
extern int fmc_device_register(struct fmc_device *tdev);
extern void fmc_device_unregister(struct fmc_device *tdev);


extern int fmc_device_register_n(struct fmc_device **devs, int n);
extern void fmc_device_unregister_n(struct fmc_device **devs, int n);


extern int fmc_match(struct device *dev, struct device_driver *drv);
extern int fmc_fill_id_info(struct fmc_device *fmc);
extern void fmc_free_id_info(struct fmc_device *fmc);
extern void fmc_dump_eeprom(const struct fmc_device *fmc);
extern void fmc_dump_sdb(const struct fmc_device *fmc);

#endif 
