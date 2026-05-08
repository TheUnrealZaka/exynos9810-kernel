

#ifndef IRMLP_FRAME_H
#define IRMLP_FRAME_H

#include <linux/skbuff.h>

#include <net/irda/discovery.h>


#define CONNECT_CMD    0x01
#define CONNECT_CNF    0x81
#define DISCONNECT     0x02
#define ACCESSMODE_CMD 0x03
#define ACCESSMODE_CNF 0x83

#define CONTROL_BIT    0x80

void irlmp_send_data_pdu(struct lap_cb *self, __u8 dlsap, __u8 slsap,
				int expedited, struct sk_buff *skb);
void irlmp_send_lcf_pdu(struct lap_cb *self, __u8 dlsap, __u8 slsap, 
			__u8 opcode, struct sk_buff *skb);
void irlmp_link_data_indication(struct lap_cb *, struct sk_buff *, 
				int unreliable);
#ifdef CONFIG_IRDA_ULTRA
void irlmp_link_unitdata_indication(struct lap_cb *, struct sk_buff *);
#endif 

void irlmp_link_connect_indication(struct lap_cb *, __u32 saddr, __u32 daddr,
				   struct qos_info *qos, struct sk_buff *skb);
void irlmp_link_connect_request(__u32 daddr);
void irlmp_link_connect_confirm(struct lap_cb *self, struct qos_info *qos, 
				struct sk_buff *skb);
void irlmp_link_disconnect_indication(struct lap_cb *, struct irlap_cb *, 
				      LAP_REASON reason, struct sk_buff *); 
void irlmp_link_discovery_confirm(struct lap_cb *self, hashbin_t *log);
void irlmp_link_discovery_indication(struct lap_cb *, discovery_t *discovery);

#endif
