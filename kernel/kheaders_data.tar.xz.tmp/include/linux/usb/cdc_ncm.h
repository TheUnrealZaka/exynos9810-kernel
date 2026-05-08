

#ifndef __LINUX_USB_CDC_NCM_H
#define __LINUX_USB_CDC_NCM_H

#define CDC_NCM_COMM_ALTSETTING_NCM		0
#define CDC_NCM_COMM_ALTSETTING_MBIM		1

#define CDC_NCM_DATA_ALTSETTING_NCM		1
#define CDC_NCM_DATA_ALTSETTING_MBIM		2


#define USB_CDC_NCM_NDP16_LENGTH_MIN		0x10


#define	CDC_NCM_NTB_MAX_SIZE_TX			32768	
#define	CDC_NCM_NTB_MAX_SIZE_RX			32768	


#define	CDC_NCM_NTB_DEF_SIZE_TX			16384	
#define	CDC_NCM_NTB_DEF_SIZE_RX			16384	


#define	CDC_NCM_MIN_DATAGRAM_SIZE		1514	


#define CDC_MBIM_MIN_DATAGRAM_SIZE		2048	

#define	CDC_NCM_MIN_TX_PKT			512	


#define	CDC_NCM_MAX_DATAGRAM_SIZE		8192	


#define	CDC_NCM_DPT_DATAGRAMS_MAX		40


#define	CDC_NCM_RESTART_TIMER_DATAGRAM_CNT	3
#define	CDC_NCM_TIMER_PENDING_CNT		2
#define CDC_NCM_TIMER_INTERVAL_USEC		400UL
#define CDC_NCM_TIMER_INTERVAL_MIN		5UL
#define CDC_NCM_TIMER_INTERVAL_MAX		(U32_MAX / NSEC_PER_USEC)


#define CDC_NCM_FLAG_NDP_TO_END			0x02	
#define CDC_MBIM_FLAG_AVOID_ALTSETTING_TOGGLE	0x04	
#define CDC_NCM_FLAG_RESET_NTB16 0x08	

#define cdc_ncm_comm_intf_is_mbim(x)  ((x)->desc.bInterfaceSubClass == USB_CDC_SUBCLASS_MBIM && \
				       (x)->desc.bInterfaceProtocol == USB_CDC_PROTO_NONE)
#define cdc_ncm_data_intf_is_mbim(x)  ((x)->desc.bInterfaceProtocol == USB_CDC_MBIM_PROTO_NTB)

struct cdc_ncm_ctx {
	struct usb_cdc_ncm_ntb_parameters ncm_parm;
	struct hrtimer tx_timer;
	struct tasklet_struct bh;

	const struct usb_cdc_ncm_desc *func_desc;
	const struct usb_cdc_mbim_desc *mbim_desc;
	const struct usb_cdc_mbim_extended_desc *mbim_extended_desc;
	const struct usb_cdc_ether_desc *ether_desc;

	struct usb_interface *control;
	struct usb_interface *data;

	struct sk_buff *tx_curr_skb;
	struct sk_buff *tx_rem_skb;
	__le32 tx_rem_sign;

	spinlock_t mtx;
	atomic_t stop;
	int drvflags;

	u32 timer_interval;
	u32 max_ndp_size;
	struct usb_cdc_ncm_ndp16 *delayed_ndp16;

	u32 tx_timer_pending;
	u32 tx_curr_frame_num;
	u32 rx_max;
	u32 tx_max;
	u32 max_datagram_size;
	u16 tx_max_datagrams;
	u16 tx_remainder;
	u16 tx_modulus;
	u16 tx_ndp_modulus;
	u16 tx_seq;
	u16 rx_seq;
	u16 min_tx_pkt;

	
	u32 tx_curr_frame_payload;
	u32 tx_reason_ntb_full;
	u32 tx_reason_ndp_full;
	u32 tx_reason_timeout;
	u32 tx_reason_max_datagram;
	u64 tx_overhead;
	u64 tx_ntbs;
	u64 rx_overhead;
	u64 rx_ntbs;
};

u8 cdc_ncm_select_altsetting(struct usb_interface *intf);
int cdc_ncm_change_mtu(struct net_device *net, int new_mtu);
int cdc_ncm_bind_common(struct usbnet *dev, struct usb_interface *intf, u8 data_altsetting, int drvflags);
void cdc_ncm_unbind(struct usbnet *dev, struct usb_interface *intf);
struct sk_buff *cdc_ncm_fill_tx_frame(struct usbnet *dev, struct sk_buff *skb, __le32 sign);
int cdc_ncm_rx_verify_nth16(struct cdc_ncm_ctx *ctx, struct sk_buff *skb_in);
int cdc_ncm_rx_verify_ndp16(struct sk_buff *skb_in, int ndpoffset);
struct sk_buff *
cdc_ncm_tx_fixup(struct usbnet *dev, struct sk_buff *skb, gfp_t flags);
int cdc_ncm_rx_fixup(struct usbnet *dev, struct sk_buff *skb_in);

#endif 
