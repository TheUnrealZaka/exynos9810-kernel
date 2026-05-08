#ifndef _NF_CONNTRACK_TCP_H
#define _NF_CONNTRACK_TCP_H

#include <uapi/linux/netfilter/nf_conntrack_tcp.h>


struct ip_ct_tcp_state {
	u_int32_t	td_end;		
	u_int32_t	td_maxend;	
	u_int32_t	td_maxwin;	
	u_int32_t	td_maxack;	
	u_int8_t	td_scale;	
	u_int8_t	flags;		
};

struct ip_ct_tcp {
	struct ip_ct_tcp_state seen[2];	
	u_int8_t	state;		
	
	u_int8_t	last_dir;	
	u_int8_t	retrans;	
	u_int8_t	last_index;	
	u_int32_t	last_seq;	
	u_int32_t	last_ack;	
	u_int32_t	last_end;	
	u_int16_t	last_win;	
	
	u_int8_t	last_wscale;	
	u_int8_t	last_flags;	
};

#endif 
