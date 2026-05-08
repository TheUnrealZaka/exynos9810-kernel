

#ifndef __LINUX_SSP_H
#define __LINUX_SSP_H

#include <linux/list.h>
#include <linux/io.h>
#include <linux/of.h>




#define SSCR0		(0x00)  
#define SSCR1		(0x04)  
#define SSSR		(0x08)  
#define SSITR		(0x0C)  
#define SSDR		(0x10)  

#define SSTO		(0x28)  
#define DDS_RATE	(0x28)  
#define SSPSP		(0x2C)  
#define SSTSA		(0x30)  
#define SSRSA		(0x34)  
#define SSTSS		(0x38)  
#define SSACD		(0x3C)  
#define SSACDD		(0x40)	


#define SSCR0_DSS	(0x0000000f)	
#define SSCR0_DataSize(x)  ((x) - 1)	
#define SSCR0_FRF	(0x00000030)	
#define SSCR0_Motorola	(0x0 << 4)	
#define SSCR0_TI	(0x1 << 4)	
#define SSCR0_National	(0x2 << 4)	
#define SSCR0_ECS	(1 << 6)	
#define SSCR0_SSE	(1 << 7)	
#define SSCR0_SCR(x)	((x) << 8)	


#define SSCR0_EDSS	(1 << 20)	
#define SSCR0_NCS	(1 << 21)	
#define SSCR0_RIM	(1 << 22)	
#define SSCR0_TUM	(1 << 23)	
#define SSCR0_FRDC	(0x07000000)	
#define SSCR0_SlotsPerFrm(x) (((x) - 1) << 24)	
#define SSCR0_FPCKE	(1 << 29)	
#define SSCR0_ACS	(1 << 30)	
#define SSCR0_MOD	(1 << 31)	


#define SSCR1_RIE	(1 << 0)	
#define SSCR1_TIE	(1 << 1)	
#define SSCR1_LBM	(1 << 2)	
#define SSCR1_SPO	(1 << 3)	
#define SSCR1_SPH	(1 << 4)	
#define SSCR1_MWDS	(1 << 5)	

#define SSSR_ALT_FRM_MASK	3	
#define SSSR_TNF	(1 << 2)	
#define SSSR_RNE	(1 << 3)	
#define SSSR_BSY	(1 << 4)	
#define SSSR_TFS	(1 << 5)	
#define SSSR_RFS	(1 << 6)	
#define SSSR_ROR	(1 << 7)	

#define RX_THRESH_DFLT	8
#define TX_THRESH_DFLT	8

#define SSSR_TFL_MASK	(0xf << 8)	
#define SSSR_RFL_MASK	(0xf << 12)	

#define SSCR1_TFT	(0x000003c0)	
#define SSCR1_TxTresh(x) (((x) - 1) << 6) 
#define SSCR1_RFT	(0x00003c00)	
#define SSCR1_RxTresh(x) (((x) - 1) << 10) 

#define RX_THRESH_CE4100_DFLT	2
#define TX_THRESH_CE4100_DFLT	2

#define CE4100_SSSR_TFL_MASK	(0x3 << 8)	
#define CE4100_SSSR_RFL_MASK	(0x3 << 12)	

#define CE4100_SSCR1_TFT	(0x000000c0)	
#define CE4100_SSCR1_TxTresh(x) (((x) - 1) << 6)	
#define CE4100_SSCR1_RFT	(0x00000c00)	
#define CE4100_SSCR1_RxTresh(x) (((x) - 1) << 10)	


#define QUARK_X1000_SSCR0_DSS	(0x1F)		
#define QUARK_X1000_SSCR0_DataSize(x)  ((x) - 1)	
#define QUARK_X1000_SSCR0_FRF	(0x3 << 5)	
#define QUARK_X1000_SSCR0_Motorola	(0x0 << 5)	

#define RX_THRESH_QUARK_X1000_DFLT	1
#define TX_THRESH_QUARK_X1000_DFLT	16

#define QUARK_X1000_SSSR_TFL_MASK	(0x1F << 8)	
#define QUARK_X1000_SSSR_RFL_MASK	(0x1F << 13)	

#define QUARK_X1000_SSCR1_TFT	(0x1F << 6)	
#define QUARK_X1000_SSCR1_TxTresh(x) (((x) - 1) << 6)	
#define QUARK_X1000_SSCR1_RFT	(0x1F << 11)	
#define QUARK_X1000_SSCR1_RxTresh(x) (((x) - 1) << 11)	
#define QUARK_X1000_SSCR1_STRF       (1 << 17)		
#define QUARK_X1000_SSCR1_EFWR	(1 << 16)		


#define SSCR0_TISSP		(1 << 4)	
#define SSCR0_PSP		(3 << 4)	
#define SSCR1_TTELP		(1 << 31)	
#define SSCR1_TTE		(1 << 30)	
#define SSCR1_EBCEI		(1 << 29)	
#define SSCR1_SCFR		(1 << 28)	
#define SSCR1_ECRA		(1 << 27)	
#define SSCR1_ECRB		(1 << 26)	
#define SSCR1_SCLKDIR		(1 << 25)	
#define SSCR1_SFRMDIR		(1 << 24)	
#define SSCR1_RWOT		(1 << 23)	
#define SSCR1_TRAIL		(1 << 22)	
#define SSCR1_TSRE		(1 << 21)	
#define SSCR1_RSRE		(1 << 20)	
#define SSCR1_TINTE		(1 << 19)	
#define SSCR1_PINTE		(1 << 18)	
#define SSCR1_IFS		(1 << 16)	
#define SSCR1_STRF		(1 << 15)	
#define SSCR1_EFWR		(1 << 14)	

#define SSSR_BCE		(1 << 23)	
#define SSSR_CSS		(1 << 22)	
#define SSSR_TUR		(1 << 21)	
#define SSSR_EOC		(1 << 20)	
#define SSSR_TINT		(1 << 19)	
#define SSSR_PINT		(1 << 18)	


#define SSPSP_SCMODE(x)		((x) << 0)	
#define SSPSP_SFRMP		(1 << 2)	
#define SSPSP_ETDS		(1 << 3)	
#define SSPSP_STRTDLY(x)	((x) << 4)	
#define SSPSP_DMYSTRT(x)	((x) << 7)	
#define SSPSP_SFRMDLY(x)	((x) << 9)	
#define SSPSP_SFRMWDTH(x)	((x) << 16)	
#define SSPSP_DMYSTOP(x)	((x) << 23)	
#define SSPSP_FSRT		(1 << 25)	


#define SSPSP_EDMYSTRT(x)	((x) << 26)     
#define SSPSP_EDMYSTOP(x)	((x) << 28)     
#define SSPSP_TIMING_MASK	(0x7f8001f0)

#define SSACD_SCDB		(1 << 3)	
#define SSACD_ACPS(x)		((x) << 4)	
#define SSACD_ACDS(x)		((x) << 0)	
#define SSACD_SCDX8		(1 << 7)	


#define SSITF			0x44		
#define SSITF_TxLoThresh(x)	(((x) - 1) << 8)
#define SSITF_TxHiThresh(x)	((x) - 1)

#define SSIRF			0x48		
#define SSIRF_RxThresh(x)	((x) - 1)

enum pxa_ssp_type {
	SSP_UNDEFINED = 0,
	PXA25x_SSP,  
	PXA25x_NSSP, 
	PXA27x_SSP,
	PXA3xx_SSP,
	PXA168_SSP,
	PXA910_SSP,
	CE4100_SSP,
	QUARK_X1000_SSP,
	LPSS_LPT_SSP, 
	LPSS_BYT_SSP,
	LPSS_BSW_SSP,
	LPSS_SPT_SSP,
	LPSS_BXT_SSP,
};

struct ssp_device {
	struct platform_device *pdev;
	struct list_head	node;

	struct clk	*clk;
	void __iomem	*mmio_base;
	unsigned long	phys_base;

	const char	*label;
	int		port_id;
	int		type;
	int		use_count;
	int		irq;
	int		drcmr_rx;
	int		drcmr_tx;

	struct device_node	*of_node;
};


static inline void pxa_ssp_write_reg(struct ssp_device *dev, u32 reg, u32 val)
{
	__raw_writel(val, dev->mmio_base + reg);
}


static inline u32 pxa_ssp_read_reg(struct ssp_device *dev, u32 reg)
{
	return __raw_readl(dev->mmio_base + reg);
}

#if IS_ENABLED(CONFIG_PXA_SSP)
struct ssp_device *pxa_ssp_request(int port, const char *label);
void pxa_ssp_free(struct ssp_device *);
struct ssp_device *pxa_ssp_request_of(const struct device_node *of_node,
				      const char *label);
#else
static inline struct ssp_device *pxa_ssp_request(int port, const char *label)
{
	return NULL;
}
static inline struct ssp_device *pxa_ssp_request_of(const struct device_node *n,
						    const char *name)
{
	return NULL;
}
static inline void pxa_ssp_free(struct ssp_device *ssp) {}
#endif

#endif
