#ifndef _NET_FLOW_DISSECTOR_H
#define _NET_FLOW_DISSECTOR_H

#include <linux/types.h>
#include <linux/in6.h>
#include <linux/siphash.h>
#include <linux/string.h>
#include <uapi/linux/if_ether.h>


struct flow_dissector_key_control {
	u16	thoff;
	u16	addr_type;
	u32	flags;
};

#define FLOW_DIS_IS_FRAGMENT	BIT(0)
#define FLOW_DIS_FIRST_FRAG	BIT(1)
#define FLOW_DIS_ENCAPSULATION	BIT(2)


struct flow_dissector_key_basic {
	__be16	n_proto;
	u8	ip_proto;
	u8	padding;
};

struct flow_dissector_key_tags {
	u32	flow_label;
};

struct flow_dissector_key_vlan {
	u16	vlan_id:12,
		vlan_priority:3;
	u16	padding;
};

struct flow_dissector_key_keyid {
	__be32	keyid;
};


struct flow_dissector_key_ipv4_addrs {
	
	__be32 src;
	__be32 dst;
};


struct flow_dissector_key_ipv6_addrs {
	
	struct in6_addr src;
	struct in6_addr dst;
};


struct flow_dissector_key_tipc_addrs {
	__be32 srcnode;
};


struct flow_dissector_key_addrs {
	union {
		struct flow_dissector_key_ipv4_addrs v4addrs;
		struct flow_dissector_key_ipv6_addrs v6addrs;
		struct flow_dissector_key_tipc_addrs tipcaddrs;
	};
};


struct flow_dissector_key_ports {
	union {
		__be32 ports;
		struct {
			__be16 src;
			__be16 dst;
		};
	};
};



struct flow_dissector_key_eth_addrs {
	
	unsigned char dst[ETH_ALEN];
	unsigned char src[ETH_ALEN];
};

enum flow_dissector_key_id {
	FLOW_DISSECTOR_KEY_CONTROL, 
	FLOW_DISSECTOR_KEY_BASIC, 
	FLOW_DISSECTOR_KEY_IPV4_ADDRS, 
	FLOW_DISSECTOR_KEY_IPV6_ADDRS, 
	FLOW_DISSECTOR_KEY_PORTS, 
	FLOW_DISSECTOR_KEY_ETH_ADDRS, 
	FLOW_DISSECTOR_KEY_TIPC_ADDRS, 
	FLOW_DISSECTOR_KEY_VLAN, 
	FLOW_DISSECTOR_KEY_FLOW_LABEL, 
	FLOW_DISSECTOR_KEY_GRE_KEYID, 
	FLOW_DISSECTOR_KEY_MPLS_ENTROPY, 

	FLOW_DISSECTOR_KEY_MAX,
};

#define FLOW_DISSECTOR_F_PARSE_1ST_FRAG		BIT(0)
#define FLOW_DISSECTOR_F_STOP_AT_L3		BIT(1)
#define FLOW_DISSECTOR_F_STOP_AT_FLOW_LABEL	BIT(2)
#define FLOW_DISSECTOR_F_STOP_AT_ENCAP		BIT(3)

struct flow_dissector_key {
	enum flow_dissector_key_id key_id;
	size_t offset; 
};

struct flow_dissector {
	unsigned int used_keys; 
	unsigned short int offset[FLOW_DISSECTOR_KEY_MAX];
};

struct flow_keys {
	struct flow_dissector_key_control control;
#define FLOW_KEYS_HASH_START_FIELD basic
	struct flow_dissector_key_basic basic __aligned(SIPHASH_ALIGNMENT);
	struct flow_dissector_key_tags tags;
	struct flow_dissector_key_vlan vlan;
	struct flow_dissector_key_keyid keyid;
	struct flow_dissector_key_ports ports;
	struct flow_dissector_key_addrs addrs;
};

#define FLOW_KEYS_HASH_OFFSET		\
	offsetof(struct flow_keys, FLOW_KEYS_HASH_START_FIELD)

__be32 flow_get_u32_src(const struct flow_keys *flow);
__be32 flow_get_u32_dst(const struct flow_keys *flow);

extern struct flow_dissector flow_keys_dissector;
extern struct flow_dissector flow_keys_buf_dissector;


#define FLOW_KEYS_DIGEST_LEN	16
struct flow_keys_digest {
	u8	data[FLOW_KEYS_DIGEST_LEN];
};

void make_flow_keys_digest(struct flow_keys_digest *digest,
			   const struct flow_keys *flow);

static inline bool flow_keys_have_l4(const struct flow_keys *keys)
{
	return (keys->ports.ports || keys->tags.flow_label);
}

u32 flow_hash_from_keys(struct flow_keys *keys);

static inline bool dissector_uses_key(const struct flow_dissector *flow_dissector,
				      enum flow_dissector_key_id key_id)
{
	return flow_dissector->used_keys & (1 << key_id);
}

static inline void *skb_flow_dissector_target(struct flow_dissector *flow_dissector,
					      enum flow_dissector_key_id key_id,
					      void *target_container)
{
	return ((char *)target_container) + flow_dissector->offset[key_id];
}

static inline void
flow_dissector_init_keys(struct flow_dissector_key_control *key_control,
			 struct flow_dissector_key_basic *key_basic)
{
	memset(key_control, 0, sizeof(*key_control));
	memset(key_basic, 0, sizeof(*key_basic));
}

#endif
