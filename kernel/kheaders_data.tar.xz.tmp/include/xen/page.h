#ifndef _XEN_PAGE_H
#define _XEN_PAGE_H

#include <asm/page.h>


#define XEN_PAGE_SHIFT	12
#define XEN_PAGE_SIZE	(_AC(1, UL) << XEN_PAGE_SHIFT)
#define XEN_PAGE_MASK	(~(XEN_PAGE_SIZE-1))
#define xen_offset_in_page(p)	((unsigned long)(p) & ~XEN_PAGE_MASK)



#define xen_pfn_to_page(xen_pfn)	\
	(pfn_to_page((unsigned long)(xen_pfn) >> (PAGE_SHIFT - XEN_PAGE_SHIFT)))
#define page_to_xen_pfn(page)		\
	((page_to_pfn(page)) << (PAGE_SHIFT - XEN_PAGE_SHIFT))

#define XEN_PFN_PER_PAGE	(PAGE_SIZE / XEN_PAGE_SIZE)

#define XEN_PFN_DOWN(x)	((x) >> XEN_PAGE_SHIFT)
#define XEN_PFN_UP(x)	(((x) + XEN_PAGE_SIZE-1) >> XEN_PAGE_SHIFT)
#define XEN_PFN_PHYS(x)	((phys_addr_t)(x) << XEN_PAGE_SHIFT)

#include <asm/xen/page.h>


static inline unsigned long xen_page_to_gfn(struct page *page)
{
	return pfn_to_gfn(page_to_xen_pfn(page));
}

struct xen_memory_region {
	unsigned long start_pfn;
	unsigned long n_pfns;
};

#define XEN_EXTRA_MEM_MAX_REGIONS 128 

extern __initdata
struct xen_memory_region xen_extra_mem[XEN_EXTRA_MEM_MAX_REGIONS];

extern unsigned long xen_released_pages;

#endif	
