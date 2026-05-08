

#ifndef _ADV7842_
#define _ADV7842_


enum adv7842_ain_sel {
	ADV7842_AIN1_2_3_NC_SYNC_1_2 = 0,
	ADV7842_AIN4_5_6_NC_SYNC_2_1 = 1,
	ADV7842_AIN7_8_9_NC_SYNC_3_1 = 2,
	ADV7842_AIN10_11_12_NC_SYNC_4_1 = 3,
	ADV7842_AIN9_4_5_6_SYNC_2_1 = 4,
};


enum adv7842_bus_order {
	ADV7842_BUS_ORDER_RGB,		
	ADV7842_BUS_ORDER_GRB,		
	ADV7842_BUS_ORDER_RBG,		
	ADV7842_BUS_ORDER_BGR,		
	ADV7842_BUS_ORDER_BRG,		
	ADV7842_BUS_ORDER_GBR,		
};


enum adv7842_inp_color_space {
	ADV7842_INP_COLOR_SPACE_LIM_RGB = 0,
	ADV7842_INP_COLOR_SPACE_FULL_RGB = 1,
	ADV7842_INP_COLOR_SPACE_LIM_YCbCr_601 = 2,
	ADV7842_INP_COLOR_SPACE_LIM_YCbCr_709 = 3,
	ADV7842_INP_COLOR_SPACE_XVYCC_601 = 4,
	ADV7842_INP_COLOR_SPACE_XVYCC_709 = 5,
	ADV7842_INP_COLOR_SPACE_FULL_YCbCr_601 = 6,
	ADV7842_INP_COLOR_SPACE_FULL_YCbCr_709 = 7,
	ADV7842_INP_COLOR_SPACE_AUTO = 0xf,
};


enum adv7842_op_format_mode_sel {
	ADV7842_OP_FORMAT_MODE0 = 0x00,
	ADV7842_OP_FORMAT_MODE1 = 0x04,
	ADV7842_OP_FORMAT_MODE2 = 0x08,
};


enum adv7842_mode {
	ADV7842_MODE_SDP,
	ADV7842_MODE_COMP,
	ADV7842_MODE_RGB,
	ADV7842_MODE_HDMI
};


enum adv7842_vid_std_select {
	
	ADV7842_SDP_VID_STD_CVBS_SD_4x1 = 0x01,
	ADV7842_SDP_VID_STD_YC_SD4_x1 = 0x09,
	
	ADV7842_RGB_VID_STD_AUTO_GRAPH_MODE = 0x07,
	
	ADV7842_HDMI_GR_VID_STD_AUTO_GRAPH_MODE = 0x02,
	
	ADV7842_HDMI_COMP_VID_STD_HD_1250P = 0x1e,
};

enum adv7842_select_input {
	ADV7842_SELECT_HDMI_PORT_A,
	ADV7842_SELECT_HDMI_PORT_B,
	ADV7842_SELECT_VGA_RGB,
	ADV7842_SELECT_VGA_COMP,
	ADV7842_SELECT_SDP_CVBS,
	ADV7842_SELECT_SDP_YC,
};

enum adv7842_drive_strength {
	ADV7842_DR_STR_LOW = 0,
	ADV7842_DR_STR_MEDIUM_LOW = 1,
	ADV7842_DR_STR_MEDIUM_HIGH = 2,
	ADV7842_DR_STR_HIGH = 3,
};

struct adv7842_sdp_csc_coeff {
	bool manual;
	u16 scaling;
	u16 A1;
	u16 A2;
	u16 A3;
	u16 A4;
	u16 B1;
	u16 B2;
	u16 B3;
	u16 B4;
	u16 C1;
	u16 C2;
	u16 C3;
	u16 C4;
};

struct adv7842_sdp_io_sync_adjustment {
	bool adjust;
	u16 hs_beg;
	u16 hs_width;
	u16 de_beg;
	u16 de_end;
	u8 vs_beg_o;
	u8 vs_beg_e;
	u8 vs_end_o;
	u8 vs_end_e;
	u8 de_v_beg_o;
	u8 de_v_beg_e;
	u8 de_v_end_o;
	u8 de_v_end_e;
};


struct adv7842_platform_data {
	
	unsigned chip_reset:1;

	
	unsigned disable_pwrdnb:1;

	
	unsigned disable_cable_det_rst:1;

	
	enum adv7842_ain_sel ain_sel;

	
	enum adv7842_bus_order bus_order;

	
	enum adv7842_op_format_mode_sel op_format_mode_sel;

	
	enum adv7842_mode mode;

	
	unsigned input;

	
	enum adv7842_vid_std_select vid_std_select;

	
	unsigned alt_gamma:1;

	
	unsigned blank_data:1;
	unsigned insert_av_codes:1;
	unsigned replicate_av_codes:1;

	
	unsigned output_bus_lsb_to_msb:1;

	
	enum adv7842_drive_strength dr_str_data;
	enum adv7842_drive_strength dr_str_clk;
	enum adv7842_drive_strength dr_str_sync;

	
	unsigned llc_dll_phase:5;

	
	unsigned sd_ram_size; 
	unsigned sd_ram_ddr:1; 

	
	unsigned hdmi_free_run_enable:1;
	
	unsigned hdmi_free_run_mode:1;

	
	unsigned sdp_free_run_auto:1;
	unsigned sdp_free_run_man_col_en:1;
	unsigned sdp_free_run_cbar_en:1;
	unsigned sdp_free_run_force:1;

	
	unsigned hpa_auto:1;

	struct adv7842_sdp_csc_coeff sdp_csc_coeff;

	struct adv7842_sdp_io_sync_adjustment sdp_io_sync_625;
	struct adv7842_sdp_io_sync_adjustment sdp_io_sync_525;

	
	u8 i2c_sdp_io;
	u8 i2c_sdp;
	u8 i2c_cp;
	u8 i2c_vdp;
	u8 i2c_afe;
	u8 i2c_hdmi;
	u8 i2c_repeater;
	u8 i2c_edid;
	u8 i2c_infoframe;
	u8 i2c_cec;
	u8 i2c_avlink;
};

#define V4L2_CID_ADV_RX_ANALOG_SAMPLING_PHASE	(V4L2_CID_DV_CLASS_BASE + 0x1000)
#define V4L2_CID_ADV_RX_FREE_RUN_COLOR_MANUAL	(V4L2_CID_DV_CLASS_BASE + 0x1001)
#define V4L2_CID_ADV_RX_FREE_RUN_COLOR		(V4L2_CID_DV_CLASS_BASE + 0x1002)


#define ADV7842_CMD_RAM_TEST _IO('V', BASE_VIDIOC_PRIVATE)

#define ADV7842_EDID_PORT_A   0
#define ADV7842_EDID_PORT_B   1
#define ADV7842_EDID_PORT_VGA 2
#define ADV7842_PAD_SOURCE    3

#endif
