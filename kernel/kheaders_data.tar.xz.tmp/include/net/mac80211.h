

#ifndef MAC80211_H
#define MAC80211_H

#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/if_ether.h>
#include <linux/skbuff.h>
#include <linux/ieee80211.h>
#include <net/cfg80211.h>
#include <net/codel.h>
#include <asm/unaligned.h>













struct device;


enum ieee80211_max_queues {
	IEEE80211_MAX_QUEUES =		16,
	IEEE80211_MAX_QUEUE_MAP =	BIT(IEEE80211_MAX_QUEUES) - 1,
};

#define IEEE80211_INVAL_HW_QUEUE	0xff


enum ieee80211_ac_numbers {
	IEEE80211_AC_VO		= 0,
	IEEE80211_AC_VI		= 1,
	IEEE80211_AC_BE		= 2,
	IEEE80211_AC_BK		= 3,
};
#define IEEE80211_NUM_ACS	4


struct ieee80211_tx_queue_params {
	u16 txop;
	u16 cw_min;
	u16 cw_max;
	u8 aifs;
	bool acm;
	bool uapsd;
};

struct ieee80211_low_level_stats {
	unsigned int dot11ACKFailureCount;
	unsigned int dot11RTSFailureCount;
	unsigned int dot11FCSErrorCount;
	unsigned int dot11RTSSuccessCount;
};


enum ieee80211_chanctx_change {
	IEEE80211_CHANCTX_CHANGE_WIDTH		= BIT(0),
	IEEE80211_CHANCTX_CHANGE_RX_CHAINS	= BIT(1),
	IEEE80211_CHANCTX_CHANGE_RADAR		= BIT(2),
	IEEE80211_CHANCTX_CHANGE_CHANNEL	= BIT(3),
	IEEE80211_CHANCTX_CHANGE_MIN_WIDTH	= BIT(4),
};


struct ieee80211_chanctx_conf {
	struct cfg80211_chan_def def;
	struct cfg80211_chan_def min_def;

	u8 rx_chains_static, rx_chains_dynamic;

	bool radar_enabled;

	u8 drv_priv[0] __aligned(sizeof(void *));
};


enum ieee80211_chanctx_switch_mode {
	CHANCTX_SWMODE_REASSIGN_VIF,
	CHANCTX_SWMODE_SWAP_CONTEXTS,
};


struct ieee80211_vif_chanctx_switch {
	struct ieee80211_vif *vif;
	struct ieee80211_chanctx_conf *old_ctx;
	struct ieee80211_chanctx_conf *new_ctx;
};


enum ieee80211_bss_change {
	BSS_CHANGED_ASSOC		= 1<<0,
	BSS_CHANGED_ERP_CTS_PROT	= 1<<1,
	BSS_CHANGED_ERP_PREAMBLE	= 1<<2,
	BSS_CHANGED_ERP_SLOT		= 1<<3,
	BSS_CHANGED_HT			= 1<<4,
	BSS_CHANGED_BASIC_RATES		= 1<<5,
	BSS_CHANGED_BEACON_INT		= 1<<6,
	BSS_CHANGED_BSSID		= 1<<7,
	BSS_CHANGED_BEACON		= 1<<8,
	BSS_CHANGED_BEACON_ENABLED	= 1<<9,
	BSS_CHANGED_CQM			= 1<<10,
	BSS_CHANGED_IBSS		= 1<<11,
	BSS_CHANGED_ARP_FILTER		= 1<<12,
	BSS_CHANGED_QOS			= 1<<13,
	BSS_CHANGED_IDLE		= 1<<14,
	BSS_CHANGED_SSID		= 1<<15,
	BSS_CHANGED_AP_PROBE_RESP	= 1<<16,
	BSS_CHANGED_PS			= 1<<17,
	BSS_CHANGED_TXPOWER		= 1<<18,
	BSS_CHANGED_P2P_PS		= 1<<19,
	BSS_CHANGED_BEACON_INFO		= 1<<20,
	BSS_CHANGED_BANDWIDTH		= 1<<21,
	BSS_CHANGED_OCB                 = 1<<22,
	BSS_CHANGED_MU_GROUPS		= 1<<23,

	
};


#define IEEE80211_BSS_ARP_ADDR_LIST_LEN 4


enum ieee80211_event_type {
	RSSI_EVENT,
	MLME_EVENT,
	BAR_RX_EVENT,
	BA_FRAME_TIMEOUT,
};


enum ieee80211_rssi_event_data {
	RSSI_EVENT_HIGH,
	RSSI_EVENT_LOW,
};


struct ieee80211_rssi_event {
	enum ieee80211_rssi_event_data data;
};


enum ieee80211_mlme_event_data {
	AUTH_EVENT,
	ASSOC_EVENT,
	DEAUTH_RX_EVENT,
	DEAUTH_TX_EVENT,
};


enum ieee80211_mlme_event_status {
	MLME_SUCCESS,
	MLME_DENIED,
	MLME_TIMEOUT,
};


struct ieee80211_mlme_event {
	enum ieee80211_mlme_event_data data;
	enum ieee80211_mlme_event_status status;
	u16 reason;
};


struct ieee80211_ba_event {
	struct ieee80211_sta *sta;
	u16 tid;
	u16 ssn;
};


struct ieee80211_event {
	enum ieee80211_event_type type;
	union {
		struct ieee80211_rssi_event rssi;
		struct ieee80211_mlme_event mlme;
		struct ieee80211_ba_event ba;
	} u;
};


struct ieee80211_mu_group_data {
	u8 membership[WLAN_MEMBERSHIP_LEN];
	u8 position[WLAN_USER_POSITION_LEN];
};


struct ieee80211_bss_conf {
	const u8 *bssid;
	
	bool assoc, ibss_joined;
	bool ibss_creator;
	u16 aid;
	
	bool use_cts_prot;
	bool use_short_preamble;
	bool use_short_slot;
	bool enable_beacon;
	u8 dtim_period;
	u16 beacon_int;
	u16 assoc_capability;
	u64 sync_tsf;
	u32 sync_device_ts;
	u8 sync_dtim_count;
	u32 basic_rates;
	struct ieee80211_rate *beacon_rate;
	int mcast_rate[NUM_NL80211_BANDS];
	u16 ht_operation_mode;
	s32 cqm_rssi_thold;
	u32 cqm_rssi_hyst;
	struct cfg80211_chan_def chandef;
	struct ieee80211_mu_group_data mu_group;
	__be32 arp_addr_list[IEEE80211_BSS_ARP_ADDR_LIST_LEN];
	int arp_addr_cnt;
	bool qos;
	bool idle;
	bool ps;
	u8 ssid[IEEE80211_MAX_SSID_LEN];
	size_t ssid_len;
	bool hidden_ssid;
	int txpower;
	enum nl80211_tx_power_setting txpower_type;
	struct ieee80211_p2p_noa_attr p2p_noa_attr;
	bool allow_p2p_go_ps;
};


enum mac80211_tx_info_flags {
	IEEE80211_TX_CTL_REQ_TX_STATUS		= BIT(0),
	IEEE80211_TX_CTL_ASSIGN_SEQ		= BIT(1),
	IEEE80211_TX_CTL_NO_ACK			= BIT(2),
	IEEE80211_TX_CTL_CLEAR_PS_FILT		= BIT(3),
	IEEE80211_TX_CTL_FIRST_FRAGMENT		= BIT(4),
	IEEE80211_TX_CTL_SEND_AFTER_DTIM	= BIT(5),
	IEEE80211_TX_CTL_AMPDU			= BIT(6),
	IEEE80211_TX_CTL_INJECTED		= BIT(7),
	IEEE80211_TX_STAT_TX_FILTERED		= BIT(8),
	IEEE80211_TX_STAT_ACK			= BIT(9),
	IEEE80211_TX_STAT_AMPDU			= BIT(10),
	IEEE80211_TX_STAT_AMPDU_NO_BACK		= BIT(11),
	IEEE80211_TX_CTL_RATE_CTRL_PROBE	= BIT(12),
	IEEE80211_TX_INTFL_OFFCHAN_TX_OK	= BIT(13),
	IEEE80211_TX_INTFL_NEED_TXPROCESSING	= BIT(14),
	IEEE80211_TX_INTFL_RETRIED		= BIT(15),
	IEEE80211_TX_INTFL_DONT_ENCRYPT		= BIT(16),
	IEEE80211_TX_CTL_NO_PS_BUFFER		= BIT(17),
	IEEE80211_TX_CTL_MORE_FRAMES		= BIT(18),
	IEEE80211_TX_INTFL_RETRANSMISSION	= BIT(19),
	IEEE80211_TX_INTFL_MLME_CONN_TX		= BIT(20),
	IEEE80211_TX_INTFL_NL80211_FRAME_TX	= BIT(21),
	IEEE80211_TX_CTL_LDPC			= BIT(22),
	IEEE80211_TX_CTL_STBC			= BIT(23) | BIT(24),
	IEEE80211_TX_CTL_TX_OFFCHAN		= BIT(25),
	IEEE80211_TX_INTFL_TKIP_MIC_FAILURE	= BIT(26),
	IEEE80211_TX_CTL_NO_CCK_RATE		= BIT(27),
	IEEE80211_TX_STATUS_EOSP		= BIT(28),
	IEEE80211_TX_CTL_USE_MINRATE		= BIT(29),
	IEEE80211_TX_CTL_DONTFRAG		= BIT(30),
	IEEE80211_TX_STAT_NOACK_TRANSMITTED	= BIT(31),
};

#define IEEE80211_TX_CTL_STBC_SHIFT		23


enum mac80211_tx_control_flags {
	IEEE80211_TX_CTRL_PORT_CTRL_PROTO	= BIT(0),
	IEEE80211_TX_CTRL_PS_RESPONSE		= BIT(1),
	IEEE80211_TX_CTRL_RATE_INJECT		= BIT(2),
	IEEE80211_TX_CTRL_AMSDU			= BIT(3),
	IEEE80211_TX_CTRL_FAST_XMIT		= BIT(4),
};


#define IEEE80211_TX_TEMPORARY_FLAGS (IEEE80211_TX_CTL_NO_ACK |		      \
	IEEE80211_TX_CTL_CLEAR_PS_FILT | IEEE80211_TX_CTL_FIRST_FRAGMENT |    \
	IEEE80211_TX_CTL_SEND_AFTER_DTIM | IEEE80211_TX_CTL_AMPDU |	      \
	IEEE80211_TX_STAT_TX_FILTERED |	IEEE80211_TX_STAT_ACK |		      \
	IEEE80211_TX_STAT_AMPDU | IEEE80211_TX_STAT_AMPDU_NO_BACK |	      \
	IEEE80211_TX_CTL_RATE_CTRL_PROBE | IEEE80211_TX_CTL_NO_PS_BUFFER |    \
	IEEE80211_TX_CTL_MORE_FRAMES | IEEE80211_TX_CTL_LDPC |		      \
	IEEE80211_TX_CTL_STBC | IEEE80211_TX_STATUS_EOSP)


enum mac80211_rate_control_flags {
	IEEE80211_TX_RC_USE_RTS_CTS		= BIT(0),
	IEEE80211_TX_RC_USE_CTS_PROTECT		= BIT(1),
	IEEE80211_TX_RC_USE_SHORT_PREAMBLE	= BIT(2),

	
	IEEE80211_TX_RC_MCS			= BIT(3),
	IEEE80211_TX_RC_GREEN_FIELD		= BIT(4),
	IEEE80211_TX_RC_40_MHZ_WIDTH		= BIT(5),
	IEEE80211_TX_RC_DUP_DATA		= BIT(6),
	IEEE80211_TX_RC_SHORT_GI		= BIT(7),
	IEEE80211_TX_RC_VHT_MCS			= BIT(8),
	IEEE80211_TX_RC_80_MHZ_WIDTH		= BIT(9),
	IEEE80211_TX_RC_160_MHZ_WIDTH		= BIT(10),
};



#define IEEE80211_TX_INFO_DRIVER_DATA_SIZE 40


#define IEEE80211_TX_INFO_RATE_DRIVER_DATA_SIZE 24


#define IEEE80211_TX_MAX_RATES	4


#define IEEE80211_TX_RATE_TABLE_SIZE	4


struct ieee80211_tx_rate {
	s8 idx;
	u16 count:5,
	    flags:11;
} __packed;

#define IEEE80211_MAX_TX_RETRY		31

static inline void ieee80211_rate_set_vht(struct ieee80211_tx_rate *rate,
					  u8 mcs, u8 nss)
{
	WARN_ON(mcs & ~0xF);
	WARN_ON((nss - 1) & ~0x7);
	rate->idx = ((nss - 1) << 4) | mcs;
}

static inline u8
ieee80211_rate_get_vht_mcs(const struct ieee80211_tx_rate *rate)
{
	return rate->idx & 0xF;
}

static inline u8
ieee80211_rate_get_vht_nss(const struct ieee80211_tx_rate *rate)
{
	return (rate->idx >> 4) + 1;
}


struct ieee80211_tx_info {
	
	u32 flags;
	u8 band;

	u8 hw_queue;

	u16 ack_frame_id;

	union {
		struct {
			union {
				
				struct {
					struct ieee80211_tx_rate rates[
						IEEE80211_TX_MAX_RATES];
					s8 rts_cts_rate_idx;
					u8 use_rts:1;
					u8 use_cts_prot:1;
					u8 short_preamble:1;
					u8 skip_table:1;
					
				};
				
				unsigned long jiffies;
			};
			
			struct ieee80211_vif *vif;
			struct ieee80211_key_conf *hw_key;
			u32 flags;
			codel_time_t enqueue_time;
		} control;
		struct {
			u64 cookie;
		} ack;
		struct {
			struct ieee80211_tx_rate rates[IEEE80211_TX_MAX_RATES];
			s32 ack_signal;
			u8 ampdu_ack_len;
			u8 ampdu_len;
			u8 antenna;
			u16 tx_time;
			void *status_driver_data[19 / sizeof(void *)];
		} status;
		struct {
			struct ieee80211_tx_rate driver_rates[
				IEEE80211_TX_MAX_RATES];
			u8 pad[4];

			void *rate_driver_data[
				IEEE80211_TX_INFO_RATE_DRIVER_DATA_SIZE / sizeof(void *)];
		};
		void *driver_data[
			IEEE80211_TX_INFO_DRIVER_DATA_SIZE / sizeof(void *)];
	};
};


struct ieee80211_scan_ies {
	const u8 *ies[NUM_NL80211_BANDS];
	size_t len[NUM_NL80211_BANDS];
	const u8 *common_ies;
	size_t common_ie_len;
};


static inline struct ieee80211_tx_info *IEEE80211_SKB_CB(struct sk_buff *skb)
{
	return (struct ieee80211_tx_info *)skb->cb;
}

static inline struct ieee80211_rx_status *IEEE80211_SKB_RXCB(struct sk_buff *skb)
{
	return (struct ieee80211_rx_status *)skb->cb;
}


static inline void
ieee80211_tx_info_clear_status(struct ieee80211_tx_info *info)
{
	int i;

	BUILD_BUG_ON(offsetof(struct ieee80211_tx_info, status.rates) !=
		     offsetof(struct ieee80211_tx_info, control.rates));
	BUILD_BUG_ON(offsetof(struct ieee80211_tx_info, status.rates) !=
		     offsetof(struct ieee80211_tx_info, driver_rates));
	BUILD_BUG_ON(offsetof(struct ieee80211_tx_info, status.rates) != 8);
	
	for (i = 0; i < IEEE80211_TX_MAX_RATES; i++)
		info->status.rates[i].count = 0;

	BUILD_BUG_ON(
	    offsetof(struct ieee80211_tx_info, status.ack_signal) != 20);
	memset(&info->status.ampdu_ack_len, 0,
	       sizeof(struct ieee80211_tx_info) -
	       offsetof(struct ieee80211_tx_info, status.ampdu_ack_len));
}



enum mac80211_rx_flags {
	RX_FLAG_MMIC_ERROR		= BIT(0),
	RX_FLAG_DECRYPTED		= BIT(1),
	RX_FLAG_MACTIME_PLCP_START	= BIT(2),
	RX_FLAG_MMIC_STRIPPED		= BIT(3),
	RX_FLAG_IV_STRIPPED		= BIT(4),
	RX_FLAG_FAILED_FCS_CRC		= BIT(5),
	RX_FLAG_FAILED_PLCP_CRC 	= BIT(6),
	RX_FLAG_MACTIME_START		= BIT(7),
	RX_FLAG_SHORTPRE		= BIT(8),
	RX_FLAG_HT			= BIT(9),
	RX_FLAG_40MHZ			= BIT(10),
	RX_FLAG_SHORT_GI		= BIT(11),
	RX_FLAG_NO_SIGNAL_VAL		= BIT(12),
	RX_FLAG_HT_GF			= BIT(13),
	RX_FLAG_AMPDU_DETAILS		= BIT(14),
	RX_FLAG_PN_VALIDATED		= BIT(15),
	RX_FLAG_DUP_VALIDATED		= BIT(16),
	RX_FLAG_AMPDU_LAST_KNOWN	= BIT(17),
	RX_FLAG_AMPDU_IS_LAST		= BIT(18),
	RX_FLAG_AMPDU_DELIM_CRC_ERROR	= BIT(19),
	RX_FLAG_AMPDU_DELIM_CRC_KNOWN	= BIT(20),
	RX_FLAG_MACTIME_END		= BIT(21),
	RX_FLAG_VHT			= BIT(22),
	RX_FLAG_LDPC			= BIT(23),
	RX_FLAG_ONLY_MONITOR		= BIT(24),
	RX_FLAG_SKIP_MONITOR		= BIT(25),
	RX_FLAG_STBC_MASK		= BIT(26) | BIT(27),
	RX_FLAG_10MHZ			= BIT(28),
	RX_FLAG_5MHZ			= BIT(29),
	RX_FLAG_AMSDU_MORE		= BIT(30),
	RX_FLAG_RADIOTAP_VENDOR_DATA	= BIT(31),
	RX_FLAG_MIC_STRIPPED		= BIT_ULL(32),
	RX_FLAG_ALLOW_SAME_PN		= BIT_ULL(33),
	RX_FLAG_ICV_STRIPPED		= BIT_ULL(34),
};

#define RX_FLAG_STBC_SHIFT		26



enum mac80211_rx_vht_flags {
	RX_VHT_FLAG_80MHZ		= BIT(0),
	RX_VHT_FLAG_160MHZ		= BIT(1),
	RX_VHT_FLAG_BF			= BIT(2),
};


struct ieee80211_rx_status {
	u64 mactime;
	u64 boottime_ns;
	u32 device_timestamp;
	u32 ampdu_reference;
	u64 flag;
	u16 freq;
	u8 vht_flag;
	u8 rate_idx;
	u8 vht_nss;
	u8 rx_flags;
	u8 band;
	u8 antenna;
	s8 signal;
	u8 chains;
	s8 chain_signal[IEEE80211_MAX_CHAINS];
	u8 ampdu_delimiter_crc;
};


struct ieee80211_vendor_radiotap {
	u32 present;
	u8 align;
	u8 oui[3];
	u8 subns;
	u8 pad;
	u16 len;
	u8 data[];
} __packed;


enum ieee80211_conf_flags {
	IEEE80211_CONF_MONITOR		= (1<<0),
	IEEE80211_CONF_PS		= (1<<1),
	IEEE80211_CONF_IDLE		= (1<<2),
	IEEE80211_CONF_OFFCHANNEL	= (1<<3),
};



enum ieee80211_conf_changed {
	IEEE80211_CONF_CHANGE_SMPS		= BIT(1),
	IEEE80211_CONF_CHANGE_LISTEN_INTERVAL	= BIT(2),
	IEEE80211_CONF_CHANGE_MONITOR		= BIT(3),
	IEEE80211_CONF_CHANGE_PS		= BIT(4),
	IEEE80211_CONF_CHANGE_POWER		= BIT(5),
	IEEE80211_CONF_CHANGE_CHANNEL		= BIT(6),
	IEEE80211_CONF_CHANGE_RETRY_LIMITS	= BIT(7),
	IEEE80211_CONF_CHANGE_IDLE		= BIT(8),
};


enum ieee80211_smps_mode {
	IEEE80211_SMPS_AUTOMATIC,
	IEEE80211_SMPS_OFF,
	IEEE80211_SMPS_STATIC,
	IEEE80211_SMPS_DYNAMIC,

	
	IEEE80211_SMPS_NUM_MODES,
};


struct ieee80211_conf {
	u32 flags;
	int power_level, dynamic_ps_timeout;

	u16 listen_interval;
	u8 ps_dtim_period;

	u8 long_frame_max_tx_count, short_frame_max_tx_count;

	struct cfg80211_chan_def chandef;
	bool radar_enabled;
	enum ieee80211_smps_mode smps_mode;
};


struct ieee80211_channel_switch {
	u64 timestamp;
	u32 device_timestamp;
	bool block_tx;
	struct cfg80211_chan_def chandef;
	u8 count;
};


enum ieee80211_vif_flags {
	IEEE80211_VIF_BEACON_FILTER		= BIT(0),
	IEEE80211_VIF_SUPPORTS_CQM_RSSI		= BIT(1),
	IEEE80211_VIF_SUPPORTS_UAPSD		= BIT(2),
	IEEE80211_VIF_GET_NOA_UPDATE		= BIT(3),
};


struct ieee80211_vif {
	enum nl80211_iftype type;
	struct ieee80211_bss_conf bss_conf;
	u8 addr[ETH_ALEN];
	bool p2p;
	bool csa_active;
	bool mu_mimo_owner;

	u8 cab_queue;
	u8 hw_queue[IEEE80211_NUM_ACS];

	struct ieee80211_txq *txq;

	struct ieee80211_chanctx_conf __rcu *chanctx_conf;

	u32 driver_flags;

#ifdef CONFIG_MAC80211_DEBUGFS
	struct dentry *debugfs_dir;
#endif

	unsigned int probe_req_reg;

	
	u8 drv_priv[0] __aligned(sizeof(void *));
};

static inline bool ieee80211_vif_is_mesh(struct ieee80211_vif *vif)
{
#ifdef CONFIG_MAC80211_MESH
	return vif->type == NL80211_IFTYPE_MESH_POINT;
#endif
	return false;
}


struct ieee80211_vif *wdev_to_ieee80211_vif(struct wireless_dev *wdev);


struct wireless_dev *ieee80211_vif_to_wdev(struct ieee80211_vif *vif);


enum ieee80211_key_flags {
	IEEE80211_KEY_FLAG_GENERATE_IV_MGMT	= BIT(0),
	IEEE80211_KEY_FLAG_GENERATE_IV		= BIT(1),
	IEEE80211_KEY_FLAG_GENERATE_MMIC	= BIT(2),
	IEEE80211_KEY_FLAG_PAIRWISE		= BIT(3),
	IEEE80211_KEY_FLAG_SW_MGMT_TX		= BIT(4),
	IEEE80211_KEY_FLAG_PUT_IV_SPACE		= BIT(5),
	IEEE80211_KEY_FLAG_RX_MGMT		= BIT(6),
	IEEE80211_KEY_FLAG_RESERVE_TAILROOM	= BIT(7),
};


struct ieee80211_key_conf {
	atomic64_t tx_pn;
	u32 cipher;
	u8 icv_len;
	u8 iv_len;
	u8 hw_key_idx;
	u8 flags;
	s8 keyidx;
	u8 keylen;
	u8 key[0];
};

#define IEEE80211_MAX_PN_LEN	16

#define TKIP_PN_TO_IV16(pn) ((u16)(pn & 0xffff))
#define TKIP_PN_TO_IV32(pn) ((u32)((pn >> 16) & 0xffffffff))


struct ieee80211_key_seq {
	union {
		struct {
			u32 iv32;
			u16 iv16;
		} tkip;
		struct {
			u8 pn[6];
		} ccmp;
		struct {
			u8 pn[6];
		} aes_cmac;
		struct {
			u8 pn[6];
		} aes_gmac;
		struct {
			u8 pn[6];
		} gcmp;
		struct {
			u8 seq[IEEE80211_MAX_PN_LEN];
			u8 seq_len;
		} hw;
	};
};


struct ieee80211_cipher_scheme {
	u32 cipher;
	u16 iftype;
	u8 hdr_len;
	u8 pn_len;
	u8 pn_off;
	u8 key_idx_off;
	u8 key_idx_mask;
	u8 key_idx_shift;
	u8 mic_len;
};


enum set_key_cmd {
	SET_KEY, DISABLE_KEY,
};


enum ieee80211_sta_state {
	
	IEEE80211_STA_NOTEXIST,
	IEEE80211_STA_NONE,
	IEEE80211_STA_AUTH,
	IEEE80211_STA_ASSOC,
	IEEE80211_STA_AUTHORIZED,
};


enum ieee80211_sta_rx_bandwidth {
	IEEE80211_STA_RX_BW_20 = 0,
	IEEE80211_STA_RX_BW_40,
	IEEE80211_STA_RX_BW_80,
	IEEE80211_STA_RX_BW_160,
};


struct ieee80211_sta_rates {
	struct rcu_head rcu_head;
	struct {
		s8 idx;
		u8 count;
		u8 count_cts;
		u8 count_rts;
		u16 flags;
	} rate[IEEE80211_TX_RATE_TABLE_SIZE];
};


struct ieee80211_sta {
	u32 supp_rates[NUM_NL80211_BANDS];
	u8 addr[ETH_ALEN];
	u16 aid;
	struct ieee80211_sta_ht_cap ht_cap;
	struct ieee80211_sta_vht_cap vht_cap;
	u8 max_rx_aggregation_subframes;
	bool wme;
	u8 uapsd_queues;
	u8 max_sp;
	u8 rx_nss;
	enum ieee80211_sta_rx_bandwidth bandwidth;
	enum ieee80211_smps_mode smps_mode;
	struct ieee80211_sta_rates __rcu *rates;
	bool tdls;
	bool tdls_initiator;
	bool mfp;
	u8 max_amsdu_subframes;
	u16 max_amsdu_len;
	bool support_p2p_ps;
	u16 max_rc_amsdu_len;

	struct ieee80211_txq *txq[IEEE80211_NUM_TIDS];

	
	u8 drv_priv[0] __aligned(sizeof(void *));
};


enum sta_notify_cmd {
	STA_NOTIFY_SLEEP, STA_NOTIFY_AWAKE,
};


struct ieee80211_tx_control {
	struct ieee80211_sta *sta;
};


struct ieee80211_txq {
	struct ieee80211_vif *vif;
	struct ieee80211_sta *sta;
	u8 tid;
	u8 ac;

	
	u8 drv_priv[0] __aligned(sizeof(void *));
};


enum ieee80211_hw_flags {
	IEEE80211_HW_HAS_RATE_CONTROL,
	IEEE80211_HW_RX_INCLUDES_FCS,
	IEEE80211_HW_HOST_BROADCAST_PS_BUFFERING,
	IEEE80211_HW_SIGNAL_UNSPEC,
	IEEE80211_HW_SIGNAL_DBM,
	IEEE80211_HW_NEED_DTIM_BEFORE_ASSOC,
	IEEE80211_HW_SPECTRUM_MGMT,
	IEEE80211_HW_AMPDU_AGGREGATION,
	IEEE80211_HW_SUPPORTS_PS,
	IEEE80211_HW_PS_NULLFUNC_STACK,
	IEEE80211_HW_SUPPORTS_DYNAMIC_PS,
	IEEE80211_HW_MFP_CAPABLE,
	IEEE80211_HW_WANT_MONITOR_VIF,
	IEEE80211_HW_NO_AUTO_VIF,
	IEEE80211_HW_SW_CRYPTO_CONTROL,
	IEEE80211_HW_SUPPORT_FAST_XMIT,
	IEEE80211_HW_REPORTS_TX_ACK_STATUS,
	IEEE80211_HW_CONNECTION_MONITOR,
	IEEE80211_HW_QUEUE_CONTROL,
	IEEE80211_HW_SUPPORTS_PER_STA_GTK,
	IEEE80211_HW_AP_LINK_PS,
	IEEE80211_HW_TX_AMPDU_SETUP_IN_HW,
	IEEE80211_HW_SUPPORTS_RC_TABLE,
	IEEE80211_HW_P2P_DEV_ADDR_FOR_INTF,
	IEEE80211_HW_TIMING_BEACON_ONLY,
	IEEE80211_HW_SUPPORTS_HT_CCK_RATES,
	IEEE80211_HW_CHANCTX_STA_CSA,
	IEEE80211_HW_SUPPORTS_CLONED_SKBS,
	IEEE80211_HW_SINGLE_SCAN_ON_ALL_BANDS,
	IEEE80211_HW_TDLS_WIDER_BW,
	IEEE80211_HW_SUPPORTS_AMSDU_IN_AMPDU,
	IEEE80211_HW_BEACON_TX_STATUS,
	IEEE80211_HW_NEEDS_UNIQUE_STA_ADDR,
	IEEE80211_HW_SUPPORTS_REORDERING_BUFFER,
	IEEE80211_HW_USES_RSS,
	IEEE80211_HW_TX_AMSDU,
	IEEE80211_HW_TX_FRAG_LIST,
	IEEE80211_HW_REPORTS_LOW_ACK,

	
	NUM_IEEE80211_HW_FLAGS
};


struct ieee80211_hw {
	struct ieee80211_conf conf;
	struct wiphy *wiphy;
	const char *rate_control_algorithm;
	void *priv;
	unsigned long flags[BITS_TO_LONGS(NUM_IEEE80211_HW_FLAGS)];
	unsigned int extra_tx_headroom;
	unsigned int extra_beacon_tailroom;
	int vif_data_size;
	int sta_data_size;
	int chanctx_data_size;
	int txq_data_size;
	u16 queues;
	u16 max_listen_interval;
	s8 max_signal;
	u8 max_rates;
	u8 max_report_rates;
	u8 max_rate_tries;
	u8 max_rx_aggregation_subframes;
	u8 max_tx_aggregation_subframes;
	u8 max_tx_fragments;
	u8 offchannel_tx_hw_queue;
	u8 radiotap_mcs_details;
	u16 radiotap_vht_details;
	struct {
		int units_pos;
		s16 accuracy;
	} radiotap_timestamp;
	netdev_features_t netdev_features;
	u8 uapsd_queues;
	u8 uapsd_max_sp_len;
	u8 n_cipher_schemes;
	const struct ieee80211_cipher_scheme *cipher_schemes;
	u8 max_nan_de_entries;
};

static inline bool _ieee80211_hw_check(struct ieee80211_hw *hw,
				       enum ieee80211_hw_flags flg)
{
	return test_bit(flg, hw->flags);
}
#define ieee80211_hw_check(hw, flg)	_ieee80211_hw_check(hw, IEEE80211_HW_##flg)

static inline void _ieee80211_hw_set(struct ieee80211_hw *hw,
				     enum ieee80211_hw_flags flg)
{
	return __set_bit(flg, hw->flags);
}
#define ieee80211_hw_set(hw, flg)	_ieee80211_hw_set(hw, IEEE80211_HW_##flg)


struct ieee80211_scan_request {
	struct ieee80211_scan_ies ies;

	
	struct cfg80211_scan_request req;
};


struct ieee80211_tdls_ch_sw_params {
	struct ieee80211_sta *sta;
	struct cfg80211_chan_def *chandef;
	u8 action_code;
	u32 status;
	u32 timestamp;
	u16 switch_time;
	u16 switch_timeout;
	struct sk_buff *tmpl_skb;
	u32 ch_sw_tm_ie;
};


struct ieee80211_hw *wiphy_to_ieee80211_hw(struct wiphy *wiphy);


static inline void SET_IEEE80211_DEV(struct ieee80211_hw *hw, struct device *dev)
{
	set_wiphy_dev(hw->wiphy, dev);
}


static inline void SET_IEEE80211_PERM_ADDR(struct ieee80211_hw *hw, const u8 *addr)
{
	memcpy(hw->wiphy->perm_addr, addr, ETH_ALEN);
}

static inline struct ieee80211_rate *
ieee80211_get_tx_rate(const struct ieee80211_hw *hw,
		      const struct ieee80211_tx_info *c)
{
	if (WARN_ON_ONCE(c->control.rates[0].idx < 0))
		return NULL;
	return &hw->wiphy->bands[c->band]->bitrates[c->control.rates[0].idx];
}

static inline struct ieee80211_rate *
ieee80211_get_rts_cts_rate(const struct ieee80211_hw *hw,
			   const struct ieee80211_tx_info *c)
{
	if (c->control.rts_cts_rate_idx < 0)
		return NULL;
	return &hw->wiphy->bands[c->band]->bitrates[c->control.rts_cts_rate_idx];
}

static inline struct ieee80211_rate *
ieee80211_get_alt_retry_rate(const struct ieee80211_hw *hw,
			     const struct ieee80211_tx_info *c, int idx)
{
	if (c->control.rates[idx + 1].idx < 0)
		return NULL;
	return &hw->wiphy->bands[c->band]->bitrates[c->control.rates[idx + 1].idx];
}


void ieee80211_free_txskb(struct ieee80211_hw *hw, struct sk_buff *skb);
















enum ieee80211_filter_flags {
	FIF_ALLMULTI		= 1<<1,
	FIF_FCSFAIL		= 1<<2,
	FIF_PLCPFAIL		= 1<<3,
	FIF_BCN_PRBRESP_PROMISC	= 1<<4,
	FIF_CONTROL		= 1<<5,
	FIF_OTHER_BSS		= 1<<6,
	FIF_PSPOLL		= 1<<7,
	FIF_PROBE_REQ		= 1<<8,
};


enum ieee80211_ampdu_mlme_action {
	IEEE80211_AMPDU_RX_START,
	IEEE80211_AMPDU_RX_STOP,
	IEEE80211_AMPDU_TX_START,
	IEEE80211_AMPDU_TX_STOP_CONT,
	IEEE80211_AMPDU_TX_STOP_FLUSH,
	IEEE80211_AMPDU_TX_STOP_FLUSH_CONT,
	IEEE80211_AMPDU_TX_OPERATIONAL,
};


struct ieee80211_ampdu_params {
	enum ieee80211_ampdu_mlme_action action;
	struct ieee80211_sta *sta;
	u16 tid;
	u16 ssn;
	u8 buf_size;
	bool amsdu;
	u16 timeout;
};


enum ieee80211_frame_release_type {
	IEEE80211_FRAME_RELEASE_PSPOLL,
	IEEE80211_FRAME_RELEASE_UAPSD,
};


enum ieee80211_rate_control_changed {
	IEEE80211_RC_BW_CHANGED		= BIT(0),
	IEEE80211_RC_SMPS_CHANGED	= BIT(1),
	IEEE80211_RC_SUPP_RATES_CHANGED	= BIT(2),
	IEEE80211_RC_NSS_CHANGED	= BIT(3),
};


enum ieee80211_roc_type {
	IEEE80211_ROC_TYPE_NORMAL = 0,
	IEEE80211_ROC_TYPE_MGMT_TX,
};


enum ieee80211_reconfig_type {
	IEEE80211_RECONFIG_TYPE_RESTART,
	IEEE80211_RECONFIG_TYPE_SUSPEND,
};


struct ieee80211_ops {
	void (*tx)(struct ieee80211_hw *hw,
		   struct ieee80211_tx_control *control,
		   struct sk_buff *skb);
	int (*start)(struct ieee80211_hw *hw);
	void (*stop)(struct ieee80211_hw *hw);
#ifdef CONFIG_PM
	int (*suspend)(struct ieee80211_hw *hw, struct cfg80211_wowlan *wowlan);
	int (*resume)(struct ieee80211_hw *hw);
	void (*set_wakeup)(struct ieee80211_hw *hw, bool enabled);
#endif
	int (*add_interface)(struct ieee80211_hw *hw,
			     struct ieee80211_vif *vif);
	int (*change_interface)(struct ieee80211_hw *hw,
				struct ieee80211_vif *vif,
				enum nl80211_iftype new_type, bool p2p);
	void (*remove_interface)(struct ieee80211_hw *hw,
				 struct ieee80211_vif *vif);
	int (*config)(struct ieee80211_hw *hw, u32 changed);
	void (*bss_info_changed)(struct ieee80211_hw *hw,
				 struct ieee80211_vif *vif,
				 struct ieee80211_bss_conf *info,
				 u32 changed);

	int (*start_ap)(struct ieee80211_hw *hw, struct ieee80211_vif *vif);
	void (*stop_ap)(struct ieee80211_hw *hw, struct ieee80211_vif *vif);

	u64 (*prepare_multicast)(struct ieee80211_hw *hw,
				 struct netdev_hw_addr_list *mc_list);
	void (*configure_filter)(struct ieee80211_hw *hw,
				 unsigned int changed_flags,
				 unsigned int *total_flags,
				 u64 multicast);
	void (*config_iface_filter)(struct ieee80211_hw *hw,
				    struct ieee80211_vif *vif,
				    unsigned int filter_flags,
				    unsigned int changed_flags);
	int (*set_tim)(struct ieee80211_hw *hw, struct ieee80211_sta *sta,
		       bool set);
	int (*set_key)(struct ieee80211_hw *hw, enum set_key_cmd cmd,
		       struct ieee80211_vif *vif, struct ieee80211_sta *sta,
		       struct ieee80211_key_conf *key);
	void (*update_tkip_key)(struct ieee80211_hw *hw,
				struct ieee80211_vif *vif,
				struct ieee80211_key_conf *conf,
				struct ieee80211_sta *sta,
				u32 iv32, u16 *phase1key);
	void (*set_rekey_data)(struct ieee80211_hw *hw,
			       struct ieee80211_vif *vif,
			       struct cfg80211_gtk_rekey_data *data);
	void (*set_default_unicast_key)(struct ieee80211_hw *hw,
					struct ieee80211_vif *vif, int idx);
	int (*hw_scan)(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
		       struct ieee80211_scan_request *req);
	void (*cancel_hw_scan)(struct ieee80211_hw *hw,
			       struct ieee80211_vif *vif);
	int (*sched_scan_start)(struct ieee80211_hw *hw,
				struct ieee80211_vif *vif,
				struct cfg80211_sched_scan_request *req,
				struct ieee80211_scan_ies *ies);
	int (*sched_scan_stop)(struct ieee80211_hw *hw,
			       struct ieee80211_vif *vif);
	void (*sw_scan_start)(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif,
			      const u8 *mac_addr);
	void (*sw_scan_complete)(struct ieee80211_hw *hw,
				 struct ieee80211_vif *vif);
	int (*get_stats)(struct ieee80211_hw *hw,
			 struct ieee80211_low_level_stats *stats);
	void (*get_key_seq)(struct ieee80211_hw *hw,
			    struct ieee80211_key_conf *key,
			    struct ieee80211_key_seq *seq);
	int (*set_frag_threshold)(struct ieee80211_hw *hw, u32 value);
	int (*set_rts_threshold)(struct ieee80211_hw *hw, u32 value);
	int (*sta_add)(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
		       struct ieee80211_sta *sta);
	int (*sta_remove)(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			  struct ieee80211_sta *sta);
#ifdef CONFIG_MAC80211_DEBUGFS
	void (*sta_add_debugfs)(struct ieee80211_hw *hw,
				struct ieee80211_vif *vif,
				struct ieee80211_sta *sta,
				struct dentry *dir);
#endif
	void (*sta_notify)(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			enum sta_notify_cmd, struct ieee80211_sta *sta);
	int (*sta_state)(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			 struct ieee80211_sta *sta,
			 enum ieee80211_sta_state old_state,
			 enum ieee80211_sta_state new_state);
	void (*sta_pre_rcu_remove)(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif,
				   struct ieee80211_sta *sta);
	void (*sta_rc_update)(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif,
			      struct ieee80211_sta *sta,
			      u32 changed);
	void (*sta_rate_tbl_update)(struct ieee80211_hw *hw,
				    struct ieee80211_vif *vif,
				    struct ieee80211_sta *sta);
	void (*sta_statistics)(struct ieee80211_hw *hw,
			       struct ieee80211_vif *vif,
			       struct ieee80211_sta *sta,
			       struct station_info *sinfo);
	int (*conf_tx)(struct ieee80211_hw *hw,
		       struct ieee80211_vif *vif, u16 ac,
		       const struct ieee80211_tx_queue_params *params);
	u64 (*get_tsf)(struct ieee80211_hw *hw, struct ieee80211_vif *vif);
	void (*set_tsf)(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			u64 tsf);
	void (*offset_tsf)(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			   s64 offset);
	void (*reset_tsf)(struct ieee80211_hw *hw, struct ieee80211_vif *vif);
	int (*tx_last_beacon)(struct ieee80211_hw *hw);
	int (*ampdu_action)(struct ieee80211_hw *hw,
			    struct ieee80211_vif *vif,
			    struct ieee80211_ampdu_params *params);
	int (*get_survey)(struct ieee80211_hw *hw, int idx,
		struct survey_info *survey);
	void (*rfkill_poll)(struct ieee80211_hw *hw);
	void (*set_coverage_class)(struct ieee80211_hw *hw, s16 coverage_class);
#ifdef CONFIG_NL80211_TESTMODE
	int (*testmode_cmd)(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			    void *data, int len);
	int (*testmode_dump)(struct ieee80211_hw *hw, struct sk_buff *skb,
			     struct netlink_callback *cb,
			     void *data, int len);
#endif
	void (*flush)(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
		      u32 queues, bool drop);
	void (*channel_switch)(struct ieee80211_hw *hw,
			       struct ieee80211_vif *vif,
			       struct ieee80211_channel_switch *ch_switch);
	int (*set_antenna)(struct ieee80211_hw *hw, u32 tx_ant, u32 rx_ant);
	int (*get_antenna)(struct ieee80211_hw *hw, u32 *tx_ant, u32 *rx_ant);

	int (*remain_on_channel)(struct ieee80211_hw *hw,
				 struct ieee80211_vif *vif,
				 struct ieee80211_channel *chan,
				 int duration,
				 enum ieee80211_roc_type type);
	int (*cancel_remain_on_channel)(struct ieee80211_hw *hw);
	int (*set_ringparam)(struct ieee80211_hw *hw, u32 tx, u32 rx);
	void (*get_ringparam)(struct ieee80211_hw *hw,
			      u32 *tx, u32 *tx_max, u32 *rx, u32 *rx_max);
	bool (*tx_frames_pending)(struct ieee80211_hw *hw);
	int (*set_bitrate_mask)(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
				const struct cfg80211_bitrate_mask *mask);
	void (*event_callback)(struct ieee80211_hw *hw,
			       struct ieee80211_vif *vif,
			       const struct ieee80211_event *event);

	void (*allow_buffered_frames)(struct ieee80211_hw *hw,
				      struct ieee80211_sta *sta,
				      u16 tids, int num_frames,
				      enum ieee80211_frame_release_type reason,
				      bool more_data);
	void (*release_buffered_frames)(struct ieee80211_hw *hw,
					struct ieee80211_sta *sta,
					u16 tids, int num_frames,
					enum ieee80211_frame_release_type reason,
					bool more_data);

	int	(*get_et_sset_count)(struct ieee80211_hw *hw,
				     struct ieee80211_vif *vif, int sset);
	void	(*get_et_stats)(struct ieee80211_hw *hw,
				struct ieee80211_vif *vif,
				struct ethtool_stats *stats, u64 *data);
	void	(*get_et_strings)(struct ieee80211_hw *hw,
				  struct ieee80211_vif *vif,
				  u32 sset, u8 *data);

	void	(*mgd_prepare_tx)(struct ieee80211_hw *hw,
				  struct ieee80211_vif *vif);

	void	(*mgd_protect_tdls_discover)(struct ieee80211_hw *hw,
					     struct ieee80211_vif *vif);

	int (*add_chanctx)(struct ieee80211_hw *hw,
			   struct ieee80211_chanctx_conf *ctx);
	void (*remove_chanctx)(struct ieee80211_hw *hw,
			       struct ieee80211_chanctx_conf *ctx);
	void (*change_chanctx)(struct ieee80211_hw *hw,
			       struct ieee80211_chanctx_conf *ctx,
			       u32 changed);
	int (*assign_vif_chanctx)(struct ieee80211_hw *hw,
				  struct ieee80211_vif *vif,
				  struct ieee80211_chanctx_conf *ctx);
	void (*unassign_vif_chanctx)(struct ieee80211_hw *hw,
				     struct ieee80211_vif *vif,
				     struct ieee80211_chanctx_conf *ctx);
	int (*switch_vif_chanctx)(struct ieee80211_hw *hw,
				  struct ieee80211_vif_chanctx_switch *vifs,
				  int n_vifs,
				  enum ieee80211_chanctx_switch_mode mode);

	void (*reconfig_complete)(struct ieee80211_hw *hw,
				  enum ieee80211_reconfig_type reconfig_type);

#if IS_ENABLED(CONFIG_IPV6)
	void (*ipv6_addr_change)(struct ieee80211_hw *hw,
				 struct ieee80211_vif *vif,
				 struct inet6_dev *idev);
#endif
	void (*channel_switch_beacon)(struct ieee80211_hw *hw,
				      struct ieee80211_vif *vif,
				      struct cfg80211_chan_def *chandef);
	int (*pre_channel_switch)(struct ieee80211_hw *hw,
				  struct ieee80211_vif *vif,
				  struct ieee80211_channel_switch *ch_switch);

	int (*post_channel_switch)(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif);

	int (*join_ibss)(struct ieee80211_hw *hw, struct ieee80211_vif *vif);
	void (*leave_ibss)(struct ieee80211_hw *hw, struct ieee80211_vif *vif);
	u32 (*get_expected_throughput)(struct ieee80211_hw *hw,
				       struct ieee80211_sta *sta);
	int (*get_txpower)(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
			   int *dbm);

	int (*tdls_channel_switch)(struct ieee80211_hw *hw,
				   struct ieee80211_vif *vif,
				   struct ieee80211_sta *sta, u8 oper_class,
				   struct cfg80211_chan_def *chandef,
				   struct sk_buff *tmpl_skb, u32 ch_sw_tm_ie);
	void (*tdls_cancel_channel_switch)(struct ieee80211_hw *hw,
					   struct ieee80211_vif *vif,
					   struct ieee80211_sta *sta);
	void (*tdls_recv_channel_switch)(struct ieee80211_hw *hw,
					 struct ieee80211_vif *vif,
					 struct ieee80211_tdls_ch_sw_params *params);

	void (*wake_tx_queue)(struct ieee80211_hw *hw,
			      struct ieee80211_txq *txq);
	void (*sync_rx_queues)(struct ieee80211_hw *hw);

	int (*start_nan)(struct ieee80211_hw *hw,
			 struct ieee80211_vif *vif,
			 struct cfg80211_nan_conf *conf);
	int (*stop_nan)(struct ieee80211_hw *hw,
			struct ieee80211_vif *vif);
	int (*nan_change_conf)(struct ieee80211_hw *hw,
			       struct ieee80211_vif *vif,
			       struct cfg80211_nan_conf *conf, u32 changes);
	int (*add_nan_func)(struct ieee80211_hw *hw,
			    struct ieee80211_vif *vif,
			    const struct cfg80211_nan_func *nan_func);
	void (*del_nan_func)(struct ieee80211_hw *hw,
			    struct ieee80211_vif *vif,
			    u8 instance_id);
};


struct ieee80211_hw *ieee80211_alloc_hw_nm(size_t priv_data_len,
					   const struct ieee80211_ops *ops,
					   const char *requested_name);


static inline
struct ieee80211_hw *ieee80211_alloc_hw(size_t priv_data_len,
					const struct ieee80211_ops *ops)
{
	return ieee80211_alloc_hw_nm(priv_data_len, ops, NULL);
}


int ieee80211_register_hw(struct ieee80211_hw *hw);


struct ieee80211_tpt_blink {
	int throughput;
	int blink_time;
};


enum ieee80211_tpt_led_trigger_flags {
	IEEE80211_TPT_LEDTRIG_FL_RADIO		= BIT(0),
	IEEE80211_TPT_LEDTRIG_FL_WORK		= BIT(1),
	IEEE80211_TPT_LEDTRIG_FL_CONNECTED	= BIT(2),
};

#ifdef CONFIG_MAC80211_LEDS
const char *__ieee80211_get_tx_led_name(struct ieee80211_hw *hw);
const char *__ieee80211_get_rx_led_name(struct ieee80211_hw *hw);
const char *__ieee80211_get_assoc_led_name(struct ieee80211_hw *hw);
const char *__ieee80211_get_radio_led_name(struct ieee80211_hw *hw);
const char *
__ieee80211_create_tpt_led_trigger(struct ieee80211_hw *hw,
				   unsigned int flags,
				   const struct ieee80211_tpt_blink *blink_table,
				   unsigned int blink_table_len);
#endif

static inline const char *ieee80211_get_tx_led_name(struct ieee80211_hw *hw)
{
#ifdef CONFIG_MAC80211_LEDS
	return __ieee80211_get_tx_led_name(hw);
#else
	return NULL;
#endif
}


static inline const char *ieee80211_get_rx_led_name(struct ieee80211_hw *hw)
{
#ifdef CONFIG_MAC80211_LEDS
	return __ieee80211_get_rx_led_name(hw);
#else
	return NULL;
#endif
}


static inline const char *ieee80211_get_assoc_led_name(struct ieee80211_hw *hw)
{
#ifdef CONFIG_MAC80211_LEDS
	return __ieee80211_get_assoc_led_name(hw);
#else
	return NULL;
#endif
}


static inline const char *ieee80211_get_radio_led_name(struct ieee80211_hw *hw)
{
#ifdef CONFIG_MAC80211_LEDS
	return __ieee80211_get_radio_led_name(hw);
#else
	return NULL;
#endif
}


static inline const char *
ieee80211_create_tpt_led_trigger(struct ieee80211_hw *hw, unsigned int flags,
				 const struct ieee80211_tpt_blink *blink_table,
				 unsigned int blink_table_len)
{
#ifdef CONFIG_MAC80211_LEDS
	return __ieee80211_create_tpt_led_trigger(hw, flags, blink_table,
						  blink_table_len);
#else
	return NULL;
#endif
}


void ieee80211_unregister_hw(struct ieee80211_hw *hw);


void ieee80211_free_hw(struct ieee80211_hw *hw);


void ieee80211_restart_hw(struct ieee80211_hw *hw);


void ieee80211_rx_napi(struct ieee80211_hw *hw, struct ieee80211_sta *sta,
		       struct sk_buff *skb, struct napi_struct *napi);


static inline void ieee80211_rx(struct ieee80211_hw *hw, struct sk_buff *skb)
{
	ieee80211_rx_napi(hw, NULL, skb, NULL);
}


void ieee80211_rx_irqsafe(struct ieee80211_hw *hw, struct sk_buff *skb);


static inline void ieee80211_rx_ni(struct ieee80211_hw *hw,
				   struct sk_buff *skb)
{
	local_bh_disable();
	ieee80211_rx(hw, skb);
	local_bh_enable();
}


int ieee80211_sta_ps_transition(struct ieee80211_sta *sta, bool start);


static inline int ieee80211_sta_ps_transition_ni(struct ieee80211_sta *sta,
						  bool start)
{
	int ret;

	local_bh_disable();
	ret = ieee80211_sta_ps_transition(sta, start);
	local_bh_enable();

	return ret;
}


void ieee80211_sta_pspoll(struct ieee80211_sta *sta);


void ieee80211_sta_uapsd_trigger(struct ieee80211_sta *sta, u8 tid);


#define IEEE80211_TX_STATUS_HEADROOM	ALIGN(14, 4)


void ieee80211_sta_set_buffered(struct ieee80211_sta *sta,
				u8 tid, bool buffered);


void ieee80211_get_tx_rates(struct ieee80211_vif *vif,
			    struct ieee80211_sta *sta,
			    struct sk_buff *skb,
			    struct ieee80211_tx_rate *dest,
			    int max_rates);


void ieee80211_tx_status(struct ieee80211_hw *hw,
			 struct sk_buff *skb);


void ieee80211_tx_status_noskb(struct ieee80211_hw *hw,
			       struct ieee80211_sta *sta,
			       struct ieee80211_tx_info *info);


static inline void ieee80211_tx_status_ni(struct ieee80211_hw *hw,
					  struct sk_buff *skb)
{
	local_bh_disable();
	ieee80211_tx_status(hw, skb);
	local_bh_enable();
}


void ieee80211_tx_status_irqsafe(struct ieee80211_hw *hw,
				 struct sk_buff *skb);


void ieee80211_report_low_ack(struct ieee80211_sta *sta, u32 num_packets);

#define IEEE80211_MAX_CSA_COUNTERS_NUM 2


struct ieee80211_mutable_offsets {
	u16 tim_offset;
	u16 tim_length;

	u16 csa_counter_offs[IEEE80211_MAX_CSA_COUNTERS_NUM];
};


struct sk_buff *
ieee80211_beacon_get_template(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif,
			      struct ieee80211_mutable_offsets *offs);


struct sk_buff *ieee80211_beacon_get_tim(struct ieee80211_hw *hw,
					 struct ieee80211_vif *vif,
					 u16 *tim_offset, u16 *tim_length);


static inline struct sk_buff *ieee80211_beacon_get(struct ieee80211_hw *hw,
						   struct ieee80211_vif *vif)
{
	return ieee80211_beacon_get_tim(hw, vif, NULL, NULL);
}


u8 ieee80211_csa_update_counter(struct ieee80211_vif *vif);


void ieee80211_csa_finish(struct ieee80211_vif *vif);


bool ieee80211_csa_is_complete(struct ieee80211_vif *vif);



struct sk_buff *ieee80211_proberesp_get(struct ieee80211_hw *hw,
					struct ieee80211_vif *vif);


struct sk_buff *ieee80211_pspoll_get(struct ieee80211_hw *hw,
				     struct ieee80211_vif *vif);


struct sk_buff *ieee80211_nullfunc_get(struct ieee80211_hw *hw,
				       struct ieee80211_vif *vif);


struct sk_buff *ieee80211_probereq_get(struct ieee80211_hw *hw,
				       const u8 *src_addr,
				       const u8 *ssid, size_t ssid_len,
				       size_t tailroom);


void ieee80211_rts_get(struct ieee80211_hw *hw, struct ieee80211_vif *vif,
		       const void *frame, size_t frame_len,
		       const struct ieee80211_tx_info *frame_txctl,
		       struct ieee80211_rts *rts);


__le16 ieee80211_rts_duration(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif, size_t frame_len,
			      const struct ieee80211_tx_info *frame_txctl);


void ieee80211_ctstoself_get(struct ieee80211_hw *hw,
			     struct ieee80211_vif *vif,
			     const void *frame, size_t frame_len,
			     const struct ieee80211_tx_info *frame_txctl,
			     struct ieee80211_cts *cts);


__le16 ieee80211_ctstoself_duration(struct ieee80211_hw *hw,
				    struct ieee80211_vif *vif,
				    size_t frame_len,
				    const struct ieee80211_tx_info *frame_txctl);


__le16 ieee80211_generic_frame_duration(struct ieee80211_hw *hw,
					struct ieee80211_vif *vif,
					enum nl80211_band band,
					size_t frame_len,
					struct ieee80211_rate *rate);


struct sk_buff *
ieee80211_get_buffered_bc(struct ieee80211_hw *hw, struct ieee80211_vif *vif);


void ieee80211_get_tkip_p1k_iv(struct ieee80211_key_conf *keyconf,
			       u32 iv32, u16 *p1k);


static inline void ieee80211_get_tkip_p1k(struct ieee80211_key_conf *keyconf,
					  struct sk_buff *skb, u16 *p1k)
{
	struct ieee80211_hdr *hdr = (struct ieee80211_hdr *)skb->data;
	const u8 *data = (u8 *)hdr + ieee80211_hdrlen(hdr->frame_control);
	u32 iv32 = get_unaligned_le32(&data[4]);

	ieee80211_get_tkip_p1k_iv(keyconf, iv32, p1k);
}


void ieee80211_get_tkip_rx_p1k(struct ieee80211_key_conf *keyconf,
			       const u8 *ta, u32 iv32, u16 *p1k);


void ieee80211_get_tkip_p2k(struct ieee80211_key_conf *keyconf,
			    struct sk_buff *skb, u8 *p2k);


u8 *ieee80211_tkip_add_iv(u8 *pos, struct ieee80211_key_conf *keyconf, u64 pn);


void ieee80211_get_key_rx_seq(struct ieee80211_key_conf *keyconf,
			      int tid, struct ieee80211_key_seq *seq);


void ieee80211_set_key_rx_seq(struct ieee80211_key_conf *keyconf,
			      int tid, struct ieee80211_key_seq *seq);


void ieee80211_remove_key(struct ieee80211_key_conf *keyconf);


struct ieee80211_key_conf *
ieee80211_gtk_rekey_add(struct ieee80211_vif *vif,
			struct ieee80211_key_conf *keyconf);


void ieee80211_gtk_rekey_notify(struct ieee80211_vif *vif, const u8 *bssid,
				const u8 *replay_ctr, gfp_t gfp);


void ieee80211_wake_queue(struct ieee80211_hw *hw, int queue);


void ieee80211_stop_queue(struct ieee80211_hw *hw, int queue);



int ieee80211_queue_stopped(struct ieee80211_hw *hw, int queue);


void ieee80211_stop_queues(struct ieee80211_hw *hw);


void ieee80211_wake_queues(struct ieee80211_hw *hw);


void ieee80211_scan_completed(struct ieee80211_hw *hw,
			      struct cfg80211_scan_info *info);


void ieee80211_sched_scan_results(struct ieee80211_hw *hw);


void ieee80211_sched_scan_stopped(struct ieee80211_hw *hw);


enum ieee80211_interface_iteration_flags {
	IEEE80211_IFACE_ITER_NORMAL	= 0,
	IEEE80211_IFACE_ITER_RESUME_ALL	= BIT(0),
	IEEE80211_IFACE_ITER_ACTIVE	= BIT(1),
};


void ieee80211_iterate_interfaces(struct ieee80211_hw *hw, u32 iter_flags,
				  void (*iterator)(void *data, u8 *mac,
						   struct ieee80211_vif *vif),
				  void *data);


static inline void
ieee80211_iterate_active_interfaces(struct ieee80211_hw *hw, u32 iter_flags,
				    void (*iterator)(void *data, u8 *mac,
						     struct ieee80211_vif *vif),
				    void *data)
{
	ieee80211_iterate_interfaces(hw,
				     iter_flags | IEEE80211_IFACE_ITER_ACTIVE,
				     iterator, data);
}


void ieee80211_iterate_active_interfaces_atomic(struct ieee80211_hw *hw,
						u32 iter_flags,
						void (*iterator)(void *data,
						    u8 *mac,
						    struct ieee80211_vif *vif),
						void *data);


void ieee80211_iterate_active_interfaces_rtnl(struct ieee80211_hw *hw,
					      u32 iter_flags,
					      void (*iterator)(void *data,
						u8 *mac,
						struct ieee80211_vif *vif),
					      void *data);


void ieee80211_iterate_stations_atomic(struct ieee80211_hw *hw,
				       void (*iterator)(void *data,
						struct ieee80211_sta *sta),
				       void *data);

void ieee80211_queue_work(struct ieee80211_hw *hw, struct work_struct *work);


void ieee80211_queue_delayed_work(struct ieee80211_hw *hw,
				  struct delayed_work *dwork,
				  unsigned long delay);


int ieee80211_start_tx_ba_session(struct ieee80211_sta *sta, u16 tid,
				  u16 timeout);


void ieee80211_start_tx_ba_cb_irqsafe(struct ieee80211_vif *vif, const u8 *ra,
				      u16 tid);


int ieee80211_stop_tx_ba_session(struct ieee80211_sta *sta, u16 tid);


void ieee80211_stop_tx_ba_cb_irqsafe(struct ieee80211_vif *vif, const u8 *ra,
				     u16 tid);


struct ieee80211_sta *ieee80211_find_sta(struct ieee80211_vif *vif,
					 const u8 *addr);


struct ieee80211_sta *ieee80211_find_sta_by_ifaddr(struct ieee80211_hw *hw,
					       const u8 *addr,
					       const u8 *localaddr);


void ieee80211_sta_block_awake(struct ieee80211_hw *hw,
			       struct ieee80211_sta *pubsta, bool block);


void ieee80211_sta_eosp(struct ieee80211_sta *pubsta);


void ieee80211_send_eosp_nullfunc(struct ieee80211_sta *pubsta, int tid);


void ieee80211_iter_keys(struct ieee80211_hw *hw,
			 struct ieee80211_vif *vif,
			 void (*iter)(struct ieee80211_hw *hw,
				      struct ieee80211_vif *vif,
				      struct ieee80211_sta *sta,
				      struct ieee80211_key_conf *key,
				      void *data),
			 void *iter_data);


void ieee80211_iter_keys_rcu(struct ieee80211_hw *hw,
			     struct ieee80211_vif *vif,
			     void (*iter)(struct ieee80211_hw *hw,
					  struct ieee80211_vif *vif,
					  struct ieee80211_sta *sta,
					  struct ieee80211_key_conf *key,
					  void *data),
			     void *iter_data);


void ieee80211_iter_chan_contexts_atomic(
	struct ieee80211_hw *hw,
	void (*iter)(struct ieee80211_hw *hw,
		     struct ieee80211_chanctx_conf *chanctx_conf,
		     void *data),
	void *iter_data);


struct sk_buff *ieee80211_ap_probereq_get(struct ieee80211_hw *hw,
					  struct ieee80211_vif *vif);


void ieee80211_beacon_loss(struct ieee80211_vif *vif);


void ieee80211_connection_loss(struct ieee80211_vif *vif);


void ieee80211_resume_disconnect(struct ieee80211_vif *vif);


void ieee80211_cqm_rssi_notify(struct ieee80211_vif *vif,
			       enum nl80211_cqm_rssi_threshold_event rssi_event,
			       gfp_t gfp);


void ieee80211_cqm_beacon_loss_notify(struct ieee80211_vif *vif, gfp_t gfp);


void ieee80211_radar_detected(struct ieee80211_hw *hw);


void ieee80211_chswitch_done(struct ieee80211_vif *vif, bool success);


void ieee80211_request_smps(struct ieee80211_vif *vif,
			    enum ieee80211_smps_mode smps_mode);


void ieee80211_ready_on_channel(struct ieee80211_hw *hw);


void ieee80211_remain_on_channel_expired(struct ieee80211_hw *hw);


void ieee80211_stop_rx_ba_session(struct ieee80211_vif *vif, u16 ba_rx_bitmap,
				  const u8 *addr);


void ieee80211_mark_rx_ba_filtered_frames(struct ieee80211_sta *pubsta, u8 tid,
					  u16 ssn, u64 filtered,
					  u16 received_mpdus);


void ieee80211_send_bar(struct ieee80211_vif *vif, u8 *ra, u16 tid, u16 ssn);


void ieee80211_start_rx_ba_session_offl(struct ieee80211_vif *vif,
					const u8 *addr, u16 tid);


void ieee80211_stop_rx_ba_session_offl(struct ieee80211_vif *vif,
				       const u8 *addr, u16 tid);




struct ieee80211_tx_rate_control {
	struct ieee80211_hw *hw;
	struct ieee80211_supported_band *sband;
	struct ieee80211_bss_conf *bss_conf;
	struct sk_buff *skb;
	struct ieee80211_tx_rate reported_rate;
	bool rts, short_preamble;
	u8 max_rate_idx;
	u32 rate_idx_mask;
	u8 *rate_idx_mcs_mask;
	bool bss;
};

struct rate_control_ops {
	const char *name;
	void *(*alloc)(struct ieee80211_hw *hw, struct dentry *debugfsdir);
	void (*free)(void *priv);

	void *(*alloc_sta)(void *priv, struct ieee80211_sta *sta, gfp_t gfp);
	void (*rate_init)(void *priv, struct ieee80211_supported_band *sband,
			  struct cfg80211_chan_def *chandef,
			  struct ieee80211_sta *sta, void *priv_sta);
	void (*rate_update)(void *priv, struct ieee80211_supported_band *sband,
			    struct cfg80211_chan_def *chandef,
			    struct ieee80211_sta *sta, void *priv_sta,
			    u32 changed);
	void (*free_sta)(void *priv, struct ieee80211_sta *sta,
			 void *priv_sta);

	void (*tx_status_noskb)(void *priv,
				struct ieee80211_supported_band *sband,
				struct ieee80211_sta *sta, void *priv_sta,
				struct ieee80211_tx_info *info);
	void (*tx_status)(void *priv, struct ieee80211_supported_band *sband,
			  struct ieee80211_sta *sta, void *priv_sta,
			  struct sk_buff *skb);
	void (*get_rate)(void *priv, struct ieee80211_sta *sta, void *priv_sta,
			 struct ieee80211_tx_rate_control *txrc);

	void (*add_sta_debugfs)(void *priv, void *priv_sta,
				struct dentry *dir);
	void (*remove_sta_debugfs)(void *priv, void *priv_sta);

	u32 (*get_expected_throughput)(void *priv_sta);
};

static inline int rate_supported(struct ieee80211_sta *sta,
				 enum nl80211_band band,
				 int index)
{
	return (sta == NULL || sta->supp_rates[band] & BIT(index));
}


bool rate_control_send_low(struct ieee80211_sta *sta,
			   void *priv_sta,
			   struct ieee80211_tx_rate_control *txrc);


static inline s8
rate_lowest_index(struct ieee80211_supported_band *sband,
		  struct ieee80211_sta *sta)
{
	int i;

	for (i = 0; i < sband->n_bitrates; i++)
		if (rate_supported(sta, sband->band, i))
			return i;

	
	WARN_ON_ONCE(1);

	
	return 0;
}

static inline
bool rate_usable_index_exists(struct ieee80211_supported_band *sband,
			      struct ieee80211_sta *sta)
{
	unsigned int i;

	for (i = 0; i < sband->n_bitrates; i++)
		if (rate_supported(sta, sband->band, i))
			return true;
	return false;
}


int rate_control_set_rates(struct ieee80211_hw *hw,
			   struct ieee80211_sta *pubsta,
			   struct ieee80211_sta_rates *rates);

int ieee80211_rate_control_register(const struct rate_control_ops *ops);
void ieee80211_rate_control_unregister(const struct rate_control_ops *ops);

static inline bool
conf_is_ht20(struct ieee80211_conf *conf)
{
	return conf->chandef.width == NL80211_CHAN_WIDTH_20;
}

static inline bool
conf_is_ht40_minus(struct ieee80211_conf *conf)
{
	return conf->chandef.width == NL80211_CHAN_WIDTH_40 &&
	       conf->chandef.center_freq1 < conf->chandef.chan->center_freq;
}

static inline bool
conf_is_ht40_plus(struct ieee80211_conf *conf)
{
	return conf->chandef.width == NL80211_CHAN_WIDTH_40 &&
	       conf->chandef.center_freq1 > conf->chandef.chan->center_freq;
}

static inline bool
conf_is_ht40(struct ieee80211_conf *conf)
{
	return conf->chandef.width == NL80211_CHAN_WIDTH_40;
}

static inline bool
conf_is_ht(struct ieee80211_conf *conf)
{
	return (conf->chandef.width != NL80211_CHAN_WIDTH_5) &&
		(conf->chandef.width != NL80211_CHAN_WIDTH_10) &&
		(conf->chandef.width != NL80211_CHAN_WIDTH_20_NOHT);
}

static inline enum nl80211_iftype
ieee80211_iftype_p2p(enum nl80211_iftype type, bool p2p)
{
	if (p2p) {
		switch (type) {
		case NL80211_IFTYPE_STATION:
			return NL80211_IFTYPE_P2P_CLIENT;
		case NL80211_IFTYPE_AP:
			return NL80211_IFTYPE_P2P_GO;
		default:
			break;
		}
	}
	return type;
}

static inline enum nl80211_iftype
ieee80211_vif_type_p2p(struct ieee80211_vif *vif)
{
	return ieee80211_iftype_p2p(vif->type, vif->p2p);
}


void ieee80211_update_mu_groups(struct ieee80211_vif *vif,
				const u8 *membership, const u8 *position);

void ieee80211_enable_rssi_reports(struct ieee80211_vif *vif,
				   int rssi_min_thold,
				   int rssi_max_thold);

void ieee80211_disable_rssi_reports(struct ieee80211_vif *vif);


int ieee80211_ave_rssi(struct ieee80211_vif *vif);


void ieee80211_report_wowlan_wakeup(struct ieee80211_vif *vif,
				    struct cfg80211_wowlan_wakeup *wakeup,
				    gfp_t gfp);


bool ieee80211_tx_prepare_skb(struct ieee80211_hw *hw,
			      struct ieee80211_vif *vif, struct sk_buff *skb,
			      int band, struct ieee80211_sta **sta);


struct ieee80211_noa_data {
	u32 next_tsf;
	bool has_next_tsf;

	u8 absent;

	u8 count[IEEE80211_P2P_NOA_DESC_MAX];
	struct {
		u32 start;
		u32 duration;
		u32 interval;
	} desc[IEEE80211_P2P_NOA_DESC_MAX];
};


int ieee80211_parse_p2p_noa(const struct ieee80211_p2p_noa_attr *attr,
			    struct ieee80211_noa_data *data, u32 tsf);


void ieee80211_update_p2p_noa(struct ieee80211_noa_data *data, u32 tsf);


void ieee80211_tdls_oper_request(struct ieee80211_vif *vif, const u8 *peer,
				 enum nl80211_tdls_operation oper,
				 u16 reason_code, gfp_t gfp);


int ieee80211_reserve_tid(struct ieee80211_sta *sta, u8 tid);


void ieee80211_unreserve_tid(struct ieee80211_sta *sta, u8 tid);


struct sk_buff *ieee80211_tx_dequeue(struct ieee80211_hw *hw,
				     struct ieee80211_txq *txq);


void ieee80211_txq_get_depth(struct ieee80211_txq *txq,
			     unsigned long *frame_cnt,
			     unsigned long *byte_cnt);


void ieee80211_nan_func_terminated(struct ieee80211_vif *vif,
				   u8 inst_id,
				   enum nl80211_nan_func_term_reason reason,
				   gfp_t gfp);


void ieee80211_nan_func_match(struct ieee80211_vif *vif,
			      struct cfg80211_nan_match_params *match,
			      gfp_t gfp);

#endif 
