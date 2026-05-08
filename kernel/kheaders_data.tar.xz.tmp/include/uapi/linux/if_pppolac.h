

#ifndef _UAPI_LINUX_IF_PPPOLAC_H
#define _UAPI_LINUX_IF_PPPOLAC_H

#include <linux/socket.h>
#include <linux/types.h>

struct sockaddr_pppolac {
	sa_family_t	sa_family;	
	unsigned int	sa_protocol;	
	int		udp_socket;
	struct __attribute__((packed)) {
		__u16	tunnel, session;
	} local, remote;
} __attribute__((packed));

#endif 
