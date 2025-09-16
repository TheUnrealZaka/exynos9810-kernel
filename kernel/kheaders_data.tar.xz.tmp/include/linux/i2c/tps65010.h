

#ifndef __LINUX_I2C_TPS65010_H
#define __LINUX_I2C_TPS65010_H



#define	TPS_CHGSTATUS		0x01
#	define	TPS_CHG_USB		(1 << 7)
#	define	TPS_CHG_AC		(1 << 6)
#	define	TPS_CHG_THERM		(1 << 5)
#	define	TPS_CHG_TERM		(1 << 4)
#	define	TPS_CHG_TAPER_TMO	(1 << 3)
#	define	TPS_CHG_CHG_TMO		(1 << 2)
#	define	TPS_CHG_PRECHG_TMO	(1 << 1)
#	define	TPS_CHG_TEMP_ERR	(1 << 0)
#define	TPS_REGSTATUS		0x02
#	define	TPS_REG_ONOFF		(1 << 7)
#	define	TPS_REG_COVER		(1 << 6)
#	define	TPS_REG_UVLO		(1 << 5)
#	define	TPS_REG_NO_CHG		(1 << 4)	
#	define	TPS_REG_PG_LD02		(1 << 3)
#	define	TPS_REG_PG_LD01		(1 << 2)
#	define	TPS_REG_PG_MAIN		(1 << 1)
#	define	TPS_REG_PG_CORE		(1 << 0)
#define	TPS_MASK1		0x03
#define	TPS_MASK2		0x04
#define	TPS_ACKINT1		0x05
#define	TPS_ACKINT2		0x06
#define	TPS_CHGCONFIG		0x07
#	define	TPS_CHARGE_POR		(1 << 7)	
#	define	TPS65013_AUA		(1 << 7)	
#	define	TPS_CHARGE_RESET	(1 << 6)
#	define	TPS_CHARGE_FAST		(1 << 5)
#	define	TPS_CHARGE_CURRENT	(3 << 3)
#	define	TPS_VBUS_500MA		(1 << 2)
#	define	TPS_VBUS_CHARGING	(1 << 1)
#	define	TPS_CHARGE_ENABLE	(1 << 0)
#define	TPS_LED1_ON		0x08
#define	TPS_LED1_PER		0x09
#define	TPS_LED2_ON		0x0a
#define	TPS_LED2_PER		0x0b
#define	TPS_VDCDC1		0x0c
#	define	TPS_ENABLE_LP		(1 << 3)
#define	TPS_VDCDC2		0x0d
#	define	TPS_LP_COREOFF	(1 << 7)
#	define 	TPS_VCORE_1_8V	(7<<4)
#	define 	TPS_VCORE_1_5V	(6 << 4)
#	define 	TPS_VCORE_1_4V	(5 << 4)
#	define 	TPS_VCORE_1_3V	(4 << 4)
#	define 	TPS_VCORE_1_2V	(3 << 4)
#	define 	TPS_VCORE_1_1V	(2 << 4)
#	define 	TPS_VCORE_1_0V	(1 << 4)
#	define 	TPS_VCORE_0_85V	(0 << 4)
#	define	TPS_VCORE_LP_1_2V (3 << 2)
#	define	TPS_VCORE_LP_1_1V (2 << 2)
#	define	TPS_VCORE_LP_1_0V (1 << 2)
#	define	TPS_VCORE_LP_0_85V (0 << 2)
#	define	TPS_VIB		(1 << 1)
#	define	TPS_VCORE_DISCH	(1 << 0)
#define	TPS_VREGS1		0x0e
#	define	TPS_LDO2_ENABLE	(1 << 7)
#	define	TPS_LDO2_OFF	(1 << 6)
#	define	TPS_VLDO2_3_0V	(3 << 4)
#	define	TPS_VLDO2_2_75V	(2 << 4)
#	define	TPS_VLDO2_2_5V	(1 << 4)
#	define	TPS_VLDO2_1_8V	(0 << 4)
#	define	TPS_LDO1_ENABLE	(1 << 3)
#	define	TPS_LDO1_OFF	(1 << 2)
#	define	TPS_VLDO1_3_0V	(3 << 0)
#	define	TPS_VLDO1_2_75V	(2 << 0)
#	define	TPS_VLDO1_2_5V	(1 << 0)
#	define	TPS_VLDO1_ADJ	(0 << 0)
#define	TPS_MASK3		0x0f
#define	TPS_DEFGPIO		0x10



#define LED1  1
#define LED2  2
#define OFF   0
#define ON    1
#define BLINK 2
#define GPIO1 1
#define GPIO2 2
#define GPIO3 3
#define GPIO4 4
#define LOW   0
#define HIGH  1




extern int tps65010_set_vbus_draw(unsigned mA);


extern int tps65010_set_gpio_out_value(unsigned gpio, unsigned value);


extern int tps65010_set_led(unsigned led, unsigned mode);


extern int tps65010_set_vib(unsigned value);


extern int tps65010_set_low_pwr(unsigned mode);


extern int tps65010_config_vregs1(unsigned value);


extern int tps65013_set_low_pwr(unsigned mode);


extern int tps65010_config_vdcdc2(unsigned value);

struct i2c_client;


struct tps65010_board {
	int				base;
	unsigned			outmask;

	int		(*setup)(struct i2c_client *client, void *context);
	int		(*teardown)(struct i2c_client *client, void *context);
	void		*context;
};

#endif 

