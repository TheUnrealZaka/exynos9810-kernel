

#ifndef PAGE_FLAGS_H
#define PAGE_FLAGS_H

#include <linux/types.h>
#include <linux/bug.h>
#include <linux/mmdebug.h>
#ifndef __GENERATING_BOUNDS_H
#include <linux/mm_types.h>
#include <generated/bounds.h>
#endif 




enum pageflags {
	PG_locked,		
	PG_waiters,		
	PG_referenced,
	PG_uptodate,
	PG_dirty,
	PG_lru,
	PG_active,
	PG_workingset,
	PG_error,
	PG_slab,
	PG_owner_priv_1,	
	PG_arch_1,
	PG_reserved,
	PG_private,		
	PG_private_2,		
	PG_writeback,		
	PG_head,		
	PG_swapcache,		
	PG_mappedtodisk,	
	PG_reclaim,		
	PG_swapbacked,		
	PG_unevictable,		
#ifdef CONFIG_MMU
	PG_mlocked,		
#endif
#ifdef CONFIG_ARCH_USES_PG_UNCACHED
	PG_uncached,		
#endif
#ifdef CONFIG_MEMORY_FAILURE
	PG_hwpoison,		
#endif
#if defined(CONFIG_IDLE_PAGE_TRACKING) && defined(CONFIG_64BIT)
	PG_young,
	PG_idle,
#endif
	__NR_PAGEFLAGS,

	
	PG_checked = PG_owner_priv_1,

	
	PG_fscache = PG_private_2,	

	
	
	PG_pinned = PG_owner_priv_1,
	
	PG_savepinned = PG_dirty,
	
	PG_foreign = PG_owner_priv_1,

	
	PG_slob_free = PG_private,

	
	PG_double_map = PG_private_2,

	
	PG_isolated = PG_reclaim,
};

#ifndef __GENERATING_BOUNDS_H

struct page;	

static inline struct page *compound_head(struct page *page)
{
	unsigned long head = READ_ONCE(page->compound_head);

	if (unlikely(head & 1))
		return (struct page *) (head - 1);
	return page;
}

static __always_inline int PageTail(struct page *page)
{
	return READ_ONCE(page->compound_head) & 1;
}

static __always_inline int PageCompound(struct page *page)
{
	return test_bit(PG_head, &page->flags) || PageTail(page);
}


#define PF_ANY(page, enforce)	page
#define PF_HEAD(page, enforce)	compound_head(page)
#define PF_ONLY_HEAD(page, enforce) ({					\
		VM_BUG_ON_PGFLAGS(PageTail(page), page);		\
		page;})
#define PF_NO_TAIL(page, enforce) ({					\
		VM_BUG_ON_PGFLAGS(enforce && PageTail(page), page);	\
		compound_head(page);})
#define PF_NO_COMPOUND(page, enforce) ({				\
		VM_BUG_ON_PGFLAGS(enforce && PageCompound(page), page);	\
		page;})


#define TESTPAGEFLAG(uname, lname, policy)				\
static __always_inline int Page##uname(struct page *page)		\
	{ return test_bit(PG_##lname, &policy(page, 0)->flags); }

#define SETPAGEFLAG(uname, lname, policy)				\
static __always_inline void SetPage##uname(struct page *page)		\
	{ set_bit(PG_##lname, &policy(page, 1)->flags); }

#define CLEARPAGEFLAG(uname, lname, policy)				\
static __always_inline void ClearPage##uname(struct page *page)		\
	{ clear_bit(PG_##lname, &policy(page, 1)->flags); }

#define __SETPAGEFLAG(uname, lname, policy)				\
static __always_inline void __SetPage##uname(struct page *page)		\
	{ __set_bit(PG_##lname, &policy(page, 1)->flags); }

#define __CLEARPAGEFLAG(uname, lname, policy)				\
static __always_inline void __ClearPage##uname(struct page *page)	\
	{ __clear_bit(PG_##lname, &policy(page, 1)->flags); }

#define TESTSETFLAG(uname, lname, policy)				\
static __always_inline int TestSetPage##uname(struct page *page)	\
	{ return test_and_set_bit(PG_##lname, &policy(page, 1)->flags); }

#define TESTCLEARFLAG(uname, lname, policy)				\
static __always_inline int TestClearPage##uname(struct page *page)	\
	{ return test_and_clear_bit(PG_##lname, &policy(page, 1)->flags); }

#define PAGEFLAG(uname, lname, policy)					\
	TESTPAGEFLAG(uname, lname, policy)				\
	SETPAGEFLAG(uname, lname, policy)				\
	CLEARPAGEFLAG(uname, lname, policy)

#define __PAGEFLAG(uname, lname, policy)				\
	TESTPAGEFLAG(uname, lname, policy)				\
	__SETPAGEFLAG(uname, lname, policy)				\
	__CLEARPAGEFLAG(uname, lname, policy)

#define TESTSCFLAG(uname, lname, policy)				\
	TESTSETFLAG(uname, lname, policy)				\
	TESTCLEARFLAG(uname, lname, policy)

#define TESTPAGEFLAG_FALSE(uname)					\
static inline int Page##uname(const struct page *page) { return 0; }

#define SETPAGEFLAG_NOOP(uname)						\
static inline void SetPage##uname(struct page *page) {  }

#define CLEARPAGEFLAG_NOOP(uname)					\
static inline void ClearPage##uname(struct page *page) {  }

#define __CLEARPAGEFLAG_NOOP(uname)					\
static inline void __ClearPage##uname(struct page *page) {  }

#define TESTSETFLAG_FALSE(uname)					\
static inline int TestSetPage##uname(struct page *page) { return 0; }

#define TESTCLEARFLAG_FALSE(uname)					\
static inline int TestClearPage##uname(struct page *page) { return 0; }

#define PAGEFLAG_FALSE(uname) TESTPAGEFLAG_FALSE(uname)			\
	SETPAGEFLAG_NOOP(uname) CLEARPAGEFLAG_NOOP(uname)

#define TESTSCFLAG_FALSE(uname)						\
	TESTSETFLAG_FALSE(uname) TESTCLEARFLAG_FALSE(uname)

__PAGEFLAG(Locked, locked, PF_NO_TAIL)
PAGEFLAG(Waiters, waiters, PF_ONLY_HEAD) __CLEARPAGEFLAG(Waiters, waiters, PF_ONLY_HEAD)
PAGEFLAG(Error, error, PF_NO_COMPOUND) TESTCLEARFLAG(Error, error, PF_NO_COMPOUND)
PAGEFLAG(Referenced, referenced, PF_HEAD)
	TESTCLEARFLAG(Referenced, referenced, PF_HEAD)
	__SETPAGEFLAG(Referenced, referenced, PF_HEAD)
PAGEFLAG(Dirty, dirty, PF_HEAD) TESTSCFLAG(Dirty, dirty, PF_HEAD)
	__CLEARPAGEFLAG(Dirty, dirty, PF_HEAD)
PAGEFLAG(LRU, lru, PF_HEAD) __CLEARPAGEFLAG(LRU, lru, PF_HEAD)
PAGEFLAG(Active, active, PF_HEAD) __CLEARPAGEFLAG(Active, active, PF_HEAD)
	TESTCLEARFLAG(Active, active, PF_HEAD)
PAGEFLAG(Workingset, workingset, PF_HEAD)
	TESTCLEARFLAG(Workingset, workingset, PF_HEAD)
__PAGEFLAG(Slab, slab, PF_NO_TAIL)
__PAGEFLAG(SlobFree, slob_free, PF_NO_TAIL)
PAGEFLAG(Checked, checked, PF_NO_COMPOUND)	   


PAGEFLAG(Pinned, pinned, PF_NO_COMPOUND)
	TESTSCFLAG(Pinned, pinned, PF_NO_COMPOUND)
PAGEFLAG(SavePinned, savepinned, PF_NO_COMPOUND);
PAGEFLAG(Foreign, foreign, PF_NO_COMPOUND);

PAGEFLAG(Reserved, reserved, PF_NO_COMPOUND)
	__CLEARPAGEFLAG(Reserved, reserved, PF_NO_COMPOUND)
PAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)
	__CLEARPAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)
	__SETPAGEFLAG(SwapBacked, swapbacked, PF_NO_TAIL)


PAGEFLAG(Private, private, PF_ANY) __SETPAGEFLAG(Private, private, PF_ANY)
	__CLEARPAGEFLAG(Private, private, PF_ANY)
PAGEFLAG(Private2, private_2, PF_ANY) TESTSCFLAG(Private2, private_2, PF_ANY)
PAGEFLAG(OwnerPriv1, owner_priv_1, PF_ANY)
	TESTCLEARFLAG(OwnerPriv1, owner_priv_1, PF_ANY)


TESTPAGEFLAG(Writeback, writeback, PF_NO_COMPOUND)
	TESTSCFLAG(Writeback, writeback, PF_NO_COMPOUND)
PAGEFLAG(MappedToDisk, mappedtodisk, PF_NO_TAIL)


PAGEFLAG(Reclaim, reclaim, PF_NO_TAIL)
	TESTCLEARFLAG(Reclaim, reclaim, PF_NO_TAIL)
PAGEFLAG(Readahead, reclaim, PF_NO_COMPOUND)
	TESTCLEARFLAG(Readahead, reclaim, PF_NO_COMPOUND)

#ifdef CONFIG_HIGHMEM

#define PageHighMem(__p) is_highmem_idx(page_zonenum(__p))
#else
PAGEFLAG_FALSE(HighMem)
#endif

#ifdef CONFIG_SWAP
PAGEFLAG(SwapCache, swapcache, PF_NO_COMPOUND)
#else
PAGEFLAG_FALSE(SwapCache)
#endif

PAGEFLAG(Unevictable, unevictable, PF_HEAD)
	__CLEARPAGEFLAG(Unevictable, unevictable, PF_HEAD)
	TESTCLEARFLAG(Unevictable, unevictable, PF_HEAD)

#ifdef CONFIG_MMU
PAGEFLAG(Mlocked, mlocked, PF_NO_TAIL)
	__CLEARPAGEFLAG(Mlocked, mlocked, PF_NO_TAIL)
	TESTSCFLAG(Mlocked, mlocked, PF_NO_TAIL)
#else
PAGEFLAG_FALSE(Mlocked) __CLEARPAGEFLAG_NOOP(Mlocked)
	TESTSCFLAG_FALSE(Mlocked)
#endif

#ifdef CONFIG_ARCH_USES_PG_UNCACHED
PAGEFLAG(Uncached, uncached, PF_NO_COMPOUND)
#else
PAGEFLAG_FALSE(Uncached)
#endif

#ifdef CONFIG_MEMORY_FAILURE
PAGEFLAG(HWPoison, hwpoison, PF_ANY)
TESTSCFLAG(HWPoison, hwpoison, PF_ANY)
#define __PG_HWPOISON (1UL << PG_hwpoison)
#else
PAGEFLAG_FALSE(HWPoison)
#define __PG_HWPOISON 0
#endif

#if defined(CONFIG_IDLE_PAGE_TRACKING) && defined(CONFIG_64BIT)
TESTPAGEFLAG(Young, young, PF_ANY)
SETPAGEFLAG(Young, young, PF_ANY)
TESTCLEARFLAG(Young, young, PF_ANY)
PAGEFLAG(Idle, idle, PF_ANY)
#endif


#define PAGE_MAPPING_ANON	0x1
#define PAGE_MAPPING_MOVABLE	0x2
#define PAGE_MAPPING_KSM	(PAGE_MAPPING_ANON | PAGE_MAPPING_MOVABLE)
#define PAGE_MAPPING_FLAGS	(PAGE_MAPPING_ANON | PAGE_MAPPING_MOVABLE)

static __always_inline int PageMappingFlags(struct page *page)
{
	return ((unsigned long)page->mapping & PAGE_MAPPING_FLAGS) != 0;
}

static __always_inline int PageAnon(struct page *page)
{
	page = compound_head(page);
	return ((unsigned long)page->mapping & PAGE_MAPPING_ANON) != 0;
}

static __always_inline int __PageMovable(struct page *page)
{
	return ((unsigned long)page->mapping & PAGE_MAPPING_FLAGS) ==
				PAGE_MAPPING_MOVABLE;
}

#ifdef CONFIG_KSM

static __always_inline int PageKsm(struct page *page)
{
	page = compound_head(page);
	return ((unsigned long)page->mapping & PAGE_MAPPING_FLAGS) ==
				PAGE_MAPPING_KSM;
}
#else
TESTPAGEFLAG_FALSE(Ksm)
#endif

u64 stable_page_flags(struct page *page);

static inline int PageUptodate(struct page *page)
{
	int ret;
	page = compound_head(page);
	ret = test_bit(PG_uptodate, &(page)->flags);
	
	if (ret)
		smp_rmb();

	return ret;
}

static __always_inline void __SetPageUptodate(struct page *page)
{
	VM_BUG_ON_PAGE(PageTail(page), page);
	smp_wmb();
	__set_bit(PG_uptodate, &page->flags);
}

static __always_inline void SetPageUptodate(struct page *page)
{
	VM_BUG_ON_PAGE(PageTail(page), page);
	
	smp_wmb();
	set_bit(PG_uptodate, &page->flags);
}

CLEARPAGEFLAG(Uptodate, uptodate, PF_NO_TAIL)

int test_clear_page_writeback(struct page *page);
int __test_set_page_writeback(struct page *page, bool keep_write);

#define test_set_page_writeback(page)			\
	__test_set_page_writeback(page, false)
#define test_set_page_writeback_keepwrite(page)	\
	__test_set_page_writeback(page, true)

static inline void set_page_writeback(struct page *page)
{
	test_set_page_writeback(page);
}

static inline void set_page_writeback_keepwrite(struct page *page)
{
	test_set_page_writeback_keepwrite(page);
}

__PAGEFLAG(Head, head, PF_ANY) CLEARPAGEFLAG(Head, head, PF_ANY)

static __always_inline void set_compound_head(struct page *page, struct page *head)
{
	WRITE_ONCE(page->compound_head, (unsigned long)head + 1);
}

static __always_inline void clear_compound_head(struct page *page)
{
	WRITE_ONCE(page->compound_head, 0);
}

#ifdef CONFIG_TRANSPARENT_HUGEPAGE
static inline void ClearPageCompound(struct page *page)
{
	BUG_ON(!PageHead(page));
	ClearPageHead(page);
}
#endif

#define PG_head_mask ((1UL << PG_head))

#ifdef CONFIG_HUGETLB_PAGE
int PageHuge(struct page *page);
int PageHeadHuge(struct page *page);
bool page_huge_active(struct page *page);
#else
TESTPAGEFLAG_FALSE(Huge)
TESTPAGEFLAG_FALSE(HeadHuge)

static inline bool page_huge_active(struct page *page)
{
	return 0;
}
#endif


#ifdef CONFIG_TRANSPARENT_HUGEPAGE

static inline int PageTransHuge(struct page *page)
{
	VM_BUG_ON_PAGE(PageTail(page), page);
	return PageHead(page);
}


static inline int PageTransCompound(struct page *page)
{
	return PageCompound(page);
}


static inline int PageTransCompoundMap(struct page *page)
{
	struct page *head;

	if (!PageTransCompound(page))
		return 0;

	if (PageAnon(page))
		return atomic_read(&page->_mapcount) < 0;

	head = compound_head(page);
	
	return atomic_read(&page->_mapcount) ==
	       atomic_read(compound_mapcount_ptr(head));
}


static inline int PageTransTail(struct page *page)
{
	return PageTail(page);
}


static inline int PageDoubleMap(struct page *page)
{
	return PageHead(page) && test_bit(PG_double_map, &page[1].flags);
}

static inline void SetPageDoubleMap(struct page *page)
{
	VM_BUG_ON_PAGE(!PageHead(page), page);
	set_bit(PG_double_map, &page[1].flags);
}

static inline void ClearPageDoubleMap(struct page *page)
{
	VM_BUG_ON_PAGE(!PageHead(page), page);
	clear_bit(PG_double_map, &page[1].flags);
}
static inline int TestSetPageDoubleMap(struct page *page)
{
	VM_BUG_ON_PAGE(!PageHead(page), page);
	return test_and_set_bit(PG_double_map, &page[1].flags);
}

static inline int TestClearPageDoubleMap(struct page *page)
{
	VM_BUG_ON_PAGE(!PageHead(page), page);
	return test_and_clear_bit(PG_double_map, &page[1].flags);
}

#else
TESTPAGEFLAG_FALSE(TransHuge)
TESTPAGEFLAG_FALSE(TransCompound)
TESTPAGEFLAG_FALSE(TransCompoundMap)
TESTPAGEFLAG_FALSE(TransTail)
PAGEFLAG_FALSE(DoubleMap)
	TESTSETFLAG_FALSE(DoubleMap)
	TESTCLEARFLAG_FALSE(DoubleMap)
#endif


#define PAGE_MAPCOUNT_OPS(uname, lname)					\
static __always_inline int Page##uname(struct page *page)		\
{									\
	return atomic_read(&page->_mapcount) ==				\
				PAGE_##lname##_MAPCOUNT_VALUE;		\
}									\
static __always_inline void __SetPage##uname(struct page *page)		\
{									\
	VM_BUG_ON_PAGE(atomic_read(&page->_mapcount) != -1, page);	\
	atomic_set(&page->_mapcount, PAGE_##lname##_MAPCOUNT_VALUE);	\
}									\
static __always_inline void __ClearPage##uname(struct page *page)	\
{									\
	VM_BUG_ON_PAGE(!Page##uname(page), page);			\
	atomic_set(&page->_mapcount, -1);				\
}


#define PAGE_BUDDY_MAPCOUNT_VALUE		(-128)
PAGE_MAPCOUNT_OPS(Buddy, BUDDY)


#define PAGE_BALLOON_MAPCOUNT_VALUE		(-256)
PAGE_MAPCOUNT_OPS(Balloon, BALLOON)


#define PAGE_KMEMCG_MAPCOUNT_VALUE		(-512)
PAGE_MAPCOUNT_OPS(Kmemcg, KMEMCG)

extern bool is_free_buddy_page(struct page *page);

__PAGEFLAG(Isolated, isolated, PF_ANY);


static inline int PageSlabPfmemalloc(struct page *page)
{
	VM_BUG_ON_PAGE(!PageSlab(page), page);
	return PageActive(page);
}

static inline void SetPageSlabPfmemalloc(struct page *page)
{
	VM_BUG_ON_PAGE(!PageSlab(page), page);
	SetPageActive(page);
}

static inline void __ClearPageSlabPfmemalloc(struct page *page)
{
	VM_BUG_ON_PAGE(!PageSlab(page), page);
	__ClearPageActive(page);
}

static inline void ClearPageSlabPfmemalloc(struct page *page)
{
	VM_BUG_ON_PAGE(!PageSlab(page), page);
	ClearPageActive(page);
}

#ifdef CONFIG_MMU
#define __PG_MLOCKED		(1UL << PG_mlocked)
#else
#define __PG_MLOCKED		0
#endif


#define PAGE_FLAGS_CHECK_AT_FREE \
	(1UL << PG_lru	 | 1UL << PG_locked    | \
	 1UL << PG_private | 1UL << PG_private_2 | \
	 1UL << PG_writeback | 1UL << PG_reserved | \
	 1UL << PG_slab	 | 1UL << PG_swapcache | 1UL << PG_active | \
	 1UL << PG_unevictable | __PG_MLOCKED)


#define PAGE_FLAGS_CHECK_AT_PREP	\
	(((1UL << NR_PAGEFLAGS) - 1) & ~__PG_HWPOISON)

#define PAGE_FLAGS_PRIVATE				\
	(1UL << PG_private | 1UL << PG_private_2)

static inline int page_has_private(struct page *page)
{
	return !!(page->flags & PAGE_FLAGS_PRIVATE);
}

#undef PF_ANY
#undef PF_HEAD
#undef PF_ONLY_HEAD
#undef PF_NO_TAIL
#undef PF_NO_COMPOUND
#endif 

#endif	
