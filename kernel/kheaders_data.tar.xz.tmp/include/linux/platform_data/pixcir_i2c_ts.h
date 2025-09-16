#ifndef	_PIXCIR_I2C_TS_H
#define	_PIXCIR_I2C_TS_H


#define PIXCIR_REG_POWER_MODE	51
#define PIXCIR_REG_INT_MODE	52


enum pixcir_power_mode {
	PIXCIR_POWER_ACTIVE,
	PIXCIR_POWER_IDLE,
	PIXCIR_POWER_HALT,
};

#define PIXCIR_POWER_MODE_MASK	0x03
#define PIXCIR_POWER_ALLOW_IDLE (1UL << 2)


enum pixcir_int_mode {
	PIXCIR_INT_PERIODICAL,
	PIXCIR_INT_DIFF_COORD,
	PIXCIR_INT_LEVEL_TOUCH,
	PIXCIR_INT_PULSE_TOUCH,
};

#define PIXCIR_INT_MODE_MASK	0x03
#define PIXCIR_INT_ENABLE	(1UL << 3)
#define PIXCIR_INT_POL_HIGH	(1UL << 2)


struct pixcir_i2c_chip_data {
	u8 max_fingers;
	bool has_hw_ids;
};

struct pixcir_ts_platform_data {
	int x_max;
	int y_max;
	struct pixcir_i2c_chip_data chip;
};

#endif
