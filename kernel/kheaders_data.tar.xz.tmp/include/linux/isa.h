

#ifndef __LINUX_ISA_H
#define __LINUX_ISA_H

#include <linux/device.h>
#include <linux/errno.h>
#include <linux/kernel.h>

struct isa_driver {
	int (*match)(struct device *, unsigned int);
	int (*probe)(struct device *, unsigned int);
	int (*remove)(struct device *, unsigned int);
	void (*shutdown)(struct device *, unsigned int);
	int (*suspend)(struct device *, unsigned int, pm_message_t);
	int (*resume)(struct device *, unsigned int);

	struct device_driver driver;
	struct device *devices;
};

#define to_isa_driver(x) container_of((x), struct isa_driver, driver)

#ifdef CONFIG_ISA_BUS_API
int isa_register_driver(struct isa_driver *, unsigned int);
void isa_unregister_driver(struct isa_driver *);
#else
static inline int isa_register_driver(struct isa_driver *d, unsigned int i)
{
	return -ENODEV;
}

static inline void isa_unregister_driver(struct isa_driver *d)
{
}
#endif


#define module_isa_driver(__isa_driver, __num_isa_dev) \
static int __init __isa_driver##_init(void) \
{ \
	return isa_register_driver(&(__isa_driver), __num_isa_dev); \
} \
module_init(__isa_driver##_init); \
static void __exit __isa_driver##_exit(void) \
{ \
	isa_unregister_driver(&(__isa_driver)); \
} \
module_exit(__isa_driver##_exit);


#define max_num_isa_dev(__isa_dev_ext) (1024 / __isa_dev_ext)

#endif 
