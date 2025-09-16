
#ifndef __EMIF_PLAT_H
#define __EMIF_PLAT_H


#define EMIF_LP_MODE_DISABLE				0
#define EMIF_LP_MODE_CLOCK_STOP				1
#define EMIF_LP_MODE_SELF_REFRESH			2
#define EMIF_LP_MODE_PWR_DN				4


#define	EMIF_HW_CAPS_LL_INTERFACE			0x00000001


#define	EMIF_4D						1
#define	EMIF_4D5					2


#define	EMIF_PHY_TYPE_ATTILAPHY				1
#define	EMIF_PHY_TYPE_INTELLIPHY			2


#define EMIF_CUSTOM_CONFIG_LPMODE			0x00000001
#define EMIF_CUSTOM_CONFIG_TEMP_ALERT_POLL_INTERVAL	0x00000002
#define EMIF_CUSTOM_CONFIG_EXTENDED_TEMP_PART		0x00000004

#ifndef __ASSEMBLY__

struct ddr_device_info {
	u32	type;
	u32	density;
	u32	io_width;
	u32	cs1_used;
	u32	cal_resistors_per_cs;
	char	manufacturer[10];
};


struct emif_custom_configs {
	u32 mask;
	u32 lpmode;
	u32 lpmode_timeout_performance;
	u32 lpmode_timeout_power;
	u32 lpmode_freq_threshold;
	u32 temp_alert_poll_interval_ms;
};


struct emif_platform_data {
	u32 hw_caps;
	struct ddr_device_info *device_info;
	const struct lpddr2_timings *timings;
	u32 timings_arr_size;
	const struct lpddr2_min_tck *min_tck;
	struct emif_custom_configs *custom_configs;
	u32 ip_rev;
	u32 phy_type;
};
#endif 

#endif 
