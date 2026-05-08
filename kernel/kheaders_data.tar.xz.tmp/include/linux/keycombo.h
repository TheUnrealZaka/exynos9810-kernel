

#ifndef _LINUX_KEYCOMBO_H
#define _LINUX_KEYCOMBO_H

#define KEYCOMBO_NAME "keycombo"


struct keycombo_platform_data {
	void (*key_down_fn)(void *);
	void (*key_up_fn)(void *);
	void *priv;
	int key_down_delay; 
	int *keys_up;
	int keys_down[]; 
};

#endif 
