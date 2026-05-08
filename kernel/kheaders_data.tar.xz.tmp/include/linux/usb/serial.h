

#ifndef __LINUX_USB_SERIAL_H
#define __LINUX_USB_SERIAL_H

#include <linux/kref.h>
#include <linux/mutex.h>
#include <linux/serial.h>
#include <linux/sysrq.h>
#include <linux/kfifo.h>


#define MAX_NUM_PORTS		8


#define RELEVANT_IFLAG(iflag)	(iflag & (IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK))


#define USB_SERIAL_WRITE_BUSY	0


struct usb_serial_port {
	struct usb_serial	*serial;
	struct tty_port		port;
	spinlock_t		lock;
	u32			minor;
	u8			port_number;

	unsigned char		*interrupt_in_buffer;
	struct urb		*interrupt_in_urb;
	__u8			interrupt_in_endpointAddress;

	unsigned char		*interrupt_out_buffer;
	int			interrupt_out_size;
	struct urb		*interrupt_out_urb;
	__u8			interrupt_out_endpointAddress;

	unsigned char		*bulk_in_buffer;
	int			bulk_in_size;
	struct urb		*read_urb;
	__u8			bulk_in_endpointAddress;

	unsigned char		*bulk_in_buffers[2];
	struct urb		*read_urbs[2];
	unsigned long		read_urbs_free;

	unsigned char		*bulk_out_buffer;
	int			bulk_out_size;
	struct urb		*write_urb;
	struct kfifo		write_fifo;

	unsigned char		*bulk_out_buffers[2];
	struct urb		*write_urbs[2];
	unsigned long		write_urbs_free;
	__u8			bulk_out_endpointAddress;

	struct async_icount	icount;
	int			tx_bytes;

	unsigned long		flags;
	wait_queue_head_t	write_wait;
	struct work_struct	work;
	char			throttled;
	char			throttle_req;
	unsigned long		sysrq; 
	struct device		dev;
};
#define to_usb_serial_port(d) container_of(d, struct usb_serial_port, dev)


static inline void *usb_get_serial_port_data(struct usb_serial_port *port)
{
	return dev_get_drvdata(&port->dev);
}

static inline void usb_set_serial_port_data(struct usb_serial_port *port,
					    void *data)
{
	dev_set_drvdata(&port->dev, data);
}


struct usb_serial {
	struct usb_device		*dev;
	struct usb_serial_driver	*type;
	struct usb_interface		*interface;
	unsigned char			disconnected:1;
	unsigned char			suspending:1;
	unsigned char			attached:1;
	unsigned char			minors_reserved:1;
	unsigned char			num_ports;
	unsigned char			num_port_pointers;
	char				num_interrupt_in;
	char				num_interrupt_out;
	char				num_bulk_in;
	char				num_bulk_out;
	struct usb_serial_port		*port[MAX_NUM_PORTS];
	struct kref			kref;
	struct mutex			disc_mutex;
	void				*private;
};
#define to_usb_serial(d) container_of(d, struct usb_serial, kref)


static inline void *usb_get_serial_data(struct usb_serial *serial)
{
	return serial->private;
}

static inline void usb_set_serial_data(struct usb_serial *serial, void *data)
{
	serial->private = data;
}


struct usb_serial_driver {
	const char *description;
	const struct usb_device_id *id_table;
	char	num_ports;

	struct list_head	driver_list;
	struct device_driver	driver;
	struct usb_driver	*usb_driver;
	struct usb_dynids	dynids;

	size_t			bulk_in_size;
	size_t			bulk_out_size;

	int (*probe)(struct usb_serial *serial, const struct usb_device_id *id);
	int (*attach)(struct usb_serial *serial);
	int (*calc_num_ports) (struct usb_serial *serial);

	void (*disconnect)(struct usb_serial *serial);
	void (*release)(struct usb_serial *serial);

	int (*port_probe)(struct usb_serial_port *port);
	int (*port_remove)(struct usb_serial_port *port);

	int (*suspend)(struct usb_serial *serial, pm_message_t message);
	int (*resume)(struct usb_serial *serial);
	int (*reset_resume)(struct usb_serial *serial);

	
	
	int  (*open)(struct tty_struct *tty, struct usb_serial_port *port);
	void (*close)(struct usb_serial_port *port);
	int  (*write)(struct tty_struct *tty, struct usb_serial_port *port,
			const unsigned char *buf, int count);
	
	int  (*write_room)(struct tty_struct *tty);
	int  (*ioctl)(struct tty_struct *tty,
		      unsigned int cmd, unsigned long arg);
	void (*set_termios)(struct tty_struct *tty,
			struct usb_serial_port *port, struct ktermios *old);
	void (*break_ctl)(struct tty_struct *tty, int break_state);
	int  (*chars_in_buffer)(struct tty_struct *tty);
	void (*wait_until_sent)(struct tty_struct *tty, long timeout);
	bool (*tx_empty)(struct usb_serial_port *port);
	void (*throttle)(struct tty_struct *tty);
	void (*unthrottle)(struct tty_struct *tty);
	int  (*tiocmget)(struct tty_struct *tty);
	int  (*tiocmset)(struct tty_struct *tty,
			 unsigned int set, unsigned int clear);
	int  (*tiocmiwait)(struct tty_struct *tty, unsigned long arg);
	int  (*get_icount)(struct tty_struct *tty,
			struct serial_icounter_struct *icount);
	
	void (*dtr_rts)(struct usb_serial_port *port, int on);
	int  (*carrier_raised)(struct usb_serial_port *port);
	
	void (*init_termios)(struct tty_struct *tty);
	
	void (*read_int_callback)(struct urb *urb);
	void (*write_int_callback)(struct urb *urb);
	void (*read_bulk_callback)(struct urb *urb);
	void (*write_bulk_callback)(struct urb *urb);
	
	void (*process_read_urb)(struct urb *urb);
	
	int (*prepare_write_buffer)(struct usb_serial_port *port,
						void *dest, size_t size);
};
#define to_usb_serial_driver(d) \
	container_of(d, struct usb_serial_driver, driver)

extern int usb_serial_register_drivers(struct usb_serial_driver *const serial_drivers[],
		const char *name, const struct usb_device_id *id_table);
extern void usb_serial_deregister_drivers(struct usb_serial_driver *const serial_drivers[]);
extern void usb_serial_port_softint(struct usb_serial_port *port);

extern int usb_serial_suspend(struct usb_interface *intf, pm_message_t message);
extern int usb_serial_resume(struct usb_interface *intf);


#ifdef CONFIG_USB_SERIAL_CONSOLE
extern void usb_serial_console_init(int minor);
extern void usb_serial_console_exit(void);
extern void usb_serial_console_disconnect(struct usb_serial *serial);
#else
static inline void usb_serial_console_init(int minor) { }
static inline void usb_serial_console_exit(void) { }
static inline void usb_serial_console_disconnect(struct usb_serial *serial) {}
#endif


extern struct usb_serial_port *usb_serial_port_get_by_minor(unsigned int minor);
extern void usb_serial_put(struct usb_serial *serial);
extern int usb_serial_generic_open(struct tty_struct *tty,
	struct usb_serial_port *port);
extern int usb_serial_generic_write_start(struct usb_serial_port *port,
							gfp_t mem_flags);
extern int usb_serial_generic_write(struct tty_struct *tty,
	struct usb_serial_port *port, const unsigned char *buf, int count);
extern void usb_serial_generic_close(struct usb_serial_port *port);
extern int usb_serial_generic_resume(struct usb_serial *serial);
extern int usb_serial_generic_write_room(struct tty_struct *tty);
extern int usb_serial_generic_chars_in_buffer(struct tty_struct *tty);
extern void usb_serial_generic_wait_until_sent(struct tty_struct *tty,
								long timeout);
extern void usb_serial_generic_read_bulk_callback(struct urb *urb);
extern void usb_serial_generic_write_bulk_callback(struct urb *urb);
extern void usb_serial_generic_throttle(struct tty_struct *tty);
extern void usb_serial_generic_unthrottle(struct tty_struct *tty);
extern int usb_serial_generic_tiocmiwait(struct tty_struct *tty,
							unsigned long arg);
extern int usb_serial_generic_get_icount(struct tty_struct *tty,
					struct serial_icounter_struct *icount);
extern int usb_serial_generic_register(void);
extern void usb_serial_generic_deregister(void);
extern int usb_serial_generic_submit_read_urbs(struct usb_serial_port *port,
						 gfp_t mem_flags);
extern void usb_serial_generic_process_read_urb(struct urb *urb);
extern int usb_serial_generic_prepare_write_buffer(struct usb_serial_port *port,
						void *dest, size_t size);
extern int usb_serial_handle_sysrq_char(struct usb_serial_port *port,
					unsigned int ch);
extern int usb_serial_handle_break(struct usb_serial_port *port);
extern void usb_serial_handle_dcd_change(struct usb_serial_port *usb_port,
					 struct tty_struct *tty,
					 unsigned int status);


extern int usb_serial_bus_register(struct usb_serial_driver *device);
extern void usb_serial_bus_deregister(struct usb_serial_driver *device);

extern struct usb_serial_driver usb_serial_generic_device;
extern struct bus_type usb_serial_bus_type;
extern struct tty_driver *usb_serial_tty_driver;

static inline void usb_serial_debug_data(struct device *dev,
					 const char *function, int size,
					 const unsigned char *data)
{
	dev_dbg(dev, "%s - length = %d, data = %*ph\n",
		function, size, size, data);
}


#define dev_err_console(usport, fmt, ...)				\
do {									\
	static bool __print_once;					\
	struct usb_serial_port *__port = (usport);			\
									\
	if (!__port->port.console || !__print_once) {			\
		__print_once = true;					\
		dev_err(&__port->dev, fmt, ##__VA_ARGS__);		\
	}								\
} while (0)


#define usb_serial_module_driver(__name, __serial_drivers, __ids)	\
static int __init usb_serial_module_init(void)				\
{									\
	return usb_serial_register_drivers(__serial_drivers,		\
					   __name, __ids);		\
}									\
module_init(usb_serial_module_init);					\
static void __exit usb_serial_module_exit(void)				\
{									\
	usb_serial_deregister_drivers(__serial_drivers);		\
}									\
module_exit(usb_serial_module_exit);

#define module_usb_serial_driver(__serial_drivers, __ids)		\
	usb_serial_module_driver(KBUILD_MODNAME, __serial_drivers, __ids)

#endif 

