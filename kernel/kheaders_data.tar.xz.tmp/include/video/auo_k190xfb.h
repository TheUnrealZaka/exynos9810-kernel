

#ifndef _LINUX_VIDEO_AUO_K190XFB_H_
#define _LINUX_VIDEO_AUO_K190XFB_H_


#define AUOK190X_QUIRK_STANDBYPARAM	(1 << 0)


#define AUOK190X_QUIRK_STANDBYBROKEN	(1 << 1)


#define AUOK190X_RESOLUTION_800_600		0
#define AUOK190X_RESOLUTION_1024_768		1
#define AUOK190X_RESOLUTION_600_800		4
#define AUOK190X_RESOLUTION_768_1024		5


struct auok190xfb_par {
	struct fb_info *info;
	struct auok190x_board *board;

	struct regulator *regulator;

	struct mutex io_lock;
	struct delayed_work work;
	wait_queue_head_t waitq;
	int resolution;
	int rotation;
	int consecutive_threshold;
	int update_cnt;

	
	int epd_type;
	int panel_size_int;
	int panel_size_float;
	int panel_model;
	int tcon_version;
	int lut_version;

	
	void (*update_partial)(struct auok190xfb_par *par, u16 y1, u16 y2);
	void (*update_all)(struct auok190xfb_par *par);
	bool (*need_refresh)(struct auok190xfb_par *par);
	void (*init)(struct auok190xfb_par *par);
	void (*recover)(struct auok190xfb_par *par);

	int update_mode; 
	int last_mode; 
	int flash;

	
	int autosuspend_delay;
	bool standby;
	bool manual_standby;
};


struct auok190x_board {
	int (*init)(struct auok190xfb_par *);
	void (*cleanup)(struct auok190xfb_par *);
	int (*wait_for_rdy)(struct auok190xfb_par *);

	void (*set_ctl)(struct auok190xfb_par *, unsigned char, u8);
	void (*set_hdb)(struct auok190xfb_par *, u16);
	u16 (*get_hdb)(struct auok190xfb_par *);

	int (*setup_irq)(struct fb_info *);

	int gpio_nsleep;
	int gpio_nrst;
	int gpio_nbusy;

	int resolution;
	int quirks;
	int fps;
};

#endif
