
#ifndef AMBA_MMCI_H
#define AMBA_MMCI_H

#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sdio_func.h>

struct embedded_sdio_data {
        struct sdio_cis cis;
        struct sdio_cccr cccr;
        struct sdio_embedded_func *funcs;
        int num_funcs;
};


struct mmci_platform_data {
	unsigned int ocr_mask;
	int (*ios_handler)(struct device *, struct mmc_ios *);
	unsigned int (*status)(struct device *);
	int	gpio_wp;
	int	gpio_cd;
	bool	cd_invert;
	struct embedded_sdio_data *embedded_sdio;
};

#endif
