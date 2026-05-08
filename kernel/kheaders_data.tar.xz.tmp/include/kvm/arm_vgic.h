
#ifndef __KVM_ARM_VGIC_H
#define __KVM_ARM_VGIC_H

#include <linux/kernel.h>
#include <linux/kvm.h>
#include <linux/irqreturn.h>
#include <linux/spinlock.h>
#include <linux/static_key.h>
#include <linux/types.h>
#include <kvm/iodev.h>
#include <linux/list.h>
#include <linux/jump_label.h>

#define VGIC_V3_MAX_CPUS	255
#define VGIC_V2_MAX_CPUS	8
#define VGIC_NR_IRQS_LEGACY     256
#define VGIC_NR_SGIS		16
#define VGIC_NR_PPIS		16
#define VGIC_NR_PRIVATE_IRQS	(VGIC_NR_SGIS + VGIC_NR_PPIS)
#define VGIC_MAX_PRIVATE	(VGIC_NR_PRIVATE_IRQS - 1)
#define VGIC_MAX_SPI		1019
#define VGIC_MAX_RESERVED	1023
#define VGIC_MIN_LPI		8192
#define KVM_IRQCHIP_NUM_PINS	(1020 - 32)

enum vgic_type {
	VGIC_V2,		
	VGIC_V3,		
};


struct vgic_global {
	
	enum vgic_type		type;

	
	phys_addr_t		vcpu_base;

	
	void __iomem		*vcpu_base_va;

	
	void __iomem		*vctrl_base;

	
	int			nr_lr;

	
	unsigned int		maint_irq;

	
	int			max_gic_vcpus;

	
	bool			can_emulate_gicv2;

	
	struct static_key_false gicv3_cpuif;
};

extern struct vgic_global kvm_vgic_global_state;

#define VGIC_V2_MAX_LRS		(1 << 6)
#define VGIC_V3_MAX_LRS		16
#define VGIC_V3_LR_INDEX(lr)	(VGIC_V3_MAX_LRS - 1 - lr)

enum vgic_irq_config {
	VGIC_CONFIG_EDGE = 0,
	VGIC_CONFIG_LEVEL
};

struct vgic_irq {
	spinlock_t irq_lock;		
	struct list_head lpi_list;	
	struct list_head ap_list;

	struct kvm_vcpu *vcpu;		

	struct kvm_vcpu *target_vcpu;	

	u32 intid;			
	bool pending;
	bool line_level;		
	bool soft_pending;		
	bool active;			
	bool enabled;
	bool hw;			
	struct kref refcount;		
	u32 hwintid;			
	union {
		u8 targets;			
		u32 mpidr;			
	};
	u8 source;			
	u8 priority;
	enum vgic_irq_config config;	
};

struct vgic_register_region;
struct vgic_its;

enum iodev_type {
	IODEV_CPUIF,
	IODEV_DIST,
	IODEV_REDIST,
	IODEV_ITS
};

struct vgic_io_device {
	gpa_t base_addr;
	union {
		struct kvm_vcpu *redist_vcpu;
		struct vgic_its *its;
	};
	const struct vgic_register_region *regions;
	enum iodev_type iodev_type;
	int nr_regions;
	struct kvm_io_device dev;
};

struct vgic_its {
	
	gpa_t			vgic_its_base;

	bool			enabled;
	bool			initialized;
	struct vgic_io_device	iodev;
	struct kvm_device	*dev;

	
	u64			baser_device_table;
	u64			baser_coll_table;

	
	struct mutex		cmd_lock;
	u64			cbaser;
	u32			creadr;
	u32			cwriter;

	
	struct mutex		its_lock;
	struct list_head	device_list;
	struct list_head	collection_list;
};

struct vgic_dist {
	bool			in_kernel;
	bool			ready;
	bool			initialized;

	
	u32			vgic_model;

	
	bool			msis_require_devid;

	int			nr_spis;

	
	
	void __iomem		*vctrl_base;

	
	gpa_t			vgic_dist_base;		
	union {
		
		gpa_t			vgic_cpu_base;
		
		gpa_t			vgic_redist_base;
	};

	
	bool			enabled;

	struct vgic_irq		*spis;

	struct vgic_io_device	dist_iodev;

	bool			has_its;

	
	u64			propbaser;

	
	spinlock_t		lpi_list_lock;
	struct list_head	lpi_list_head;
	int			lpi_list_count;
};

struct vgic_v2_cpu_if {
	u32		vgic_hcr;
	u32		vgic_vmcr;
	u32		vgic_misr;	
	u64		vgic_eisr;	
	u64		vgic_elrsr;	
	u32		vgic_apr;
	u32		vgic_lr[VGIC_V2_MAX_LRS];
};

struct vgic_v3_cpu_if {
	u32		vgic_hcr;
	u32		vgic_vmcr;
	u32		vgic_sre;	
	u32		vgic_misr;	
	u32		vgic_eisr;	
	u32		vgic_elrsr;	
	u32		vgic_ap0r[4];
	u32		vgic_ap1r[4];
	u64		vgic_lr[VGIC_V3_MAX_LRS];
};

struct vgic_cpu {
	
	union {
		struct vgic_v2_cpu_if	vgic_v2;
		struct vgic_v3_cpu_if	vgic_v3;
	};

	unsigned int used_lrs;
	struct vgic_irq private_irqs[VGIC_NR_PRIVATE_IRQS];

	spinlock_t ap_list_lock;	

	
	struct list_head ap_list_head;

	u64 live_lrs;

	
	struct vgic_io_device	rd_iodev;
	struct vgic_io_device	sgi_iodev;

	
	u64 pendbaser;

	bool lpis_enabled;
};

extern struct static_key_false vgic_v2_cpuif_trap;

int kvm_vgic_addr(struct kvm *kvm, unsigned long type, u64 *addr, bool write);
void kvm_vgic_early_init(struct kvm *kvm);
int kvm_vgic_create(struct kvm *kvm, u32 type);
void kvm_vgic_destroy(struct kvm *kvm);
void kvm_vgic_vcpu_early_init(struct kvm_vcpu *vcpu);
void kvm_vgic_vcpu_destroy(struct kvm_vcpu *vcpu);
int kvm_vgic_map_resources(struct kvm *kvm);
int kvm_vgic_hyp_init(void);

int kvm_vgic_inject_irq(struct kvm *kvm, int cpuid, unsigned int intid,
			bool level);
int kvm_vgic_inject_mapped_irq(struct kvm *kvm, int cpuid, unsigned int intid,
			       bool level);
int kvm_vgic_map_phys_irq(struct kvm_vcpu *vcpu, u32 virt_irq, u32 phys_irq);
int kvm_vgic_unmap_phys_irq(struct kvm_vcpu *vcpu, unsigned int virt_irq);
bool kvm_vgic_map_is_active(struct kvm_vcpu *vcpu, unsigned int virt_irq);

int kvm_vgic_vcpu_pending_irq(struct kvm_vcpu *vcpu);

#define irqchip_in_kernel(k)	(!!((k)->arch.vgic.in_kernel))
#define vgic_initialized(k)	((k)->arch.vgic.initialized)
#define vgic_ready(k)		((k)->arch.vgic.ready)
#define vgic_valid_spi(k, i)	(((i) >= VGIC_NR_PRIVATE_IRQS) && \
			((i) < (k)->arch.vgic.nr_spis + VGIC_NR_PRIVATE_IRQS))

bool kvm_vcpu_has_pending_irqs(struct kvm_vcpu *vcpu);
void kvm_vgic_sync_hwstate(struct kvm_vcpu *vcpu);
void kvm_vgic_flush_hwstate(struct kvm_vcpu *vcpu);

void vgic_v3_dispatch_sgi(struct kvm_vcpu *vcpu, u64 reg);


static inline int kvm_vgic_get_max_vcpus(void)
{
	return kvm_vgic_global_state.max_gic_vcpus;
}

int kvm_send_userspace_msi(struct kvm *kvm, struct kvm_msi *msi);


int kvm_vgic_setup_default_irq_routing(struct kvm *kvm);

#endif 
