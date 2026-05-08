

#ifndef _PN544_H_
#define _PN544_H_

#include <linux/i2c.h>

enum {
	NFC_GPIO_ENABLE,
	NFC_GPIO_FW_RESET,
	NFC_GPIO_IRQ
};


struct pn544_nfc_platform_data {
	int (*request_resources) (struct i2c_client *client);
	void (*free_resources) (void);
	void (*enable) (int fw);
	int (*test) (void);
	void (*disable) (void);
	int (*get_gpio)(int type);
};

#endif 
