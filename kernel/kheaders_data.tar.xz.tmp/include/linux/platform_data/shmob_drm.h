

#ifndef __SHMOB_DRM_H__
#define __SHMOB_DRM_H__

#include <linux/kernel.h>

#include <drm/drm_mode.h>

struct sh_mobile_meram_cfg;
struct sh_mobile_meram_info;

enum shmob_drm_clk_source {
	SHMOB_DRM_CLK_BUS,
	SHMOB_DRM_CLK_PERIPHERAL,
	SHMOB_DRM_CLK_EXTERNAL,
};

enum shmob_drm_interface {
	SHMOB_DRM_IFACE_RGB8,		
	SHMOB_DRM_IFACE_RGB9,		
	SHMOB_DRM_IFACE_RGB12A,		
	SHMOB_DRM_IFACE_RGB12B,		
	SHMOB_DRM_IFACE_RGB16,		
	SHMOB_DRM_IFACE_RGB18,		
	SHMOB_DRM_IFACE_RGB24,		
	SHMOB_DRM_IFACE_YUV422,		
	SHMOB_DRM_IFACE_SYS8A,		
	SHMOB_DRM_IFACE_SYS8B,		
	SHMOB_DRM_IFACE_SYS8C,		
	SHMOB_DRM_IFACE_SYS8D,		
	SHMOB_DRM_IFACE_SYS9,		
	SHMOB_DRM_IFACE_SYS12,		
	SHMOB_DRM_IFACE_SYS16A,		
	SHMOB_DRM_IFACE_SYS16B,		
	SHMOB_DRM_IFACE_SYS16C,		
	SHMOB_DRM_IFACE_SYS18,		
	SHMOB_DRM_IFACE_SYS24,		
};

struct shmob_drm_backlight_data {
	const char *name;
	int max_brightness;
	int (*get_brightness)(void);
	int (*set_brightness)(int brightness);
};

struct shmob_drm_panel_data {
	unsigned int width_mm;		
	unsigned int height_mm;		
	struct drm_mode_modeinfo mode;
};

struct shmob_drm_sys_interface_data {
	unsigned int read_latch:6;
	unsigned int read_setup:8;
	unsigned int read_cycle:8;
	unsigned int read_strobe:8;
	unsigned int write_setup:8;
	unsigned int write_cycle:8;
	unsigned int write_strobe:8;
	unsigned int cs_setup:3;
	unsigned int vsync_active_high:1;
	unsigned int vsync_dir_input:1;
};

#define SHMOB_DRM_IFACE_FL_DWPOL (1 << 0) 
#define SHMOB_DRM_IFACE_FL_DIPOL (1 << 1) 
#define SHMOB_DRM_IFACE_FL_DAPOL (1 << 2) 
#define SHMOB_DRM_IFACE_FL_HSCNT (1 << 3) 
#define SHMOB_DRM_IFACE_FL_DWCNT (1 << 4) 

struct shmob_drm_interface_data {
	enum shmob_drm_interface interface;
	struct shmob_drm_sys_interface_data sys;
	unsigned int clk_div;
	unsigned int flags;
};

struct shmob_drm_platform_data {
	enum shmob_drm_clk_source clk_source;
	struct shmob_drm_interface_data iface;
	struct shmob_drm_panel_data panel;
	struct shmob_drm_backlight_data backlight;
	const struct sh_mobile_meram_cfg *meram;
};

#endif 
