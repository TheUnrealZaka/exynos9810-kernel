
#ifndef _XT_DSCP_TARGET_H
#define _XT_DSCP_TARGET_H
#include <linux/netfilter/xt_dscp.h>
#include <linux/types.h>


struct xt_DSCP_info {
	__u8 dscp;
};

struct xt_tos_target_info {
	__u8 tos_value;
	__u8 tos_mask;
};

#endif 
