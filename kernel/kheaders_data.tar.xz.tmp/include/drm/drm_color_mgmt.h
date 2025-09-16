

#ifndef __DRM_COLOR_MGMT_H__
#define __DRM_COLOR_MGMT_H__

#include <linux/ctype.h>

void drm_crtc_enable_color_mgmt(struct drm_crtc *crtc,
				uint degamma_lut_size,
				bool has_ctm,
				uint gamma_lut_size);

int drm_mode_crtc_set_gamma_size(struct drm_crtc *crtc,
				 int gamma_size);


static inline uint32_t drm_color_lut_extract(uint32_t user_input,
					     uint32_t bit_precision)
{
	uint32_t val = user_input;
	uint32_t max = 0xffff >> (16 - bit_precision);

	
	if (bit_precision < 16) {
		val += 1UL << (16 - bit_precision - 1);
		val >>= 16 - bit_precision;
	}

	return clamp_val(val, 0, max);
}


#endif
