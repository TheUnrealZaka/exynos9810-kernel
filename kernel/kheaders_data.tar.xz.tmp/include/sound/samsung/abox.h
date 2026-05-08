

#ifndef __ABOX_H
#define __ABOX_H

#include <linux/device.h>
#include <linux/irqreturn.h>
#include <linux/scatterlist.h>
#include <sound/soc.h>
#include <sound/samsung/abox_ipc.h>

#ifdef CONFIG_SND_SOC_SAMSUNG_AUDIO
#define CHANGE_DEV_PRINT
#include <sound/samsung/sec_audio_debug.h>
#endif


typedef irqreturn_t (*abox_irq_handler_t)(int ipc_id, void *dev_id,
		ABOX_IPC_MSG *msg);

#ifdef CONFIG_SND_SOC_SAMSUNG_ABOX

extern bool abox_is_on(void);


extern unsigned int abox_get_requiring_int_freq_in_khz(void);


extern unsigned int abox_get_requiring_aud_freq_in_khz(void);


extern int abox_request_ipc(struct device *dev,
		int hw_irq, const void *supplement,
		size_t size, int atomic, int sync);


static inline int abox_start_ipc_transaction(struct device *dev,
		int hw_irq, const void *supplement,
		size_t size, int atomic, int sync)
{
	return abox_request_ipc(dev, hw_irq, supplement, size, atomic, sync);
}


extern int abox_register_irq_handler(struct device *dev, int ipc_id,
		abox_irq_handler_t irq_handler, void *dev_id);


extern int abox_hw_params_fixup_helper(struct snd_soc_pcm_runtime *rtd,
		struct snd_pcm_hw_params *params, int stream);

extern int abox_iommu_map(struct device *dev, unsigned long iova,
		phys_addr_t paddr, size_t size);


extern int abox_iommu_map_sg(struct device *dev, unsigned long iova,
		struct scatterlist *sg, unsigned int nents,
		int prot, size_t bytes, void *area);


extern int abox_iommu_unmap(struct device *dev, unsigned long iova,
		phys_addr_t paddr, size_t size);


extern phys_addr_t abox_iova_to_phys(struct device *dev, unsigned long iova);


extern void *abox_iova_to_virt(struct device *dev, unsigned long iova);


#else 

static inline bool abox_is_on(void)
{
	return false;
}

static inline unsigned int abox_get_requiring_int_freq_in_khz(void)
{
	return 0;
}

static inline int abox_request_ipc(struct device *dev,
		int hw_irq, const void *supplement,
		size_t size, int atomic, int sync)
{
	return -ENODEV;
}

static inline int abox_register_irq_handler(struct device *dev, int ipc_id,
		abox_irq_handler_t irq_handler, void *dev_id)
{
	return -ENODEV;
}

static inline int abox_hw_params_fixup_helper(struct snd_soc_pcm_runtime *rtd,
		struct snd_pcm_hw_params *params)
{
	return -ENODEV;
}

static inline int abox_iommu_map(struct device *dev, unsigned long iova,
		phys_addr_t paddr, size_t size)
{
	return -ENODEV;
}

static inline int abox_iommu_map_sg(struct device *dev, unsigned long iova,
		struct scatterlist *sg, unsigned int nents,
		int prot, size_t bytes, void *area)
{
	return -ENODEV;
}

static inline int abox_iommu_unmap(struct device *dev, unsigned long iova,
		phys_addr_t paddr, size_t size)
{
	return -ENODEV;
}

static inline phys_addr_t abox_iova_to_phys(struct device *dev,
		unsigned long iova)
{
	return 0;
}

static inline void *abox_iova_to_virt(struct device *dev, unsigned long iova)
{
	return ERR_PTR(-ENODEV);
}

#endif 

#endif 

