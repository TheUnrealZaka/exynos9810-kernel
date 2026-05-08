

#ifndef _ASM_X86_AMD_IOMMU_H
#define _ASM_X86_AMD_IOMMU_H

#include <linux/types.h>


struct amd_iommu_pi_data {
	u32 ga_tag;
	u32 prev_ga_tag;
	u64 base;
	bool is_guest_mode;
	struct vcpu_data *vcpu_data;
	void *ir_data;
};

#ifdef CONFIG_AMD_IOMMU

struct task_struct;
struct pci_dev;

extern int amd_iommu_detect(void);
extern int amd_iommu_init_hardware(void);


#define AMD_PRI_DEV_ERRATUM_ENABLE_RESET		0
#define AMD_PRI_DEV_ERRATUM_LIMIT_REQ_ONE		1

extern void amd_iommu_enable_device_erratum(struct pci_dev *pdev, u32 erratum);


extern int amd_iommu_init_device(struct pci_dev *pdev, int pasids);


extern void amd_iommu_free_device(struct pci_dev *pdev);


extern int amd_iommu_bind_pasid(struct pci_dev *pdev, int pasid,
				struct task_struct *task);


extern void amd_iommu_unbind_pasid(struct pci_dev *pdev, int pasid);


#define AMD_IOMMU_INV_PRI_RSP_SUCCESS	0
#define AMD_IOMMU_INV_PRI_RSP_INVALID	1
#define AMD_IOMMU_INV_PRI_RSP_FAIL	2

typedef int (*amd_iommu_invalid_ppr_cb)(struct pci_dev *pdev,
					int pasid,
					unsigned long address,
					u16);

extern int amd_iommu_set_invalid_ppr_cb(struct pci_dev *pdev,
					amd_iommu_invalid_ppr_cb cb);

#define PPR_FAULT_EXEC	(1 << 1)
#define PPR_FAULT_READ  (1 << 2)
#define PPR_FAULT_WRITE (1 << 5)
#define PPR_FAULT_USER  (1 << 6)
#define PPR_FAULT_RSVD  (1 << 7)
#define PPR_FAULT_GN    (1 << 8)



#define AMD_IOMMU_DEVICE_FLAG_ATS_SUP     0x1    
#define AMD_IOMMU_DEVICE_FLAG_PRI_SUP     0x2    
#define AMD_IOMMU_DEVICE_FLAG_PASID_SUP   0x4    
#define AMD_IOMMU_DEVICE_FLAG_EXEC_SUP    0x8    
#define AMD_IOMMU_DEVICE_FLAG_PRIV_SUP   0x10    

struct amd_iommu_device_info {
	int max_pasids;
	u32 flags;
};

extern int amd_iommu_device_info(struct pci_dev *pdev,
				 struct amd_iommu_device_info *info);



typedef void (*amd_iommu_invalidate_ctx)(struct pci_dev *pdev, int pasid);

extern int amd_iommu_set_invalidate_ctx_cb(struct pci_dev *pdev,
					   amd_iommu_invalidate_ctx cb);
#else 

static inline int amd_iommu_detect(void) { return -ENODEV; }

#endif 

#if defined(CONFIG_AMD_IOMMU) && defined(CONFIG_IRQ_REMAP)


extern int amd_iommu_register_ga_log_notifier(int (*notifier)(u32));

extern int
amd_iommu_update_ga(int cpu, bool is_run, void *data);

#else 

static inline int
amd_iommu_register_ga_log_notifier(int (*notifier)(u32))
{
	return 0;
}

static inline int
amd_iommu_update_ga(int cpu, bool is_run, void *data)
{
	return 0;
}

#endif 

#endif 
