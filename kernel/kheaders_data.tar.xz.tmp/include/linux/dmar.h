

#ifndef __DMAR_H__
#define __DMAR_H__

#include <linux/acpi.h>
#include <linux/types.h>
#include <linux/msi.h>
#include <linux/irqreturn.h>
#include <linux/rwsem.h>
#include <linux/rcupdate.h>

struct acpi_dmar_header;

#ifdef	CONFIG_X86
# define	DMAR_UNITS_SUPPORTED	MAX_IO_APICS
#else
# define	DMAR_UNITS_SUPPORTED	64
#endif


#define DMAR_INTR_REMAP		0x1
#define DMAR_X2APIC_OPT_OUT	0x2

struct intel_iommu;

struct dmar_dev_scope {
	struct device __rcu *dev;
	u8 bus;
	u8 devfn;
};

#ifdef CONFIG_DMAR_TABLE
extern struct acpi_table_header *dmar_tbl;
struct dmar_drhd_unit {
	struct list_head list;		
	struct  acpi_dmar_header *hdr;	
	u64	reg_base_addr;		
	struct	dmar_dev_scope *devices;
	int	devices_cnt;		
	u16	segment;		
	u8	ignored:1; 		
	u8	include_all:1;
	struct intel_iommu *iommu;
};

struct dmar_pci_path {
	u8 bus;
	u8 device;
	u8 function;
};

struct dmar_pci_notify_info {
	struct pci_dev			*dev;
	unsigned long			event;
	int				bus;
	u16				seg;
	u16				level;
	struct dmar_pci_path		path[];
}  __attribute__((packed));

extern struct rw_semaphore dmar_global_lock;
extern struct list_head dmar_drhd_units;

#define for_each_drhd_unit(drhd) \
	list_for_each_entry_rcu(drhd, &dmar_drhd_units, list)

#define for_each_active_drhd_unit(drhd)					\
	list_for_each_entry_rcu(drhd, &dmar_drhd_units, list)		\
		if (drhd->ignored) {} else

#define for_each_active_iommu(i, drhd)					\
	list_for_each_entry_rcu(drhd, &dmar_drhd_units, list)		\
		if (i=drhd->iommu, drhd->ignored) {} else

#define for_each_iommu(i, drhd)						\
	list_for_each_entry_rcu(drhd, &dmar_drhd_units, list)		\
		if (i=drhd->iommu, 0) {} else 

static inline bool dmar_rcu_check(void)
{
	return rwsem_is_locked(&dmar_global_lock) ||
	       system_state == SYSTEM_BOOTING;
}

#define	dmar_rcu_dereference(p)	rcu_dereference_check((p), dmar_rcu_check())

#define	for_each_dev_scope(a, c, p, d)	\
	for ((p) = 0; ((d) = (p) < (c) ? dmar_rcu_dereference((a)[(p)].dev) : \
			NULL, (p) < (c)); (p)++)

#define	for_each_active_dev_scope(a, c, p, d)	\
	for_each_dev_scope((a), (c), (p), (d))	if (!(d)) { continue; } else

extern int dmar_table_init(void);
extern int dmar_dev_scope_init(void);
extern int dmar_parse_dev_scope(void *start, void *end, int *cnt,
				struct dmar_dev_scope **devices, u16 segment);
extern void *dmar_alloc_dev_scope(void *start, void *end, int *cnt);
extern void dmar_free_dev_scope(struct dmar_dev_scope **devices, int *cnt);
extern int dmar_insert_dev_scope(struct dmar_pci_notify_info *info,
				 void *start, void*end, u16 segment,
				 struct dmar_dev_scope *devices,
				 int devices_cnt);
extern int dmar_remove_dev_scope(struct dmar_pci_notify_info *info,
				 u16 segment, struct dmar_dev_scope *devices,
				 int count);

extern int detect_intel_iommu(void);
extern int enable_drhd_fault_handling(void);
extern int dmar_device_add(acpi_handle handle);
extern int dmar_device_remove(acpi_handle handle);

static inline int dmar_res_noop(struct acpi_dmar_header *hdr, void *arg)
{
	return 0;
}

#ifdef CONFIG_INTEL_IOMMU
extern int iommu_detected, no_iommu;
extern int intel_iommu_init(void);
extern int dmar_parse_one_rmrr(struct acpi_dmar_header *header, void *arg);
extern int dmar_parse_one_atsr(struct acpi_dmar_header *header, void *arg);
extern int dmar_check_one_atsr(struct acpi_dmar_header *hdr, void *arg);
extern int dmar_release_one_atsr(struct acpi_dmar_header *hdr, void *arg);
extern int dmar_iommu_hotplug(struct dmar_drhd_unit *dmaru, bool insert);
extern int dmar_iommu_notify_scope_dev(struct dmar_pci_notify_info *info);
#else 
static inline int intel_iommu_init(void) { return -ENODEV; }

#define	dmar_parse_one_rmrr		dmar_res_noop
#define	dmar_parse_one_atsr		dmar_res_noop
#define	dmar_check_one_atsr		dmar_res_noop
#define	dmar_release_one_atsr		dmar_res_noop

static inline int dmar_iommu_notify_scope_dev(struct dmar_pci_notify_info *info)
{
	return 0;
}

static inline int dmar_iommu_hotplug(struct dmar_drhd_unit *dmaru, bool insert)
{
	return 0;
}
#endif 

#ifdef CONFIG_IRQ_REMAP
extern int dmar_ir_hotplug(struct dmar_drhd_unit *dmaru, bool insert);
#else  
static inline int dmar_ir_hotplug(struct dmar_drhd_unit *dmaru, bool insert)
{ return 0; }
#endif 

#else 

static inline int dmar_device_add(void *handle)
{
	return 0;
}

static inline int dmar_device_remove(void *handle)
{
	return 0;
}

#endif 

struct irte {
	union {
		
		struct {
			__u64	present		: 1,  
				fpd		: 1,  
				__res0		: 6,  
				avail		: 4,  
				__res1		: 3,  
				pst		: 1,  
				vector		: 8,  
				__res2		: 40; 
		};

		
		struct {
			__u64	r_present	: 1,  
				r_fpd		: 1,  
				dst_mode	: 1,  
				redir_hint	: 1,  
				trigger_mode	: 1,  
				dlvry_mode	: 3,  
				r_avail		: 4,  
				r_res0		: 4,  
				r_vector	: 8,  
				r_res1		: 8,  
				dest_id		: 32; 
		};

		
		struct {
			__u64	p_present	: 1,  
				p_fpd		: 1,  
				p_res0		: 6,  
				p_avail		: 4,  
				p_res1		: 2,  
				p_urgent	: 1,  
				p_pst		: 1,  
				p_vector	: 8,  
				p_res2		: 14, 
				pda_l		: 26; 
		};
		__u64 low;
	};

	union {
		
		struct {
			__u64	sid		: 16,  
				sq		: 2,   
				svt		: 2,   
				__res3		: 44;  
		};

		
		struct {
			__u64	p_sid		: 16,  
				p_sq		: 2,   
				p_svt		: 2,   
				p_res3		: 12,  
				pda_h		: 32;  
		};
		__u64 high;
	};
};

static inline void dmar_copy_shared_irte(struct irte *dst, struct irte *src)
{
	dst->present	= src->present;
	dst->fpd	= src->fpd;
	dst->avail	= src->avail;
	dst->pst	= src->pst;
	dst->vector	= src->vector;
	dst->sid	= src->sid;
	dst->sq		= src->sq;
	dst->svt	= src->svt;
}

#define PDA_LOW_BIT    26
#define PDA_HIGH_BIT   32

enum {
	IRQ_REMAP_XAPIC_MODE,
	IRQ_REMAP_X2APIC_MODE,
};


struct irq_data;
extern void dmar_msi_unmask(struct irq_data *data);
extern void dmar_msi_mask(struct irq_data *data);
extern void dmar_msi_read(int irq, struct msi_msg *msg);
extern void dmar_msi_write(int irq, struct msi_msg *msg);
extern int dmar_set_interrupt(struct intel_iommu *iommu);
extern irqreturn_t dmar_fault(int irq, void *dev_id);
extern int dmar_alloc_hwirq(int id, int node, void *arg);
extern void dmar_free_hwirq(int irq);

#endif 
