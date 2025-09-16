


#ifndef _DRM_CACHE_H_
#define _DRM_CACHE_H_

void drm_clflush_pages(struct page *pages[], unsigned long num_pages);

static inline bool drm_arch_can_wc_memory(void)
{
#if defined(CONFIG_PPC) && !defined(CONFIG_NOT_COHERENT_CACHE)
	return false;
#elif defined(CONFIG_MIPS) && defined(CONFIG_CPU_LOONGSON3)
	return false;
#elif defined(CONFIG_ARM) || defined(CONFIG_ARM64)
	
	return false;
#else
	return true;
#endif
}

#endif
