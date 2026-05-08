#ifndef __NET_NCSI_H
#define __NET_NCSI_H


enum {
	ncsi_dev_state_registered	= 0x0000,
	ncsi_dev_state_functional	= 0x0100,
	ncsi_dev_state_probe		= 0x0200,
	ncsi_dev_state_config		= 0x0300,
	ncsi_dev_state_suspend		= 0x0400,
};

struct ncsi_dev {
	int               state;
	int		  link_up;
	struct net_device *dev;
	void		  (*handler)(struct ncsi_dev *ndev);
};

#ifdef CONFIG_NET_NCSI
struct ncsi_dev *ncsi_register_dev(struct net_device *dev,
				   void (*notifier)(struct ncsi_dev *nd));
int ncsi_start_dev(struct ncsi_dev *nd);
void ncsi_stop_dev(struct ncsi_dev *nd);
void ncsi_unregister_dev(struct ncsi_dev *nd);
#else 
static inline struct ncsi_dev *ncsi_register_dev(struct net_device *dev,
					void (*notifier)(struct ncsi_dev *nd))
{
	return NULL;
}

static inline int ncsi_start_dev(struct ncsi_dev *nd)
{
	return -ENOTTY;
}

static void ncsi_stop_dev(struct ncsi_dev *nd)
{
}

static inline void ncsi_unregister_dev(struct ncsi_dev *nd)
{
}
#endif 

#endif 
