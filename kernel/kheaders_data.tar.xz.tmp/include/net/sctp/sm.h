

#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/slab.h>
#include <linux/in.h>
#include <net/sctp/command.h>
#include <net/sctp/sctp.h>

#ifndef __sctp_sm_h__
#define __sctp_sm_h__


typedef enum {
	SCTP_DISPOSITION_DISCARD,	 
	SCTP_DISPOSITION_CONSUME,	 
	SCTP_DISPOSITION_NOMEM,		 
	SCTP_DISPOSITION_DELETE_TCB,	 
	SCTP_DISPOSITION_ABORT,		 
	SCTP_DISPOSITION_VIOLATION,	 
	SCTP_DISPOSITION_NOT_IMPL,	 
	SCTP_DISPOSITION_ERROR,		 
	SCTP_DISPOSITION_BUG,		 
} sctp_disposition_t;

typedef struct {
	int name;
	int action;
} sctp_sm_command_t;

typedef sctp_disposition_t (sctp_state_fn_t) (struct net *,
					      const struct sctp_endpoint *,
					      const struct sctp_association *,
					      const sctp_subtype_t type,
					      void *arg,
					      sctp_cmd_seq_t *);
typedef void (sctp_timer_event_t) (unsigned long);
typedef struct {
	sctp_state_fn_t *fn;
	const char *name;
} sctp_sm_table_entry_t;




sctp_state_fn_t sctp_sf_not_impl;
sctp_state_fn_t sctp_sf_bug;


sctp_state_fn_t sctp_sf_timer_ignore;


sctp_state_fn_t sctp_sf_do_9_1_abort;
sctp_state_fn_t sctp_sf_cookie_wait_abort;
sctp_state_fn_t sctp_sf_cookie_echoed_abort;
sctp_state_fn_t sctp_sf_shutdown_pending_abort;
sctp_state_fn_t sctp_sf_shutdown_sent_abort;
sctp_state_fn_t sctp_sf_shutdown_ack_sent_abort;
sctp_state_fn_t sctp_sf_do_5_1B_init;
sctp_state_fn_t sctp_sf_do_5_1C_ack;
sctp_state_fn_t sctp_sf_do_5_1D_ce;
sctp_state_fn_t sctp_sf_do_5_1E_ca;
sctp_state_fn_t sctp_sf_do_4_C;
sctp_state_fn_t sctp_sf_eat_data_6_2;
sctp_state_fn_t sctp_sf_eat_data_fast_4_4;
sctp_state_fn_t sctp_sf_eat_sack_6_2;
sctp_state_fn_t sctp_sf_operr_notify;
sctp_state_fn_t sctp_sf_t1_init_timer_expire;
sctp_state_fn_t sctp_sf_t1_cookie_timer_expire;
sctp_state_fn_t sctp_sf_t2_timer_expire;
sctp_state_fn_t sctp_sf_t4_timer_expire;
sctp_state_fn_t sctp_sf_t5_timer_expire;
sctp_state_fn_t sctp_sf_sendbeat_8_3;
sctp_state_fn_t sctp_sf_beat_8_3;
sctp_state_fn_t sctp_sf_backbeat_8_3;
sctp_state_fn_t sctp_sf_do_9_2_final;
sctp_state_fn_t sctp_sf_do_9_2_shutdown;
sctp_state_fn_t sctp_sf_do_9_2_shut_ctsn;
sctp_state_fn_t sctp_sf_do_ecn_cwr;
sctp_state_fn_t sctp_sf_do_ecne;
sctp_state_fn_t sctp_sf_ootb;
sctp_state_fn_t sctp_sf_pdiscard;
sctp_state_fn_t sctp_sf_violation;
sctp_state_fn_t sctp_sf_discard_chunk;
sctp_state_fn_t sctp_sf_do_5_2_1_siminit;
sctp_state_fn_t sctp_sf_do_5_2_2_dupinit;
sctp_state_fn_t sctp_sf_do_5_2_3_initack;
sctp_state_fn_t sctp_sf_do_5_2_4_dupcook;
sctp_state_fn_t sctp_sf_unk_chunk;
sctp_state_fn_t sctp_sf_do_8_5_1_E_sa;
sctp_state_fn_t sctp_sf_cookie_echoed_err;
sctp_state_fn_t sctp_sf_do_asconf;
sctp_state_fn_t sctp_sf_do_asconf_ack;
sctp_state_fn_t sctp_sf_do_9_2_reshutack;
sctp_state_fn_t sctp_sf_eat_fwd_tsn;
sctp_state_fn_t sctp_sf_eat_fwd_tsn_fast;
sctp_state_fn_t sctp_sf_eat_auth;


sctp_state_fn_t sctp_sf_do_prm_asoc;
sctp_state_fn_t sctp_sf_do_prm_send;
sctp_state_fn_t sctp_sf_do_9_2_prm_shutdown;
sctp_state_fn_t sctp_sf_cookie_wait_prm_shutdown;
sctp_state_fn_t sctp_sf_cookie_echoed_prm_shutdown;
sctp_state_fn_t sctp_sf_do_9_1_prm_abort;
sctp_state_fn_t sctp_sf_cookie_wait_prm_abort;
sctp_state_fn_t sctp_sf_cookie_echoed_prm_abort;
sctp_state_fn_t sctp_sf_shutdown_pending_prm_abort;
sctp_state_fn_t sctp_sf_shutdown_sent_prm_abort;
sctp_state_fn_t sctp_sf_shutdown_ack_sent_prm_abort;
sctp_state_fn_t sctp_sf_error_closed;
sctp_state_fn_t sctp_sf_error_shutdown;
sctp_state_fn_t sctp_sf_ignore_primitive;
sctp_state_fn_t sctp_sf_do_prm_requestheartbeat;
sctp_state_fn_t sctp_sf_do_prm_asconf;


sctp_state_fn_t sctp_sf_do_no_pending_tsn;
sctp_state_fn_t sctp_sf_do_9_2_start_shutdown;
sctp_state_fn_t sctp_sf_do_9_2_shutdown_ack;
sctp_state_fn_t sctp_sf_ignore_other;
sctp_state_fn_t sctp_sf_cookie_wait_icmp_abort;


sctp_state_fn_t sctp_sf_do_6_3_3_rtx;
sctp_state_fn_t sctp_sf_do_6_2_sack;
sctp_state_fn_t sctp_sf_autoclose_timer_expire;


__u8 sctp_get_chunk_type(struct sctp_chunk *chunk);
const sctp_sm_table_entry_t *sctp_sm_lookup_event(struct net *,
					    sctp_event_t,
					    sctp_state_t,
					    sctp_subtype_t);
int sctp_chunk_iif(const struct sctp_chunk *);
struct sctp_association *sctp_make_temp_asoc(const struct sctp_endpoint *,
					     struct sctp_chunk *,
					     gfp_t gfp);
__u32 sctp_generate_verification_tag(void);
void sctp_populate_tie_tags(__u8 *cookie, __u32 curTag, __u32 hisTag);


struct sctp_chunk *sctp_make_init(const struct sctp_association *,
			     const struct sctp_bind_addr *,
			     gfp_t gfp, int vparam_len);
struct sctp_chunk *sctp_make_init_ack(const struct sctp_association *,
				 const struct sctp_chunk *,
				 const gfp_t gfp,
				 const int unkparam_len);
struct sctp_chunk *sctp_make_cookie_echo(const struct sctp_association *,
				    const struct sctp_chunk *);
struct sctp_chunk *sctp_make_cookie_ack(const struct sctp_association *,
				   const struct sctp_chunk *);
struct sctp_chunk *sctp_make_cwr(const struct sctp_association *,
				 const __u32 lowest_tsn,
				 const struct sctp_chunk *);
struct sctp_chunk * sctp_make_datafrag_empty(struct sctp_association *,
					const struct sctp_sndrcvinfo *sinfo,
					int len, const __u8 flags,
					__u16 ssn, gfp_t gfp);
struct sctp_chunk *sctp_make_ecne(const struct sctp_association *,
				  const __u32);
struct sctp_chunk *sctp_make_sack(const struct sctp_association *);
struct sctp_chunk *sctp_make_shutdown(const struct sctp_association *asoc,
				      const struct sctp_chunk *chunk);
struct sctp_chunk *sctp_make_shutdown_ack(const struct sctp_association *asoc,
					  const struct sctp_chunk *);
struct sctp_chunk *sctp_make_shutdown_complete(const struct sctp_association *,
					  const struct sctp_chunk *);
void sctp_init_cause(struct sctp_chunk *, __be16 cause, size_t);
struct sctp_chunk *sctp_make_abort(const struct sctp_association *,
			      const struct sctp_chunk *,
			      const size_t hint);
struct sctp_chunk *sctp_make_abort_no_data(const struct sctp_association *,
				      const struct sctp_chunk *,
				      __u32 tsn);
struct sctp_chunk *sctp_make_abort_user(const struct sctp_association *,
					struct msghdr *, size_t msg_len);
struct sctp_chunk *sctp_make_abort_violation(const struct sctp_association *,
				   const struct sctp_chunk *,
				   const __u8 *,
				   const size_t );
struct sctp_chunk *sctp_make_violation_paramlen(const struct sctp_association *,
				   const struct sctp_chunk *,
				   struct sctp_paramhdr *);
struct sctp_chunk *sctp_make_violation_max_retrans(const struct sctp_association *,
						   const struct sctp_chunk *);
struct sctp_chunk *sctp_make_heartbeat(const struct sctp_association *,
				  const struct sctp_transport *);
struct sctp_chunk *sctp_make_heartbeat_ack(const struct sctp_association *,
				      const struct sctp_chunk *,
				      const void *payload,
				      const size_t paylen);
struct sctp_chunk *sctp_make_op_error(const struct sctp_association *,
				 const struct sctp_chunk *chunk,
				 __be16 cause_code,
				 const void *payload,
				 size_t paylen,
				 size_t reserve_tail);

struct sctp_chunk *sctp_make_asconf_update_ip(struct sctp_association *,
					      union sctp_addr *,
					      struct sockaddr *,
					      int, __be16);
struct sctp_chunk *sctp_make_asconf_set_prim(struct sctp_association *asoc,
					     union sctp_addr *addr);
bool sctp_verify_asconf(const struct sctp_association *asoc,
			struct sctp_chunk *chunk, bool addr_param_needed,
			struct sctp_paramhdr **errp);
struct sctp_chunk *sctp_process_asconf(struct sctp_association *asoc,
				       struct sctp_chunk *asconf);
int sctp_process_asconf_ack(struct sctp_association *asoc,
			    struct sctp_chunk *asconf_ack);
struct sctp_chunk *sctp_make_fwdtsn(const struct sctp_association *asoc,
				    __u32 new_cum_tsn, size_t nstreams,
				    struct sctp_fwdtsn_skip *skiplist);
struct sctp_chunk *sctp_make_auth(const struct sctp_association *asoc);

void sctp_chunk_assign_tsn(struct sctp_chunk *);
void sctp_chunk_assign_ssn(struct sctp_chunk *);



int sctp_do_sm(struct net *net, sctp_event_t event_type, sctp_subtype_t subtype,
	       sctp_state_t state,
               struct sctp_endpoint *,
               struct sctp_association *asoc,
               void *event_arg,
	       gfp_t gfp);


void sctp_generate_t3_rtx_event(unsigned long peer);
void sctp_generate_heartbeat_event(unsigned long peer);
void sctp_generate_proto_unreach_event(unsigned long peer);

void sctp_ootb_pkt_free(struct sctp_packet *);

struct sctp_association *sctp_unpack_cookie(const struct sctp_endpoint *,
				       const struct sctp_association *,
				       struct sctp_chunk *,
				       gfp_t gfp, int *err,
				       struct sctp_chunk **err_chk_p);
int sctp_addip_addr_config(struct sctp_association *, sctp_param_t,
			   struct sockaddr_storage*, int);


__u32 sctp_generate_tag(const struct sctp_endpoint *);
__u32 sctp_generate_tsn(const struct sctp_endpoint *);


extern sctp_timer_event_t *sctp_timer_events[SCTP_NUM_TIMEOUT_TYPES];



static inline __u16 sctp_data_size(struct sctp_chunk *chunk)
{
	__u16 size;

	size = ntohs(chunk->chunk_hdr->length);
	size -= sizeof(sctp_data_chunk_t);

	return size;
}


#define TSN_lt(a,b)	\
	(typecheck(__u32, a) && \
	 typecheck(__u32, b) && \
	 ((__s32)((a) - (b)) < 0))

#define TSN_lte(a,b)	\
	(typecheck(__u32, a) && \
	 typecheck(__u32, b) && \
	 ((__s32)((a) - (b)) <= 0))


#define SSN_lt(a,b)		\
	(typecheck(__u16, a) && \
	 typecheck(__u16, b) && \
	 ((__s16)((a) - (b)) < 0))


#define ADDIP_SERIAL_gte(a,b)	\
	(typecheck(__u32, a) && \
	 typecheck(__u32, b) && \
	 ((__s32)((b) - (a)) <= 0))


static inline int
sctp_vtag_verify(const struct sctp_chunk *chunk,
		 const struct sctp_association *asoc)
{
	
        if (ntohl(chunk->sctp_hdr->vtag) == asoc->c.my_vtag)
                return 1;

	return 0;
}


static inline int
sctp_vtag_verify_either(const struct sctp_chunk *chunk,
			const struct sctp_association *asoc)
{
        
        if ((!sctp_test_T_bit(chunk) &&
             (ntohl(chunk->sctp_hdr->vtag) == asoc->c.my_vtag)) ||
	    (sctp_test_T_bit(chunk) && asoc->c.peer_vtag &&
	     (ntohl(chunk->sctp_hdr->vtag) == asoc->c.peer_vtag))) {
                return 1;
	}

	return 0;
}

#endif 
