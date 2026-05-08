
#ifndef _PARPORT_H_
#define _PARPORT_H_


#include <linux/jiffies.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/irqreturn.h>
#include <linux/semaphore.h>
#include <linux/device.h>
#include <asm/ptrace.h>
#include <uapi/linux/parport.h>


struct parport;
struct pardevice;

struct pc_parport_state {
	unsigned int ctr;
	unsigned int ecr;
};

struct ax_parport_state {
	unsigned int ctr;
	unsigned int ecr;
	unsigned int dcsr;
};


struct amiga_parport_state {
       unsigned char data;     
       unsigned char datadir;  
       unsigned char status;   
       unsigned char statusdir;
};

struct ax88796_parport_state {
	unsigned char cpr;
};

struct ip32_parport_state {
	unsigned int dcr;
	unsigned int ecr;
};

struct parport_state {
	union {
		struct pc_parport_state pc;
		
		struct ax_parport_state ax;
		struct amiga_parport_state amiga;
		struct ax88796_parport_state ax88796;
		
		struct ip32_parport_state ip32;
		void *misc; 
	} u;
};

struct parport_operations {
	
	void (*write_data)(struct parport *, unsigned char);
	unsigned char (*read_data)(struct parport *);

	void (*write_control)(struct parport *, unsigned char);
	unsigned char (*read_control)(struct parport *);
	unsigned char (*frob_control)(struct parport *, unsigned char mask,
				      unsigned char val);

	unsigned char (*read_status)(struct parport *);

	
	void (*enable_irq)(struct parport *);
	void (*disable_irq)(struct parport *);

	
	void (*data_forward) (struct parport *);
	void (*data_reverse) (struct parport *);

	
	void (*init_state)(struct pardevice *, struct parport_state *);
	void (*save_state)(struct parport *, struct parport_state *);
	void (*restore_state)(struct parport *, struct parport_state *);

	
	size_t (*epp_write_data) (struct parport *port, const void *buf,
				  size_t len, int flags);
	size_t (*epp_read_data) (struct parport *port, void *buf, size_t len,
				 int flags);
	size_t (*epp_write_addr) (struct parport *port, const void *buf,
				  size_t len, int flags);
	size_t (*epp_read_addr) (struct parport *port, void *buf, size_t len,
				 int flags);

	size_t (*ecp_write_data) (struct parport *port, const void *buf,
				  size_t len, int flags);
	size_t (*ecp_read_data) (struct parport *port, void *buf, size_t len,
				 int flags);
	size_t (*ecp_write_addr) (struct parport *port, const void *buf,
				  size_t len, int flags);

	size_t (*compat_write_data) (struct parport *port, const void *buf,
				     size_t len, int flags);
	size_t (*nibble_read_data) (struct parport *port, void *buf,
				    size_t len, int flags);
	size_t (*byte_read_data) (struct parport *port, void *buf,
				  size_t len, int flags);
	struct module *owner;
};

struct parport_device_info {
	parport_device_class class;
	const char *class_name;
	const char *mfr;
	const char *model;
	const char *cmdset;
	const char *description;
};




struct pardevice {
	const char *name;
	struct parport *port;
	int daisy;
	int (*preempt)(void *);
	void (*wakeup)(void *);
	void *private;
	void (*irq_func)(void *);
	unsigned int flags;
	struct pardevice *next;
	struct pardevice *prev;
	struct device dev;
	bool devmodel;
	struct parport_state *state;     
	wait_queue_head_t wait_q;
	unsigned long int time;
	unsigned long int timeslice;
	volatile long int timeout;
	unsigned long waiting;		 
	struct pardevice *waitprev;
	struct pardevice *waitnext;
	void * sysctl_table;
};

#define to_pardevice(n) container_of(n, struct pardevice, dev)




enum ieee1284_phase {
	IEEE1284_PH_FWD_DATA,
	IEEE1284_PH_FWD_IDLE,
	IEEE1284_PH_TERMINATE,
	IEEE1284_PH_NEGOTIATION,
	IEEE1284_PH_HBUSY_DNA,
	IEEE1284_PH_REV_IDLE,
	IEEE1284_PH_HBUSY_DAVAIL,
	IEEE1284_PH_REV_DATA,
	IEEE1284_PH_ECP_SETUP,
	IEEE1284_PH_ECP_FWD_TO_REV,
	IEEE1284_PH_ECP_REV_TO_FWD,
	IEEE1284_PH_ECP_DIR_UNKNOWN,
};
struct ieee1284_info {
	int mode;
	volatile enum ieee1284_phase phase;
	struct semaphore irq;
};


struct parport {
	unsigned long base;	
	unsigned long base_hi;  
	unsigned int size;	
	const char *name;
	unsigned int modes;
	int irq;		
	int dma;
	int muxport;		
	int portnum;		
	struct device *dev;	
	struct device bus_dev;	
	struct parport *physport;
				

	struct pardevice *devices;
	struct pardevice *cad;	
	int daisy;		
	int muxsel;		

	struct pardevice *waithead;
	struct pardevice *waittail;

	struct list_head list;
	unsigned int flags;

	void *sysctl_table;
	struct parport_device_info probe_info[5]; 
	struct ieee1284_info ieee1284;

	struct parport_operations *ops;
	void *private_data;     

	int number;		
	spinlock_t pardevice_lock;
	spinlock_t waitlist_lock;
	rwlock_t cad_lock;

	int spintime;
	atomic_t ref_count;

	unsigned long devflags;
#define PARPORT_DEVPROC_REGISTERED	0
	struct pardevice *proc_device;	

	struct list_head full_list;
	struct parport *slaves[3];
};

#define to_parport_dev(n) container_of(n, struct parport, bus_dev)

#define DEFAULT_SPIN_TIME 500 

struct parport_driver {
	const char *name;
	void (*attach) (struct parport *);
	void (*detach) (struct parport *);
	void (*match_port)(struct parport *);
	int (*probe)(struct pardevice *);
	struct device_driver driver;
	bool devmodel;
	struct list_head list;
};

#define to_parport_driver(n) container_of(n, struct parport_driver, driver)

int parport_bus_init(void);
void parport_bus_exit(void);


struct parport *parport_register_port(unsigned long base, int irq, int dma,
				      struct parport_operations *ops);


void parport_announce_port (struct parport *port);


extern void parport_remove_port(struct parport *port);



int __must_check __parport_register_driver(struct parport_driver *,
					   struct module *,
					   const char *mod_name);

#define parport_register_driver(driver)             \
	__parport_register_driver(driver, THIS_MODULE, KBUILD_MODNAME)


extern void parport_unregister_driver (struct parport_driver *);
void parport_unregister_driver(struct parport_driver *);


extern struct parport *parport_find_number (int);
extern struct parport *parport_find_base (unsigned long);


extern irqreturn_t parport_irq_handler(int irq, void *dev_id);


extern struct parport *parport_get_port (struct parport *);
extern void parport_put_port (struct parport *);
void parport_del_port(struct parport *);

struct pardev_cb {
	int (*preempt)(void *);
	void (*wakeup)(void *);
	void *private;
	void (*irq_func)(void *);
	unsigned int flags;
};


struct pardevice *parport_register_device(struct parport *port, 
			  const char *name,
			  int (*pf)(void *), void (*kf)(void *),
			  void (*irq_func)(void *), 
			  int flags, void *handle);

struct pardevice *
parport_register_dev_model(struct parport *port, const char *name,
			   const struct pardev_cb *par_dev_cb, int cnt);


extern void parport_unregister_device(struct pardevice *dev);


extern int parport_claim(struct pardevice *dev);


extern int parport_claim_or_block(struct pardevice *dev);



extern void parport_release(struct pardevice *dev);


static __inline__ int parport_yield(struct pardevice *dev)
{
	unsigned long int timeslip = (jiffies - dev->time);
	if ((dev->port->waithead == NULL) || (timeslip < dev->timeslice))
		return 0;
	parport_release(dev);
	return parport_claim(dev);
}


static __inline__ int parport_yield_blocking(struct pardevice *dev)
{
	unsigned long int timeslip = (jiffies - dev->time);
	if ((dev->port->waithead == NULL) || (timeslip < dev->timeslice))
		return 0;
	parport_release(dev);
	return parport_claim_or_block(dev);
}


#define PARPORT_DEV_TRAN		0	
#define PARPORT_DEV_LURK		(1<<0)	
#define PARPORT_DEV_EXCL		(1<<1)	

#define PARPORT_FLAG_EXCL		(1<<1)	


extern void parport_ieee1284_interrupt (void *);
extern int parport_negotiate (struct parport *, int mode);
extern ssize_t parport_write (struct parport *, const void *buf, size_t len);
extern ssize_t parport_read (struct parport *, void *buf, size_t len);

#define PARPORT_INACTIVITY_O_NONBLOCK 1
extern long parport_set_timeout (struct pardevice *, long inactivity);

extern int parport_wait_event (struct parport *, long timeout);
extern int parport_wait_peripheral (struct parport *port,
				    unsigned char mask,
				    unsigned char val);
extern int parport_poll_peripheral (struct parport *port,
				    unsigned char mask,
				    unsigned char val,
				    int usec);


extern size_t parport_ieee1284_write_compat (struct parport *,
					     const void *, size_t, int);
extern size_t parport_ieee1284_read_nibble (struct parport *,
					    void *, size_t, int);
extern size_t parport_ieee1284_read_byte (struct parport *,
					  void *, size_t, int);
extern size_t parport_ieee1284_ecp_read_data (struct parport *,
					      void *, size_t, int);
extern size_t parport_ieee1284_ecp_write_data (struct parport *,
					       const void *, size_t, int);
extern size_t parport_ieee1284_ecp_write_addr (struct parport *,
					       const void *, size_t, int);
extern size_t parport_ieee1284_epp_write_data (struct parport *,
					       const void *, size_t, int);
extern size_t parport_ieee1284_epp_read_data (struct parport *,
					      void *, size_t, int);
extern size_t parport_ieee1284_epp_write_addr (struct parport *,
					       const void *, size_t, int);
extern size_t parport_ieee1284_epp_read_addr (struct parport *,
					      void *, size_t, int);


extern int parport_daisy_init (struct parport *port);
extern void parport_daisy_fini (struct parport *port);
extern struct pardevice *parport_open (int devnum, const char *name);
extern void parport_close (struct pardevice *dev);
extern ssize_t parport_device_id (int devnum, char *buffer, size_t len);
extern void parport_daisy_deselect_all (struct parport *port);
extern int parport_daisy_select (struct parport *port, int daisy, int mode);


static inline void parport_generic_irq(struct parport *port)
{
	parport_ieee1284_interrupt (port);
	read_lock(&port->cad_lock);
	if (port->cad && port->cad->irq_func)
		port->cad->irq_func(port->cad->private);
	read_unlock(&port->cad_lock);
}


extern int parport_proc_register(struct parport *pp);
extern int parport_proc_unregister(struct parport *pp);
extern int parport_device_proc_register(struct pardevice *device);
extern int parport_device_proc_unregister(struct pardevice *device);


#if !defined(CONFIG_PARPORT_NOT_PC)

#include <linux/parport_pc.h>
#define parport_write_data(p,x)            parport_pc_write_data(p,x)
#define parport_read_data(p)               parport_pc_read_data(p)
#define parport_write_control(p,x)         parport_pc_write_control(p,x)
#define parport_read_control(p)            parport_pc_read_control(p)
#define parport_frob_control(p,m,v)        parport_pc_frob_control(p,m,v)
#define parport_read_status(p)             parport_pc_read_status(p)
#define parport_enable_irq(p)              parport_pc_enable_irq(p)
#define parport_disable_irq(p)             parport_pc_disable_irq(p)
#define parport_data_forward(p)            parport_pc_data_forward(p)
#define parport_data_reverse(p)            parport_pc_data_reverse(p)

#else  


#define parport_write_data(p,x)            (p)->ops->write_data(p,x)
#define parport_read_data(p)               (p)->ops->read_data(p)
#define parport_write_control(p,x)         (p)->ops->write_control(p,x)
#define parport_read_control(p)            (p)->ops->read_control(p)
#define parport_frob_control(p,m,v)        (p)->ops->frob_control(p,m,v)
#define parport_read_status(p)             (p)->ops->read_status(p)
#define parport_enable_irq(p)              (p)->ops->enable_irq(p)
#define parport_disable_irq(p)             (p)->ops->disable_irq(p)
#define parport_data_forward(p)            (p)->ops->data_forward(p)
#define parport_data_reverse(p)            (p)->ops->data_reverse(p)

#endif 

extern unsigned long parport_default_timeslice;
extern int parport_default_spintime;

#endif 
