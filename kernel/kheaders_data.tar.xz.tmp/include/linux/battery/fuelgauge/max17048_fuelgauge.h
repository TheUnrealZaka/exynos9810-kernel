

#ifndef __MAX17048_FUELGAUGE_H
#define __MAX17048_FUELGAUGE_H __FILE__


#define SEC_FUELGAUGE_I2C_SLAVEADDR (0x6D >> 1)

#define MAX17048_VCELL_MSB	0x02
#define MAX17048_VCELL_LSB	0x03
#define MAX17048_SOC_MSB		0x04
#define MAX17048_SOC_LSB		0x05
#define MAX17048_MODE_MSB	0x06
#define MAX17048_MODE_LSB		0x07
#define MAX17048_VER_MSB		0x08
#define MAX17048_VER_LSB		0x09
#define MAX17048_RCOMP_MSB	0x0C
#define MAX17048_RCOMP_LSB	0x0D
#define MAX17048_OCV_MSB		0x0E
#define MAX17048_OCV_LSB		0x0F
#define MAX17048_CMD_MSB		0xFE
#define MAX17048_CMD_LSB		0xFF

#define RCOMP0_TEMP		20

#define AVER_SAMPLE_CNT		5

struct battery_data_t {
	u8 RCOMP0;
	u8 RCOMP_charging;
	int temp_cohot;
	int temp_cocold;
	bool is_using_model_data;
	u8 *type_str;
};

struct sec_fg_info {
	bool dummy;
};

#endif 
