

#ifndef S5P_HDMI_H
#define S5P_HDMI_H

struct i2c_board_info;


struct s5p_hdmi_platform_data {
	int hdmiphy_bus;
	struct i2c_board_info *hdmiphy_info;
	int mhl_bus;
	struct i2c_board_info *mhl_info;
	int hpd_gpio;
};

#endif 
