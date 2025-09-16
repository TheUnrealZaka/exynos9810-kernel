#ifndef __PLATFORM_DATA_SDHCI_S3C_H
#define __PLATFORM_DATA_SDHCI_S3C_H

struct platform_device;

enum cd_types {
	S3C_SDHCI_CD_INTERNAL,	
	S3C_SDHCI_CD_EXTERNAL,	
	S3C_SDHCI_CD_GPIO,	
	S3C_SDHCI_CD_NONE,	
	S3C_SDHCI_CD_PERMANENT,	
};


struct s3c_sdhci_platdata {
	unsigned int	max_width;
	unsigned int	host_caps;
	unsigned int	host_caps2;
	unsigned int	pm_caps;
	enum cd_types	cd_type;

	int		ext_cd_gpio;
	bool		ext_cd_gpio_invert;
	int	(*ext_cd_init)(void (*notify_func)(struct platform_device *,
						   int state));
	int	(*ext_cd_cleanup)(void (*notify_func)(struct platform_device *,
						      int state));

	void	(*cfg_gpio)(struct platform_device *dev, int width);
};


#endif 
