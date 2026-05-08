

#ifndef	__LINUX_USB_USBNET_H
#define	__LINUX_USB_USBNET_H


struct usbnet {
	
	struct usb_device	*udev;
	struct usb_interface	*intf;
	struct driver_info	*driver_info;
	const char		*driver_name;
	void			*driver_priv;
	wait_queue_head_t	wait;
	struct mutex		phy_mutex;
	unsigned char		suspend_count;
	unsigned char		pkt_cnt, pkt_err;
	unsigned short		rx_qlen, tx_qlen;
	unsigned		can_dma_sg:1;

	
	unsigned		in, out;
	struct usb_host_endpoint *status;
	unsigned		maxpacket;
	struct timer_list	delay;
	const char		*padding_pkt;

	
	struct net_device	*net;
	int			msg_enable;
	unsigned long		data[5];
	u32			xid;
	u32			hard_mtu;	
	size_t			rx_urb_size;	
	struct mii_if_info	mii;

	
	struct sk_buff_head	rxq;
	struct sk_buff_head	txq;
	struct sk_buff_head	done;
	struct sk_buff_head	rxq_pause;
	struct urb		*interrupt;
	unsigned		interrupt_count;
	struct mutex		interrupt_mutex;
	struct usb_anchor	deferred;
	struct tasklet_struct	bh;

	struct work_struct	kevent;
	unsigned long		flags;
#		define EVENT_TX_HALT	0
#		define EVENT_RX_HALT	1
#		define EVENT_RX_MEMORY	2
#		define EVENT_STS_SPLIT	3
#		define EVENT_LINK_RESET	4
#		define EVENT_RX_PAUSED	5
#		define EVENT_DEV_ASLEEP 6
#		define EVENT_DEV_OPEN	7
#		define EVENT_DEVICE_REPORT_IDLE	8
#		define EVENT_NO_RUNTIME_PM	9
#		define EVENT_RX_KILL	10
#		define EVENT_LINK_CHANGE	11
#		define EVENT_SET_RX_MODE	12
#		define EVENT_NO_IP_ALIGN	13
	u32			rx_speed;	
	u32			tx_speed;	
};

static inline struct usb_driver *driver_of(struct usb_interface *intf)
{
	return to_usb_driver(intf->dev.driver);
}


struct driver_info {
	char		*description;

	int		flags;

#define FLAG_FRAMING_NC	0x0001		
#define FLAG_FRAMING_GL	0x0002		
#define FLAG_FRAMING_Z	0x0004		
#define FLAG_FRAMING_RN	0x0008		

#define FLAG_NO_SETINT	0x0010		
#define FLAG_ETHER	0x0020		

#define FLAG_FRAMING_AX 0x0040		
#define FLAG_WLAN	0x0080		
#define FLAG_AVOID_UNLINK_URBS 0x0100	
#define FLAG_SEND_ZLP	0x0200		
#define FLAG_WWAN	0x0400		

#define FLAG_LINK_INTR	0x0800		

#define FLAG_POINTTOPOINT 0x1000	


#define FLAG_MULTI_PACKET	0x2000
#define FLAG_RX_ASSEMBLE	0x4000	
#define FLAG_NOARP		0x8000	

	
	int	(*bind)(struct usbnet *, struct usb_interface *);

	
	void	(*unbind)(struct usbnet *, struct usb_interface *);

	
	int	(*reset)(struct usbnet *);

	
	int	(*stop)(struct usbnet *);

	
	int	(*check_connect)(struct usbnet *);

	
	int	(*manage_power)(struct usbnet *, int);

	
	void	(*status)(struct usbnet *, struct urb *);

	
	int	(*link_reset)(struct usbnet *);

	
	int	(*rx_fixup)(struct usbnet *dev, struct sk_buff *skb);

	
	struct sk_buff	*(*tx_fixup)(struct usbnet *dev,
				struct sk_buff *skb, gfp_t flags);

	
	void	(*recover)(struct usbnet *dev);

	
	int	(*early_init)(struct usbnet *dev);

	
	void	(*indication)(struct usbnet *dev, void *ind, int indlen);

	
	void	(*set_rx_mode)(struct usbnet *dev);

	
	int		in;		
	int		out;		

	unsigned long	data;		
};


extern int usbnet_probe(struct usb_interface *, const struct usb_device_id *);
extern int usbnet_suspend(struct usb_interface *, pm_message_t);
extern int usbnet_resume(struct usb_interface *);
extern void usbnet_disconnect(struct usb_interface *);
extern void usbnet_device_suggests_idle(struct usbnet *dev);

extern int usbnet_read_cmd(struct usbnet *dev, u8 cmd, u8 reqtype,
		    u16 value, u16 index, void *data, u16 size);
extern int usbnet_write_cmd(struct usbnet *dev, u8 cmd, u8 reqtype,
		    u16 value, u16 index, const void *data, u16 size);
extern int usbnet_read_cmd_nopm(struct usbnet *dev, u8 cmd, u8 reqtype,
		    u16 value, u16 index, void *data, u16 size);
extern int usbnet_write_cmd_nopm(struct usbnet *dev, u8 cmd, u8 reqtype,
		    u16 value, u16 index, const void *data, u16 size);
extern int usbnet_write_cmd_async(struct usbnet *dev, u8 cmd, u8 reqtype,
		    u16 value, u16 index, const void *data, u16 size);


struct cdc_state {
	struct usb_cdc_header_desc	*header;
	struct usb_cdc_union_desc	*u;
	struct usb_cdc_ether_desc	*ether;
	struct usb_interface		*control;
	struct usb_interface		*data;
};

extern int usbnet_generic_cdc_bind(struct usbnet *, struct usb_interface *);
extern int usbnet_cdc_bind(struct usbnet *, struct usb_interface *);
extern void usbnet_cdc_unbind(struct usbnet *, struct usb_interface *);
extern void usbnet_cdc_status(struct usbnet *, struct urb *);


#define	DEFAULT_FILTER	(USB_CDC_PACKET_TYPE_BROADCAST \
			|USB_CDC_PACKET_TYPE_ALL_MULTICAST \
			|USB_CDC_PACKET_TYPE_PROMISCUOUS \
			|USB_CDC_PACKET_TYPE_DIRECTED)



enum skb_state {
	illegal = 0,
	tx_start, tx_done,
	rx_start, rx_done, rx_cleanup,
	unlink_start
};

struct skb_data {	
	struct urb		*urb;
	struct usbnet		*dev;
	enum skb_state		state;
	long			length;
	unsigned long		packets;
};


static inline void
usbnet_set_skb_tx_stats(struct sk_buff *skb,
			unsigned long packets, long bytes_delta)
{
	struct skb_data *entry = (struct skb_data *) skb->cb;

	entry->packets = packets;
	entry->length = bytes_delta;
}

extern int usbnet_open(struct net_device *net);
extern int usbnet_stop(struct net_device *net);
extern netdev_tx_t usbnet_start_xmit(struct sk_buff *skb,
				     struct net_device *net);
extern void usbnet_tx_timeout(struct net_device *net);
extern int usbnet_change_mtu(struct net_device *net, int new_mtu);

extern int usbnet_get_endpoints(struct usbnet *, struct usb_interface *);
extern int usbnet_get_ethernet_addr(struct usbnet *, int);
extern void usbnet_defer_kevent(struct usbnet *, int);
extern void usbnet_skb_return(struct usbnet *, struct sk_buff *);
extern void usbnet_unlink_rx_urbs(struct usbnet *);

extern void usbnet_pause_rx(struct usbnet *);
extern void usbnet_resume_rx(struct usbnet *);
extern void usbnet_purge_paused_rxq(struct usbnet *);

extern int usbnet_get_settings(struct net_device *net,
			       struct ethtool_cmd *cmd);
extern int usbnet_set_settings(struct net_device *net,
			       struct ethtool_cmd *cmd);
extern u32 usbnet_get_link(struct net_device *net);
extern u32 usbnet_get_msglevel(struct net_device *);
extern void usbnet_set_msglevel(struct net_device *, u32);
extern void usbnet_get_drvinfo(struct net_device *, struct ethtool_drvinfo *);
extern int usbnet_nway_reset(struct net_device *net);

extern int usbnet_manage_power(struct usbnet *, int);
extern void usbnet_link_change(struct usbnet *, bool, bool);

extern int usbnet_status_start(struct usbnet *dev, gfp_t mem_flags);
extern void usbnet_status_stop(struct usbnet *dev);

extern void usbnet_update_max_qlen(struct usbnet *dev);

#endif 
