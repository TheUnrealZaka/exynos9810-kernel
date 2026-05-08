

#ifndef IIO_FREQUENCY_AD9523_H_
#define IIO_FREQUENCY_AD9523_H_

enum outp_drv_mode {
	TRISTATE,
	LVPECL_8mA,
	LVDS_4mA,
	LVDS_7mA,
	HSTL0_16mA,
	HSTL1_8mA,
	CMOS_CONF1,
	CMOS_CONF2,
	CMOS_CONF3,
	CMOS_CONF4,
	CMOS_CONF5,
	CMOS_CONF6,
	CMOS_CONF7,
	CMOS_CONF8,
	CMOS_CONF9
};

enum ref_sel_mode {
	NONEREVERTIVE_STAY_ON_REFB,
	REVERT_TO_REFA,
	SELECT_REFA,
	SELECT_REFB,
	EXT_REF_SEL
};



struct ad9523_channel_spec {
	unsigned		channel_num;
	bool			divider_output_invert_en;
	bool			sync_ignore_en;
	bool			low_power_mode_en;
				 
	bool			use_alt_clock_src;
	bool			output_dis;
	enum outp_drv_mode	driver_mode;
	unsigned char		divider_phase;
	unsigned short		channel_divider;
	char			extended_name[16];
};

enum pll1_rzero_resistor {
	RZERO_883_OHM,
	RZERO_677_OHM,
	RZERO_341_OHM,
	RZERO_135_OHM,
	RZERO_10_OHM,
	RZERO_USE_EXT_RES = 8,
};

enum rpole2_resistor {
	RPOLE2_900_OHM,
	RPOLE2_450_OHM,
	RPOLE2_300_OHM,
	RPOLE2_225_OHM,
};

enum rzero_resistor {
	RZERO_3250_OHM,
	RZERO_2750_OHM,
	RZERO_2250_OHM,
	RZERO_2100_OHM,
	RZERO_3000_OHM,
	RZERO_2500_OHM,
	RZERO_2000_OHM,
	RZERO_1850_OHM,
};

enum cpole1_capacitor {
	CPOLE1_0_PF,
	CPOLE1_8_PF,
	CPOLE1_16_PF,
	CPOLE1_24_PF,
	_CPOLE1_24_PF, 
	CPOLE1_32_PF,
	CPOLE1_40_PF,
	CPOLE1_48_PF,
};



struct ad9523_platform_data {
	unsigned long vcxo_freq;

	
	bool				refa_diff_rcv_en;
	bool				refb_diff_rcv_en;
	bool				zd_in_diff_en;
	bool				osc_in_diff_en;

	
	bool				refa_cmos_neg_inp_en;
	bool				refb_cmos_neg_inp_en;
	bool				zd_in_cmos_neg_inp_en;
	bool				osc_in_cmos_neg_inp_en;

	
	unsigned short			refa_r_div;
	unsigned short			refb_r_div;
	unsigned short			pll1_feedback_div;
	unsigned short			pll1_charge_pump_current_nA;
	bool				zero_delay_mode_internal_en;
	bool				osc_in_feedback_en;
	enum pll1_rzero_resistor	pll1_loop_filter_rzero;

	
	enum ref_sel_mode		ref_mode;

	
	unsigned int			pll2_charge_pump_current_nA;
	unsigned char			pll2_ndiv_a_cnt;
	unsigned char			pll2_ndiv_b_cnt;
	bool				pll2_freq_doubler_en;
	unsigned char			pll2_r2_div;
	unsigned char			pll2_vco_diff_m1; 
	unsigned char			pll2_vco_diff_m2; 

	
	enum rpole2_resistor		rpole2;
	enum rzero_resistor		rzero;
	enum cpole1_capacitor		cpole1;
	bool				rzero_bypass_en;

	
	int				num_channels;
	struct ad9523_channel_spec	*channels;

	char				name[SPI_NAME_SIZE];
};

#endif 
