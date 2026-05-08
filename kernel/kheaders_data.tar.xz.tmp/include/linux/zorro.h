

#ifndef _LINUX_ZORRO_H
#define _LINUX_ZORRO_H


#include <uapi/linux/zorro.h>

#include <linux/device.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/mod_devicetable.h>

#include <asm/zorro.h>


    

struct zorro_dev {
    struct ExpansionRom rom;
    zorro_id id;
    struct zorro_driver *driver;	
    struct device dev;			
    u16 slotaddr;
    u16 slotsize;
    char name[64];
    struct resource resource;
};

#define	to_zorro_dev(n)	container_of(n, struct zorro_dev, dev)


    

extern struct bus_type zorro_bus_type;


    

struct zorro_driver {
    struct list_head node;
    char *name;
    const struct zorro_device_id *id_table;	
    int (*probe)(struct zorro_dev *z, const struct zorro_device_id *id);	
    void (*remove)(struct zorro_dev *z);	
    struct device_driver driver;
};

#define	to_zorro_driver(drv)	container_of(drv, struct zorro_driver, driver)


#define zorro_for_each_dev(dev)	\
	for (dev = &zorro_autocon[0]; dev < zorro_autocon+zorro_num_autocon; dev++)



extern int zorro_register_driver(struct zorro_driver *);
extern void zorro_unregister_driver(struct zorro_driver *);
extern const struct zorro_device_id *zorro_match_device(const struct zorro_device_id *ids, const struct zorro_dev *z);
static inline struct zorro_driver *zorro_dev_driver(const struct zorro_dev *z)
{
    return z->driver;
}


extern unsigned int zorro_num_autocon;	
extern struct zorro_dev *zorro_autocon;


    

struct zorro_dev_init {
	struct ExpansionRom rom;
	u16 slotaddr;
	u16 slotsize;
	u32 boardaddr;
	u32 boardsize;
};

extern struct zorro_dev_init zorro_autocon_init[ZORRO_NUM_AUTO] __initdata;


    

extern struct zorro_dev *zorro_find_device(zorro_id id,
					   struct zorro_dev *from);

#define zorro_resource_start(z)	((z)->resource.start)
#define zorro_resource_end(z)	((z)->resource.end)
#define zorro_resource_len(z)	(resource_size(&(z)->resource))
#define zorro_resource_flags(z)	((z)->resource.flags)

#define zorro_request_device(z, name) \
    request_mem_region(zorro_resource_start(z), zorro_resource_len(z), name)
#define zorro_release_device(z) \
    release_mem_region(zorro_resource_start(z), zorro_resource_len(z))


static inline void *zorro_get_drvdata (struct zorro_dev *z)
{
	return dev_get_drvdata(&z->dev);
}

static inline void zorro_set_drvdata (struct zorro_dev *z, void *data)
{
	dev_set_drvdata(&z->dev, data);
}


    

extern DECLARE_BITMAP(zorro_unused_z2ram, 128);

#define Z2RAM_START		(0x00200000)
#define Z2RAM_END		(0x00a00000)
#define Z2RAM_SIZE		(0x00800000)
#define Z2RAM_CHUNKSIZE		(0x00010000)
#define Z2RAM_CHUNKMASK		(0x0000ffff)
#define Z2RAM_CHUNKSHIFT	(16)


#endif 
