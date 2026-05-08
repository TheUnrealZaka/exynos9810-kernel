
#ifndef AT86RF230_H
#define AT86RF230_H

struct at86rf230_platform_data {
	int rstn;
	int slp_tr;
	int dig2;
	u8 xtal_trim;
};

#endif
