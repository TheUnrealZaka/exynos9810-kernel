
#ifndef _VPFE_TYPES_H
#define _VPFE_TYPES_H

#ifdef __KERNEL__

enum vpfe_pin_pol {
	VPFE_PINPOL_POSITIVE,
	VPFE_PINPOL_NEGATIVE
};

enum vpfe_hw_if_type {
	
	VPFE_BT656,
	
	VPFE_BT1120,
	
	VPFE_RAW_BAYER,
	
	VPFE_YCBCR_SYNC_8,
	
	VPFE_YCBCR_SYNC_16,
	
	VPFE_BT656_10BIT
};


struct vpfe_hw_if_param {
	enum vpfe_hw_if_type if_type;
	enum vpfe_pin_pol hdpol;
	enum vpfe_pin_pol vdpol;
};

#endif
#endif
