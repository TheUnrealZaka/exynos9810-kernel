
#ifndef _LINUX_IN6_H
#define _LINUX_IN6_H

#include <uapi/linux/in6.h>


extern const struct in6_addr in6addr_any;
#define IN6ADDR_ANY_INIT { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } } }
extern const struct in6_addr in6addr_loopback;
#define IN6ADDR_LOOPBACK_INIT { { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 } } }
extern const struct in6_addr in6addr_linklocal_allnodes;
#define IN6ADDR_LINKLOCAL_ALLNODES_INIT	\
		{ { { 0xff,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1 } } }
extern const struct in6_addr in6addr_linklocal_allrouters;
#define IN6ADDR_LINKLOCAL_ALLROUTERS_INIT \
		{ { { 0xff,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2 } } }
extern const struct in6_addr in6addr_interfacelocal_allnodes;
#define IN6ADDR_INTERFACELOCAL_ALLNODES_INIT \
		{ { { 0xff,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1 } } }
extern const struct in6_addr in6addr_interfacelocal_allrouters;
#define IN6ADDR_INTERFACELOCAL_ALLROUTERS_INIT \
		{ { { 0xff,1,0,0,0,0,0,0,0,0,0,0,0,0,0,2 } } }
extern const struct in6_addr in6addr_sitelocal_allrouters;
#define IN6ADDR_SITELOCAL_ALLROUTERS_INIT \
		{ { { 0xff,5,0,0,0,0,0,0,0,0,0,0,0,0,0,2 } } }
#endif
