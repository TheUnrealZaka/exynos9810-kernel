

#ifndef __DEV_RIL_BRIDGE_H__
#define __DEV_RIL_BRIDGE_H__

#define IPC_SYSTEM_CP_CHANNEL_INFO	0x01
#define IPC_SYSTEM_CHARGING_DOCK_INFO	0x02

struct __packed sipc_fmt_hdr {
	u16 len;
	u8  msg_seq;
	u8  ack_seq;
	u8  main_cmd;
	u8  sub_cmd;
	u8  cmd_type;
};

struct dev_ril_bridge_msg {
	unsigned int dev_id;
	unsigned int data_len;
	void *data;
};

#ifdef CONFIG_DEV_RIL_BRIDGE
extern int register_dev_ril_bridge_event_notifier(struct notifier_block *nb);
extern int unregister_dev_ril_bridge_event_notifier(struct notifier_block *nb);
extern int dev_ril_bridge_send_msg(int id, int size, void *buf);

#else
static inline int register_dev_ril_bridge_event_notifier(
		struct notifier_block *nb)	{}
static inline int unregister_dev_ril_bridge_event_notifier(
		struct notifier_block *nb)	{}
static inline int dev_ril_bridge_send_msg(int id, int size, void *buf) {}
#endif

#endif
