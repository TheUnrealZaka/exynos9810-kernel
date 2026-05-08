
#ifndef __LINUX_NET_XDP_H__
#define __LINUX_NET_XDP_H__



struct xdp_rxq_info {
	struct net_device *dev;
	u32 queue_index;
	u32 reg_state;
} ____cacheline_aligned; 

int xdp_rxq_info_reg(struct xdp_rxq_info *xdp_rxq,
		     struct net_device *dev, u32 queue_index);
void xdp_rxq_info_unreg(struct xdp_rxq_info *xdp_rxq);
void xdp_rxq_info_unused(struct xdp_rxq_info *xdp_rxq);
bool xdp_rxq_info_is_reg(struct xdp_rxq_info *xdp_rxq);

#endif 
