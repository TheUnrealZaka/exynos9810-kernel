

#ifndef __net_sctp_command_h__
#define __net_sctp_command_h__

#include <net/sctp/constants.h>
#include <net/sctp/structs.h>


typedef enum {
	SCTP_CMD_NOP = 0,	
	SCTP_CMD_NEW_ASOC,	
	SCTP_CMD_DELETE_TCB,	
	SCTP_CMD_NEW_STATE,	
	SCTP_CMD_REPORT_TSN,	
	SCTP_CMD_GEN_SACK,	
	SCTP_CMD_PROCESS_SACK,	
	SCTP_CMD_GEN_INIT_ACK,	
	SCTP_CMD_PEER_INIT,	
	SCTP_CMD_GEN_COOKIE_ECHO, 
	SCTP_CMD_CHUNK_ULP,	
	SCTP_CMD_EVENT_ULP,	
	SCTP_CMD_REPLY,		
	SCTP_CMD_SEND_PKT,	
	SCTP_CMD_RETRAN,	
	SCTP_CMD_ECN_CE,        
	SCTP_CMD_ECN_ECNE,	
	SCTP_CMD_ECN_CWR,	
	SCTP_CMD_TIMER_START,	
	SCTP_CMD_TIMER_START_ONCE, 
	SCTP_CMD_TIMER_RESTART,	
	SCTP_CMD_TIMER_STOP,	
	SCTP_CMD_INIT_CHOOSE_TRANSPORT, 
	SCTP_CMD_INIT_COUNTER_RESET, 
	SCTP_CMD_INIT_COUNTER_INC,   
	SCTP_CMD_INIT_RESTART,  
	SCTP_CMD_COOKIEECHO_RESTART,  
	SCTP_CMD_INIT_FAILED,   
	SCTP_CMD_REPORT_DUP,	
	SCTP_CMD_STRIKE,	
	SCTP_CMD_HB_TIMERS_START,    
	SCTP_CMD_HB_TIMER_UPDATE,    
	SCTP_CMD_HB_TIMERS_STOP,     
	SCTP_CMD_TRANSPORT_HB_SENT,  
	SCTP_CMD_TRANSPORT_IDLE,     
	SCTP_CMD_TRANSPORT_ON,       
	SCTP_CMD_REPORT_ERROR,   
	SCTP_CMD_REPORT_BAD_TAG, 
	SCTP_CMD_PROCESS_CTSN,   
	SCTP_CMD_ASSOC_FAILED,	 
	SCTP_CMD_DISCARD_PACKET, 
	SCTP_CMD_GEN_SHUTDOWN,   
	SCTP_CMD_UPDATE_ASSOC,   
	SCTP_CMD_PURGE_OUTQUEUE, 
	SCTP_CMD_SETUP_T2,       
	SCTP_CMD_RTO_PENDING,	 
	SCTP_CMD_PART_DELIVER,	 
	SCTP_CMD_RENEGE,         
	SCTP_CMD_SETUP_T4,	 
	SCTP_CMD_PROCESS_OPERR,  
	SCTP_CMD_REPORT_FWDTSN,	 
	SCTP_CMD_PROCESS_FWDTSN, 
	SCTP_CMD_CLEAR_INIT_TAG, 
	SCTP_CMD_DEL_NON_PRIMARY, 
	SCTP_CMD_T3_RTX_TIMERS_STOP, 
	SCTP_CMD_FORCE_PRIM_RETRAN,  
	SCTP_CMD_SET_SK_ERR,	 
	SCTP_CMD_ASSOC_CHANGE,	 
	SCTP_CMD_ADAPTATION_IND, 
	SCTP_CMD_ASSOC_SHKEY,    
	SCTP_CMD_T1_RETRAN,	 
	SCTP_CMD_UPDATE_INITTAG, 
	SCTP_CMD_SEND_MSG,	 
	SCTP_CMD_SEND_NEXT_ASCONF, 
	SCTP_CMD_PURGE_ASCONF_QUEUE, 
	SCTP_CMD_SET_ASOC,	 
	SCTP_CMD_LAST
} sctp_verb_t;


#define SCTP_MAX_NUM_COMMANDS 20

typedef union {
	void *zero_all;	
	__s32 i32;
	__u32 u32;
	__be32 be32;
	__u16 u16;
	__u8 u8;
	int error;
	__be16 err;
	sctp_state_t state;
	sctp_event_timeout_t to;
	struct sctp_chunk *chunk;
	struct sctp_association *asoc;
	struct sctp_transport *transport;
	struct sctp_bind_addr *bp;
	sctp_init_chunk_t *init;
	struct sctp_ulpevent *ulpevent;
	struct sctp_packet *packet;
	sctp_sackhdr_t *sackh;
	struct sctp_datamsg *msg;
} sctp_arg_t;



#define SCTP_ARG_CONSTRUCTOR(name, type, elt) \
static inline sctp_arg_t	\
SCTP_## name (type arg)		\
{ sctp_arg_t retval;\
  retval.zero_all = NULL;\
  retval.elt = arg;\
  return retval;\
}

SCTP_ARG_CONSTRUCTOR(I32,	__s32, i32)
SCTP_ARG_CONSTRUCTOR(U32,	__u32, u32)
SCTP_ARG_CONSTRUCTOR(BE32,	__be32, be32)
SCTP_ARG_CONSTRUCTOR(U16,	__u16, u16)
SCTP_ARG_CONSTRUCTOR(U8,	__u8, u8)
SCTP_ARG_CONSTRUCTOR(ERROR,     int, error)
SCTP_ARG_CONSTRUCTOR(PERR,      __be16, err)	
SCTP_ARG_CONSTRUCTOR(STATE,	sctp_state_t, state)
SCTP_ARG_CONSTRUCTOR(TO,	sctp_event_timeout_t, to)
SCTP_ARG_CONSTRUCTOR(CHUNK,	struct sctp_chunk *, chunk)
SCTP_ARG_CONSTRUCTOR(ASOC,	struct sctp_association *, asoc)
SCTP_ARG_CONSTRUCTOR(TRANSPORT,	struct sctp_transport *, transport)
SCTP_ARG_CONSTRUCTOR(BA,	struct sctp_bind_addr *, bp)
SCTP_ARG_CONSTRUCTOR(PEER_INIT,	sctp_init_chunk_t *, init)
SCTP_ARG_CONSTRUCTOR(ULPEVENT,  struct sctp_ulpevent *, ulpevent)
SCTP_ARG_CONSTRUCTOR(PACKET,	struct sctp_packet *, packet)
SCTP_ARG_CONSTRUCTOR(SACKH,	sctp_sackhdr_t *, sackh)
SCTP_ARG_CONSTRUCTOR(DATAMSG,	struct sctp_datamsg *, msg)

static inline sctp_arg_t SCTP_FORCE(void)
{
	return SCTP_I32(1);
}

static inline sctp_arg_t SCTP_NOFORCE(void)
{
	return SCTP_I32(0);
}

static inline sctp_arg_t SCTP_NULL(void)
{
	sctp_arg_t retval;
	retval.zero_all = NULL;
	return retval;
}

typedef struct {
	sctp_arg_t obj;
	sctp_verb_t verb;
} sctp_cmd_t;

typedef struct {
	sctp_cmd_t cmds[SCTP_MAX_NUM_COMMANDS];
	sctp_cmd_t *last_used_slot;
	sctp_cmd_t *next_cmd;
} sctp_cmd_seq_t;



static inline int sctp_init_cmd_seq(sctp_cmd_seq_t *seq)
{
	
	seq->last_used_slot = seq->cmds + SCTP_MAX_NUM_COMMANDS;
	seq->next_cmd = seq->last_used_slot;
	return 1;		
}



static inline void sctp_add_cmd_sf(sctp_cmd_seq_t *seq, sctp_verb_t verb,
				   sctp_arg_t obj)
{
	sctp_cmd_t *cmd = seq->last_used_slot - 1;

	BUG_ON(cmd < seq->cmds);

	cmd->verb = verb;
	cmd->obj = obj;
	seq->last_used_slot = cmd;
}


static inline sctp_cmd_t *sctp_next_cmd(sctp_cmd_seq_t *seq)
{
	if (seq->next_cmd <= seq->last_used_slot)
		return NULL;

	return --seq->next_cmd;
}

#endif 
