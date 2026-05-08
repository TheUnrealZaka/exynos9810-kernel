

#ifndef _LINUX_I2C_SMBUS_H
#define _LINUX_I2C_SMBUS_H

#include <linux/i2c.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>



struct i2c_smbus_alert_setup {
	unsigned int		alert_edge_triggered:1;
	int			irq;
};

struct i2c_client *i2c_setup_smbus_alert(struct i2c_adapter *adapter,
					 struct i2c_smbus_alert_setup *setup);
int i2c_handle_smbus_alert(struct i2c_client *ara);


struct smbus_host_notify {
	struct i2c_adapter	*adapter;
	struct work_struct	work;
	spinlock_t		lock;
	bool			pending;
	u16			payload;
	u8			addr;
};

struct smbus_host_notify *i2c_setup_smbus_host_notify(struct i2c_adapter *adap);
int i2c_handle_smbus_host_notify(struct smbus_host_notify *host_notify,
				 unsigned short addr, unsigned int data);

#endif 
