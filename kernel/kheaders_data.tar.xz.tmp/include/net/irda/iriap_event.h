

#ifndef IRIAP_FSM_H
#define IRIAP_FSM_H


struct iriap_cb;


typedef enum {
	
	S_DISCONNECT,
	S_CONNECTING,
	S_CALL,

	
	S_MAKE_CALL,
	S_CALLING,
	S_OUTSTANDING,
	S_REPLYING,
	S_WAIT_FOR_CALL,
	S_WAIT_ACTIVE,

	
	R_DISCONNECT,
	R_CALL,
	
	
	R_WAITING,
	R_WAIT_ACTIVE,
	R_RECEIVING,
	R_EXECUTE,
	R_RETURNING,
} IRIAP_STATE;

typedef enum {
	IAP_CALL_REQUEST,
	IAP_CALL_REQUEST_GVBC,
	IAP_CALL_RESPONSE,
	IAP_RECV_F_LST,
	IAP_LM_DISCONNECT_INDICATION,
	IAP_LM_CONNECT_INDICATION,
	IAP_LM_CONNECT_CONFIRM,
} IRIAP_EVENT;

void iriap_next_client_state   (struct iriap_cb *self, IRIAP_STATE state);
void iriap_next_call_state     (struct iriap_cb *self, IRIAP_STATE state);
void iriap_next_server_state   (struct iriap_cb *self, IRIAP_STATE state);
void iriap_next_r_connect_state(struct iriap_cb *self, IRIAP_STATE state);


void iriap_do_client_event(struct iriap_cb *self, IRIAP_EVENT event, 
			   struct sk_buff *skb);
void iriap_do_call_event  (struct iriap_cb *self, IRIAP_EVENT event, 
			   struct sk_buff *skb);

void iriap_do_server_event   (struct iriap_cb *self, IRIAP_EVENT event, 
			      struct sk_buff *skb);
void iriap_do_r_connect_event(struct iriap_cb *self, IRIAP_EVENT event, 
			      struct sk_buff *skb);

#endif 

