#ifndef __SOUND_FSI_H
#define __SOUND_FSI_H


#include <linux/clk.h>
#include <sound/soc.h>


#define SH_FSI_FMT_SPDIF		(1 << 0) 
#define SH_FSI_ENABLE_STREAM_MODE	(1 << 1) 
#define SH_FSI_CLK_CPG			(1 << 2) 

struct sh_fsi_port_info {
	unsigned long flags;
	int tx_id;
	int rx_id;
};

struct sh_fsi_platform_info {
	struct sh_fsi_port_info port_a;
	struct sh_fsi_port_info port_b;
};

#endif 
