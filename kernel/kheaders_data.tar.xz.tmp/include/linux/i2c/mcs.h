

#ifndef __LINUX_MCS_H
#define __LINUX_MCS_H

#define MCS_KEY_MAP(v, c)	((((v) & 0xff) << 16) | ((c) & 0xffff))
#define MCS_KEY_VAL(v)		(((v) >> 16) & 0xff)
#define MCS_KEY_CODE(v)		((v) & 0xffff)

struct mcs_platform_data {
	void (*poweron)(bool);
	void (*cfg_pin)(void);

	
	unsigned int x_size;
	unsigned int y_size;

	
	const u32 *keymap;
	unsigned int keymap_size;
	unsigned int key_maxval;
	bool no_autorepeat;
};

#endif	
