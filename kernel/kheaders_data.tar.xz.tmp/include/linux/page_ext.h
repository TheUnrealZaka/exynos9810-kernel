#ifndef __LINUX_PAGE_EXT_H
#define __LINUX_PAGE_EXT_H

#include <linux/types.h>
#include <linux/stacktrace.h>
#include <linux/stackdepot.h>

struct pglist_data;
struct page_ext_operations {
	size_t offset;
	size_t size;
	bool (*need)(void);
	void (*init)(void);
};

#ifdef CONFIG_PAGE_EXTENSION



enum page_ext_flags {
	PAGE_EXT_DEBUG_POISON,		
	PAGE_EXT_DEBUG_GUARD,
	PAGE_EXT_OWNER,
#if defined(CONFIG_IDLE_PAGE_TRACKING) && !defined(CONFIG_64BIT)
	PAGE_EXT_YOUNG,
	PAGE_EXT_IDLE,
#endif
};


struct page_ext {
	unsigned long flags;
};

extern void pgdat_page_ext_init(struct pglist_data *pgdat);

#ifdef CONFIG_SPARSEMEM
static inline void page_ext_init_flatmem(void)
{
}
extern void page_ext_init(void);
#else
extern void page_ext_init_flatmem(void);
static inline void page_ext_init(void)
{
}
#endif

struct page_ext *lookup_page_ext(struct page *page);

#else 
struct page_ext;

static inline void pgdat_page_ext_init(struct pglist_data *pgdat)
{
}

static inline struct page_ext *lookup_page_ext(struct page *page)
{
	return NULL;
}

static inline void page_ext_init(void)
{
}

static inline void page_ext_init_flatmem(void)
{
}
#endif 
#endif 
