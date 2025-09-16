

#ifndef __LINUX_VIDEOMODE_H
#define __LINUX_VIDEOMODE_H

#include <linux/types.h>
#include <video/display_timing.h>


struct videomode {
	unsigned long pixelclock;	

	u32 hactive;
	u32 hfront_porch;
	u32 hback_porch;
	u32 hsync_len;

	u32 vactive;
	u32 vfront_porch;
	u32 vback_porch;
	u32 vsync_len;

	enum display_flags flags; 
};


void videomode_from_timing(const struct display_timing *dt,
			  struct videomode *vm);


int videomode_from_timings(const struct display_timings *disp,
			  struct videomode *vm, unsigned int index);

#endif
