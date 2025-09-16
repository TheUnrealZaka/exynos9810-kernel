

#ifndef _LINUX_KS8851_MLL_H
#define _LINUX_KS8851_MLL_H

#include <linux/if_ether.h>


struct ks8851_mll_platform_data {
	u8 mac_addr[ETH_ALEN];
};

#endif
