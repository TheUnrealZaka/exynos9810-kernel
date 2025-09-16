

#ifndef _ST21NFCA_HCI_H_
#define _ST21NFCA_HCI_H_

#include <linux/i2c.h>

#define ST21NFCA_HCI_DRIVER_NAME "st21nfca_hci"

struct st21nfca_nfc_platform_data {
	unsigned int gpio_ena;
	unsigned int irq_polarity;
	bool is_ese_present;
	bool is_uicc_present;
};

#endif 
