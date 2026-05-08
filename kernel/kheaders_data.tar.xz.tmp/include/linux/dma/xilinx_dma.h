

#ifndef __DMA_XILINX_DMA_H
#define __DMA_XILINX_DMA_H

#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>


struct xilinx_vdma_config {
	int frm_dly;
	int gen_lock;
	int master;
	int frm_cnt_en;
	int park;
	int park_frm;
	int coalesc;
	int delay;
	int reset;
	int ext_fsync;
};


enum xdma_ip_type {
	XDMA_TYPE_AXIDMA = 0,
	XDMA_TYPE_CDMA,
	XDMA_TYPE_VDMA,
};

int xilinx_vdma_channel_set_config(struct dma_chan *dchan,
					struct xilinx_vdma_config *cfg);

#endif
