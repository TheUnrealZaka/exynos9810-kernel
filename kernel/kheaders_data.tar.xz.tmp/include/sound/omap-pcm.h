

#ifndef __OMAP_PCM_H__
#define __OMAP_PCM_H__

#if IS_ENABLED(CONFIG_SND_OMAP_SOC)
int omap_pcm_platform_register(struct device *dev);
#else
static inline int omap_pcm_platform_register(struct device *dev)
{
	return 0;
}
#endif 

#endif 
