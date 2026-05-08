

#ifndef __LINUX_PLATFORM_DATA_SI5351_H__
#define __LINUX_PLATFORM_DATA_SI5351_H__


enum si5351_pll_src {
	SI5351_PLL_SRC_DEFAULT = 0,
	SI5351_PLL_SRC_XTAL = 1,
	SI5351_PLL_SRC_CLKIN = 2,
};


enum si5351_multisynth_src {
	SI5351_MULTISYNTH_SRC_DEFAULT = 0,
	SI5351_MULTISYNTH_SRC_VCO0 = 1,
	SI5351_MULTISYNTH_SRC_VCO1 = 2,
};


enum si5351_clkout_src {
	SI5351_CLKOUT_SRC_DEFAULT = 0,
	SI5351_CLKOUT_SRC_MSYNTH_N = 1,
	SI5351_CLKOUT_SRC_MSYNTH_0_4 = 2,
	SI5351_CLKOUT_SRC_XTAL = 3,
	SI5351_CLKOUT_SRC_CLKIN = 4,
};


enum si5351_drive_strength {
	SI5351_DRIVE_DEFAULT = 0,
	SI5351_DRIVE_2MA = 2,
	SI5351_DRIVE_4MA = 4,
	SI5351_DRIVE_6MA = 6,
	SI5351_DRIVE_8MA = 8,
};


enum si5351_disable_state {
	SI5351_DISABLE_DEFAULT = 0,
	SI5351_DISABLE_LOW,
	SI5351_DISABLE_HIGH,
	SI5351_DISABLE_FLOATING,
	SI5351_DISABLE_NEVER,
};


struct si5351_clkout_config {
	enum si5351_multisynth_src multisynth_src;
	enum si5351_clkout_src clkout_src;
	enum si5351_drive_strength drive;
	enum si5351_disable_state disable_state;
	bool pll_master;
	unsigned long rate;
};


struct si5351_platform_data {
	enum si5351_pll_src pll_src[2];
	struct si5351_clkout_config clkout[8];
};

#endif
