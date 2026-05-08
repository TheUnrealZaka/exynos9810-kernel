

#ifndef MADERA_CODEC_PDATA_H
#define MADERA_CODEC_PDATA_H

#include <linux/kernel.h>

#define MADERA_MAX_INPUT		6
#define MADERA_MAX_MUXED_CHANNELS	4
#define MADERA_MAX_OUTPUT		6
#define MADERA_MAX_AIF			4
#define MADERA_MAX_PDM_SPK		2
#define MADERA_MAX_DSP			7


struct madera_codec_pdata {
	u32 max_channels_clocked[MADERA_MAX_AIF];

	u32 dmic_ref[MADERA_MAX_INPUT];

	u32 inmode[MADERA_MAX_INPUT][MADERA_MAX_MUXED_CHANNELS];

	bool out_mono[MADERA_MAX_OUTPUT];

	u32 pdm_fmt[MADERA_MAX_PDM_SPK];

	u32 pdm_mute[MADERA_MAX_PDM_SPK];

	bool auxpdm_slave_mode;

	bool auxpdm_falling_edge;
};

#endif
