

#ifndef __DRM_I2C_SIL164_H__
#define __DRM_I2C_SIL164_H__


struct sil164_encoder_params {
	enum {
		SIL164_INPUT_EDGE_FALLING = 0,
		SIL164_INPUT_EDGE_RISING
	} input_edge;

	enum {
		SIL164_INPUT_WIDTH_12BIT = 0,
		SIL164_INPUT_WIDTH_24BIT
	} input_width;

	enum {
		SIL164_INPUT_SINGLE_EDGE = 0,
		SIL164_INPUT_DUAL_EDGE
	} input_dual;

	enum {
		SIL164_PLL_FILTER_ON = 0,
		SIL164_PLL_FILTER_OFF,
	} pll_filter;

	int input_skew; 
	int duallink_skew; 
};

#endif
