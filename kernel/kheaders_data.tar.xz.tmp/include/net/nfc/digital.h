

#ifndef __NFC_DIGITAL_H
#define __NFC_DIGITAL_H

#include <linux/skbuff.h>
#include <net/nfc/nfc.h>


enum {
	NFC_DIGITAL_CONFIG_RF_TECH = 0,
	NFC_DIGITAL_CONFIG_FRAMING,
};


enum {
	NFC_DIGITAL_RF_TECH_106A = 0,
	NFC_DIGITAL_RF_TECH_212F,
	NFC_DIGITAL_RF_TECH_424F,
	NFC_DIGITAL_RF_TECH_ISO15693,
	NFC_DIGITAL_RF_TECH_106B,

	NFC_DIGITAL_RF_TECH_LAST,
};


enum {
	NFC_DIGITAL_FRAMING_NFCA_SHORT = 0,
	NFC_DIGITAL_FRAMING_NFCA_STANDARD,
	NFC_DIGITAL_FRAMING_NFCA_STANDARD_WITH_CRC_A,
	NFC_DIGITAL_FRAMING_NFCA_ANTICOL_COMPLETE,

	NFC_DIGITAL_FRAMING_NFCA_T1T,
	NFC_DIGITAL_FRAMING_NFCA_T2T,
	NFC_DIGITAL_FRAMING_NFCA_T4T,
	NFC_DIGITAL_FRAMING_NFCA_NFC_DEP,

	NFC_DIGITAL_FRAMING_NFCF,
	NFC_DIGITAL_FRAMING_NFCF_T3T,
	NFC_DIGITAL_FRAMING_NFCF_NFC_DEP,
	NFC_DIGITAL_FRAMING_NFC_DEP_ACTIVATED,

	NFC_DIGITAL_FRAMING_ISO15693_INVENTORY,
	NFC_DIGITAL_FRAMING_ISO15693_T5T,

	NFC_DIGITAL_FRAMING_NFCB,
	NFC_DIGITAL_FRAMING_NFCB_T4T,

	NFC_DIGITAL_FRAMING_LAST,
};

#define DIGITAL_MDAA_NFCID1_SIZE 3

struct digital_tg_mdaa_params {
	u16 sens_res;
	u8 nfcid1[DIGITAL_MDAA_NFCID1_SIZE];
	u8 sel_res;

	u8 nfcid2[NFC_NFCID2_MAXSIZE];
	u16 sc;
};

struct nfc_digital_dev;


typedef void (*nfc_digital_cmd_complete_t)(struct nfc_digital_dev *ddev,
					   void *arg, struct sk_buff *resp);


struct nfc_digital_ops {
	int (*in_configure_hw)(struct nfc_digital_dev *ddev, int type,
			       int param);
	int (*in_send_cmd)(struct nfc_digital_dev *ddev, struct sk_buff *skb,
			   u16 timeout, nfc_digital_cmd_complete_t cb,
			   void *arg);

	int (*tg_configure_hw)(struct nfc_digital_dev *ddev, int type,
			       int param);
	int (*tg_send_cmd)(struct nfc_digital_dev *ddev, struct sk_buff *skb,
			   u16 timeout, nfc_digital_cmd_complete_t cb,
			   void *arg);
	int (*tg_listen)(struct nfc_digital_dev *ddev, u16 timeout,
			 nfc_digital_cmd_complete_t cb, void *arg);
	int (*tg_listen_mdaa)(struct nfc_digital_dev *ddev,
			      struct digital_tg_mdaa_params *mdaa_params,
			      u16 timeout, nfc_digital_cmd_complete_t cb,
			      void *arg);
	int (*tg_listen_md)(struct nfc_digital_dev *ddev, u16 timeout,
			    nfc_digital_cmd_complete_t cb, void *arg);
	int (*tg_get_rf_tech)(struct nfc_digital_dev *ddev, u8 *rf_tech);

	int (*switch_rf)(struct nfc_digital_dev *ddev, bool on);
	void (*abort_cmd)(struct nfc_digital_dev *ddev);
};

#define NFC_DIGITAL_POLL_MODE_COUNT_MAX	6 

typedef int (*digital_poll_t)(struct nfc_digital_dev *ddev, u8 rf_tech);

struct digital_poll_tech {
	u8 rf_tech;
	digital_poll_t poll_func;
};


#define NFC_DIGITAL_DRV_CAPS_IN_CRC	0x0001
#define NFC_DIGITAL_DRV_CAPS_TG_CRC	0x0002

struct nfc_digital_dev {
	struct nfc_dev *nfc_dev;
	struct nfc_digital_ops *ops;

	u32 protocols;

	int tx_headroom;
	int tx_tailroom;

	u32 driver_capabilities;
	void *driver_data;

	struct digital_poll_tech poll_techs[NFC_DIGITAL_POLL_MODE_COUNT_MAX];
	u8 poll_tech_count;
	u8 poll_tech_index;
	struct mutex poll_lock;

	struct work_struct cmd_work;
	struct work_struct cmd_complete_work;
	struct list_head cmd_queue;
	struct mutex cmd_lock;

	struct delayed_work poll_work;

	u8 curr_protocol;
	u8 curr_rf_tech;
	u8 curr_nfc_dep_pni;
	u8 did;
	u16 dep_rwt;

	u8 local_payload_max;
	u8 remote_payload_max;

	struct sk_buff *chaining_skb;
	struct digital_data_exch *data_exch;

	int atn_count;
	int nack_count;

	struct sk_buff *saved_skb;

	u16 target_fsc;

	int (*skb_check_crc)(struct sk_buff *skb);
	void (*skb_add_crc)(struct sk_buff *skb);
};

struct nfc_digital_dev *nfc_digital_allocate_device(struct nfc_digital_ops *ops,
						    __u32 supported_protocols,
						    __u32 driver_capabilities,
						    int tx_headroom,
						    int tx_tailroom);
void nfc_digital_free_device(struct nfc_digital_dev *ndev);
int nfc_digital_register_device(struct nfc_digital_dev *ndev);
void nfc_digital_unregister_device(struct nfc_digital_dev *ndev);

static inline void nfc_digital_set_parent_dev(struct nfc_digital_dev *ndev,
					      struct device *dev)
{
	nfc_set_parent_dev(ndev->nfc_dev, dev);
}

static inline void nfc_digital_set_drvdata(struct nfc_digital_dev *dev,
					   void *data)
{
	dev->driver_data = data;
}

static inline void *nfc_digital_get_drvdata(struct nfc_digital_dev *dev)
{
	return dev->driver_data;
}

#endif 
