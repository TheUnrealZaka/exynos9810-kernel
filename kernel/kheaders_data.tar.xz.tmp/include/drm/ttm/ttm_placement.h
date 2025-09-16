


#ifndef _TTM_PLACEMENT_H_
#define _TTM_PLACEMENT_H_

#include <linux/types.h>



#define TTM_PL_SYSTEM           0
#define TTM_PL_TT               1
#define TTM_PL_VRAM             2
#define TTM_PL_PRIV             3

#define TTM_PL_FLAG_SYSTEM      (1 << TTM_PL_SYSTEM)
#define TTM_PL_FLAG_TT          (1 << TTM_PL_TT)
#define TTM_PL_FLAG_VRAM        (1 << TTM_PL_VRAM)
#define TTM_PL_FLAG_PRIV        (1 << TTM_PL_PRIV)
#define TTM_PL_MASK_MEM         0x0000FFFF



#define TTM_PL_FLAG_CACHED      (1 << 16)
#define TTM_PL_FLAG_UNCACHED    (1 << 17)
#define TTM_PL_FLAG_WC          (1 << 18)
#define TTM_PL_FLAG_NO_EVICT    (1 << 21)
#define TTM_PL_FLAG_TOPDOWN     (1 << 22)

#define TTM_PL_MASK_CACHING     (TTM_PL_FLAG_CACHED | \
				 TTM_PL_FLAG_UNCACHED | \
				 TTM_PL_FLAG_WC)

#define TTM_PL_MASK_MEMTYPE     (TTM_PL_MASK_MEM | TTM_PL_MASK_CACHING)


struct ttm_place {
	unsigned	fpfn;
	unsigned	lpfn;
	uint32_t	flags;
};


struct ttm_placement {
	unsigned		num_placement;
	const struct ttm_place	*placement;
	unsigned		num_busy_placement;
	const struct ttm_place	*busy_placement;
};

#endif
