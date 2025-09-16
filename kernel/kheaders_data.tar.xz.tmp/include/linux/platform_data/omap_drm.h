

#ifndef __PLATFORM_DATA_OMAP_DRM_H__
#define __PLATFORM_DATA_OMAP_DRM_H__


struct omap_kms_platform_data {
	
	int ovl_cnt;
	const int *ovl_ids;

	
	int pln_cnt;
	const int *pln_ids;

	int mgr_cnt;
	const int *mgr_ids;

	int dev_cnt;
	const char **dev_names;
};

struct omap_drm_platform_data {
	uint32_t omaprev;
	struct omap_kms_platform_data *kms_pdata;
};

#endif 
