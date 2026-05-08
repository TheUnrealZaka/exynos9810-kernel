

#ifndef CAIF_DEVICE_H_
#define CAIF_DEVICE_H_
#include <linux/kernel.h>
#include <linux/net.h>
#include <linux/netdevice.h>
#include <linux/caif/caif_socket.h>
#include <net/caif/caif_device.h>


 struct caif_dev_common {
	void (*flowctrl)(struct net_device *net, int on);
	enum caif_link_selector link_select;
	int use_frag;
	int use_fcs;
	int use_stx;
};

#endif	
