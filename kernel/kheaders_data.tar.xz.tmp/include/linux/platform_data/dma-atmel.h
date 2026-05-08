
#ifndef AT_HDMAC_H
#define AT_HDMAC_H

#include <linux/dmaengine.h>


struct at_dma_platform_data {
	unsigned int	nr_channels;
	dma_cap_mask_t  cap_mask;
};


struct at_dma_slave {
	struct device		*dma_dev;
	u32			cfg;
};



#define ATC_PER_MSB(h)	((0x30U & (h)) >> 4)	

#define	ATC_SRC_PER(h)		(0xFU & (h))	
#define	ATC_DST_PER(h)		((0xFU & (h)) <<  4)	
#define	ATC_SRC_REP		(0x1 <<  8)	
#define	ATC_SRC_H2SEL		(0x1 <<  9)	
#define		ATC_SRC_H2SEL_SW	(0x0 <<  9)
#define		ATC_SRC_H2SEL_HW	(0x1 <<  9)
#define	ATC_SRC_PER_MSB(h)	(ATC_PER_MSB(h) << 10)	
#define	ATC_DST_REP		(0x1 << 12)	
#define	ATC_DST_H2SEL		(0x1 << 13)	
#define		ATC_DST_H2SEL_SW	(0x0 << 13)
#define		ATC_DST_H2SEL_HW	(0x1 << 13)
#define	ATC_DST_PER_MSB(h)	(ATC_PER_MSB(h) << 14)	
#define	ATC_SOD			(0x1 << 16)	
#define	ATC_LOCK_IF		(0x1 << 20)	
#define	ATC_LOCK_B		(0x1 << 21)	
#define	ATC_LOCK_IF_L		(0x1 << 22)	
#define		ATC_LOCK_IF_L_CHUNK	(0x0 << 22)
#define		ATC_LOCK_IF_L_BUFFER	(0x1 << 22)
#define	ATC_AHB_PROT_MASK	(0x7 << 24)	
#define	ATC_FIFOCFG_MASK	(0x3 << 28)	
#define		ATC_FIFOCFG_LARGESTBURST	(0x0 << 28)
#define		ATC_FIFOCFG_HALFFIFO		(0x1 << 28)
#define		ATC_FIFOCFG_ENOUGHSPACE		(0x2 << 28)


#endif 
