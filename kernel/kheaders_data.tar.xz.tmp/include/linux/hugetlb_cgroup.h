

#ifndef _LINUX_HUGETLB_CGROUP_H
#define _LINUX_HUGETLB_CGROUP_H

#include <linux/mmdebug.h>

struct hugetlb_cgroup;

#define HUGETLB_CGROUP_MIN_ORDER	2

#ifdef CONFIG_CGROUP_HUGETLB

static inline struct hugetlb_cgroup *hugetlb_cgroup_from_page(struct page *page)
{
	VM_BUG_ON_PAGE(!PageHuge(page), page);

	if (compound_order(page) < HUGETLB_CGROUP_MIN_ORDER)
		return NULL;
	return (struct hugetlb_cgroup *)page[2].private;
}

static inline
int set_hugetlb_cgroup(struct page *page, struct hugetlb_cgroup *h_cg)
{
	VM_BUG_ON_PAGE(!PageHuge(page), page);

	if (compound_order(page) < HUGETLB_CGROUP_MIN_ORDER)
		return -1;
	page[2].private	= (unsigned long)h_cg;
	return 0;
}

static inline bool hugetlb_cgroup_disabled(void)
{
	return !cgroup_subsys_enabled(hugetlb_cgrp_subsys);
}

extern int hugetlb_cgroup_charge_cgroup(int idx, unsigned long nr_pages,
					struct hugetlb_cgroup **ptr);
extern void hugetlb_cgroup_commit_charge(int idx, unsigned long nr_pages,
					 struct hugetlb_cgroup *h_cg,
					 struct page *page);
extern void hugetlb_cgroup_uncharge_page(int idx, unsigned long nr_pages,
					 struct page *page);
extern void hugetlb_cgroup_uncharge_cgroup(int idx, unsigned long nr_pages,
					   struct hugetlb_cgroup *h_cg);
extern void hugetlb_cgroup_file_init(void) __init;
extern void hugetlb_cgroup_migrate(struct page *oldhpage,
				   struct page *newhpage);

#else
static inline struct hugetlb_cgroup *hugetlb_cgroup_from_page(struct page *page)
{
	return NULL;
}

static inline
int set_hugetlb_cgroup(struct page *page, struct hugetlb_cgroup *h_cg)
{
	return 0;
}

static inline bool hugetlb_cgroup_disabled(void)
{
	return true;
}

static inline int
hugetlb_cgroup_charge_cgroup(int idx, unsigned long nr_pages,
			     struct hugetlb_cgroup **ptr)
{
	return 0;
}

static inline void
hugetlb_cgroup_commit_charge(int idx, unsigned long nr_pages,
			     struct hugetlb_cgroup *h_cg,
			     struct page *page)
{
}

static inline void
hugetlb_cgroup_uncharge_page(int idx, unsigned long nr_pages, struct page *page)
{
}

static inline void
hugetlb_cgroup_uncharge_cgroup(int idx, unsigned long nr_pages,
			       struct hugetlb_cgroup *h_cg)
{
}

static inline void hugetlb_cgroup_file_init(void)
{
}

static inline void hugetlb_cgroup_migrate(struct page *oldhpage,
					  struct page *newhpage)
{
}

#endif  
#endif
