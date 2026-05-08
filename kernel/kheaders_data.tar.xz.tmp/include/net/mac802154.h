
#ifndef NET_MAC802154_H
#define NET_MAC802154_H

#include <asm/unaligned.h>
#include <net/af_ieee802154.h>
#include <linux/ieee802154.h>
#include <linux/skbuff.h>

#include <net/cfg802154.h>


enum ieee802154_hw_addr_filt_flags {
	IEEE802154_AFILT_SADDR_CHANGED		= BIT(0),
	IEEE802154_AFILT_IEEEADDR_CHANGED	= BIT(1),
	IEEE802154_AFILT_PANID_CHANGED		= BIT(2),
	IEEE802154_AFILT_PANC_CHANGED		= BIT(3),
};


struct ieee802154_hw_addr_filt {
	__le16	pan_id;
	__le16	short_addr;
	__le64	ieee_addr;
	bool	pan_coord;
};


struct ieee802154_hw {
	
	int	extra_tx_headroom;
	u32	flags;
	struct	device *parent;
	void	*priv;

	
	struct	wpan_phy *phy;
};


enum ieee802154_hw_flags {
	IEEE802154_HW_TX_OMIT_CKSUM	= BIT(0),
	IEEE802154_HW_LBT		= BIT(1),
	IEEE802154_HW_CSMA_PARAMS	= BIT(2),
	IEEE802154_HW_FRAME_RETRIES	= BIT(3),
	IEEE802154_HW_AFILT		= BIT(4),
	IEEE802154_HW_PROMISCUOUS	= BIT(5),
	IEEE802154_HW_RX_OMIT_CKSUM	= BIT(6),
	IEEE802154_HW_RX_DROP_BAD_CKSUM	= BIT(7),
};


#define IEEE802154_HW_OMIT_CKSUM	(IEEE802154_HW_TX_OMIT_CKSUM | \
					 IEEE802154_HW_RX_OMIT_CKSUM)


struct ieee802154_ops {
	struct module	*owner;
	int		(*start)(struct ieee802154_hw *hw);
	void		(*stop)(struct ieee802154_hw *hw);
	int		(*xmit_sync)(struct ieee802154_hw *hw,
				     struct sk_buff *skb);
	int		(*xmit_async)(struct ieee802154_hw *hw,
				      struct sk_buff *skb);
	int		(*ed)(struct ieee802154_hw *hw, u8 *level);
	int		(*set_channel)(struct ieee802154_hw *hw, u8 page,
				       u8 channel);
	int		(*set_hw_addr_filt)(struct ieee802154_hw *hw,
					    struct ieee802154_hw_addr_filt *filt,
					    unsigned long changed);
	int		(*set_txpower)(struct ieee802154_hw *hw, s32 mbm);
	int		(*set_lbt)(struct ieee802154_hw *hw, bool on);
	int		(*set_cca_mode)(struct ieee802154_hw *hw,
					const struct wpan_phy_cca *cca);
	int		(*set_cca_ed_level)(struct ieee802154_hw *hw, s32 mbm);
	int		(*set_csma_params)(struct ieee802154_hw *hw,
					   u8 min_be, u8 max_be, u8 retries);
	int		(*set_frame_retries)(struct ieee802154_hw *hw,
					     s8 retries);
	int             (*set_promiscuous_mode)(struct ieee802154_hw *hw,
						const bool on);
};


static inline __le16 ieee802154_get_fc_from_skb(const struct sk_buff *skb)
{
	__le16 fc;

	
	if (WARN_ON(!skb_mac_header_was_set(skb) ||
		    (skb_tail_pointer(skb) -
		     skb_mac_header(skb)) < IEEE802154_FC_LEN))
		return cpu_to_le16(0);

	memcpy(&fc, skb_mac_header(skb), IEEE802154_FC_LEN);
	return fc;
}


static inline unsigned char *ieee802154_skb_dst_pan(__le16 fc,
						    const struct sk_buff *skb)
{
	unsigned char *dst_pan;

	switch (ieee802154_daddr_mode(fc)) {
	case cpu_to_le16(IEEE802154_FCTL_ADDR_NONE):
		dst_pan = NULL;
		break;
	case cpu_to_le16(IEEE802154_FCTL_DADDR_SHORT):
	case cpu_to_le16(IEEE802154_FCTL_DADDR_EXTENDED):
		dst_pan = skb_mac_header(skb) +
			  IEEE802154_FC_LEN +
			  IEEE802154_SEQ_LEN;
		break;
	default:
		WARN_ONCE(1, "invalid addr mode detected");
		dst_pan = NULL;
		break;
	}

	return dst_pan;
}


static inline unsigned char *ieee802154_skb_src_pan(__le16 fc,
						    const struct sk_buff *skb)
{
	unsigned char *src_pan;

	switch (ieee802154_saddr_mode(fc)) {
	case cpu_to_le16(IEEE802154_FCTL_ADDR_NONE):
		src_pan = NULL;
		break;
	case cpu_to_le16(IEEE802154_FCTL_SADDR_SHORT):
	case cpu_to_le16(IEEE802154_FCTL_SADDR_EXTENDED):
		
		if (ieee802154_is_intra_pan(fc)) {
			src_pan = ieee802154_skb_dst_pan(fc, skb);
			break;
		}

		switch (ieee802154_daddr_mode(fc)) {
		case cpu_to_le16(IEEE802154_FCTL_ADDR_NONE):
			src_pan = skb_mac_header(skb) +
				  IEEE802154_FC_LEN +
				  IEEE802154_SEQ_LEN;
			break;
		case cpu_to_le16(IEEE802154_FCTL_DADDR_SHORT):
			src_pan = skb_mac_header(skb) +
				  IEEE802154_FC_LEN +
				  IEEE802154_SEQ_LEN +
				  IEEE802154_PAN_ID_LEN +
				  IEEE802154_SHORT_ADDR_LEN;
			break;
		case cpu_to_le16(IEEE802154_FCTL_DADDR_EXTENDED):
			src_pan = skb_mac_header(skb) +
				  IEEE802154_FC_LEN +
				  IEEE802154_SEQ_LEN +
				  IEEE802154_PAN_ID_LEN +
				  IEEE802154_EXTENDED_ADDR_LEN;
			break;
		default:
			WARN_ONCE(1, "invalid addr mode detected");
			src_pan = NULL;
			break;
		}
		break;
	default:
		WARN_ONCE(1, "invalid addr mode detected");
		src_pan = NULL;
		break;
	}

	return src_pan;
}


static inline bool ieee802154_skb_is_intra_pan_addressing(__le16 fc,
							  const struct sk_buff *skb)
{
	unsigned char *dst_pan = ieee802154_skb_dst_pan(fc, skb),
		      *src_pan = ieee802154_skb_src_pan(fc, skb);

	
	if (!dst_pan || !src_pan)
		return false;

	return !memcmp(dst_pan, src_pan, IEEE802154_PAN_ID_LEN);
}


static inline void ieee802154_be64_to_le64(void *le64_dst, const void *be64_src)
{
	put_unaligned_le64(get_unaligned_be64(be64_src), le64_dst);
}


static inline void ieee802154_le64_to_be64(void *be64_dst, const void *le64_src)
{
	put_unaligned_be64(get_unaligned_le64(le64_src), be64_dst);
}


static inline void ieee802154_le16_to_be16(void *be16_dst, const void *le16_src)
{
	put_unaligned_be16(get_unaligned_le16(le16_src), be16_dst);
}


static inline void ieee802154_be16_to_le16(void *le16_dst, const void *be16_src)
{
	put_unaligned_le16(get_unaligned_be16(be16_src), le16_dst);
}


struct ieee802154_hw *
ieee802154_alloc_hw(size_t priv_data_len, const struct ieee802154_ops *ops);


void ieee802154_free_hw(struct ieee802154_hw *hw);


int ieee802154_register_hw(struct ieee802154_hw *hw);


void ieee802154_unregister_hw(struct ieee802154_hw *hw);


void ieee802154_rx_irqsafe(struct ieee802154_hw *hw, struct sk_buff *skb,
			   u8 lqi);

void ieee802154_wake_queue(struct ieee802154_hw *hw);


void ieee802154_stop_queue(struct ieee802154_hw *hw);


void ieee802154_xmit_complete(struct ieee802154_hw *hw, struct sk_buff *skb,
			      bool ifs_handling);

#endif 
