

#ifndef __ASM_ARCH_MX21_USBH
#define __ASM_ARCH_MX21_USBH

enum mx21_usbh_xcvr {
	
	MX21_USBXCVR_TXDIF_RXDIF = 0,
	MX21_USBXCVR_TXDIF_RXSE = 1,
	MX21_USBXCVR_TXSE_RXDIF = 2,
	MX21_USBXCVR_TXSE_RXSE = 3,
};

struct mx21_usbh_platform_data {
	enum mx21_usbh_xcvr host_xcvr; 
	enum mx21_usbh_xcvr otg_xcvr; 
	u16 	enable_host1:1,
		enable_host2:1,
		enable_otg_host:1, 
		host1_xcverless:1, 
		host1_txenoe:1, 
		otg_ext_xcvr:1, 
		unused:10;
};

#endif 
