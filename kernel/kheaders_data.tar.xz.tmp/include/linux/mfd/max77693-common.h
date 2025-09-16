

#ifndef __LINUX_MFD_MAX77693_COMMON_H
#define __LINUX_MFD_MAX77693_COMMON_H

enum max77693_types {
	TYPE_MAX77693_UNKNOWN,
	TYPE_MAX77693,
	TYPE_MAX77843,

	TYPE_MAX77693_NUM,
};


struct max77693_dev {
	struct device *dev;
	struct i2c_client *i2c;		
	struct i2c_client *i2c_muic;	
	struct i2c_client *i2c_haptic;	
	struct i2c_client *i2c_chg;	

	enum max77693_types type;

	struct regmap *regmap;
	struct regmap *regmap_muic;
	struct regmap *regmap_haptic;	
	struct regmap *regmap_chg;	

	struct regmap_irq_chip_data *irq_data_led;
	struct regmap_irq_chip_data *irq_data_topsys;
	struct regmap_irq_chip_data *irq_data_chg; 
	struct regmap_irq_chip_data *irq_data_muic;

	int irq;
};


#endif 
