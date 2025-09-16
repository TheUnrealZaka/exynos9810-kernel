

#ifndef LINUX_RIO_H
#define LINUX_RIO_H

#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/rio_regs.h>
#include <linux/mod_devicetable.h>
#ifdef CONFIG_RAPIDIO_DMA_ENGINE
#include <linux/dmaengine.h>
#endif

#define RIO_NO_HOPCOUNT		-1
#define RIO_INVALID_DESTID	0xffff

#define RIO_MAX_MPORTS		8
#define RIO_MAX_MPORT_RESOURCES	16
#define RIO_MAX_DEV_RESOURCES	16
#define RIO_MAX_MPORT_NAME	40

#define RIO_GLOBAL_TABLE	0xff	

#define RIO_INVALID_ROUTE	0xff	

#define RIO_MAX_ROUTE_ENTRIES(size)	(size ? (1 << 16) : (1 << 8))
#define RIO_ANY_DESTID(size)		(size ? 0xffff : 0xff)

#define RIO_MAX_MBOX		4
#define RIO_MAX_MSG_SIZE	0x1000


#define RIO_SUCCESSFUL			0x00
#define RIO_BAD_SIZE			0x81


#define RIO_DOORBELL_RESOURCE	0
#define RIO_INB_MBOX_RESOURCE	1
#define RIO_OUTB_MBOX_RESOURCE	2

#define RIO_PW_MSG_SIZE		64


#define RIO_CTAG_RESRVD	0xfffe0000 
#define RIO_CTAG_UDEVID	0x0001ffff 

extern struct bus_type rio_bus_type;
extern struct class rio_mport_class;

struct rio_mport;
struct rio_dev;
union rio_pw_msg;


struct rio_switch {
	struct list_head node;
	u8 *route_table;
	u32 port_ok;
	struct rio_switch_ops *ops;
	spinlock_t lock;
	struct rio_dev *nextdev[0];
};


struct rio_switch_ops {
	struct module *owner;
	int (*add_entry) (struct rio_mport *mport, u16 destid, u8 hopcount,
			  u16 table, u16 route_destid, u8 route_port);
	int (*get_entry) (struct rio_mport *mport, u16 destid, u8 hopcount,
			  u16 table, u16 route_destid, u8 *route_port);
	int (*clr_table) (struct rio_mport *mport, u16 destid, u8 hopcount,
			  u16 table);
	int (*set_domain) (struct rio_mport *mport, u16 destid, u8 hopcount,
			   u8 sw_domain);
	int (*get_domain) (struct rio_mport *mport, u16 destid, u8 hopcount,
			   u8 *sw_domain);
	int (*em_init) (struct rio_dev *dev);
	int (*em_handle) (struct rio_dev *dev, u8 swport);
};

enum rio_device_state {
	RIO_DEVICE_INITIALIZING,
	RIO_DEVICE_RUNNING,
	RIO_DEVICE_GONE,
	RIO_DEVICE_SHUTDOWN,
};


struct rio_dev {
	struct list_head global_list;	
	struct list_head net_list;	
	struct rio_net *net;	
	bool do_enum;
	u16 did;
	u16 vid;
	u32 device_rev;
	u16 asm_did;
	u16 asm_vid;
	u16 asm_rev;
	u16 efptr;
	u32 pef;
	u32 swpinfo;
	u32 src_ops;
	u32 dst_ops;
	u32 comp_tag;
	u32 phys_efptr;
	u32 phys_rmap;
	u32 em_efptr;
	u64 dma_mask;
	struct rio_driver *driver;	
	struct device dev;	
	struct resource riores[RIO_MAX_DEV_RESOURCES];
	int (*pwcback) (struct rio_dev *rdev, union rio_pw_msg *msg, int step);
	u16 destid;
	u8 hopcount;
	struct rio_dev *prev;
	atomic_t state;
	struct rio_switch rswitch[0];	
};

#define rio_dev_g(n) list_entry(n, struct rio_dev, global_list)
#define rio_dev_f(n) list_entry(n, struct rio_dev, net_list)
#define	to_rio_dev(n) container_of(n, struct rio_dev, dev)
#define sw_to_rio_dev(n) container_of(n, struct rio_dev, rswitch[0])
#define	to_rio_mport(n) container_of(n, struct rio_mport, dev)
#define	to_rio_net(n) container_of(n, struct rio_net, dev)


struct rio_msg {
	struct resource *res;
	void (*mcback) (struct rio_mport * mport, void *dev_id, int mbox, int slot);
};


struct rio_dbell {
	struct list_head node;
	struct resource *res;
	void (*dinb) (struct rio_mport *mport, void *dev_id, u16 src, u16 dst, u16 info);
	void *dev_id;
};


struct rio_mport {
	struct list_head dbells;	
	struct list_head pwrites;	
	struct list_head node;	
	struct list_head nnode;	
	struct rio_net *net;	
	struct mutex lock;
	struct resource iores;
	struct resource riores[RIO_MAX_MPORT_RESOURCES];
	struct rio_msg inb_msg[RIO_MAX_MBOX];
	struct rio_msg outb_msg[RIO_MAX_MBOX];
	int host_deviceid;	
	struct rio_ops *ops;	
	unsigned char id;	
	unsigned char index;	
	unsigned int sys_size;	
	u32 phys_efptr;
	u32 phys_rmap;
	unsigned char name[RIO_MAX_MPORT_NAME];
	struct device dev;
	void *priv;		
#ifdef CONFIG_RAPIDIO_DMA_ENGINE
	struct dma_device	dma;
#endif
	struct rio_scan *nscan;
	atomic_t state;
	unsigned int pwe_refcnt;
};

static inline int rio_mport_is_running(struct rio_mport *mport)
{
	return atomic_read(&mport->state) == RIO_DEVICE_RUNNING;
}


#define RIO_SCAN_ENUM_NO_WAIT	0x00000001 


struct rio_net {
	struct list_head node;	
	struct list_head devices;	
	struct list_head switches;	
	struct list_head mports;	
	struct rio_mport *hport;	
	unsigned char id;	
	struct device dev;
	void *enum_data;	
	void (*release)(struct rio_net *net);
};

enum rio_link_speed {
	RIO_LINK_DOWN = 0, 
	RIO_LINK_125 = 1, 
	RIO_LINK_250 = 2, 
	RIO_LINK_312 = 3, 
	RIO_LINK_500 = 4, 
	RIO_LINK_625 = 5  
};

enum rio_link_width {
	RIO_LINK_1X  = 0,
	RIO_LINK_1XR = 1,
	RIO_LINK_2X  = 3,
	RIO_LINK_4X  = 2,
	RIO_LINK_8X  = 4,
	RIO_LINK_16X = 5
};

enum rio_mport_flags {
	RIO_MPORT_DMA	 = (1 << 0), 
	RIO_MPORT_DMA_SG = (1 << 1), 
	RIO_MPORT_IBSG	 = (1 << 2), 
};


struct rio_mport_attr {
	int flags;
	int link_speed;
	int link_width;

	
	int dma_max_sge;
	int dma_max_size;
	int dma_align;
};




struct rio_ops {
	int (*lcread) (struct rio_mport *mport, int index, u32 offset, int len,
			u32 *data);
	int (*lcwrite) (struct rio_mport *mport, int index, u32 offset, int len,
			u32 data);
	int (*cread) (struct rio_mport *mport, int index, u16 destid,
			u8 hopcount, u32 offset, int len, u32 *data);
	int (*cwrite) (struct rio_mport *mport, int index, u16 destid,
			u8 hopcount, u32 offset, int len, u32 data);
	int (*dsend) (struct rio_mport *mport, int index, u16 destid, u16 data);
	int (*pwenable) (struct rio_mport *mport, int enable);
	int (*open_outb_mbox)(struct rio_mport *mport, void *dev_id,
			      int mbox, int entries);
	void (*close_outb_mbox)(struct rio_mport *mport, int mbox);
	int  (*open_inb_mbox)(struct rio_mport *mport, void *dev_id,
			     int mbox, int entries);
	void (*close_inb_mbox)(struct rio_mport *mport, int mbox);
	int  (*add_outb_message)(struct rio_mport *mport, struct rio_dev *rdev,
				 int mbox, void *buffer, size_t len);
	int (*add_inb_buffer)(struct rio_mport *mport, int mbox, void *buf);
	void *(*get_inb_message)(struct rio_mport *mport, int mbox);
	int (*map_inb)(struct rio_mport *mport, dma_addr_t lstart,
			u64 rstart, u64 size, u32 flags);
	void (*unmap_inb)(struct rio_mport *mport, dma_addr_t lstart);
	int (*query_mport)(struct rio_mport *mport,
			   struct rio_mport_attr *attr);
	int (*map_outb)(struct rio_mport *mport, u16 destid, u64 rstart,
			u32 size, u32 flags, dma_addr_t *laddr);
	void (*unmap_outb)(struct rio_mport *mport, u16 destid, u64 rstart);
};

#define RIO_RESOURCE_MEM	0x00000100
#define RIO_RESOURCE_DOORBELL	0x00000200
#define RIO_RESOURCE_MAILBOX	0x00000400

#define RIO_RESOURCE_CACHEABLE	0x00010000
#define RIO_RESOURCE_PCI	0x00020000

#define RIO_RESOURCE_BUSY	0x80000000


struct rio_driver {
	struct list_head node;
	char *name;
	const struct rio_device_id *id_table;
	int (*probe) (struct rio_dev * dev, const struct rio_device_id * id);
	void (*remove) (struct rio_dev * dev);
	void (*shutdown)(struct rio_dev *dev);
	int (*suspend) (struct rio_dev * dev, u32 state);
	int (*resume) (struct rio_dev * dev);
	int (*enable_wake) (struct rio_dev * dev, u32 state, int enable);
	struct device_driver driver;
};

#define	to_rio_driver(drv) container_of(drv,struct rio_driver, driver)

union rio_pw_msg {
	struct {
		u32 comptag;	
		u32 errdetect;	
		u32 is_port;	
		u32 ltlerrdet;	
		u32 padding[12];
	} em;
	u32 raw[RIO_PW_MSG_SIZE/sizeof(u32)];
};

#ifdef CONFIG_RAPIDIO_DMA_ENGINE


enum rio_write_type {
	RDW_DEFAULT,		
	RDW_ALL_NWRITE,		
	RDW_ALL_NWRITE_R,	
	RDW_LAST_NWRITE_R,	
};

struct rio_dma_ext {
	u16 destid;
	u64 rio_addr;	
	u8  rio_addr_u;  
	enum rio_write_type wr_type; 
};

struct rio_dma_data {
	
	struct scatterlist	*sg;	
	unsigned int		sg_len;	
	
	u64 rio_addr;	
	u8  rio_addr_u;  
	enum rio_write_type wr_type; 
};

static inline struct rio_mport *dma_to_mport(struct dma_device *ddev)
{
	return container_of(ddev, struct rio_mport, dma);
}
#endif 


struct rio_scan {
	struct module *owner;
	int (*enumerate)(struct rio_mport *mport, u32 flags);
	int (*discover)(struct rio_mport *mport, u32 flags);
};


struct rio_scan_node {
	int mport_id;
	struct list_head node;
	struct rio_scan *ops;
};


extern int rio_mport_initialize(struct rio_mport *);
extern int rio_register_mport(struct rio_mport *);
extern int rio_unregister_mport(struct rio_mport *);
extern int rio_open_inb_mbox(struct rio_mport *, void *, int, int);
extern void rio_close_inb_mbox(struct rio_mport *, int);
extern int rio_open_outb_mbox(struct rio_mport *, void *, int, int);
extern void rio_close_outb_mbox(struct rio_mport *, int);
extern int rio_query_mport(struct rio_mport *port,
			   struct rio_mport_attr *mport_attr);

#endif				
