

#ifndef __DA7219_PDATA_H
#define __DA7219_PDATA_H


enum da7219_micbias_voltage {
	DA7219_MICBIAS_1_6V = 0,
	DA7219_MICBIAS_1_8V,
	DA7219_MICBIAS_2_0V,
	DA7219_MICBIAS_2_2V,
	DA7219_MICBIAS_2_4V,
	DA7219_MICBIAS_2_6V,
};


enum da7219_mic_amp_in_sel {
	DA7219_MIC_AMP_IN_SEL_DIFF = 0,
	DA7219_MIC_AMP_IN_SEL_SE_P,
	DA7219_MIC_AMP_IN_SEL_SE_N,
};

struct da7219_aad_pdata;

struct da7219_pdata {
	bool wakeup_source;

	
	enum da7219_micbias_voltage micbias_lvl;
	enum da7219_mic_amp_in_sel mic_amp_in_sel;

	
	struct da7219_aad_pdata *aad_pdata;
};

#endif 
