







#ifndef _LINUX_I2C_ALGO_PCF_H
#define _LINUX_I2C_ALGO_PCF_H

struct i2c_algo_pcf_data {
	void *data;		
	void (*setpcf) (void *data, int ctl, int val);
	int  (*getpcf) (void *data, int ctl);
	int  (*getown) (void *data);
	int  (*getclock) (void *data);
	void (*waitforpin) (void *data);

	void (*xfer_begin) (void *data);
	void (*xfer_end) (void *data);

	
	unsigned long lab_mdelay;
};

int i2c_pcf_add_bus(struct i2c_adapter *);

#endif 
