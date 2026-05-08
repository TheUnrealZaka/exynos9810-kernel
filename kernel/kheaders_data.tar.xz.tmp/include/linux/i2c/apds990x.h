

#ifndef __APDS990X_H__
#define __APDS990X_H__


#define APDS_IRLED_CURR_12mA	0x3
#define APDS_IRLED_CURR_25mA	0x2
#define APDS_IRLED_CURR_50mA	0x1
#define APDS_IRLED_CURR_100mA	0x0


#define APDS_PARAM_SCALE 4096
struct apds990x_chip_factors {
	int ga;
	int cf1;
	int irf1;
	int cf2;
	int irf2;
	int df;
};



struct apds990x_platform_data {
	struct apds990x_chip_factors cf;
	u8     pdrive;
	u8     ppcount;
	int    (*setup_resources)(void);
	int    (*release_resources)(void);
};

#endif
