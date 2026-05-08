

#ifndef _LINUX_KEYRESET_H
#define _LINUX_KEYRESET_H

#define KEYRESET_NAME "keyreset"

struct keyreset_platform_data {
	int (*reset_fn)(void);
	int key_down_delay;
	int *keys_up;
	int keys_down[]; 
};

#endif 
