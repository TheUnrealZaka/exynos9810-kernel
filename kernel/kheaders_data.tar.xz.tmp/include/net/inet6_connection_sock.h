
#ifndef _INET6_CONNECTION_SOCK_H
#define _INET6_CONNECTION_SOCK_H

#include <linux/types.h>

struct inet_bind_bucket;
struct request_sock;
struct sk_buff;
struct sock;
struct sockaddr;

int inet6_csk_bind_conflict(const struct sock *sk,
			    const struct inet_bind_bucket *tb, bool relax);

struct dst_entry *inet6_csk_route_req(const struct sock *sk, struct flowi6 *fl6,
				      const struct request_sock *req, u8 proto);

void inet6_csk_addr2sockaddr(struct sock *sk, struct sockaddr *uaddr);

int inet6_csk_xmit(struct sock *sk, struct sk_buff *skb, struct flowi *fl);

struct dst_entry *inet6_csk_update_pmtu(struct sock *sk, u32 mtu);
#endif 
