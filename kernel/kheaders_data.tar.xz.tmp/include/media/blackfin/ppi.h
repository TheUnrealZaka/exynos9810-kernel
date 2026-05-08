

#ifndef _PPI_H_
#define _PPI_H_

#include <linux/interrupt.h>
#include <asm/blackfin.h>
#include <asm/bfin_ppi.h>


#ifdef EPPI_EN
#define PORT_EN EPPI_EN
#define PORT_DIR EPPI_DIR
#define DMA32 0
#define PACK_EN PACKEN
#endif


#ifdef EPPI0_CTL2
#define PORT_EN EPPI_CTL_EN
#define PORT_DIR EPPI_CTL_DIR
#define PACK_EN EPPI_CTL_PACKEN
#define DMA32 0
#define DLEN_8 EPPI_CTL_DLEN08
#define DLEN_16 EPPI_CTL_DLEN16
#endif

struct ppi_if;

struct ppi_params {
	u32 width;              
	u32 height;             
	u32 hdelay;             
	u32 vdelay;             
	u32 line;               
	u32 frame;              
	u32 hsync;              
	u32 vsync;              
	int bpp;                
	int dlen;               
	u32 ppi_control;        
	u32 int_mask;           
};

struct ppi_ops {
	int (*attach_irq)(struct ppi_if *ppi, irq_handler_t handler);
	void (*detach_irq)(struct ppi_if *ppi);
	int (*start)(struct ppi_if *ppi);
	int (*stop)(struct ppi_if *ppi);
	int (*set_params)(struct ppi_if *ppi, struct ppi_params *params);
	void (*update_addr)(struct ppi_if *ppi, unsigned long addr);
};

enum ppi_type {
	PPI_TYPE_PPI,
	PPI_TYPE_EPPI,
	PPI_TYPE_EPPI3,
};

struct ppi_info {
	enum ppi_type type;
	int dma_ch;
	int irq_err;
	void __iomem *base;
	const unsigned short *pin_req;
};

struct ppi_if {
	struct device *dev;
	unsigned long ppi_control;
	const struct ppi_ops *ops;
	const struct ppi_info *info;
	bool err_int; 
	bool err; 
	void *priv;
};

struct ppi_if *ppi_create_instance(struct platform_device *pdev,
			const struct ppi_info *info);
void ppi_delete_instance(struct ppi_if *ppi);
#endif
