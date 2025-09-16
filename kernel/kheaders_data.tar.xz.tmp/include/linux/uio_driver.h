

#ifndef _UIO_DRIVER_H_
#define _UIO_DRIVER_H_

#include <linux/fs.h>
#include <linux/interrupt.h>

struct module;
struct uio_map;


struct uio_mem {
	const char		*name;
	phys_addr_t		addr;
	resource_size_t		size;
	int			memtype;
	void __iomem		*internal_addr;
	struct uio_map		*map;
};

#define MAX_UIO_MAPS	5

struct uio_portio;


struct uio_port {
	const char		*name;
	unsigned long		start;
	unsigned long		size;
	int			porttype;
	struct uio_portio	*portio;
};

#define MAX_UIO_PORT_REGIONS	5

struct uio_device {
        struct module           *owner;
        struct device           *dev;
        int                     minor;
        atomic_t                event;
        struct fasync_struct    *async_queue;
        wait_queue_head_t       wait;
        struct uio_info         *info;
        struct kobject          *map_dir;
        struct kobject          *portio_dir;
};


struct uio_info {
	struct uio_device	*uio_dev;
	const char		*name;
	const char		*version;
	struct uio_mem		mem[MAX_UIO_MAPS];
	struct uio_port		port[MAX_UIO_PORT_REGIONS];
	long			irq;
	unsigned long		irq_flags;
	void			*priv;
	irqreturn_t (*handler)(int irq, struct uio_info *dev_info);
	int (*mmap)(struct uio_info *info, struct vm_area_struct *vma);
	int (*open)(struct uio_info *info, struct inode *inode);
	int (*release)(struct uio_info *info, struct inode *inode);
	int (*irqcontrol)(struct uio_info *info, s32 irq_on);
};

extern int __must_check
	__uio_register_device(struct module *owner,
			      struct device *parent,
			      struct uio_info *info);


#define uio_register_device(parent, info) \
	__uio_register_device(THIS_MODULE, parent, info)

extern void uio_unregister_device(struct uio_info *info);
extern void uio_event_notify(struct uio_info *info);


#define UIO_IRQ_CUSTOM	-1
#define UIO_IRQ_NONE	0


#define UIO_MEM_NONE	0
#define UIO_MEM_PHYS	1
#define UIO_MEM_LOGICAL	2
#define UIO_MEM_VIRTUAL 3


#define UIO_PORT_NONE	0
#define UIO_PORT_X86	1
#define UIO_PORT_GPIO	2
#define UIO_PORT_OTHER	3

#endif 
