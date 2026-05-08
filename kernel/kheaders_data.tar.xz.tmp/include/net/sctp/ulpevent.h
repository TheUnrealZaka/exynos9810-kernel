

#ifndef __sctp_ulpevent_h__
#define __sctp_ulpevent_h__



struct sctp_ulpevent {
	struct sctp_association *asoc;
	struct sctp_chunk *chunk;
	unsigned int rmem_len;
	__u32 ppid;
	__u32 tsn;
	__u32 cumtsn;
	__u16 stream;
	__u16 ssn;
	__u16 flags;
	__u16 msg_flags;
};


static inline struct sk_buff *sctp_event2skb(const struct sctp_ulpevent *ev)
{
	return container_of((void *)ev, struct sk_buff, cb);
}


static inline struct sctp_ulpevent *sctp_skb2event(struct sk_buff *skb)
{
	return (struct sctp_ulpevent *)skb->cb;
}

void sctp_ulpevent_free(struct sctp_ulpevent *);
int sctp_ulpevent_is_notification(const struct sctp_ulpevent *);
unsigned int sctp_queue_purge_ulpevents(struct sk_buff_head *list);

struct sctp_ulpevent *sctp_ulpevent_make_assoc_change(
	const struct sctp_association *asoc,
	__u16 flags,
	__u16 state,
	__u16 error,
	__u16 outbound,
	__u16 inbound,
	struct sctp_chunk *chunk,
	gfp_t gfp);

struct sctp_ulpevent *sctp_ulpevent_make_peer_addr_change(
	const struct sctp_association *asoc,
	const struct sockaddr_storage *aaddr,
	int flags,
	int state,
	int error,
	gfp_t gfp);

struct sctp_ulpevent *sctp_ulpevent_make_remote_error(
	const struct sctp_association *asoc,
	struct sctp_chunk *chunk,
	__u16 flags,
	gfp_t gfp);
struct sctp_ulpevent *sctp_ulpevent_make_send_failed(
	const struct sctp_association *asoc,
	struct sctp_chunk *chunk,
	__u16 flags,
	__u32 error,
	gfp_t gfp);

struct sctp_ulpevent *sctp_ulpevent_make_shutdown_event(
	const struct sctp_association *asoc,
	__u16 flags,
	gfp_t gfp);

struct sctp_ulpevent *sctp_ulpevent_make_pdapi(
	const struct sctp_association *asoc,
	__u32 indication, gfp_t gfp);

struct sctp_ulpevent *sctp_ulpevent_make_adaptation_indication(
	const struct sctp_association *asoc, gfp_t gfp);

struct sctp_ulpevent *sctp_ulpevent_make_rcvmsg(struct sctp_association *asoc,
	struct sctp_chunk *chunk,
	gfp_t gfp);

struct sctp_ulpevent *sctp_ulpevent_make_authkey(
	const struct sctp_association *asoc, __u16 key_id,
	__u32 indication, gfp_t gfp);

struct sctp_ulpevent *sctp_ulpevent_make_sender_dry_event(
	const struct sctp_association *asoc, gfp_t gfp);

void sctp_ulpevent_read_sndrcvinfo(const struct sctp_ulpevent *event,
				   struct msghdr *);
void sctp_ulpevent_read_rcvinfo(const struct sctp_ulpevent *event,
				struct msghdr *);
void sctp_ulpevent_read_nxtinfo(const struct sctp_ulpevent *event,
				struct msghdr *, struct sock *sk);

__u16 sctp_ulpevent_get_notification_type(const struct sctp_ulpevent *event);


static inline int sctp_ulpevent_type_enabled(__u16 sn_type,
					     struct sctp_event_subscribe *mask)
{
	int offset = sn_type - SCTP_SN_TYPE_BASE;
	char *amask = (char *) mask;

	if (offset >= sizeof(struct sctp_event_subscribe))
		return 0;
	return amask[offset];
}


static inline int sctp_ulpevent_is_enabled(const struct sctp_ulpevent *event,
					   struct sctp_event_subscribe *mask)
{
	__u16 sn_type;
	int enabled = 1;

	if (sctp_ulpevent_is_notification(event)) {
		sn_type = sctp_ulpevent_get_notification_type(event);
		enabled = sctp_ulpevent_type_enabled(sn_type, mask);
	}
	return enabled;
}

#endif 
