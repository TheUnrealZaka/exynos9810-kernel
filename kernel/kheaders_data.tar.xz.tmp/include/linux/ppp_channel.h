#ifndef _PPP_CHANNEL_H_
#define _PPP_CHANNEL_H_


#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/poll.h>
#include <net/net_namespace.h>

struct ppp_channel;

struct ppp_channel_ops {
	
	int	(*start_xmit)(struct ppp_channel *, struct sk_buff *);
	
	int	(*ioctl)(struct ppp_channel *, unsigned int, unsigned long);
};

struct ppp_channel {
	void		*private;	
	const struct ppp_channel_ops *ops; 
	int		mtu;		
	int		hdrlen;		
	void		*ppp;		
	int		speed;		
	
	int		latency;	
};

#ifdef __KERNEL__

extern void ppp_output_wakeup(struct ppp_channel *);


extern void ppp_input(struct ppp_channel *, struct sk_buff *);


extern void ppp_input_error(struct ppp_channel *, int code);


extern int ppp_register_net_channel(struct net *, struct ppp_channel *);


extern int ppp_register_channel(struct ppp_channel *);


extern void ppp_unregister_channel(struct ppp_channel *);


extern int ppp_channel_index(struct ppp_channel *);


extern int ppp_unit_number(struct ppp_channel *);


extern char *ppp_dev_name(struct ppp_channel *);



#endif 
#endif
