


#define OMAP_HSMMC_SUPPORTS_DUAL_VOLT		BIT(0)
#define OMAP_HSMMC_BROKEN_MULTIBLOCK_READ	BIT(1)
#define OMAP_HSMMC_SWAKEUP_MISSING		BIT(2)

struct omap_hsmmc_dev_attr {
	u8 flags;
};

struct mmc_card;

struct omap_hsmmc_platform_data {
	
	struct device *dev;

	
	unsigned int max_freq;

	
	u8 controller_flags;

	
	u16 reg_offset;

	
	u32 caps;	
	u32 pm_caps;	

	
	unsigned internal_clock:1;

	
	unsigned nonremovable:1;

	
	unsigned no_regulator_off_init:1;

	
#define HSMMC_HAS_PBIAS		(1 << 0)
#define HSMMC_HAS_UPDATED_RESET	(1 << 1)
#define HSMMC_HAS_HSPE_SUPPORT	(1 << 2)
	unsigned features;

	int gpio_cd;			
	int gpio_cod;			
	int gpio_wp;			

	int (*set_power)(struct device *dev, int power_on, int vdd);
	void (*remux)(struct device *dev, int power_on);
	
	void (*before_set_reg)(struct device *dev, int power_on, int vdd);
	
	void (*after_set_reg)(struct device *dev, int power_on, int vdd);
	
	void (*init_card)(struct mmc_card *card);

	const char *name;
	u32 ocr_mask;
};
