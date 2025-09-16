
#ifndef _FRAD_H_
#define _FRAD_H_

#include <uapi/linux/if_frad.h>


#if defined(CONFIG_DLCI) || defined(CONFIG_DLCI_MODULE)


struct frhdr
{
   unsigned char  control;

   
   unsigned char  pad;

   unsigned char  NLPID;
   unsigned char  OUI[3];
   __be16 PID;

#define IP_NLPID pad 
} __packed;


#define FRAD_I_UI		0x03

#define FRAD_P_PADDING		0x00
#define FRAD_P_Q933		0x08
#define FRAD_P_SNAP		0x80
#define FRAD_P_CLNP		0x81
#define FRAD_P_IP		0xCC

struct dlci_local
{
   struct net_device      *master;
   struct net_device      *slave;
   struct dlci_conf       config;
   int                    configured;
   struct list_head	  list;

   
   void              (*receive)(struct sk_buff *skb, struct net_device *);
};

struct frad_local
{
   struct net_device_stats stats;

   
   struct net_device     *master[CONFIG_DLCI_MAX];
   short             dlci[CONFIG_DLCI_MAX];

   struct frad_conf  config;
   int               configured;	
   int               initialized;	

   
   int               (*activate)(struct net_device *, struct net_device *);
   int               (*deactivate)(struct net_device *, struct net_device *);
   int               (*assoc)(struct net_device *, struct net_device *);
   int               (*deassoc)(struct net_device *, struct net_device *);
   int               (*dlci_conf)(struct net_device *, struct net_device *, int get);

   
   struct timer_list timer;
   int               type;		
   int               state;		
   int               buffer;		
};

#endif 

extern void dlci_ioctl_set(int (*hook)(unsigned int, void __user *));

#endif
