

#ifndef _ST_NCI_H_
#define _ST_NCI_H_

#define ST_NCI_DRIVER_NAME "st_nci"

struct st_nci_nfc_platform_data {
	unsigned int gpio_reset;
	unsigned int irq_polarity;
	bool is_ese_present;
	bool is_uicc_present;
};

#endif 
