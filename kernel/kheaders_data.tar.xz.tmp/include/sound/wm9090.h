

#ifndef __LINUX_SND_WM9090_H
#define __LINUX_SND_WM9090_H

struct wm9090_platform_data {
	
	unsigned int lin1_diff:1;
	unsigned int lin2_diff:1;

	
	unsigned int agc_ena:1;
	u16 agc[3];
};

#endif
