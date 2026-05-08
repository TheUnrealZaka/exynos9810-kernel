

#ifndef __VTS_H
#define __VTS_H

#include <linux/platform_device.h>

#ifdef CONFIG_SND_SOC_SAMSUNG_VTS

extern int vts_acquire_sram(struct platform_device *pdev, int vts);


extern int vts_release_sram(struct platform_device *pdev, int vts);


extern int vts_clear_sram(struct platform_device *pdev);


extern volatile bool vts_is_on(void);
extern bool vts_is_recognitionrunning(void);
#else 
static inline int vts_acquire_sram(struct platform_device *pdev, int vts)
{ return -ENODEV; }
static inline int vts_release_sram(struct platform_device *pdev, int vts)
{ return -ENODEV; }
static inline int vts_clear_sram(struct platform_device *pdev) { return -ENODEV; }
static inline bool vts_is_on(void) { return false; }
static inline bool vts_is_recognitionrunning(void) { return false; }
#endif 

#endif 
