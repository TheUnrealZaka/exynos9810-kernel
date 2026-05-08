
#ifndef _DM644X_CCDC_H
#define _DM644X_CCDC_H
#include <media/davinci/ccdc_types.h>
#include <media/davinci/vpfe_types.h>


enum ccdc_sample_length {
	CCDC_SAMPLE_1PIXELS,
	CCDC_SAMPLE_2PIXELS,
	CCDC_SAMPLE_4PIXELS,
	CCDC_SAMPLE_8PIXELS,
	CCDC_SAMPLE_16PIXELS
};


enum ccdc_sample_line {
	CCDC_SAMPLE_1LINES,
	CCDC_SAMPLE_2LINES,
	CCDC_SAMPLE_4LINES,
	CCDC_SAMPLE_8LINES,
	CCDC_SAMPLE_16LINES
};


enum ccdc_gamma_width {
	CCDC_GAMMA_BITS_15_6,	
	CCDC_GAMMA_BITS_14_5,
	CCDC_GAMMA_BITS_13_4,
	CCDC_GAMMA_BITS_12_3,
	CCDC_GAMMA_BITS_11_2,
	CCDC_GAMMA_BITS_10_1,
	CCDC_GAMMA_BITS_09_0	
};


static inline u8 ccdc_gamma_width_max_bit(enum ccdc_gamma_width width)
{
	return 15 - width;
}

enum ccdc_data_size {
	CCDC_DATA_16BITS,
	CCDC_DATA_15BITS,
	CCDC_DATA_14BITS,
	CCDC_DATA_13BITS,
	CCDC_DATA_12BITS,
	CCDC_DATA_11BITS,
	CCDC_DATA_10BITS,
	CCDC_DATA_8BITS
};


static inline u8 ccdc_data_size_max_bit(enum ccdc_data_size sz)
{
	return sz == CCDC_DATA_8BITS ? 7 : 15 - sz;
}


struct ccdc_a_law {
	
	unsigned char enable;
	
	enum ccdc_gamma_width gamma_wd;
};


struct ccdc_black_clamp {
	unsigned char enable;
	
	enum ccdc_sample_length sample_pixel;
	
	enum ccdc_sample_line sample_ln;
	
	unsigned short start_pixel;
	
	unsigned short sgain;
	
	unsigned short dc_sub;
};


struct ccdc_black_compensation {
	
	char r;
	
	char gr;
	
	char b;
	
	char gb;
};


struct ccdc_fault_pixel {
	
	unsigned char enable;
	
	unsigned short fp_num;
	
	unsigned long fpc_table_addr;
};


struct ccdc_config_params_raw {
	
	enum ccdc_data_size data_sz;
	
	struct ccdc_a_law alaw;
	
	struct ccdc_black_clamp blk_clamp;
	
	struct ccdc_black_compensation blk_comp;
	
	struct ccdc_fault_pixel fault_pxl;
};


#ifdef __KERNEL__
#include <linux/io.h>

#define FP_NUM_BYTES		4

#define NUM_EXTRAPIXELS		8
#define NUM_EXTRALINES		8


#define CCDC_WIN_PAL     {0, 0, 720, 576}

#define CCDC_WIN_VGA	{0, 0, (640 + NUM_EXTRAPIXELS), (480 + NUM_EXTRALINES)}


struct ccdc_params_raw {
	
	enum ccdc_pixfmt pix_fmt;
	
	enum ccdc_frmfmt frm_fmt;
	
	struct v4l2_rect win;
	
	enum vpfe_pin_pol fid_pol;
	
	enum vpfe_pin_pol vd_pol;
	
	enum vpfe_pin_pol hd_pol;
	
	enum ccdc_buftype buf_type;
	
	unsigned char image_invert_enable;
	
	struct ccdc_config_params_raw config_params;
};

struct ccdc_params_ycbcr {
	
	enum ccdc_pixfmt pix_fmt;
	
	enum ccdc_frmfmt frm_fmt;
	
	struct v4l2_rect win;
	
	enum vpfe_pin_pol fid_pol;
	
	enum vpfe_pin_pol vd_pol;
	
	enum vpfe_pin_pol hd_pol;
	
	int bt656_enable;
	
	enum ccdc_pixorder pix_order;
	
	enum ccdc_buftype buf_type;
};
#endif
#endif				
