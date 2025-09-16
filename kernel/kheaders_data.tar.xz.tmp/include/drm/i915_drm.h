
#ifndef _I915_DRM_H_
#define _I915_DRM_H_

#include <drm/i915_pciids.h>
#include <uapi/drm/i915_drm.h>


extern unsigned long i915_read_mch_val(void);
extern bool i915_gpu_raise(void);
extern bool i915_gpu_lower(void);
extern bool i915_gpu_busy(void);
extern bool i915_gpu_turbo_disable(void);


#define INTEL_GMCH_CTRL		0x52
#define INTEL_GMCH_VGA_DISABLE  (1 << 1)
#define SNB_GMCH_CTRL		0x50
#define    SNB_GMCH_GGMS_SHIFT	8 
#define    SNB_GMCH_GGMS_MASK	0x3
#define    SNB_GMCH_GMS_SHIFT   3 
#define    SNB_GMCH_GMS_MASK    0x1f
#define    BDW_GMCH_GGMS_SHIFT	6
#define    BDW_GMCH_GGMS_MASK	0x3
#define    BDW_GMCH_GMS_SHIFT   8
#define    BDW_GMCH_GMS_MASK    0xff

#define I830_GMCH_CTRL			0x52

#define I830_GMCH_GMS_MASK		0x70
#define I830_GMCH_GMS_LOCAL		0x10
#define I830_GMCH_GMS_STOLEN_512	0x20
#define I830_GMCH_GMS_STOLEN_1024	0x30
#define I830_GMCH_GMS_STOLEN_8192	0x40

#define I855_GMCH_GMS_MASK		0xF0
#define I855_GMCH_GMS_STOLEN_0M		0x0
#define I855_GMCH_GMS_STOLEN_1M		(0x1 << 4)
#define I855_GMCH_GMS_STOLEN_4M		(0x2 << 4)
#define I855_GMCH_GMS_STOLEN_8M		(0x3 << 4)
#define I855_GMCH_GMS_STOLEN_16M	(0x4 << 4)
#define I855_GMCH_GMS_STOLEN_32M	(0x5 << 4)
#define I915_GMCH_GMS_STOLEN_48M	(0x6 << 4)
#define I915_GMCH_GMS_STOLEN_64M	(0x7 << 4)
#define G33_GMCH_GMS_STOLEN_128M	(0x8 << 4)
#define G33_GMCH_GMS_STOLEN_256M	(0x9 << 4)
#define INTEL_GMCH_GMS_STOLEN_96M	(0xa << 4)
#define INTEL_GMCH_GMS_STOLEN_160M	(0xb << 4)
#define INTEL_GMCH_GMS_STOLEN_224M	(0xc << 4)
#define INTEL_GMCH_GMS_STOLEN_352M	(0xd << 4)

#define I830_DRB3		0x63
#define I85X_DRB3		0x43
#define I865_TOUD		0xc4

#define I830_ESMRAMC		0x91
#define I845_ESMRAMC		0x9e
#define I85X_ESMRAMC		0x61
#define    TSEG_ENABLE		(1 << 0)
#define    I830_TSEG_SIZE_512K	(0 << 1)
#define    I830_TSEG_SIZE_1M	(1 << 1)
#define    I845_TSEG_SIZE_MASK	(3 << 1)
#define    I845_TSEG_SIZE_512K	(2 << 1)
#define    I845_TSEG_SIZE_1M	(3 << 1)

#define INTEL_BSM 0x5c
#define   INTEL_BSM_MASK	(-(1u << 20))

#endif				
