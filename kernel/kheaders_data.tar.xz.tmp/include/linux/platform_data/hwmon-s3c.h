

#ifndef __HWMON_S3C_H__
#define __HWMON_S3C_H__


struct s3c_hwmon_chcfg {
	const char	*name;
	unsigned int	mult;
	unsigned int	div;
};


struct s3c_hwmon_pdata {
	struct s3c_hwmon_chcfg	*in[8];
};


extern void __init s3c_hwmon_set_platdata(struct s3c_hwmon_pdata *pd);

#endif 
