

#ifndef PHY_QCOM_UFS_H_
#define PHY_QCOM_UFS_H_

#include "phy.h"


int ufs_qcom_phy_enable_ref_clk(struct phy *phy);


void ufs_qcom_phy_disable_ref_clk(struct phy *phy);


void ufs_qcom_phy_enable_dev_ref_clk(struct phy *phy);


void ufs_qcom_phy_disable_dev_ref_clk(struct phy *phy);

int ufs_qcom_phy_enable_iface_clk(struct phy *phy);
void ufs_qcom_phy_disable_iface_clk(struct phy *phy);
int ufs_qcom_phy_start_serdes(struct phy *phy);
int ufs_qcom_phy_set_tx_lane_enable(struct phy *phy, u32 tx_lanes);
int ufs_qcom_phy_calibrate_phy(struct phy *phy, bool is_rate_B);
int ufs_qcom_phy_is_pcs_ready(struct phy *phy);
void ufs_qcom_phy_save_controller_version(struct phy *phy,
			u8 major, u16 minor, u16 step);

#endif 
