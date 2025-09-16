

#ifndef	__MACH_PATA_PXA_H__
#define	__MACH_PATA_PXA_H__

struct pata_pxa_pdata {
	
	uint32_t	dma_dreq;
	
	uint32_t	reg_shift;
	
	uint32_t	irq_flags;
};

#endif	
