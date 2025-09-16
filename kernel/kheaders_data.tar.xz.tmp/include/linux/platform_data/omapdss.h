

#ifndef __OMAPDSS_PDATA_H
#define __OMAPDSS_PDATA_H

enum omapdss_version {
	OMAPDSS_VER_UNKNOWN = 0,
	OMAPDSS_VER_OMAP24xx,
	OMAPDSS_VER_OMAP34xx_ES1,	
	OMAPDSS_VER_OMAP34xx_ES3,	
	OMAPDSS_VER_OMAP3630,
	OMAPDSS_VER_AM35xx,
	OMAPDSS_VER_OMAP4430_ES1,	
	OMAPDSS_VER_OMAP4430_ES2,	
	OMAPDSS_VER_OMAP4,		
	OMAPDSS_VER_OMAP5,
	OMAPDSS_VER_AM43xx,
	OMAPDSS_VER_DRA7xx,
};


struct omap_dss_board_info {
	const char *default_display_name;
	int (*dsi_enable_pads)(int dsi_id, unsigned int lane_mask);
	void (*dsi_disable_pads)(int dsi_id, unsigned int lane_mask);
	int (*set_min_bus_tput)(struct device *dev, unsigned long r);
	enum omapdss_version version;
};

#endif 
