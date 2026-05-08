

#ifndef __SOUND_MAX98506_PDATA_H__
#define __SOUND_MAX98506_PDATA_H__

#include <sound/maxim_dsm.h>

#define MAX98506_I2C_ADDR	0x62
#define MAX98506_I2C_ADDR_S (MAX98506_I2C_ADDR >> 1)


enum volume_step {
	MAX98506_VSTEP_0 = 0,
	MAX98506_VSTEP_1,
	MAX98506_VSTEP_2,
	MAX98506_VSTEP_3,
	MAX98506_VSTEP_4,
	MAX98506_VSTEP_5,
	MAX98506_VSTEP_6,
	MAX98506_VSTEP_7,
	MAX98506_VSTEP_8,
	MAX98506_VSTEP_9,
	MAX98506_VSTEP_10,
	MAX98506_VSTEP_11,
	MAX98506_VSTEP_12,
	MAX98506_VSTEP_13,
	MAX98506_VSTEP_14,
	MAX98506_VSTEP_15,
	MAX98506_VSTEP_MAX,
};


enum one_stop_mode {
	MAX98506_OSM_MONO_L = 0,
	MAX98506_OSM_MONO_R,
	MAX98506_OSM_RCV_L,
	MAX98506_OSM_RCV_R,
	MAX98506_OSM_STEREO,
	MAX98506_OSM_STEREO_MODE2,
	MAX98506_OSM_MAX,
};

#ifdef CONFIG_SND_SOC_MAXIM_DSM_CAL
extern struct class *g_class;
#else
struct class *g_class;
#endif 

struct max98506_volume_step_info {
	int length;
	int vol_step;
	int adc_thres;
	unsigned int boost_step[MAX98506_VSTEP_MAX];
	bool adc_status;
};

struct max98506_pc_active {
	u32 capture_active;
	u32 playback_active:1;
};

#ifdef CONFIG_SND_SOC_MAXIM_DSM
#define MAX98506_PINFO_SZ	PARAM_OFFSET_MAX
#else
#define MAX98506_PINFO_SZ	6
#endif 

struct max98506_pdata {
	int sysclk;
	u32 spk_gain;
	u32 vmon_slot;
	bool i2c_pull_up;
#ifdef USE_MAX98506_IRQ
	int irq;
#endif 
	uint32_t pinfo[MAX98506_PINFO_SZ];
	const uint32_t *reg_arr;
	uint32_t reg_arr_len;
	int sub_reg;
	int interleave;
	int osm;
	uint32_t boostv;
	uint32_t rev_id;
	bool nodsm;
};

#endif 
