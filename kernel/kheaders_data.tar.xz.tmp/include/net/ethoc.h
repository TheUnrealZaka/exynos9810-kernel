

#ifndef LINUX_NET_ETHOC_H
#define LINUX_NET_ETHOC_H 1

struct ethoc_platform_data {
	u8 hwaddr[IFHWADDRLEN];
	s8 phy_id;
	u32 eth_clkfreq;
	bool big_endian;
};

#endif 

