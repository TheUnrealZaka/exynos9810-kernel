

#ifndef _PTP_CLASSIFY_H_
#define _PTP_CLASSIFY_H_

#include <linux/ip.h>
#include <linux/skbuff.h>

#define PTP_CLASS_NONE  0x00 
#define PTP_CLASS_V1    0x01 
#define PTP_CLASS_V2    0x02 
#define PTP_CLASS_VMASK 0x0f 
#define PTP_CLASS_IPV4  0x10 
#define PTP_CLASS_IPV6  0x20 
#define PTP_CLASS_L2    0x40 
#define PTP_CLASS_PMASK	0x70 
#define PTP_CLASS_VLAN	0x80 

#define PTP_CLASS_V1_IPV4 (PTP_CLASS_V1 | PTP_CLASS_IPV4)
#define PTP_CLASS_V1_IPV6 (PTP_CLASS_V1 | PTP_CLASS_IPV6) 
#define PTP_CLASS_V2_IPV4 (PTP_CLASS_V2 | PTP_CLASS_IPV4)
#define PTP_CLASS_V2_IPV6 (PTP_CLASS_V2 | PTP_CLASS_IPV6)
#define PTP_CLASS_V2_L2   (PTP_CLASS_V2 | PTP_CLASS_L2)
#define PTP_CLASS_V2_VLAN (PTP_CLASS_V2 | PTP_CLASS_VLAN)
#define PTP_CLASS_L4      (PTP_CLASS_IPV4 | PTP_CLASS_IPV6)

#define PTP_EV_PORT 319
#define PTP_GEN_BIT 0x08 

#define OFF_PTP_SOURCE_UUID	22 
#define OFF_PTP_SEQUENCE_ID	30
#define OFF_PTP_CONTROL		32 


#define IP6_HLEN	40
#define UDP_HLEN	8
#define OFF_IHL		14
#define IPV4_HLEN(data) (((struct iphdr *)(data + OFF_IHL))->ihl << 2)

#if defined(CONFIG_NET_PTP_CLASSIFY)

unsigned int ptp_classify_raw(const struct sk_buff *skb);

void __init ptp_classifier_init(void);
#else
static inline void ptp_classifier_init(void)
{
}
#endif
#endif 
