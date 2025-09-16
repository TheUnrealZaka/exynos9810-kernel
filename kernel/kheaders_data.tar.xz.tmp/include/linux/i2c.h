








#ifndef _LINUX_I2C_H
#define _LINUX_I2C_H

#include <linux/mod_devicetable.h>
#include <linux/device.h>	
#include <linux/sched.h>	
#include <linux/mutex.h>
#include <linux/of.h>		
#include <linux/swab.h>		
#include <uapi/linux/i2c.h>

extern struct bus_type i2c_bus_type;
extern struct device_type i2c_adapter_type;



struct i2c_msg;
struct i2c_algorithm;
struct i2c_adapter;
struct i2c_client;
struct i2c_driver;
union i2c_smbus_data;
struct i2c_board_info;
enum i2c_slave_event;
typedef int (*i2c_slave_cb_t)(struct i2c_client *, enum i2c_slave_event, u8 *);

struct module;

#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)

extern int i2c_master_send(const struct i2c_client *client, const char *buf,
			   int count);
extern int i2c_master_recv(const struct i2c_client *client, char *buf,
			   int count);


extern int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs,
			int num);

extern int __i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs,
			  int num);


extern s32 i2c_smbus_xfer(struct i2c_adapter *adapter, u16 addr,
			  unsigned short flags, char read_write, u8 command,
			  int size, union i2c_smbus_data *data);



extern s32 i2c_smbus_read_byte(const struct i2c_client *client);
extern s32 i2c_smbus_write_byte(const struct i2c_client *client, u8 value);
extern s32 i2c_smbus_read_byte_data(const struct i2c_client *client,
				    u8 command);
extern s32 i2c_smbus_write_byte_data(const struct i2c_client *client,
				     u8 command, u8 value);
extern s32 i2c_smbus_read_word_data(const struct i2c_client *client,
				    u8 command);
extern s32 i2c_smbus_write_word_data(const struct i2c_client *client,
				     u8 command, u16 value);

static inline s32
i2c_smbus_read_word_swapped(const struct i2c_client *client, u8 command)
{
	s32 value = i2c_smbus_read_word_data(client, command);

	return (value < 0) ? value : swab16(value);
}

static inline s32
i2c_smbus_write_word_swapped(const struct i2c_client *client,
			     u8 command, u16 value)
{
	return i2c_smbus_write_word_data(client, command, swab16(value));
}


extern s32 i2c_smbus_read_block_data(const struct i2c_client *client,
				     u8 command, u8 *values);
extern s32 i2c_smbus_write_block_data(const struct i2c_client *client,
				      u8 command, u8 length, const u8 *values);

extern s32 i2c_smbus_read_i2c_block_data(const struct i2c_client *client,
					 u8 command, u8 length, u8 *values);
extern s32 i2c_smbus_write_i2c_block_data(const struct i2c_client *client,
					  u8 command, u8 length,
					  const u8 *values);
extern s32
i2c_smbus_read_i2c_block_data_or_emulated(const struct i2c_client *client,
					  u8 command, u8 length, u8 *values);
#endif 

enum i2c_alert_protocol {
	I2C_PROTOCOL_SMBUS_ALERT,
	I2C_PROTOCOL_SMBUS_HOST_NOTIFY,
};


struct i2c_driver {
	unsigned int class;

	
	int (*attach_adapter)(struct i2c_adapter *) __deprecated;

	
	int (*probe)(struct i2c_client *, const struct i2c_device_id *);
	int (*remove)(struct i2c_client *);

	
	void (*shutdown)(struct i2c_client *);

	
	void (*alert)(struct i2c_client *, enum i2c_alert_protocol protocol,
		      unsigned int data);

	
	int (*command)(struct i2c_client *client, unsigned int cmd, void *arg);

	struct device_driver driver;
	const struct i2c_device_id *id_table;

	
	int (*detect)(struct i2c_client *, struct i2c_board_info *);
	const unsigned short *address_list;
	struct list_head clients;
};
#define to_i2c_driver(d) container_of(d, struct i2c_driver, driver)


struct i2c_client {
	unsigned short flags;		
	unsigned short addr;		
					
					
	char name[I2C_NAME_SIZE];
	struct i2c_adapter *adapter;	
	struct device dev;		
	int irq;			
	struct list_head detected;
#if IS_ENABLED(CONFIG_I2C_SLAVE)
	i2c_slave_cb_t slave_cb;	
#endif
};
#define to_i2c_client(d) container_of(d, struct i2c_client, dev)

extern struct i2c_client *i2c_verify_client(struct device *dev);
extern struct i2c_adapter *i2c_verify_adapter(struct device *dev);

static inline struct i2c_client *kobj_to_i2c_client(struct kobject *kobj)
{
	struct device * const dev = container_of(kobj, struct device, kobj);
	return to_i2c_client(dev);
}

static inline void *i2c_get_clientdata(const struct i2c_client *dev)
{
	return dev_get_drvdata(&dev->dev);
}

static inline void i2c_set_clientdata(struct i2c_client *dev, void *data)
{
	dev_set_drvdata(&dev->dev, data);
}



#if IS_ENABLED(CONFIG_I2C_SLAVE)
enum i2c_slave_event {
	I2C_SLAVE_READ_REQUESTED,
	I2C_SLAVE_WRITE_REQUESTED,
	I2C_SLAVE_READ_PROCESSED,
	I2C_SLAVE_WRITE_RECEIVED,
	I2C_SLAVE_STOP,
};

extern int i2c_slave_register(struct i2c_client *client, i2c_slave_cb_t slave_cb);
extern int i2c_slave_unregister(struct i2c_client *client);

static inline int i2c_slave_event(struct i2c_client *client,
				  enum i2c_slave_event event, u8 *val)
{
	return client->slave_cb(client, event, val);
}
#endif


struct i2c_board_info {
	char		type[I2C_NAME_SIZE];
	unsigned short	flags;
	unsigned short	addr;
	void		*platform_data;
	struct dev_archdata	*archdata;
	struct device_node *of_node;
	struct fwnode_handle *fwnode;
	int		irq;
};


#define I2C_BOARD_INFO(dev_type, dev_addr) \
	.type = dev_type, .addr = (dev_addr)


#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)

extern struct i2c_client *
i2c_new_device(struct i2c_adapter *adap, struct i2c_board_info const *info);


extern struct i2c_client *
i2c_new_probed_device(struct i2c_adapter *adap,
		      struct i2c_board_info *info,
		      unsigned short const *addr_list,
		      int (*probe)(struct i2c_adapter *, unsigned short addr));


extern int i2c_probe_func_quick_read(struct i2c_adapter *, unsigned short addr);


extern struct i2c_client *
i2c_new_dummy(struct i2c_adapter *adap, u16 address);

extern struct i2c_client *
i2c_new_secondary_device(struct i2c_client *client,
				const char *name,
				u16 default_addr);

extern void i2c_unregister_device(struct i2c_client *);
#endif 


#ifdef CONFIG_I2C_BOARDINFO
extern int
i2c_register_board_info(int busnum, struct i2c_board_info const *info,
			unsigned n);
#else
static inline int
i2c_register_board_info(int busnum, struct i2c_board_info const *info,
			unsigned n)
{
	return 0;
}
#endif 


struct i2c_algorithm {
	
	
	int (*master_xfer)(struct i2c_adapter *adap, struct i2c_msg *msgs,
			   int num);
	int (*smbus_xfer) (struct i2c_adapter *adap, u16 addr,
			   unsigned short flags, char read_write,
			   u8 command, int size, union i2c_smbus_data *data);

	
	u32 (*functionality) (struct i2c_adapter *);

#if IS_ENABLED(CONFIG_I2C_SLAVE)
	int (*reg_slave)(struct i2c_client *client);
	int (*unreg_slave)(struct i2c_client *client);
#endif
};


struct i2c_lock_operations {
	void (*lock_bus)(struct i2c_adapter *, unsigned int flags);
	int (*trylock_bus)(struct i2c_adapter *, unsigned int flags);
	void (*unlock_bus)(struct i2c_adapter *, unsigned int flags);
};


struct i2c_timings {
	u32 bus_freq_hz;
	u32 scl_rise_ns;
	u32 scl_fall_ns;
	u32 scl_int_delay_ns;
	u32 sda_fall_ns;
};


struct i2c_bus_recovery_info {
	int (*recover_bus)(struct i2c_adapter *);

	int (*get_scl)(struct i2c_adapter *);
	void (*set_scl)(struct i2c_adapter *, int val);
	int (*get_sda)(struct i2c_adapter *);

	void (*prepare_recovery)(struct i2c_adapter *);
	void (*unprepare_recovery)(struct i2c_adapter *);

	
	int scl_gpio;
	int sda_gpio;
};

int i2c_recover_bus(struct i2c_adapter *adap);


int i2c_generic_gpio_recovery(struct i2c_adapter *adap);
int i2c_generic_scl_recovery(struct i2c_adapter *adap);



struct i2c_adapter_quirks {
	u64 flags;
	int max_num_msgs;
	u16 max_write_len;
	u16 max_read_len;
	u16 max_comb_1st_msg_len;
	u16 max_comb_2nd_msg_len;
};


#define I2C_AQ_COMB			BIT(0)

#define I2C_AQ_COMB_WRITE_FIRST		BIT(1)

#define I2C_AQ_COMB_READ_SECOND		BIT(2)

#define I2C_AQ_COMB_SAME_ADDR		BIT(3)

#define I2C_AQ_COMB_WRITE_THEN_READ	(I2C_AQ_COMB | I2C_AQ_COMB_WRITE_FIRST | \
					 I2C_AQ_COMB_READ_SECOND | I2C_AQ_COMB_SAME_ADDR)

#define I2C_AQ_NO_CLK_STRETCH		BIT(4)


struct i2c_adapter {
	struct module *owner;
	unsigned int class;		  
	const struct i2c_algorithm *algo; 
	void *algo_data;

	
	const struct i2c_lock_operations *lock_ops;
	struct rt_mutex bus_lock;
	struct rt_mutex mux_lock;

	int timeout;			
	int retries;
	struct device dev;		

	int nr;
	char name[48];
	struct completion dev_released;

	struct mutex userspace_clients_lock;
	struct list_head userspace_clients;

	struct i2c_bus_recovery_info *bus_recovery_info;
	const struct i2c_adapter_quirks *quirks;
};
#define to_i2c_adapter(d) container_of(d, struct i2c_adapter, dev)

static inline void *i2c_get_adapdata(const struct i2c_adapter *dev)
{
	return dev_get_drvdata(&dev->dev);
}

static inline void i2c_set_adapdata(struct i2c_adapter *dev, void *data)
{
	dev_set_drvdata(&dev->dev, data);
}

static inline struct i2c_adapter *
i2c_parent_is_i2c_adapter(const struct i2c_adapter *adapter)
{
#if IS_ENABLED(CONFIG_I2C_MUX)
	struct device *parent = adapter->dev.parent;

	if (parent != NULL && parent->type == &i2c_adapter_type)
		return to_i2c_adapter(parent);
	else
#endif
		return NULL;
}

int i2c_for_each_dev(void *data, int (*fn)(struct device *, void *));


#define I2C_LOCK_ROOT_ADAPTER BIT(0)
#define I2C_LOCK_SEGMENT      BIT(1)


static inline void
i2c_lock_bus(struct i2c_adapter *adapter, unsigned int flags)
{
	adapter->lock_ops->lock_bus(adapter, flags);
}


static inline int
i2c_trylock_bus(struct i2c_adapter *adapter, unsigned int flags)
{
	return adapter->lock_ops->trylock_bus(adapter, flags);
}


static inline void
i2c_unlock_bus(struct i2c_adapter *adapter, unsigned int flags)
{
	adapter->lock_ops->unlock_bus(adapter, flags);
}

static inline void
i2c_lock_adapter(struct i2c_adapter *adapter)
{
	i2c_lock_bus(adapter, I2C_LOCK_ROOT_ADAPTER);
}

static inline void
i2c_unlock_adapter(struct i2c_adapter *adapter)
{
	i2c_unlock_bus(adapter, I2C_LOCK_ROOT_ADAPTER);
}


#define I2C_CLIENT_PEC		0x04	
#define I2C_CLIENT_TEN		0x10	
					
#define I2C_CLIENT_SLAVE	0x20	
#define I2C_CLIENT_WAKE		0x80	
#define I2C_CLIENT_SCCB		0x9000	
					
#define I2C_CLIENT_SPEEDY	0x40	
					


#define I2C_CLASS_HWMON		(1<<0)	
#define I2C_CLASS_DDC		(1<<3)	
#define I2C_CLASS_SPD		(1<<7)	
#define I2C_CLASS_DEPRECATED	(1<<8)	


#define I2C_CLIENT_END		0xfffeU


#define I2C_ADDRS(addr, addrs...) \
	((const unsigned short []){ addr, ## addrs, I2C_CLIENT_END })





#if defined(CONFIG_I2C) || defined(CONFIG_I2C_MODULE)
extern int i2c_add_adapter(struct i2c_adapter *);
extern void i2c_del_adapter(struct i2c_adapter *);
extern int i2c_add_numbered_adapter(struct i2c_adapter *);

extern int i2c_register_driver(struct module *, struct i2c_driver *);
extern void i2c_del_driver(struct i2c_driver *);


#define i2c_add_driver(driver) \
	i2c_register_driver(THIS_MODULE, driver)

extern struct i2c_client *i2c_use_client(struct i2c_client *client);
extern void i2c_release_client(struct i2c_client *client);


extern void i2c_clients_command(struct i2c_adapter *adap,
				unsigned int cmd, void *arg);

extern struct i2c_adapter *i2c_get_adapter(int nr);
extern void i2c_put_adapter(struct i2c_adapter *adap);
extern unsigned int i2c_adapter_depth(struct i2c_adapter *adapter);

void i2c_parse_fw_timings(struct device *dev, struct i2c_timings *t, bool use_defaults);


static inline u32 i2c_get_functionality(struct i2c_adapter *adap)
{
	return adap->algo->functionality(adap);
}


static inline int i2c_check_functionality(struct i2c_adapter *adap, u32 func)
{
	return (func & i2c_get_functionality(adap)) == func;
}


static inline bool i2c_check_quirks(struct i2c_adapter *adap, u64 quirks)
{
	if (!adap->quirks)
		return false;
	return (adap->quirks->flags & quirks) == quirks;
}


static inline int i2c_adapter_id(struct i2c_adapter *adap)
{
	return adap->nr;
}

static inline u8 i2c_8bit_addr_from_msg(const struct i2c_msg *msg)
{
	return (msg->addr << 1) | (msg->flags & I2C_M_RD ? 1 : 0);
}


#define module_i2c_driver(__i2c_driver) \
	module_driver(__i2c_driver, i2c_add_driver, \
			i2c_del_driver)


#define builtin_i2c_driver(__i2c_driver) \
	builtin_driver(__i2c_driver, i2c_add_driver)

#endif 

#if IS_ENABLED(CONFIG_OF)

extern struct i2c_client *of_find_i2c_device_by_node(struct device_node *node);


extern struct i2c_adapter *of_find_i2c_adapter_by_node(struct device_node *node);


struct i2c_adapter *of_get_i2c_adapter_by_node(struct device_node *node);

#else

static inline struct i2c_client *of_find_i2c_device_by_node(struct device_node *node)
{
	return NULL;
}

static inline struct i2c_adapter *of_find_i2c_adapter_by_node(struct device_node *node)
{
	return NULL;
}

static inline struct i2c_adapter *of_get_i2c_adapter_by_node(struct device_node *node)
{
	return NULL;
}
#endif 

#if IS_ENABLED(CONFIG_ACPI)
u32 i2c_acpi_find_bus_speed(struct device *dev);
#else
static inline u32 i2c_acpi_find_bus_speed(struct device *dev)
{
	return 0;
}
#endif 

#endif 
