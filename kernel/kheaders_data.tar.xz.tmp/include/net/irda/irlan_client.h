

#ifndef IRLAN_CLIENT_H
#define IRLAN_CLIENT_H

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>

#include <net/irda/irias_object.h>
#include <net/irda/irlan_event.h>

void irlan_client_discovery_indication(discinfo_t *, DISCOVERY_MODE, void *);
void irlan_client_wakeup(struct irlan_cb *self, __u32 saddr, __u32 daddr);

void irlan_client_parse_response(struct irlan_cb *self, struct sk_buff *skb);
void irlan_client_get_value_confirm(int result, __u16 obj_id, 
				    struct ias_value *value, void *priv);
#endif
