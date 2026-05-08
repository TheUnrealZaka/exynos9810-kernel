

#ifndef __FSL_GUTS_H__
#define __FSL_GUTS_H__

#include <linux/types.h>
#include <linux/io.h>


struct ccsr_guts {
	__be32	porpllsr;	
	__be32	porbmsr;	
	__be32	porimpscr;	
	__be32	pordevsr;	
	__be32	pordbgmsr;	
	__be32	pordevsr2;	
	u8	res018[0x20 - 0x18];
	__be32	porcir;		
	u8	res024[0x30 - 0x24];
	__be32	gpiocr;		
	u8	res034[0x40 - 0x34];
	__be32	gpoutdr;	
	u8	res044[0x50 - 0x44];
	__be32	gpindr;		
	u8	res054[0x60 - 0x54];
	__be32	pmuxcr;		
        __be32  pmuxcr2;	
        __be32  dmuxcr;		
        u8	res06c[0x70 - 0x6c];
	__be32	devdisr;	
#define CCSR_GUTS_DEVDISR_TB1	0x00001000
#define CCSR_GUTS_DEVDISR_TB0	0x00004000
	__be32	devdisr2;	
	u8	res078[0x7c - 0x78];
	__be32  pmjcr;		
	__be32	powmgtcsr;	
	__be32  pmrccr;		
	__be32  pmpdccr;	
	__be32  pmcdr;		
	__be32	mcpsumr;	
	__be32	rstrscr;	
	__be32  ectrstcr;	
	__be32  autorstsr;	
	__be32	pvr;		
	__be32	svr;		
	u8	res0a8[0xb0 - 0xa8];
	__be32	rstcr;		
	u8	res0b4[0xc0 - 0xb4];
	__be32  iovselsr;	
	u8	res0c4[0x100 - 0xc4];
	__be32	rcwsr[16];	
	u8	res140[0x224 - 0x140];
	__be32  iodelay1;	
	__be32  iodelay2;	
	u8	res22c[0x604 - 0x22c];
	__be32	pamubypenr; 	
	u8	res608[0x800 - 0x608];
	__be32	clkdvdr;	
	u8	res804[0x900 - 0x804];
	__be32	ircr;		
	u8	res904[0x908 - 0x904];
	__be32	dmacr;		
	u8	res90c[0x914 - 0x90c];
	__be32	elbccr;		
	u8	res918[0xb20 - 0x918];
	__be32	ddr1clkdr;	
	__be32	ddr2clkdr;	
	__be32	ddrclkdr;	
	u8	resb2c[0xe00 - 0xb2c];
	__be32	clkocr;		
	u8	rese04[0xe10 - 0xe04];
	__be32	ddrdllcr;	
	u8	rese14[0xe20 - 0xe14];
	__be32	lbcdllcr;	
	__be32  cpfor;		
	u8	rese28[0xf04 - 0xe28];
	__be32	srds1cr0;	
	__be32	srds1cr1;	
	u8	resf0c[0xf2c - 0xf0c];
	__be32  itcr;		
	u8	resf30[0xf40 - 0xf30];
	__be32	srds2cr0;	
	__be32	srds2cr1;	
} __attribute__ ((packed));



#define MPC85xx_PMUXCR_QE(x) (0x8000 >> (x))

#ifdef CONFIG_PPC_86xx

#define CCSR_GUTS_DMACR_DEV_SSI	0	
#define CCSR_GUTS_DMACR_DEV_IR	1	


static inline void guts_set_dmacr(struct ccsr_guts __iomem *guts,
	unsigned int co, unsigned int ch, unsigned int device)
{
	unsigned int shift = 16 + (8 * (1 - co) + 2 * (3 - ch));

	clrsetbits_be32(&guts->dmacr, 3 << shift, device << shift);
}

#define CCSR_GUTS_PMUXCR_LDPSEL		0x00010000
#define CCSR_GUTS_PMUXCR_SSI1_MASK	0x0000C000	
#define CCSR_GUTS_PMUXCR_SSI1_LA	0x00000000	
#define CCSR_GUTS_PMUXCR_SSI1_HI	0x00004000	
#define CCSR_GUTS_PMUXCR_SSI1_SSI	0x00008000	
#define CCSR_GUTS_PMUXCR_SSI2_MASK	0x00003000	
#define CCSR_GUTS_PMUXCR_SSI2_LA	0x00000000	
#define CCSR_GUTS_PMUXCR_SSI2_HI	0x00001000	
#define CCSR_GUTS_PMUXCR_SSI2_SSI	0x00002000	
#define CCSR_GUTS_PMUXCR_LA_22_25_LA	0x00000000	
#define CCSR_GUTS_PMUXCR_LA_22_25_HI	0x00000400	
#define CCSR_GUTS_PMUXCR_DBGDRV		0x00000200	
#define CCSR_GUTS_PMUXCR_DMA2_0		0x00000008
#define CCSR_GUTS_PMUXCR_DMA2_3		0x00000004
#define CCSR_GUTS_PMUXCR_DMA1_0		0x00000002
#define CCSR_GUTS_PMUXCR_DMA1_3		0x00000001


static inline void guts_set_pmuxcr_dma(struct ccsr_guts __iomem *guts,
	unsigned int co, unsigned int ch, unsigned int value)
{
	if ((ch == 0) || (ch == 3)) {
		unsigned int shift = 2 * (co + 1) - (ch & 1) - 1;

		clrsetbits_be32(&guts->pmuxcr, 1 << shift, value << shift);
	}
}

#define CCSR_GUTS_CLKDVDR_PXCKEN	0x80000000
#define CCSR_GUTS_CLKDVDR_SSICKEN	0x20000000
#define CCSR_GUTS_CLKDVDR_PXCKINV	0x10000000
#define CCSR_GUTS_CLKDVDR_PXCKDLY_SHIFT 25
#define CCSR_GUTS_CLKDVDR_PXCKDLY_MASK	0x06000000
#define CCSR_GUTS_CLKDVDR_PXCKDLY(x) \
	(((x) & 3) << CCSR_GUTS_CLKDVDR_PXCKDLY_SHIFT)
#define CCSR_GUTS_CLKDVDR_PXCLK_SHIFT	16
#define CCSR_GUTS_CLKDVDR_PXCLK_MASK	0x001F0000
#define CCSR_GUTS_CLKDVDR_PXCLK(x) (((x) & 31) << CCSR_GUTS_CLKDVDR_PXCLK_SHIFT)
#define CCSR_GUTS_CLKDVDR_SSICLK_MASK	0x000000FF
#define CCSR_GUTS_CLKDVDR_SSICLK(x) ((x) & CCSR_GUTS_CLKDVDR_SSICLK_MASK)

#endif

struct ccsr_rcpm_v1 {
	u8	res0000[4];
	__be32	cdozsr;	    
	u8	res0008[4];
	__be32	cdozcr;	    
	u8	res0010[4];
	__be32	cnapsr;	    
	u8	res0018[4];
	__be32	cnapcr;	    
	u8	res0020[4];
	__be32	cdozpsr;    
	u8	res0028[4];
	__be32	cnappsr;    
	u8	res0030[4];
	__be32	cwaitsr;    
	u8	res0038[4];
	__be32	cwdtdsr;    
	__be32	powmgtcsr;  
#define RCPM_POWMGTCSR_SLP	0x00020000
	u8	res0044[12];
	__be32	ippdexpcr;  
	u8	res0054[16];
	__be32	cpmimr;	    
	u8	res0068[4];
	__be32	cpmcimr;    
	u8	res0070[4];
	__be32	cpmmcmr;    
	u8	res0078[4];
	__be32	cpmnmimr;   
	u8	res0080[4];
	__be32	ctbenr;	    
	u8	res0088[4];
	__be32	ctbckselr;  
	u8	res0090[4];
	__be32	ctbhltcr;   
	u8	res0098[4];
	__be32	cmcpmaskcr; 
};

struct ccsr_rcpm_v2 {
	u8	res_00[12];
	__be32	tph10sr0;	
	u8	res_10[12];
	__be32	tph10setr0;	
	u8	res_20[12];
	__be32	tph10clrr0;	
	u8	res_30[12];
	__be32	tph10psr0;	
	u8	res_40[12];
	__be32	twaitsr0;	
	u8	res_50[96];
	__be32	pcph15sr;	
	__be32	pcph15setr;	
	__be32	pcph15clrr;	
	__be32	pcph15psr;	
	u8	res_c0[16];
	__be32	pcph20sr;	
	__be32	pcph20setr;	
	__be32	pcph20clrr;	
	__be32	pcph20psr;	
	__be32	pcpw20sr;	
	u8	res_e0[12];
	__be32	pcph30sr;	
	__be32	pcph30setr;	
	__be32	pcph30clrr;	
	__be32	pcph30psr;	
	u8	res_100[32];
	__be32	ippwrgatecr;	
	u8	res_124[12];
	__be32	powmgtcsr;	
#define RCPM_POWMGTCSR_LPM20_RQ		0x00100000
#define RCPM_POWMGTCSR_LPM20_ST		0x00000200
#define RCPM_POWMGTCSR_P_LPM20_ST	0x00000100
	u8	res_134[12];
	__be32	ippdexpcr[4];	
	u8	res_150[12];
	__be32	tpmimr0;	
	u8	res_160[12];
	__be32	tpmcimr0;	
	u8	res_170[12];
	__be32	tpmmcmr0;	
	u8	res_180[12];
	__be32	tpmnmimr0;	
	u8	res_190[12];
	__be32	tmcpmaskcr0;	
	__be32	pctbenr;	
	__be32	pctbclkselr;	
	__be32	tbclkdivr;	
	u8	res_1ac[4];
	__be32	ttbhltcr[4];	
	__be32	clpcl10sr;	
	__be32	clpcl10setr;	
	__be32	clpcl10clrr;	
	__be32	clpcl10psr;	
	__be32	cddslpsetr;	
	__be32	cddslpclrr;	
	__be32	cdpwroksetr;	
	__be32	cdpwrokclrr;	
	__be32	cdpwrensr;	
	__be32	cddslsr;	
	u8	res_1e8[8];
	__be32	dslpcntcr[8];	
	u8	res_300[3568];
};

#endif
