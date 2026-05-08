


#ifndef _DRM_MM_H_
#define _DRM_MM_H_


#include <linux/bug.h>
#include <linux/rbtree.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#ifdef CONFIG_DEBUG_FS
#include <linux/seq_file.h>
#endif

enum drm_mm_search_flags {
	DRM_MM_SEARCH_DEFAULT =		0,
	DRM_MM_SEARCH_BEST =		1 << 0,
	DRM_MM_SEARCH_BELOW =		1 << 1,
};

enum drm_mm_allocator_flags {
	DRM_MM_CREATE_DEFAULT =		0,
	DRM_MM_CREATE_TOP =		1 << 0,
};

#define DRM_MM_BOTTOMUP DRM_MM_SEARCH_DEFAULT, DRM_MM_CREATE_DEFAULT
#define DRM_MM_TOPDOWN DRM_MM_SEARCH_BELOW, DRM_MM_CREATE_TOP

struct drm_mm_node {
	struct list_head node_list;
	struct list_head hole_stack;
	struct rb_node rb;
	unsigned hole_follows : 1;
	unsigned scanned_block : 1;
	unsigned scanned_prev_free : 1;
	unsigned scanned_next_free : 1;
	unsigned scanned_preceeds_hole : 1;
	unsigned allocated : 1;
	unsigned long color;
	u64 start;
	u64 size;
	u64 __subtree_last;
	struct drm_mm *mm;
};

struct drm_mm {
	
	struct list_head hole_stack;
	
	struct drm_mm_node head_node;
	
	struct rb_root interval_tree;

	unsigned int scan_check_range : 1;
	unsigned scan_alignment;
	unsigned long scan_color;
	u64 scan_size;
	u64 scan_hit_start;
	u64 scan_hit_end;
	unsigned scanned_blocks;
	u64 scan_start;
	u64 scan_end;
	struct drm_mm_node *prev_scanned_node;

	void (*color_adjust)(struct drm_mm_node *node, unsigned long color,
			     u64 *start, u64 *end);
};


static inline bool drm_mm_node_allocated(struct drm_mm_node *node)
{
	return node->allocated;
}


static inline bool drm_mm_initialized(struct drm_mm *mm)
{
	return mm->hole_stack.next;
}

static inline u64 __drm_mm_hole_node_start(struct drm_mm_node *hole_node)
{
	return hole_node->start + hole_node->size;
}


static inline u64 drm_mm_hole_node_start(struct drm_mm_node *hole_node)
{
	BUG_ON(!hole_node->hole_follows);
	return __drm_mm_hole_node_start(hole_node);
}

static inline u64 __drm_mm_hole_node_end(struct drm_mm_node *hole_node)
{
	return list_next_entry(hole_node, node_list)->start;
}


static inline u64 drm_mm_hole_node_end(struct drm_mm_node *hole_node)
{
	return __drm_mm_hole_node_end(hole_node);
}


#define drm_mm_for_each_node(entry, mm) list_for_each_entry(entry, \
						&(mm)->head_node.node_list, \
						node_list)

#define __drm_mm_for_each_hole(entry, mm, hole_start, hole_end, backwards) \
	for (entry = list_entry((backwards) ? (mm)->hole_stack.prev : (mm)->hole_stack.next, struct drm_mm_node, hole_stack); \
	     &entry->hole_stack != &(mm)->hole_stack ? \
	     hole_start = drm_mm_hole_node_start(entry), \
	     hole_end = drm_mm_hole_node_end(entry), \
	     1 : 0; \
	     entry = list_entry((backwards) ? entry->hole_stack.prev : entry->hole_stack.next, struct drm_mm_node, hole_stack))


#define drm_mm_for_each_hole(entry, mm, hole_start, hole_end) \
	__drm_mm_for_each_hole(entry, mm, hole_start, hole_end, 0)


int drm_mm_reserve_node(struct drm_mm *mm, struct drm_mm_node *node);

int drm_mm_insert_node_generic(struct drm_mm *mm,
			       struct drm_mm_node *node,
			       u64 size,
			       unsigned alignment,
			       unsigned long color,
			       enum drm_mm_search_flags sflags,
			       enum drm_mm_allocator_flags aflags);

static inline int drm_mm_insert_node(struct drm_mm *mm,
				     struct drm_mm_node *node,
				     u64 size,
				     unsigned alignment,
				     enum drm_mm_search_flags flags)
{
	return drm_mm_insert_node_generic(mm, node, size, alignment, 0, flags,
					  DRM_MM_CREATE_DEFAULT);
}

int drm_mm_insert_node_in_range_generic(struct drm_mm *mm,
					struct drm_mm_node *node,
					u64 size,
					unsigned alignment,
					unsigned long color,
					u64 start,
					u64 end,
					enum drm_mm_search_flags sflags,
					enum drm_mm_allocator_flags aflags);

static inline int drm_mm_insert_node_in_range(struct drm_mm *mm,
					      struct drm_mm_node *node,
					      u64 size,
					      unsigned alignment,
					      u64 start,
					      u64 end,
					      enum drm_mm_search_flags flags)
{
	return drm_mm_insert_node_in_range_generic(mm, node, size, alignment,
						   0, start, end, flags,
						   DRM_MM_CREATE_DEFAULT);
}

void drm_mm_remove_node(struct drm_mm_node *node);
void drm_mm_replace_node(struct drm_mm_node *old, struct drm_mm_node *new);
void drm_mm_init(struct drm_mm *mm,
		 u64 start,
		 u64 size);
void drm_mm_takedown(struct drm_mm *mm);
bool drm_mm_clean(struct drm_mm *mm);

struct drm_mm_node *
drm_mm_interval_first(struct drm_mm *mm, u64 start, u64 last);

struct drm_mm_node *
drm_mm_interval_next(struct drm_mm_node *node, u64 start, u64 last);

void drm_mm_init_scan(struct drm_mm *mm,
		      u64 size,
		      unsigned alignment,
		      unsigned long color);
void drm_mm_init_scan_with_range(struct drm_mm *mm,
				 u64 size,
				 unsigned alignment,
				 unsigned long color,
				 u64 start,
				 u64 end);
bool drm_mm_scan_add_block(struct drm_mm_node *node);
bool drm_mm_scan_remove_block(struct drm_mm_node *node);

void drm_mm_debug_table(struct drm_mm *mm, const char *prefix);
#ifdef CONFIG_DEBUG_FS
int drm_mm_dump_table(struct seq_file *m, struct drm_mm *mm);
#endif

#endif
