
#ifndef _PCI_HOTPLUG_H
#define _PCI_HOTPLUG_H


struct hotplug_slot_ops {
	struct module *owner;
	const char *mod_name;
	int (*enable_slot)		(struct hotplug_slot *slot);
	int (*disable_slot)		(struct hotplug_slot *slot);
	int (*set_attention_status)	(struct hotplug_slot *slot, u8 value);
	int (*hardware_test)		(struct hotplug_slot *slot, u32 value);
	int (*get_power_status)		(struct hotplug_slot *slot, u8 *value);
	int (*get_attention_status)	(struct hotplug_slot *slot, u8 *value);
	int (*get_latch_status)		(struct hotplug_slot *slot, u8 *value);
	int (*get_adapter_status)	(struct hotplug_slot *slot, u8 *value);
	int (*reset_slot)		(struct hotplug_slot *slot, int probe);
};


struct hotplug_slot_info {
	u8	power_status;
	u8	attention_status;
	u8	latch_status;
	u8	adapter_status;
};


struct hotplug_slot {
	struct hotplug_slot_ops		*ops;
	struct hotplug_slot_info	*info;
	void (*release) (struct hotplug_slot *slot);
	void				*private;

	
	struct list_head		slot_list;
	struct pci_slot			*pci_slot;
};

static inline const char *hotplug_slot_name(const struct hotplug_slot *slot)
{
	return pci_slot_name(slot->pci_slot);
}

int __pci_hp_register(struct hotplug_slot *slot, struct pci_bus *pbus, int nr,
		      const char *name, struct module *owner,
		      const char *mod_name);
int pci_hp_deregister(struct hotplug_slot *slot);
int __must_check pci_hp_change_slot_info(struct hotplug_slot *slot,
					 struct hotplug_slot_info *info);


#define pci_hp_register(slot, pbus, devnr, name) \
	__pci_hp_register(slot, pbus, devnr, name, THIS_MODULE, KBUILD_MODNAME)


struct hpp_type0 {
	u32 revision;
	u8  cache_line_size;
	u8  latency_timer;
	u8  enable_serr;
	u8  enable_perr;
};


struct hpp_type1 {
	u32 revision;
	u8  max_mem_read;
	u8  avg_max_split;
	u16 tot_max_split;
};


struct hpp_type2 {
	u32 revision;
	u32 unc_err_mask_and;
	u32 unc_err_mask_or;
	u32 unc_err_sever_and;
	u32 unc_err_sever_or;
	u32 cor_err_mask_and;
	u32 cor_err_mask_or;
	u32 adv_err_cap_and;
	u32 adv_err_cap_or;
	u16 pci_exp_devctl_and;
	u16 pci_exp_devctl_or;
	u16 pci_exp_lnkctl_and;
	u16 pci_exp_lnkctl_or;
	u32 sec_unc_err_sever_and;
	u32 sec_unc_err_sever_or;
	u32 sec_unc_err_mask_and;
	u32 sec_unc_err_mask_or;
};

struct hotplug_params {
	struct hpp_type0 *t0;		
	struct hpp_type1 *t1;		
	struct hpp_type2 *t2;		
	struct hpp_type0 type0_data;
	struct hpp_type1 type1_data;
	struct hpp_type2 type2_data;
};

#ifdef CONFIG_ACPI
#include <linux/acpi.h>
int pci_get_hp_params(struct pci_dev *dev, struct hotplug_params *hpp);
int acpi_get_hp_hw_control_from_firmware(struct pci_dev *dev, u32 flags);
int acpi_pci_check_ejectable(struct pci_bus *pbus, acpi_handle handle);
int acpi_pci_detect_ejectable(acpi_handle handle);
#else
static inline int pci_get_hp_params(struct pci_dev *dev,
				    struct hotplug_params *hpp)
{
	return -ENODEV;
}
#endif
#endif
