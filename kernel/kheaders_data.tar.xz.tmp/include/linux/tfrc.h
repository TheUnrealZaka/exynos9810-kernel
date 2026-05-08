#ifndef _LINUX_TFRC_H_
#define _LINUX_TFRC_H_

#include <linux/types.h>


struct tfrc_rx_info {
  	__u32 tfrcrx_x_recv;
	__u32 tfrcrx_rtt;
  	__u32 tfrcrx_p;
};


struct tfrc_tx_info {
	__u64 tfrctx_x;
	__u64 tfrctx_x_recv;
	__u32 tfrctx_x_calc;
	__u32 tfrctx_rtt;
	__u32 tfrctx_p;
	__u32 tfrctx_rto;
	__u32 tfrctx_ipi;
};

#endif 
