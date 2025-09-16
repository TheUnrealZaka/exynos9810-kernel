
#ifndef _LINUX_IN_H
#define _LINUX_IN_H


#include <linux/errno.h>
#include <uapi/linux/in.h>

static inline int proto_ports_offset(int proto)
{
	switch (proto) {
	case IPPROTO_TCP:
	case IPPROTO_UDP:
	case IPPROTO_DCCP:
	case IPPROTO_ESP:	
	case IPPROTO_SCTP:
	case IPPROTO_UDPLITE:
		return 0;
	case IPPROTO_AH:	
		return 4;
	default:
		return -EINVAL;
	}
}

static inline bool ipv4_is_loopback(__be32 addr)
{
	return (addr & htonl(0xff000000)) == htonl(0x7f000000);
}

static inline bool ipv4_is_multicast(__be32 addr)
{
	return (addr & htonl(0xf0000000)) == htonl(0xe0000000);
}

static inline bool ipv4_is_local_multicast(__be32 addr)
{
	return (addr & htonl(0xffffff00)) == htonl(0xe0000000);
}

static inline bool ipv4_is_lbcast(__be32 addr)
{
	
	return addr == htonl(INADDR_BROADCAST);
}

static inline bool ipv4_is_zeronet(__be32 addr)
{
	return (addr & htonl(0xff000000)) == htonl(0x00000000);
}



static inline bool ipv4_is_private_10(__be32 addr)
{
	return (addr & htonl(0xff000000)) == htonl(0x0a000000);
}

static inline bool ipv4_is_private_172(__be32 addr)
{
	return (addr & htonl(0xfff00000)) == htonl(0xac100000);
}

static inline bool ipv4_is_private_192(__be32 addr)
{
	return (addr & htonl(0xffff0000)) == htonl(0xc0a80000);
}

static inline bool ipv4_is_linklocal_169(__be32 addr)
{
	return (addr & htonl(0xffff0000)) == htonl(0xa9fe0000);
}

static inline bool ipv4_is_anycast_6to4(__be32 addr)
{
	return (addr & htonl(0xffffff00)) == htonl(0xc0586300);
}

static inline bool ipv4_is_test_192(__be32 addr)
{
	return (addr & htonl(0xffffff00)) == htonl(0xc0000200);
}

static inline bool ipv4_is_test_198(__be32 addr)
{
	return (addr & htonl(0xfffe0000)) == htonl(0xc6120000);
}
#endif	
