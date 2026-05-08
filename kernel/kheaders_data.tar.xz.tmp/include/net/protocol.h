
 
#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <linux/in6.h>
#include <linux/skbuff.h>
#if IS_ENABLED(CONFIG_IPV6)
#include <linux/ipv6.h>
#endif
#include <linux/netdevice.h>


#define MAX_INET_PROTOS		256


struct net_protocol {
	void			(*early_demux)(struct sk_buff *skb);
	int			(*handler)(struct sk_buff *skb);
	void			(*err_handler)(struct sk_buff *skb, u32 info);
	unsigned int		no_policy:1,
				netns_ok:1,
				
				icmp_strict_tag_validation:1;
};

#if IS_ENABLED(CONFIG_IPV6)
struct inet6_protocol {
	void	(*early_demux)(struct sk_buff *skb);

	int	(*handler)(struct sk_buff *skb);

	void	(*err_handler)(struct sk_buff *skb,
			       struct inet6_skb_parm *opt,
			       u8 type, u8 code, int offset,
			       __be32 info);
	unsigned int	flags;	
};

#define INET6_PROTO_NOPOLICY	0x1
#define INET6_PROTO_FINAL	0x2
#endif

struct net_offload {
	struct offload_callbacks callbacks;
	unsigned int		 flags;	
};

#define INET6_PROTO_GSO_EXTHDR	0x1


struct inet_protosw {
	struct list_head list;

        
	unsigned short	 type;	   
	unsigned short	 protocol; 

	struct proto	 *prot;
	const struct proto_ops *ops;
  
	unsigned char	 flags;      
};
#define INET_PROTOSW_REUSE 0x01	     
#define INET_PROTOSW_PERMANENT 0x02  
#define INET_PROTOSW_ICSK      0x04  

extern const struct net_protocol __rcu *inet_protos[MAX_INET_PROTOS];
extern const struct net_offload __rcu *inet_offloads[MAX_INET_PROTOS];
extern const struct net_offload __rcu *inet6_offloads[MAX_INET_PROTOS];

#if IS_ENABLED(CONFIG_IPV6)
extern const struct inet6_protocol __rcu *inet6_protos[MAX_INET_PROTOS];
#endif

int inet_add_protocol(const struct net_protocol *prot, unsigned char num);
int inet_del_protocol(const struct net_protocol *prot, unsigned char num);
int inet_add_offload(const struct net_offload *prot, unsigned char num);
int inet_del_offload(const struct net_offload *prot, unsigned char num);
void inet_register_protosw(struct inet_protosw *p);
void inet_unregister_protosw(struct inet_protosw *p);

#if IS_ENABLED(CONFIG_IPV6)
int inet6_add_protocol(const struct inet6_protocol *prot, unsigned char num);
int inet6_del_protocol(const struct inet6_protocol *prot, unsigned char num);
int inet6_register_protosw(struct inet_protosw *p);
void inet6_unregister_protosw(struct inet_protosw *p);
#endif
int inet6_add_offload(const struct net_offload *prot, unsigned char num);
int inet6_del_offload(const struct net_offload *prot, unsigned char num);

#endif	
