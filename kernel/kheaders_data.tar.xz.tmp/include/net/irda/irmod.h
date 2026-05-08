

#ifndef IRMOD_H
#define IRMOD_H


typedef enum {
	STATUS_OK,
	STATUS_ABORTED,
	STATUS_NO_ACTIVITY,
	STATUS_NOISY,
	STATUS_REMOTE,
} LINK_STATUS;

typedef enum {
	LOCK_NO_CHANGE,
	LOCK_LOCKED,
	LOCK_UNLOCKED,
} LOCK_STATUS;

typedef enum { FLOW_STOP, FLOW_START } LOCAL_FLOW;


typedef enum {
	LM_USER_REQUEST = 1,  
	LM_LAP_DISCONNECT,    
	LM_CONNECT_FAILURE,   
	LM_LAP_RESET,         
	LM_INIT_DISCONNECT,   
	LM_LSAP_NOTCONN,      
	LM_NON_RESP_CLIENT,   
	LM_NO_AVAIL_CLIENT,   
	LM_CONN_HALF_OPEN,    
	LM_BAD_SOURCE_ADDR,   
} LM_REASON;
#define LM_UNKNOWN 0xff       


struct qos_info;		


typedef struct {
	int (*data_indication)(void *priv, void *sap, struct sk_buff *skb);
	int (*udata_indication)(void *priv, void *sap, struct sk_buff *skb);
	void (*connect_confirm)(void *instance, void *sap, 
				struct qos_info *qos, __u32 max_sdu_size,
				__u8 max_header_size, struct sk_buff *skb);
	void (*connect_indication)(void *instance, void *sap, 
				   struct qos_info *qos, __u32 max_sdu_size, 
				   __u8 max_header_size, struct sk_buff *skb);
	void (*disconnect_indication)(void *instance, void *sap, 
				      LM_REASON reason, struct sk_buff *);
	void (*flow_indication)(void *instance, void *sap, LOCAL_FLOW flow);
	void (*status_indication)(void *instance,
				  LINK_STATUS link, LOCK_STATUS lock);
	void *instance; 
	char name[16];  
} notify_t;

#define NOTIFY_MAX_NAME 16


void irda_notify_init(notify_t *notify);


#define irda_lock(lock)		(! test_and_set_bit(0, (void *) (lock)))
#define irda_unlock(lock)	(test_and_clear_bit(0, (void *) (lock)))

#endif 









