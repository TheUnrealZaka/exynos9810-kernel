#ifndef _LINUX_MEMBLOCK_H
#define _LINUX_MEMBLOCK_H
#ifdef __KERNEL__

#ifdef CONFIG_HAVE_MEMBLOCK


#include <linux/init.h>
#include <linux/mm.h>

#define INIT_MEMBLOCK_REGIONS	128
#define INIT_PHYSMEM_REGIONS	4


enum {
	MEMBLOCK_NONE		= 0x0,	
	MEMBLOCK_HOTPLUG	= 0x1,	
	MEMBLOCK_MIRROR		= 0x2,	
	MEMBLOCK_NOMAP		= 0x4,	
};

struct memblock_region {
	phys_addr_t base;
	phys_addr_t size;
	unsigned long flags;
#ifdef CONFIG_HAVE_MEMBLOCK_NODE_MAP
	int nid;
#endif
};

struct memblock_type {
	unsigned long cnt;	
	unsigned long max;	
	phys_addr_t total_size;	
	struct memblock_region *regions;
};

struct memblock {
	bool bottom_up;  
	phys_addr_t current_limit;
	struct memblock_type memory;
	struct memblock_type reserved;
#ifdef CONFIG_HAVE_MEMBLOCK_PHYS_MAP
	struct memblock_type physmem;
#endif
};

extern struct memblock memblock;
extern int memblock_debug;
#ifdef CONFIG_MOVABLE_NODE

extern bool movable_node_enabled;
#endif 

#ifdef CONFIG_ARCH_DISCARD_MEMBLOCK
#define __init_memblock __meminit
#define __initdata_memblock __meminitdata
void memblock_discard(void);
#else
#define __init_memblock
#define __initdata_memblock
#endif

#define memblock_dbg(fmt, ...) \
	if (memblock_debug) printk(KERN_INFO pr_fmt(fmt), ##__VA_ARGS__)

phys_addr_t memblock_find_in_range_node(phys_addr_t size, phys_addr_t align,
					phys_addr_t start, phys_addr_t end,
					int nid, ulong flags);
phys_addr_t memblock_find_in_range(phys_addr_t start, phys_addr_t end,
				   phys_addr_t size, phys_addr_t align);
void memblock_allow_resize(void);
int memblock_add_node(phys_addr_t base, phys_addr_t size, int nid);
int memblock_add(phys_addr_t base, phys_addr_t size);
int memblock_remove(phys_addr_t base, phys_addr_t size);
int memblock_free(phys_addr_t base, phys_addr_t size);
int memblock_reserve(phys_addr_t base, phys_addr_t size);
void memblock_trim_memory(phys_addr_t align);
bool memblock_overlaps_region(struct memblock_type *type,
			      phys_addr_t base, phys_addr_t size);
int memblock_mark_hotplug(phys_addr_t base, phys_addr_t size);
int memblock_clear_hotplug(phys_addr_t base, phys_addr_t size);
int memblock_mark_mirror(phys_addr_t base, phys_addr_t size);
int memblock_mark_nomap(phys_addr_t base, phys_addr_t size);
ulong choose_memblock_flags(void);


int memblock_add_range(struct memblock_type *type,
		       phys_addr_t base, phys_addr_t size,
		       int nid, unsigned long flags);

void __next_mem_range(u64 *idx, int nid, ulong flags,
		      struct memblock_type *type_a,
		      struct memblock_type *type_b, phys_addr_t *out_start,
		      phys_addr_t *out_end, int *out_nid);

void __next_mem_range_rev(u64 *idx, int nid, ulong flags,
			  struct memblock_type *type_a,
			  struct memblock_type *type_b, phys_addr_t *out_start,
			  phys_addr_t *out_end, int *out_nid);

void __next_reserved_mem_region(u64 *idx, phys_addr_t *out_start,
				phys_addr_t *out_end);

void __memblock_free_early(phys_addr_t base, phys_addr_t size);
void __memblock_free_late(phys_addr_t base, phys_addr_t size);


#define for_each_mem_range(i, type_a, type_b, nid, flags,		\
			   p_start, p_end, p_nid)			\
	for (i = 0, __next_mem_range(&i, nid, flags, type_a, type_b,	\
				     p_start, p_end, p_nid);		\
	     i != (u64)ULLONG_MAX;					\
	     __next_mem_range(&i, nid, flags, type_a, type_b,		\
			      p_start, p_end, p_nid))


#define for_each_mem_range_rev(i, type_a, type_b, nid, flags,		\
			       p_start, p_end, p_nid)			\
	for (i = (u64)ULLONG_MAX,					\
		     __next_mem_range_rev(&i, nid, flags, type_a, type_b,\
					  p_start, p_end, p_nid);	\
	     i != (u64)ULLONG_MAX;					\
	     __next_mem_range_rev(&i, nid, flags, type_a, type_b,	\
				  p_start, p_end, p_nid))


#define for_each_reserved_mem_region(i, p_start, p_end)			\
	for (i = 0UL, __next_reserved_mem_region(&i, p_start, p_end);	\
	     i != (u64)ULLONG_MAX;					\
	     __next_reserved_mem_region(&i, p_start, p_end))

#ifdef CONFIG_MOVABLE_NODE
static inline bool memblock_is_hotpluggable(struct memblock_region *m)
{
	return m->flags & MEMBLOCK_HOTPLUG;
}

static inline bool __init_memblock movable_node_is_enabled(void)
{
	return movable_node_enabled;
}
#else
static inline bool memblock_is_hotpluggable(struct memblock_region *m)
{
	return false;
}
static inline bool movable_node_is_enabled(void)
{
	return false;
}
#endif

static inline bool memblock_is_mirror(struct memblock_region *m)
{
	return m->flags & MEMBLOCK_MIRROR;
}

static inline bool memblock_is_nomap(struct memblock_region *m)
{
	return m->flags & MEMBLOCK_NOMAP;
}

#ifdef CONFIG_HAVE_MEMBLOCK_NODE_MAP
int memblock_search_pfn_nid(unsigned long pfn, unsigned long *start_pfn,
			    unsigned long  *end_pfn);
void __next_mem_pfn_range(int *idx, int nid, unsigned long *out_start_pfn,
			  unsigned long *out_end_pfn, int *out_nid);


#define for_each_mem_pfn_range(i, nid, p_start, p_end, p_nid)		\
	for (i = -1, __next_mem_pfn_range(&i, nid, p_start, p_end, p_nid); \
	     i >= 0; __next_mem_pfn_range(&i, nid, p_start, p_end, p_nid))
#endif 


#define for_each_free_mem_range(i, nid, flags, p_start, p_end, p_nid)	\
	for_each_mem_range(i, &memblock.memory, &memblock.reserved,	\
			   nid, flags, p_start, p_end, p_nid)


#define for_each_free_mem_range_reverse(i, nid, flags, p_start, p_end,	\
					p_nid)				\
	for_each_mem_range_rev(i, &memblock.memory, &memblock.reserved,	\
			       nid, flags, p_start, p_end, p_nid)

static inline void memblock_set_region_flags(struct memblock_region *r,
					     unsigned long flags)
{
	r->flags |= flags;
}

static inline void memblock_clear_region_flags(struct memblock_region *r,
					       unsigned long flags)
{
	r->flags &= ~flags;
}

#ifdef CONFIG_HAVE_MEMBLOCK_NODE_MAP
int memblock_set_node(phys_addr_t base, phys_addr_t size,
		      struct memblock_type *type, int nid);

static inline void memblock_set_region_node(struct memblock_region *r, int nid)
{
	r->nid = nid;
}

static inline int memblock_get_region_node(const struct memblock_region *r)
{
	return r->nid;
}
#else
static inline void memblock_set_region_node(struct memblock_region *r, int nid)
{
}

static inline int memblock_get_region_node(const struct memblock_region *r)
{
	return 0;
}
#endif 

phys_addr_t memblock_alloc_nid(phys_addr_t size, phys_addr_t align, int nid);
phys_addr_t memblock_alloc_try_nid(phys_addr_t size, phys_addr_t align, int nid);

phys_addr_t memblock_alloc(phys_addr_t size, phys_addr_t align);

#ifdef CONFIG_MOVABLE_NODE

static inline void __init memblock_set_bottom_up(bool enable)
{
	memblock.bottom_up = enable;
}


static inline bool memblock_bottom_up(void)
{
	return memblock.bottom_up;
}
#else
static inline void __init memblock_set_bottom_up(bool enable) {}
static inline bool memblock_bottom_up(void) { return false; }
#endif


#define MEMBLOCK_ALLOC_ANYWHERE	(~(phys_addr_t)0)
#define MEMBLOCK_ALLOC_ACCESSIBLE	0

phys_addr_t __init memblock_alloc_range(phys_addr_t size, phys_addr_t align,
					phys_addr_t start, phys_addr_t end,
					ulong flags);
phys_addr_t memblock_alloc_base(phys_addr_t size, phys_addr_t align,
				phys_addr_t max_addr);
phys_addr_t __memblock_alloc_base(phys_addr_t size, phys_addr_t align,
				  phys_addr_t max_addr);
phys_addr_t memblock_phys_mem_size(void);
phys_addr_t memblock_reserved_size(void);
phys_addr_t memblock_mem_size(unsigned long limit_pfn);
phys_addr_t memblock_start_of_DRAM(void);
phys_addr_t memblock_end_of_DRAM(void);
void memblock_enforce_memory_limit(phys_addr_t memory_limit);
void memblock_mem_limit_remove_map(phys_addr_t limit);
bool memblock_is_memory(phys_addr_t addr);
int memblock_is_map_memory(phys_addr_t addr);
int memblock_is_region_memory(phys_addr_t base, phys_addr_t size);
bool memblock_is_reserved(phys_addr_t addr);
bool memblock_is_region_reserved(phys_addr_t base, phys_addr_t size);

extern void __memblock_dump_all(void);

static inline void memblock_dump_all(void)
{
	if (memblock_debug)
		__memblock_dump_all();
}


void memblock_set_current_limit(phys_addr_t limit);


phys_addr_t memblock_get_current_limit(void);




static inline unsigned long memblock_region_memory_base_pfn(const struct memblock_region *reg)
{
	return PFN_UP(reg->base);
}


static inline unsigned long memblock_region_memory_end_pfn(const struct memblock_region *reg)
{
	return PFN_DOWN(reg->base + reg->size);
}


static inline unsigned long memblock_region_reserved_base_pfn(const struct memblock_region *reg)
{
	return PFN_DOWN(reg->base);
}


static inline unsigned long memblock_region_reserved_end_pfn(const struct memblock_region *reg)
{
	return PFN_UP(reg->base + reg->size);
}

#define for_each_memblock(memblock_type, region)					\
	for (region = memblock.memblock_type.regions;					\
	     region < (memblock.memblock_type.regions + memblock.memblock_type.cnt);	\
	     region++)

#define for_each_memblock_type(memblock_type, rgn)			\
	for (idx = 0, rgn = &memblock_type->regions[0];			\
	     idx < memblock_type->cnt;					\
	     idx++, rgn = &memblock_type->regions[idx])

#ifdef CONFIG_MEMTEST
extern void early_memtest(phys_addr_t start, phys_addr_t end);
#else
static inline void early_memtest(phys_addr_t start, phys_addr_t end)
{
}
#endif

extern unsigned long memblock_reserved_memory_within(phys_addr_t start_addr,
		phys_addr_t end_addr);
#else
static inline phys_addr_t memblock_alloc(phys_addr_t size, phys_addr_t align)
{
	return 0;
}

static inline unsigned long memblock_reserved_memory_within(phys_addr_t start_addr,
		phys_addr_t end_addr)
{
	return 0;
}

#endif 

#endif 

#endif 
