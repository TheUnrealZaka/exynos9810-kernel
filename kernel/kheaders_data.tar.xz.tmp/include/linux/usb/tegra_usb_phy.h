

#ifndef __TEGRA_USB_PHY_H
#define __TEGRA_USB_PHY_H

#include <linux/clk.h>
#include <linux/usb/otg.h>



struct tegra_phy_soc_config {
	bool utmi_pll_config_in_car_module;
	bool has_hostpc;
	bool requires_usbmode_setup;
	bool requires_extra_tuning_parameters;
};

struct tegra_utmip_config {
	u8 hssync_start_delay;
	u8 elastic_limit;
	u8 idle_wait_delay;
	u8 term_range_adj;
	bool xcvr_setup_use_fuses;
	u8 xcvr_setup;
	u8 xcvr_lsfslew;
	u8 xcvr_lsrslew;
	u8 xcvr_hsslew;
	u8 hssquelch_level;
	u8 hsdiscon_level;
};

enum tegra_usb_phy_port_speed {
	TEGRA_USB_PHY_PORT_SPEED_FULL = 0,
	TEGRA_USB_PHY_PORT_SPEED_LOW,
	TEGRA_USB_PHY_PORT_SPEED_HIGH,
};

struct tegra_xtal_freq;

struct tegra_usb_phy {
	int instance;
	const struct tegra_xtal_freq *freq;
	void __iomem *regs;
	void __iomem *pad_regs;
	struct clk *clk;
	struct clk *pll_u;
	struct clk *pad_clk;
	struct regulator *vbus;
	enum usb_dr_mode mode;
	void *config;
	const struct tegra_phy_soc_config *soc_config;
	struct usb_phy *ulpi;
	struct usb_phy u_phy;
	bool is_legacy_phy;
	bool is_ulpi_phy;
	int reset_gpio;
};

void tegra_usb_phy_preresume(struct usb_phy *phy);

void tegra_usb_phy_postresume(struct usb_phy *phy);

void tegra_ehci_phy_restore_start(struct usb_phy *phy,
				 enum tegra_usb_phy_port_speed port_speed);

void tegra_ehci_phy_restore_end(struct usb_phy *phy);

#endif 
