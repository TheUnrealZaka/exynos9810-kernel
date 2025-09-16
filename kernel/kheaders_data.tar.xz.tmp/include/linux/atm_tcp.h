



#ifndef LINUX_ATM_TCP_H
#define LINUX_ATM_TCP_H

#include <uapi/linux/atm_tcp.h>


struct atm_tcp_ops {
	int (*attach)(struct atm_vcc *vcc,int itf);
	int (*create_persistent)(int itf);
	int (*remove_persistent)(int itf);
	struct module *owner;
};

extern struct atm_tcp_ops atm_tcp_ops;

#endif
