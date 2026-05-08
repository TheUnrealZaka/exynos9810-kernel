

#define OMAP_MMC_MAX_SLOTS	2

struct mmc_card;

struct omap_mmc_platform_data {
	
	struct device *dev;

	
	unsigned nr_slots:2;

	
	unsigned int max_freq;

	
	int (*switch_slot)(struct device *dev, int slot);
	
	int (*init)(struct device *dev);
	void (*cleanup)(struct device *dev);
	void (*shutdown)(struct device *dev);

	
	int (*get_context_loss_count)(struct device *dev);

	
	u8 controller_flags;

	
	u16 reg_offset;

	struct omap_mmc_slot_data {

		
		u8  wires;	
		u32 caps;	
		u32 pm_caps;	

		
		unsigned nomux:1;

		
		unsigned cover:1;

		
		unsigned internal_clock:1;

		
		unsigned nonremovable:1;

		
		unsigned power_saving:1;

		
		unsigned no_off:1;

		
		unsigned no_regulator_off_init:1;

		
		unsigned vcc_aux_disable_is_sleep:1;

		
#define MMC_OMAP7XX		(1 << 3)
#define MMC_OMAP15XX		(1 << 4)
#define MMC_OMAP16XX		(1 << 5)
		unsigned features;

		int switch_pin;			
		int gpio_wp;			

		int (*set_bus_mode)(struct device *dev, int slot, int bus_mode);
		int (*set_power)(struct device *dev, int slot,
				 int power_on, int vdd);
		int (*get_ro)(struct device *dev, int slot);
		void (*remux)(struct device *dev, int slot, int power_on);
		
		void (*before_set_reg)(struct device *dev, int slot,
				       int power_on, int vdd);
		
		void (*after_set_reg)(struct device *dev, int slot,
				      int power_on, int vdd);
		
		void (*init_card)(struct mmc_card *card);

		
		int (*get_cover_state)(struct device *dev, int slot);

		const char *name;
		u32 ocr_mask;

		
		int card_detect_irq;
		int (*card_detect)(struct device *dev, int slot);

		unsigned int ban_openended:1;

	} slots[OMAP_MMC_MAX_SLOTS];
};
