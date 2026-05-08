

#ifndef __NET_TC_TUNNEL_KEY_H
#define __NET_TC_TUNNEL_KEY_H

#include <net/act_api.h>

struct tcf_tunnel_key_params {
	struct rcu_head		rcu;
	int			tcft_action;
	struct metadata_dst     *tcft_enc_metadata;
};

struct tcf_tunnel_key {
	struct tc_action	      common;
	struct tcf_tunnel_key_params __rcu *params;
};

#define to_tunnel_key(a) ((struct tcf_tunnel_key *)a)

#endif 
