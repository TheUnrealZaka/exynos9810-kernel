

#ifndef IRCOMM_TTY_ATTACH_H
#define IRCOMM_TTY_ATTACH_H

#include <net/irda/ircomm_tty.h>

typedef enum {
        IRCOMM_TTY_IDLE,
	IRCOMM_TTY_SEARCH,
        IRCOMM_TTY_QUERY_PARAMETERS,
	IRCOMM_TTY_QUERY_LSAP_SEL,
	IRCOMM_TTY_SETUP,
        IRCOMM_TTY_READY,
} IRCOMM_TTY_STATE;


typedef enum {
	IRCOMM_TTY_ATTACH_CABLE,
	IRCOMM_TTY_DETACH_CABLE,
	IRCOMM_TTY_DATA_REQUEST,
	IRCOMM_TTY_DATA_INDICATION,
	IRCOMM_TTY_DISCOVERY_REQUEST,
	IRCOMM_TTY_DISCOVERY_INDICATION,
	IRCOMM_TTY_CONNECT_CONFIRM,
	IRCOMM_TTY_CONNECT_INDICATION,
	IRCOMM_TTY_DISCONNECT_REQUEST,
	IRCOMM_TTY_DISCONNECT_INDICATION,
	IRCOMM_TTY_WD_TIMER_EXPIRED,
	IRCOMM_TTY_GOT_PARAMETERS,
	IRCOMM_TTY_GOT_LSAPSEL,
} IRCOMM_TTY_EVENT;


struct ircomm_tty_info {
        __u32     saddr;               
        __u32     daddr;               
        __u8      dlsap_sel;
};

extern const char *const ircomm_state[];
extern const char *const ircomm_tty_state[];

int ircomm_tty_do_event(struct ircomm_tty_cb *self, IRCOMM_TTY_EVENT event,
			struct sk_buff *skb, struct ircomm_tty_info *info);


int  ircomm_tty_attach_cable(struct ircomm_tty_cb *self);
void ircomm_tty_detach_cable(struct ircomm_tty_cb *self);
void ircomm_tty_connect_confirm(void *instance, void *sap, 
				struct qos_info *qos, 
				__u32 max_sdu_size, 
				__u8 max_header_size, 
				struct sk_buff *skb);
void ircomm_tty_disconnect_indication(void *instance, void *sap, 
				      LM_REASON reason,
				      struct sk_buff *skb);
void ircomm_tty_connect_indication(void *instance, void *sap, 
				   struct qos_info *qos, 
				   __u32 max_sdu_size,
				   __u8 max_header_size, 
				   struct sk_buff *skb);
int ircomm_tty_send_initial_parameters(struct ircomm_tty_cb *self);
void ircomm_tty_link_established(struct ircomm_tty_cb *self);

#endif 
