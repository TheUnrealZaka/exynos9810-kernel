

#ifndef __ARCH_ARM_OMAP_VOLTAGE_H
#define __ARCH_ARM_OMAP_VOLTAGE_H


struct omap_volt_data {
	u32	volt_nominal;
	u32	sr_efuse_offs;
	u8	sr_errminlimit;
	u8	vp_errgain;
};
struct voltagedomain;

struct voltagedomain *voltdm_lookup(const char *name);
int voltdm_scale(struct voltagedomain *voltdm, unsigned long target_volt);
unsigned long voltdm_get_voltage(struct voltagedomain *voltdm);
struct omap_volt_data *omap_voltage_get_voltdata(struct voltagedomain *voltdm,
		unsigned long volt);
#endif
