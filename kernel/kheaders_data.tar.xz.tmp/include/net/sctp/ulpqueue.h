

#ifndef __sctp_ulpqueue_h__
#define __sctp_ulpqueue_h__


struct sctp_ulpq {
	char pd_mode;
	struct sctp_association *asoc;
	struct sk_buff_head reasm;
	struct sk_buff_head lobby;
};


struct sctp_ulpq *sctp_ulpq_init(struct sctp_ulpq *,
				 struct sctp_association *);
void sctp_ulpq_flush(struct sctp_ulpq *ulpq);
void sctp_ulpq_free(struct sctp_ulpq *);


int sctp_ulpq_tail_data(struct sctp_ulpq *, struct sctp_chunk *, gfp_t);


int sctp_ulpq_tail_event(struct sctp_ulpq *, struct sctp_ulpevent *ev);


void sctp_ulpq_renege(struct sctp_ulpq *, struct sctp_chunk *, gfp_t);


void sctp_ulpq_partial_delivery(struct sctp_ulpq *, gfp_t);


void sctp_ulpq_abort_pd(struct sctp_ulpq *, gfp_t);


int sctp_clear_pd(struct sock *sk, struct sctp_association *asoc);


void sctp_ulpq_skip(struct sctp_ulpq *ulpq, __u16 sid, __u16 ssn);

void sctp_ulpq_reasm_flushtsn(struct sctp_ulpq *, __u32);
#endif 







