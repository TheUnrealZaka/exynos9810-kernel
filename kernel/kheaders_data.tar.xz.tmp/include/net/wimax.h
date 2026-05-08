

#ifndef __NET__WIMAX_H__
#define __NET__WIMAX_H__

#include <linux/wimax.h>
#include <net/genetlink.h>
#include <linux/netdevice.h>

struct net_device;
struct genl_info;
struct wimax_dev;


struct wimax_dev {
	struct net_device *net_dev;
	struct list_head id_table_node;
	struct mutex mutex;		
	struct mutex mutex_reset;
	enum wimax_st state;

	int (*op_msg_from_user)(struct wimax_dev *wimax_dev,
				const char *,
				const void *, size_t,
				const struct genl_info *info);
	int (*op_rfkill_sw_toggle)(struct wimax_dev *wimax_dev,
				   enum wimax_rf_state);
	int (*op_reset)(struct wimax_dev *wimax_dev);

	struct rfkill *rfkill;
	unsigned int rf_hw;
	unsigned int rf_sw;
	char name[32];

	struct dentry *debugfs_dentry;
};




void wimax_dev_init(struct wimax_dev *);
int wimax_dev_add(struct wimax_dev *, struct net_device *);
void wimax_dev_rm(struct wimax_dev *);

static inline
struct wimax_dev *net_dev_to_wimax(struct net_device *net_dev)
{
	return netdev_priv(net_dev);
}

static inline
struct device *wimax_dev_to_dev(struct wimax_dev *wimax_dev)
{
	return wimax_dev->net_dev->dev.parent;
}

void wimax_state_change(struct wimax_dev *, enum wimax_st);
enum wimax_st wimax_state_get(struct wimax_dev *);


void wimax_report_rfkill_hw(struct wimax_dev *, enum wimax_rf_state);
void wimax_report_rfkill_sw(struct wimax_dev *, enum wimax_rf_state);



struct sk_buff *wimax_msg_alloc(struct wimax_dev *, const char *, const void *,
				size_t, gfp_t);
int wimax_msg_send(struct wimax_dev *, struct sk_buff *);
int wimax_msg(struct wimax_dev *, const char *, const void *, size_t, gfp_t);

const void *wimax_msg_data_len(struct sk_buff *, size_t *);
const void *wimax_msg_data(struct sk_buff *);
ssize_t wimax_msg_len(struct sk_buff *);



int wimax_rfkill(struct wimax_dev *, enum wimax_rf_state);
int wimax_reset(struct wimax_dev *);

#endif 
