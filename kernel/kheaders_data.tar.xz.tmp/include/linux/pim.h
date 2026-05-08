#ifndef __LINUX_PIM_H
#define __LINUX_PIM_H

#include <asm/byteorder.h>


#define PIM_V1_VERSION		cpu_to_be32(0x10000000)
#define PIM_V1_REGISTER		1


#define PIM_VERSION		2
#define PIM_REGISTER		1

#define PIM_NULL_REGISTER	cpu_to_be32(0x40000000)

static inline bool ipmr_pimsm_enabled(void)
{
	return IS_BUILTIN(CONFIG_IP_PIMSM_V1) || IS_BUILTIN(CONFIG_IP_PIMSM_V2);
}


struct pimreghdr
{
	__u8	type;
	__u8	reserved;
	__be16	csum;
	__be32	flags;
};

struct sk_buff;
extern int pim_rcv_v1(struct sk_buff *);
#endif
