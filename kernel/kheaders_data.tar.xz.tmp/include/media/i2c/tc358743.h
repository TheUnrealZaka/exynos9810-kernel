



#ifndef _TC358743_
#define _TC358743_

enum tc358743_ddc5v_delays {
	DDC5V_DELAY_0_MS,
	DDC5V_DELAY_50_MS,
	DDC5V_DELAY_100_MS,
	DDC5V_DELAY_200_MS,
};

enum tc358743_hdmi_detection_delay {
	HDMI_MODE_DELAY_0_MS,
	HDMI_MODE_DELAY_25_MS,
	HDMI_MODE_DELAY_50_MS,
	HDMI_MODE_DELAY_100_MS,
};

struct tc358743_platform_data {
	
	u32 refclk_hz; 

	
	enum tc358743_ddc5v_delays ddc5v_delay;

	bool enable_hdcp;

	
	u16 fifo_level;

	
	u16 pll_prd;
	u16 pll_fbd;

	
	u32 lineinitcnt;	
	u32 lptxtimecnt;	
	u32 tclk_headercnt;	
	u32 tclk_trailcnt;	
	u32 ths_headercnt;	
	u32 twakeup;		
	u32 tclk_postcnt;	
	u32 ths_trailcnt;	
	u32 hstxvregcnt;	

	
	enum tc358743_hdmi_detection_delay hdmi_detection_delay;

	
	bool hdmi_phy_auto_reset_tmds_detected;

	
	bool hdmi_phy_auto_reset_tmds_in_range;

	
	bool hdmi_phy_auto_reset_tmds_valid;

	
	bool hdmi_phy_auto_reset_hsync_out_of_range;

	
	bool hdmi_phy_auto_reset_vsync_out_of_range;
};



#define TC358743_CID_AUDIO_SAMPLING_RATE (V4L2_CID_USER_TC358743_BASE + 0)

#define TC358743_CID_AUDIO_PRESENT       (V4L2_CID_USER_TC358743_BASE + 1)

#endif
