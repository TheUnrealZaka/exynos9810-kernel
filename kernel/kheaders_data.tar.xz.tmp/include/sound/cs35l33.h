

#ifndef __CS35L33_H
#define __CS35L33_H

struct cs35l33_hg {
	bool enable_hg_algo;
	unsigned int mem_depth;
	unsigned int release_rate;
	unsigned int hd_rm;
	unsigned int ldo_thld;
	unsigned int ldo_path_disable;
	unsigned int ldo_entry_delay;
	bool vp_hg_auto;
	unsigned int vp_hg;
	unsigned int vp_hg_rate;
	unsigned int vp_hg_va;
};

struct cs35l33_pdata {
	
	unsigned int boost_ctl;

	
	unsigned int boost_ipk;

	
	unsigned int amp_drv_sel;

	
	unsigned int ramp_rate;

	
	unsigned int imon_adc_scale;

	
	struct cs35l33_hg hg_config;
};

#endif 
