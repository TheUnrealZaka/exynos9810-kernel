#ifndef __PLATFORM_DATA_VIDEO_S3C
#define __PLATFORM_DATA_VIDEO_S3C


#define S3C_FB_MAX_WIN	(5)


struct s3c_fb_pd_win {
	unsigned short		default_bpp;
	unsigned short		max_bpp;
	unsigned short		xres;
	unsigned short		yres;
	unsigned short		virtual_x;
	unsigned short		virtual_y;
};


struct s3c_fb_platdata {
	void	(*setup_gpio)(void);

	struct s3c_fb_pd_win	*win[S3C_FB_MAX_WIN];
	struct fb_videomode     *vtiming;

	u32			 vidcon0;
	u32			 vidcon1;
};

#endif
