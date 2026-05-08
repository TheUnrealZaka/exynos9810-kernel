

#ifndef NET_IRDA_H
#define NET_IRDA_H

#include <linux/skbuff.h>		
#include <linux/kernel.h>
#include <linux/if.h>			
#include <linux/irda.h>

typedef __u32 magic_t;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE 
#define FALSE 0
#endif


#ifndef SMALL
#define SMALL 5
#endif

#ifndef IRDA_MIN 
#define IRDA_MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef IRDA_ALIGN
#  define IRDA_ALIGN __attribute__((aligned))
#endif

#ifdef CONFIG_IRDA_DEBUG
#define IRDA_ASSERT(expr, func) \
do { if(!(expr)) { \
	printk( "Assertion failed! %s:%s:%d %s\n", \
		__FILE__,__func__,__LINE__,(#expr) ); \
	func } } while (0)
#define IRDA_ASSERT_LABEL(label)	label
#else
#define IRDA_ASSERT(expr, func) do { (void)(expr); } while (0)
#define IRDA_ASSERT_LABEL(label)
#endif 



#define IRTTY_MAGIC        0x2357
#define LAP_MAGIC          0x1357
#define LMP_MAGIC          0x4321
#define LMP_LSAP_MAGIC     0x69333
#define LMP_LAP_MAGIC      0x3432
#define IRDA_DEVICE_MAGIC  0x63454
#define IAS_MAGIC          0x007
#define TTP_MAGIC          0x241169
#define TTP_TSAP_MAGIC     0x4345
#define IROBEX_MAGIC       0x341324
#define HB_MAGIC           0x64534
#define IRLAN_MAGIC        0x754
#define IAS_OBJECT_MAGIC   0x34234
#define IAS_ATTRIB_MAGIC   0x45232
#define IRDA_TASK_MAGIC    0x38423

#define IAS_DEVICE_ID 0x0000 
#define IAS_PNP_ID    0xd342
#define IAS_OBEX_ID   0x34323
#define IAS_IRLAN_ID  0x34234
#define IAS_IRCOMM_ID 0x2343
#define IAS_IRLPT_ID  0x9876

struct net_device;
struct packet_type;

void irda_proc_register(void);
void irda_proc_unregister(void);

int irda_sysctl_register(void);
void irda_sysctl_unregister(void);

int irsock_init(void);
void irsock_cleanup(void);

int irda_nl_register(void);
void irda_nl_unregister(void);

int irlap_driver_rcv(struct sk_buff *skb, struct net_device *dev,
		     struct packet_type *ptype, struct net_device *orig_dev);

#endif 
