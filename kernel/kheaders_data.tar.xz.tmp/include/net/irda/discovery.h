

#ifndef DISCOVERY_H
#define DISCOVERY_H

#include <asm/param.h>

#include <net/irda/irda.h>
#include <net/irda/irqueue.h>		
#include <net/irda/irlap_event.h>	

#define DISCOVERY_EXPIRE_TIMEOUT (2*sysctl_discovery_timeout*HZ)
#define DISCOVERY_DEFAULT_SLOTS  0


typedef union {
	__u16 word;
	__u8  byte[2];
} __u16_host_order;


typedef enum {
	DISCOVERY_LOG,		
	DISCOVERY_ACTIVE,	
	DISCOVERY_PASSIVE,	
	EXPIRY_TIMEOUT,		
} DISCOVERY_MODE;

#define NICKNAME_MAX_LEN 21


typedef struct irda_device_info		discinfo_t;	


typedef struct discovery_t {
	irda_queue_t	q;		

	discinfo_t	data;		
	int		name_len;	

	LAP_REASON	condition;	
	int		gen_addr_bit;	
	int		nslots;		
	unsigned long	timestamp;	
	unsigned long	firststamp;	
} discovery_t;

void irlmp_add_discovery(hashbin_t *cachelog, discovery_t *discovery);
void irlmp_add_discovery_log(hashbin_t *cachelog, hashbin_t *log);
void irlmp_expire_discoveries(hashbin_t *log, __u32 saddr, int force);
struct irda_device_info *irlmp_copy_discoveries(hashbin_t *log, int *pn,
						__u16 mask, int old_entries);

#endif
