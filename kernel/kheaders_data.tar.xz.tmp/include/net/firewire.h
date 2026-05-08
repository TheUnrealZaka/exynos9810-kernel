#ifndef _NET_FIREWIRE_H
#define _NET_FIREWIRE_H


#define FWNET_ALEN	16
union fwnet_hwaddr {
	u8 u[FWNET_ALEN];
	
	struct {
		__be64 uniq_id;		
		u8 max_rec;		
		u8 sspd;		
		__be16 fifo_hi;		
		__be32 fifo_lo;		
	} __packed uc;
};


#define FWNET_HLEN	18
struct fwnet_header {
	u8 h_dest[FWNET_ALEN];	
	__be16 h_proto;		
} __packed;

#endif
