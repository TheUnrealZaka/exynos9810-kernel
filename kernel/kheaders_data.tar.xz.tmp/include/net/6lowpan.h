





#ifndef __6LOWPAN_H__
#define __6LOWPAN_H__

#include <linux/debugfs.h>

#include <net/ipv6.h>
#include <net/net_namespace.h>


#include <net/mac802154.h>

#define EUI64_ADDR_LEN		8

#define LOWPAN_NHC_MAX_ID_LEN	1

#define LOWPAN_NHC_MAX_HDR_LEN	(sizeof(struct udphdr))

#define LOWPAN_IPHC_MAX_HEADER_LEN	(2 + 1 + LOWPAN_NHC_MAX_ID_LEN)

#define LOWPAN_IPHC_MAX_HC_BUF_LEN	(sizeof(struct ipv6hdr) +	\
					 LOWPAN_IPHC_MAX_HEADER_LEN +	\
					 LOWPAN_NHC_MAX_HDR_LEN)

#define LOWPAN_IPHC_CTX_TABLE_SIZE	(1 << 4)

#define LOWPAN_DISPATCH_IPV6		0x41 
#define LOWPAN_DISPATCH_IPHC		0x60 
#define LOWPAN_DISPATCH_IPHC_MASK	0xe0

static inline bool lowpan_is_ipv6(u8 dispatch)
{
	return dispatch == LOWPAN_DISPATCH_IPV6;
}

static inline bool lowpan_is_iphc(u8 dispatch)
{
	return (dispatch & LOWPAN_DISPATCH_IPHC_MASK) == LOWPAN_DISPATCH_IPHC;
}

#define LOWPAN_PRIV_SIZE(llpriv_size)	\
	(sizeof(struct lowpan_dev) + llpriv_size)

enum lowpan_lltypes {
	LOWPAN_LLTYPE_BTLE,
	LOWPAN_LLTYPE_IEEE802154,
};

enum lowpan_iphc_ctx_flags {
	LOWPAN_IPHC_CTX_FLAG_ACTIVE,
	LOWPAN_IPHC_CTX_FLAG_COMPRESSION,
};

struct lowpan_iphc_ctx {
	u8 id;
	struct in6_addr pfx;
	u8 plen;
	unsigned long flags;
};

struct lowpan_iphc_ctx_table {
	spinlock_t lock;
	const struct lowpan_iphc_ctx_ops *ops;
	struct lowpan_iphc_ctx table[LOWPAN_IPHC_CTX_TABLE_SIZE];
};

static inline bool lowpan_iphc_ctx_is_active(const struct lowpan_iphc_ctx *ctx)
{
	return test_bit(LOWPAN_IPHC_CTX_FLAG_ACTIVE, &ctx->flags);
}

static inline bool
lowpan_iphc_ctx_is_compression(const struct lowpan_iphc_ctx *ctx)
{
	return test_bit(LOWPAN_IPHC_CTX_FLAG_COMPRESSION, &ctx->flags);
}

struct lowpan_dev {
	enum lowpan_lltypes lltype;
	struct dentry *iface_debugfs;
	struct lowpan_iphc_ctx_table ctx;

	
	u8 priv[0] __aligned(sizeof(void *));
};

struct lowpan_802154_neigh {
	__le16 short_addr;
};

static inline
struct lowpan_802154_neigh *lowpan_802154_neigh(void *neigh_priv)
{
	return neigh_priv;
}

static inline
struct lowpan_dev *lowpan_dev(const struct net_device *dev)
{
	return netdev_priv(dev);
}


struct lowpan_802154_dev {
	struct net_device	*wdev; 
	u16			fragment_tag;
};

static inline struct
lowpan_802154_dev *lowpan_802154_dev(const struct net_device *dev)
{
	return (struct lowpan_802154_dev *)lowpan_dev(dev)->priv;
}

struct lowpan_802154_cb {
	u16 d_tag;
	unsigned int d_size;
	u8 d_offset;
};

static inline
struct lowpan_802154_cb *lowpan_802154_cb(const struct sk_buff *skb)
{
	BUILD_BUG_ON(sizeof(struct lowpan_802154_cb) > sizeof(skb->cb));
	return (struct lowpan_802154_cb *)skb->cb;
}

static inline void lowpan_iphc_uncompress_eui64_lladdr(struct in6_addr *ipaddr,
						       const void *lladdr)
{
	
	ipaddr->s6_addr[0] = 0xFE;
	ipaddr->s6_addr[1] = 0x80;
	memcpy(&ipaddr->s6_addr[8], lladdr, EUI64_ADDR_LEN);
	
	ipaddr->s6_addr[8] ^= 0x02;
}

#ifdef DEBUG

static inline void raw_dump_inline(const char *caller, char *msg,
				   const unsigned char *buf, int len)
{
	if (msg)
		pr_debug("%s():%s: ", caller, msg);

	print_hex_dump_debug("", DUMP_PREFIX_NONE, 16, 1, buf, len, false);
}


static inline void raw_dump_table(const char *caller, char *msg,
				  const unsigned char *buf, int len)
{
	if (msg)
		pr_debug("%s():%s:\n", caller, msg);

	print_hex_dump_debug("\t", DUMP_PREFIX_OFFSET, 16, 1, buf, len, false);
}
#else
static inline void raw_dump_table(const char *caller, char *msg,
				  const unsigned char *buf, int len) { }
static inline void raw_dump_inline(const char *caller, char *msg,
				   const unsigned char *buf, int len) { }
#endif


static inline bool lowpan_fetch_skb(struct sk_buff *skb, void *data,
				    unsigned int len)
{
	if (unlikely(!pskb_may_pull(skb, len)))
		return true;

	skb_copy_from_linear_data(skb, data, len);
	skb_pull(skb, len);

	return false;
}

static inline bool lowpan_802154_is_valid_src_short_addr(__le16 addr)
{
	
	return !(addr & cpu_to_le16(0x8000));
}

static inline void lowpan_push_hc_data(u8 **hc_ptr, const void *data,
				       const size_t len)
{
	memcpy(*hc_ptr, data, len);
	*hc_ptr += len;
}

int lowpan_register_netdevice(struct net_device *dev,
			      enum lowpan_lltypes lltype);
int lowpan_register_netdev(struct net_device *dev,
			   enum lowpan_lltypes lltype);
void lowpan_unregister_netdevice(struct net_device *dev);
void lowpan_unregister_netdev(struct net_device *dev);


int lowpan_header_decompress(struct sk_buff *skb, const struct net_device *dev,
			     const void *daddr, const void *saddr);


int lowpan_header_compress(struct sk_buff *skb, const struct net_device *dev,
			   const void *daddr, const void *saddr);

#endif 
