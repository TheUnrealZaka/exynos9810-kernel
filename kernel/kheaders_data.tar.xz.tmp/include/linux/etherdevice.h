
#ifndef _LINUX_ETHERDEVICE_H
#define _LINUX_ETHERDEVICE_H

#include <linux/if_ether.h>
#include <linux/netdevice.h>
#include <linux/random.h>
#include <asm/unaligned.h>
#include <asm/bitsperlong.h>

#ifdef __KERNEL__
struct device;
int eth_platform_get_mac_address(struct device *dev, u8 *mac_addr);
unsigned char *arch_get_platform_get_mac_address(void);
u32 eth_get_headlen(void *data, unsigned int max_len);
__be16 eth_type_trans(struct sk_buff *skb, struct net_device *dev);
extern const struct header_ops eth_header_ops;

int eth_header(struct sk_buff *skb, struct net_device *dev, unsigned short type,
	       const void *daddr, const void *saddr, unsigned len);
int eth_header_parse(const struct sk_buff *skb, unsigned char *haddr);
int eth_header_cache(const struct neighbour *neigh, struct hh_cache *hh,
		     __be16 type);
void eth_header_cache_update(struct hh_cache *hh, const struct net_device *dev,
			     const unsigned char *haddr);
int eth_prepare_mac_addr_change(struct net_device *dev, void *p);
void eth_commit_mac_addr_change(struct net_device *dev, void *p);
int eth_mac_addr(struct net_device *dev, void *p);
int eth_change_mtu(struct net_device *dev, int new_mtu);
int eth_validate_addr(struct net_device *dev);

struct net_device *alloc_etherdev_mqs(int sizeof_priv, unsigned int txqs,
					    unsigned int rxqs);
#define alloc_etherdev(sizeof_priv) alloc_etherdev_mq(sizeof_priv, 1)
#define alloc_etherdev_mq(sizeof_priv, count) alloc_etherdev_mqs(sizeof_priv, count, count)

struct sk_buff **eth_gro_receive(struct sk_buff **head,
				 struct sk_buff *skb);
int eth_gro_complete(struct sk_buff *skb, int nhoff);


static const u8 eth_reserved_addr_base[ETH_ALEN] __aligned(2) =
{ 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };


static inline bool is_link_local_ether_addr(const u8 *addr)
{
	__be16 *a = (__be16 *)addr;
	static const __be16 *b = (const __be16 *)eth_reserved_addr_base;
	static const __be16 m = cpu_to_be16(0xfff0);

#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	return (((*(const u32 *)addr) ^ (*(const u32 *)b)) |
		(__force int)((a[2] ^ b[2]) & m)) == 0;
#else
	return ((a[0] ^ b[0]) | (a[1] ^ b[1]) | ((a[2] ^ b[2]) & m)) == 0;
#endif
}


static inline bool is_zero_ether_addr(const u8 *addr)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	return ((*(const u32 *)addr) | (*(const u16 *)(addr + 4))) == 0;
#else
	return (*(const u16 *)(addr + 0) |
		*(const u16 *)(addr + 2) |
		*(const u16 *)(addr + 4)) == 0;
#endif
}


static inline bool is_multicast_ether_addr(const u8 *addr)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	u32 a = *(const u32 *)addr;
#else
	u16 a = *(const u16 *)addr;
#endif
#ifdef __BIG_ENDIAN
	return 0x01 & (a >> ((sizeof(a) * 8) - 8));
#else
	return 0x01 & a;
#endif
}

static inline bool is_multicast_ether_addr_64bits(const u8 *addr)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) && BITS_PER_LONG == 64
#ifdef __BIG_ENDIAN
	return 0x01 & ((*(const u64 *)addr) >> 56);
#else
	return 0x01 & (*(const u64 *)addr);
#endif
#else
	return is_multicast_ether_addr(addr);
#endif
}


static inline bool is_local_ether_addr(const u8 *addr)
{
	return 0x02 & addr[0];
}


static inline bool is_broadcast_ether_addr(const u8 *addr)
{
	return (*(const u16 *)(addr + 0) &
		*(const u16 *)(addr + 2) &
		*(const u16 *)(addr + 4)) == 0xffff;
}


static inline bool is_unicast_ether_addr(const u8 *addr)
{
	return !is_multicast_ether_addr(addr);
}


static inline bool is_valid_ether_addr(const u8 *addr)
{
	
	return !is_multicast_ether_addr(addr) && !is_zero_ether_addr(addr);
}


static inline bool eth_proto_is_802_3(__be16 proto)
{
#ifndef __BIG_ENDIAN
	
	proto &= htons(0xFF00);
#endif
	
	return (__force u16)proto >= (__force u16)htons(ETH_P_802_3_MIN);
}


static inline void eth_random_addr(u8 *addr)
{
	get_random_bytes(addr, ETH_ALEN);
	addr[0] &= 0xfe;	
	addr[0] |= 0x02;	
}

#define random_ether_addr(addr) eth_random_addr(addr)


static inline void eth_broadcast_addr(u8 *addr)
{
	memset(addr, 0xff, ETH_ALEN);
}


static inline void eth_zero_addr(u8 *addr)
{
	memset(addr, 0x00, ETH_ALEN);
}


static inline void eth_hw_addr_random(struct net_device *dev)
{
	dev->addr_assign_type = NET_ADDR_RANDOM;
	eth_random_addr(dev->dev_addr);
}


static inline void ether_addr_copy(u8 *dst, const u8 *src)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	*(u32 *)dst = *(const u32 *)src;
	*(u16 *)(dst + 4) = *(const u16 *)(src + 4);
#else
	u16 *a = (u16 *)dst;
	const u16 *b = (const u16 *)src;

	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
#endif
}


static inline void eth_hw_addr_inherit(struct net_device *dst,
				       struct net_device *src)
{
	dst->addr_assign_type = src->addr_assign_type;
	ether_addr_copy(dst->dev_addr, src->dev_addr);
}


static inline bool ether_addr_equal(const u8 *addr1, const u8 *addr2)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	u32 fold = ((*(const u32 *)addr1) ^ (*(const u32 *)addr2)) |
		   ((*(const u16 *)(addr1 + 4)) ^ (*(const u16 *)(addr2 + 4)));

	return fold == 0;
#else
	const u16 *a = (const u16 *)addr1;
	const u16 *b = (const u16 *)addr2;

	return ((a[0] ^ b[0]) | (a[1] ^ b[1]) | (a[2] ^ b[2])) == 0;
#endif
}



static inline bool ether_addr_equal_64bits(const u8 *addr1, const u8 *addr2)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) && BITS_PER_LONG == 64
	u64 fold = (*(const u64 *)addr1) ^ (*(const u64 *)addr2);

#ifdef __BIG_ENDIAN
	return (fold >> 16) == 0;
#else
	return (fold << 16) == 0;
#endif
#else
	return ether_addr_equal(addr1, addr2);
#endif
}


static inline bool ether_addr_equal_unaligned(const u8 *addr1, const u8 *addr2)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	return ether_addr_equal(addr1, addr2);
#else
	return memcmp(addr1, addr2, ETH_ALEN) == 0;
#endif
}


static inline bool ether_addr_equal_masked(const u8 *addr1, const u8 *addr2,
					   const u8 *mask)
{
	int i;

	for (i = 0; i < ETH_ALEN; i++) {
		if ((addr1[i] ^ addr2[i]) & mask[i])
			return false;
	}

	return true;
}


static inline bool is_etherdev_addr(const struct net_device *dev,
				    const u8 addr[6 + 2])
{
	struct netdev_hw_addr *ha;
	bool res = false;

	rcu_read_lock();
	for_each_dev_addr(dev, ha) {
		res = ether_addr_equal_64bits(addr, ha->addr);
		if (res)
			break;
	}
	rcu_read_unlock();
	return res;
}
#endif	



static inline unsigned long compare_ether_header(const void *a, const void *b)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS) && BITS_PER_LONG == 64
	unsigned long fold;

	
	fold = *(unsigned long *)a ^ *(unsigned long *)b;
	fold |= *(unsigned long *)(a + 6) ^ *(unsigned long *)(b + 6);
	return fold;
#else
	u32 *a32 = (u32 *)((u8 *)a + 2);
	u32 *b32 = (u32 *)((u8 *)b + 2);

	return (*(u16 *)a ^ *(u16 *)b) | (a32[0] ^ b32[0]) |
	       (a32[1] ^ b32[1]) | (a32[2] ^ b32[2]);
#endif
}


static inline int eth_skb_pad(struct sk_buff *skb)
{
	return skb_put_padto(skb, ETH_ZLEN);
}

#endif	
