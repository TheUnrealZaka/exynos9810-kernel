
#ifndef __PIC32_SDHCI_PDATA_H__
#define __PIC32_SDHCI_PDATA_H__

struct pic32_sdhci_platform_data {
	
	int (*setup_dma)(u32 rfifo, u32 wfifo);
};

#endif
