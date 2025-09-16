
#ifndef _LINUX_BALLOON_COMPACTION_H
#define _LINUX_BALLOON_COMPACTION_H
#include <linux/pagemap.h>
#include <linux/page-flags.h>
#include <linux/migrate.h>
#include <linux/gfp.h>
#include <linux/err.h>
#include <linux/fs.h>


struct balloon_dev_info {
	unsigned long isolated_pages;	
	spinlock_t pages_lock;		
	struct list_head pages;		
	int (*migratepage)(struct balloon_dev_info *, struct page *newpage,
			struct page *page, enum migrate_mode mode);
	struct inode *inode;
};

extern struct page *balloon_page_enqueue(struct balloon_dev_info *b_dev_info);
extern struct page *balloon_page_dequeue(struct balloon_dev_info *b_dev_info);

static inline void balloon_devinfo_init(struct balloon_dev_info *balloon)
{
	balloon->isolated_pages = 0;
	spin_lock_init(&balloon->pages_lock);
	INIT_LIST_HEAD(&balloon->pages);
	balloon->migratepage = NULL;
	balloon->inode = NULL;
}

#ifdef CONFIG_BALLOON_COMPACTION
extern const struct address_space_operations balloon_aops;
extern bool balloon_page_isolate(struct page *page,
				isolate_mode_t mode);
extern void balloon_page_putback(struct page *page);
extern int balloon_page_migrate(struct address_space *mapping,
				struct page *newpage,
				struct page *page, enum migrate_mode mode);


static inline void balloon_page_insert(struct balloon_dev_info *balloon,
				       struct page *page)
{
	__SetPageBalloon(page);
	__SetPageMovable(page, balloon->inode->i_mapping);
	set_page_private(page, (unsigned long)balloon);
	list_add(&page->lru, &balloon->pages);
}


static inline void balloon_page_delete(struct page *page)
{
	__ClearPageBalloon(page);
	__ClearPageMovable(page);
	set_page_private(page, 0);
	
	if (!PageIsolated(page))
		list_del(&page->lru);
}


static inline struct balloon_dev_info *balloon_page_device(struct page *page)
{
	return (struct balloon_dev_info *)page_private(page);
}

static inline gfp_t balloon_mapping_gfp_mask(void)
{
	return GFP_HIGHUSER_MOVABLE;
}

#else 

static inline void balloon_page_insert(struct balloon_dev_info *balloon,
				       struct page *page)
{
	__SetPageBalloon(page);
	list_add(&page->lru, &balloon->pages);
}

static inline void balloon_page_delete(struct page *page)
{
	__ClearPageBalloon(page);
	list_del(&page->lru);
}

static inline bool __is_movable_balloon_page(struct page *page)
{
	return false;
}

static inline bool balloon_page_movable(struct page *page)
{
	return false;
}

static inline bool isolated_balloon_page(struct page *page)
{
	return false;
}

static inline bool balloon_page_isolate(struct page *page)
{
	return false;
}

static inline void balloon_page_putback(struct page *page)
{
	return;
}

static inline int balloon_page_migrate(struct page *newpage,
				struct page *page, enum migrate_mode mode)
{
	return 0;
}

static inline gfp_t balloon_mapping_gfp_mask(void)
{
	return GFP_HIGHUSER;
}

#endif 
#endif 
