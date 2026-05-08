

#ifndef _ARIZONA_PDATA_H
#define _ARIZONA_PDATA_H

#include <dt-bindings/mfd/arizona.h>
#include <linux/regulator/arizona-ldo1.h>
#include <linux/regulator/arizona-micsupp.h>

#define ARIZONA_GPN_DIR_MASK                     0x8000  
#define ARIZONA_GPN_DIR_SHIFT                        15  
#define ARIZONA_GPN_DIR_WIDTH                         1  
#define ARIZONA_GPN_PU_MASK                      0x4000  
#define ARIZONA_GPN_PU_SHIFT                         14  
#define ARIZONA_GPN_PU_WIDTH                          1  
#define ARIZONA_GPN_PD_MASK                      0x2000  
#define ARIZONA_GPN_PD_SHIFT                         13  
#define ARIZONA_GPN_PD_WIDTH                          1  
#define ARIZONA_GPN_LVL_MASK                     0x0800  
#define ARIZONA_GPN_LVL_SHIFT                        11  
#define ARIZONA_GPN_LVL_WIDTH                         1  
#define ARIZONA_GPN_POL_MASK                     0x0400  
#define ARIZONA_GPN_POL_SHIFT                        10  
#define ARIZONA_GPN_POL_WIDTH                         1  
#define ARIZONA_GPN_OP_CFG_MASK                  0x0200  
#define ARIZONA_GPN_OP_CFG_SHIFT                      9  
#define ARIZONA_GPN_OP_CFG_WIDTH                      1  
#define ARIZONA_GPN_DB_MASK                      0x0100  
#define ARIZONA_GPN_DB_SHIFT                          8  
#define ARIZONA_GPN_DB_WIDTH                          1  
#define ARIZONA_GPN_FN_MASK                      0x007F  
#define ARIZONA_GPN_FN_SHIFT                          0  
#define ARIZONA_GPN_FN_WIDTH                          7  

#define ARIZONA_MAX_GPIO 5

#define ARIZONA_MAX_INPUT 4

#define ARIZONA_MAX_MICBIAS 3

#define ARIZONA_MAX_OUTPUT 6

#define ARIZONA_MAX_AIF 3

#define ARIZONA_HAP_ACT_ERM 0
#define ARIZONA_HAP_ACT_LRA 2

#define ARIZONA_MAX_PDM_SPK 2

struct regulator_init_data;

struct arizona_micbias {
	int mV;                    
	unsigned int ext_cap:1;    
	unsigned int discharge:1;  
	unsigned int soft_start:1; 
	unsigned int bypass:1;     
};

struct arizona_micd_config {
	unsigned int src;
	unsigned int bias;
	bool gpio;
};

struct arizona_micd_range {
	int max;  
	int key;  
};

struct arizona_pdata {
	int reset;      

	
	struct arizona_micsupp_pdata micvdd;

	
	struct arizona_ldo1_pdata ldo1;

	
	int clk32k_src;

	
	unsigned int irq_flags;

	
	int gpio_base;

	
	unsigned int gpio_defaults[ARIZONA_MAX_GPIO];

	
	unsigned int max_channels_clocked[ARIZONA_MAX_AIF];

	
	bool jd_gpio5;

	
	bool jd_gpio5_nopull;

	
	bool jd_invert;

	
	bool hpdet_acc_id;

	
	bool hpdet_acc_id_line;

	
	int hpdet_id_gpio;

	
	unsigned int hpdet_channel;

	
	bool micd_software_compare;

	
	unsigned int micd_detect_debounce;

	
	int micd_pol_gpio;

	
	unsigned int micd_bias_start_time;

	
	unsigned int micd_rate;

	
	unsigned int micd_dbtime;

	
	unsigned int micd_timeout;

	
	bool micd_force_micbias;

	
	const struct arizona_micd_range *micd_ranges;
	int num_micd_ranges;

	
	struct arizona_micd_config *micd_configs;
	int num_micd_configs;

	
	int dmic_ref[ARIZONA_MAX_INPUT];

	
	struct arizona_micbias micbias[ARIZONA_MAX_MICBIAS];

	
	int inmode[ARIZONA_MAX_INPUT];

	
	int out_mono[ARIZONA_MAX_OUTPUT];

	
	unsigned int spk_mute[ARIZONA_MAX_PDM_SPK];

	
	unsigned int spk_fmt[ARIZONA_MAX_PDM_SPK];

	
	unsigned int hap_act;

	
	int irq_gpio;

	
	unsigned int gpsw;
};

#endif
