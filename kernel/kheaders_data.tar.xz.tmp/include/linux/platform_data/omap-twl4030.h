

#ifndef _OMAP_TWL4030_H_
#define _OMAP_TWL4030_H_


#define OMAP_TWL4030_LEFT	(1 << 0)
#define OMAP_TWL4030_RIGHT	(1 << 1)

struct omap_tw4030_pdata {
	const char *card_name;
	
	bool voice_connected;

	
	bool	custom_routing;
	
	u8	has_hs;
	u8	has_hf;
	u8	has_predriv;
	u8	has_carkit;
	bool	has_ear;

	bool	has_mainmic;
	bool	has_submic;
	bool	has_hsmic;
	bool	has_carkitmic;
	bool	has_digimic0;
	bool	has_digimic1;
	u8	has_linein;

	
	int jack_detect;
};

#endif 
