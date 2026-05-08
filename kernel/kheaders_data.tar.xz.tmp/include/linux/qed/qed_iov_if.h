

#ifndef _QED_IOV_IF_H
#define _QED_IOV_IF_H

#include <linux/qed/qed_if.h>


struct qed_iov_hv_ops {
	int (*configure)(struct qed_dev *cdev, int num_vfs_param);

	int (*set_mac) (struct qed_dev *cdev, u8 *mac, int vfid);

	int (*set_vlan) (struct qed_dev *cdev, u16 vid, int vfid);

	int (*get_config) (struct qed_dev *cdev, int vf_id,
			   struct ifla_vf_info *ivi);

	int (*set_link_state) (struct qed_dev *cdev, int vf_id,
			       int link_state);

	int (*set_spoof) (struct qed_dev *cdev, int vfid, bool val);

	int (*set_rate) (struct qed_dev *cdev, int vfid,
			 u32 min_rate, u32 max_rate);
};

#endif
