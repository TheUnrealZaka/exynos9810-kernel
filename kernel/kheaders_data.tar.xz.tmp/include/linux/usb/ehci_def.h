

#ifndef __LINUX_USB_EHCI_DEF_H
#define __LINUX_USB_EHCI_DEF_H

#include <linux/usb/ehci-dbgp.h>




struct ehci_caps {
	
	u32		hc_capbase;
#define HC_LENGTH(ehci, p)	(0x00ff&((p) >>  \
				(ehci_big_endian_capbase(ehci) ? 24 : 0)))
#define HC_VERSION(ehci, p)	(0xffff&((p) >>  \
				(ehci_big_endian_capbase(ehci) ? 0 : 16)))
	u32		hcs_params;     
#define HCS_DEBUG_PORT(p)	(((p)>>20)&0xf)	
#define HCS_INDICATOR(p)	((p)&(1 << 16))	
#define HCS_N_CC(p)		(((p)>>12)&0xf)	
#define HCS_N_PCC(p)		(((p)>>8)&0xf)	
#define HCS_PORTROUTED(p)	((p)&(1 << 7))	
#define HCS_PPC(p)		((p)&(1 << 4))	
#define HCS_N_PORTS(p)		(((p)>>0)&0xf)	

	u32		hcc_params;      

#define HCC_32FRAME_PERIODIC_LIST(p)	((p)&(1 << 19))
#define HCC_PER_PORT_CHANGE_EVENT(p)	((p)&(1 << 18))
#define HCC_LPM(p)			((p)&(1 << 17))
#define HCC_HW_PREFETCH(p)		((p)&(1 << 16))

#define HCC_EXT_CAPS(p)		(((p)>>8)&0xff)	
#define HCC_ISOC_CACHE(p)       ((p)&(1 << 7))  
#define HCC_ISOC_THRES(p)       (((p)>>4)&0x7)  
#define HCC_CANPARK(p)		((p)&(1 << 2))  
#define HCC_PGM_FRAMELISTLEN(p) ((p)&(1 << 1))  
#define HCC_64BIT_ADDR(p)       ((p)&(1))       
	u8		portroute[8];	 
};



struct ehci_regs {

	
	u32		command;


#define CMD_HIRD	(0xf<<24)	
#define CMD_PPCEE	(1<<15)		
#define CMD_FSP		(1<<14)		
#define CMD_ASPE	(1<<13)		
#define CMD_PSPE	(1<<12)		

#define CMD_PARK	(1<<11)		
#define CMD_PARK_CNT(c)	(((c)>>8)&3)	
#define CMD_LRESET	(1<<7)		
#define CMD_IAAD	(1<<6)		
#define CMD_ASE		(1<<5)		
#define CMD_PSE		(1<<4)		

#define CMD_RESET	(1<<1)		
#define CMD_RUN		(1<<0)		

	
	u32		status;
#define STS_PPCE_MASK	(0xff<<16)	
#define STS_ASS		(1<<15)		
#define STS_PSS		(1<<14)		
#define STS_RECL	(1<<13)		
#define STS_HALT	(1<<12)		

	
#define STS_IAA		(1<<5)		
#define STS_FATAL	(1<<4)		
#define STS_FLR		(1<<3)		
#define STS_PCD		(1<<2)		
#define STS_ERR		(1<<1)		
#define STS_INT		(1<<0)		

	
	u32		intr_enable;

	
	u32		frame_index;	
	
	u32		segment;	
	
	u32		frame_list;	
	
	u32		async_next;	

	u32		reserved1[2];

	
	u32		txfill_tuning;	
#define TXFIFO_DEFAULT	(8<<16)		

	u32		reserved2[6];

	
	u32		configured_flag;
#define FLAG_CF		(1<<0)		

	
	u32		port_status[0];	

#define PORTSC_SUSPEND_STS_ACK 0
#define PORTSC_SUSPEND_STS_NYET 1
#define PORTSC_SUSPEND_STS_STALL 2
#define PORTSC_SUSPEND_STS_ERR 3

#define PORT_DEV_ADDR	(0x7f<<25)		
#define PORT_SSTS	(0x3<<23)		

#define PORT_WKOC_E	(1<<22)		
#define PORT_WKDISC_E	(1<<21)		
#define PORT_WKCONN_E	(1<<20)		

#define PORT_TEST(x)	(((x)&0xf)<<16)	
#define PORT_TEST_PKT	PORT_TEST(0x4)	
#define PORT_TEST_FORCE	PORT_TEST(0x5)	
#define PORT_LED_OFF	(0<<14)
#define PORT_LED_AMBER	(1<<14)
#define PORT_LED_GREEN	(2<<14)
#define PORT_LED_MASK	(3<<14)
#define PORT_OWNER	(1<<13)		
#define PORT_POWER	(1<<12)		
#define PORT_USB11(x) (((x)&(3<<10)) == (1<<10))	


#define PORT_LPM	(1<<9)		
#define PORT_RESET	(1<<8)		
#define PORT_SUSPEND	(1<<7)		
#define PORT_RESUME	(1<<6)		
#define PORT_OCC	(1<<5)		
#define PORT_OC		(1<<4)		
#define PORT_PEC	(1<<3)		
#define PORT_PE		(1<<2)		
#define PORT_CSC	(1<<1)		
#define PORT_CONNECT	(1<<0)		
#define PORT_RWC_BITS   (PORT_CSC | PORT_PEC | PORT_OCC)

	u32		reserved3[9];

	
	u32		usbmode;	
#define USBMODE_SDIS	(1<<3)		
#define USBMODE_BE	(1<<2)		
#define USBMODE_CM_HC	(3<<0)		
#define USBMODE_CM_IDLE	(0<<0)		

	u32		reserved4[6];


	
	u32		hostpc[0];	
#define HOSTPC_PHCD	(1<<22)		
#define HOSTPC_PSPD	(3<<25)		

	u32		reserved5[17];

	
	u32		usbmode_ex;	
#define USBMODE_EX_VBPS	(1<<5)		
#define USBMODE_EX_HC	(3<<0)		
};

#endif 
