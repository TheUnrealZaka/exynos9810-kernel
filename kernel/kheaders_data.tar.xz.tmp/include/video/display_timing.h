

#ifndef __LINUX_DISPLAY_TIMING_H
#define __LINUX_DISPLAY_TIMING_H

#include <linux/bitops.h>
#include <linux/types.h>

enum display_flags {
	DISPLAY_FLAGS_HSYNC_LOW		= BIT(0),
	DISPLAY_FLAGS_HSYNC_HIGH	= BIT(1),
	DISPLAY_FLAGS_VSYNC_LOW		= BIT(2),
	DISPLAY_FLAGS_VSYNC_HIGH	= BIT(3),

	
	DISPLAY_FLAGS_DE_LOW		= BIT(4),
	DISPLAY_FLAGS_DE_HIGH		= BIT(5),
	
	DISPLAY_FLAGS_PIXDATA_POSEDGE	= BIT(6),
	
	DISPLAY_FLAGS_PIXDATA_NEGEDGE	= BIT(7),
	DISPLAY_FLAGS_INTERLACED	= BIT(8),
	DISPLAY_FLAGS_DOUBLESCAN	= BIT(9),
	DISPLAY_FLAGS_DOUBLECLK		= BIT(10),
};


struct timing_entry {
	u32 min;
	u32 typ;
	u32 max;
};


struct display_timing {
	struct timing_entry pixelclock;

	struct timing_entry hactive;		
	struct timing_entry hfront_porch;	
	struct timing_entry hback_porch;	
	struct timing_entry hsync_len;		

	struct timing_entry vactive;		
	struct timing_entry vfront_porch;	
	struct timing_entry vback_porch;	
	struct timing_entry vsync_len;		

	enum display_flags flags;		
};


struct display_timings {
	unsigned int num_timings;
	unsigned int native_mode;

	struct display_timing **timings;
};


static inline struct display_timing *display_timings_get(const struct
							 display_timings *disp,
							 unsigned int index)
{
	if (disp->num_timings > index)
		return disp->timings[index];
	else
		return NULL;
}

void display_timings_release(struct display_timings *disp);

#endif
