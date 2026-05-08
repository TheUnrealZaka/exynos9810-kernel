

#ifndef IRLAN_ETH_H
#define IRLAN_ETH_H

struct net_device *alloc_irlandev(const char *name);
int  irlan_eth_receive(void *instance, void *sap, struct sk_buff *skb);

void irlan_eth_flow_indication( void *instance, void *sap, LOCAL_FLOW flow);
#endif
