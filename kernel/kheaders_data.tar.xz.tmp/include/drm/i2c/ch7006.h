

#ifndef __DRM_I2C_CH7006_H__
#define __DRM_I2C_CH7006_H__


struct ch7006_encoder_params {
	enum {
		CH7006_FORMAT_RGB16 = 0,
		CH7006_FORMAT_YCrCb24m16,
		CH7006_FORMAT_RGB24m16,
		CH7006_FORMAT_RGB15,
		CH7006_FORMAT_RGB24m12C,
		CH7006_FORMAT_RGB24m12I,
		CH7006_FORMAT_RGB24m8,
		CH7006_FORMAT_RGB16m8,
		CH7006_FORMAT_RGB15m8,
		CH7006_FORMAT_YCrCb24m8,
	} input_format;

	enum {
		CH7006_CLOCK_SLAVE = 0,
		CH7006_CLOCK_MASTER,
	} clock_mode;

	enum {
		CH7006_CLOCK_EDGE_NEG = 0,
		CH7006_CLOCK_EDGE_POS,
	} clock_edge;

	int xcm, pcm;

	enum {
		CH7006_SYNC_SLAVE = 0,
		CH7006_SYNC_MASTER,
	} sync_direction;

	enum {
		CH7006_SYNC_SEPARATED = 0,
		CH7006_SYNC_EMBEDDED,
	} sync_encoding;

	enum {
		CH7006_POUT_1_8V = 0,
		CH7006_POUT_3_3V,
	} pout_level;

	enum {
		CH7006_ACTIVE_HSYNC = 0,
		CH7006_ACTIVE_DSTART,
	} active_detect;
};

#endif
