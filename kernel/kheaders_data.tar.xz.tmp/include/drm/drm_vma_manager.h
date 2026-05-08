#ifndef __DRM_VMA_MANAGER_H__
#define __DRM_VMA_MANAGER_H__



#include <drm/drm_mm.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/rbtree.h>
#include <linux/spinlock.h>
#include <linux/types.h>

struct drm_file;

struct drm_vma_offset_file {
	struct rb_node vm_rb;
	struct drm_file *vm_tag;
	unsigned long vm_count;
};

struct drm_vma_offset_node {
	rwlock_t vm_lock;
	struct drm_mm_node vm_node;
	struct rb_root vm_files;
	bool readonly:1;
};

struct drm_vma_offset_manager {
	rwlock_t vm_lock;
	struct drm_mm vm_addr_space_mm;
};

void drm_vma_offset_manager_init(struct drm_vma_offset_manager *mgr,
				 unsigned long page_offset, unsigned long size);
void drm_vma_offset_manager_destroy(struct drm_vma_offset_manager *mgr);

struct drm_vma_offset_node *drm_vma_offset_lookup_locked(struct drm_vma_offset_manager *mgr,
							   unsigned long start,
							   unsigned long pages);
int drm_vma_offset_add(struct drm_vma_offset_manager *mgr,
		       struct drm_vma_offset_node *node, unsigned long pages);
void drm_vma_offset_remove(struct drm_vma_offset_manager *mgr,
			   struct drm_vma_offset_node *node);

int drm_vma_node_allow(struct drm_vma_offset_node *node, struct drm_file *tag);
void drm_vma_node_revoke(struct drm_vma_offset_node *node,
			 struct drm_file *tag);
bool drm_vma_node_is_allowed(struct drm_vma_offset_node *node,
			     struct drm_file *tag);


static inline struct drm_vma_offset_node *
drm_vma_offset_exact_lookup_locked(struct drm_vma_offset_manager *mgr,
				   unsigned long start,
				   unsigned long pages)
{
	struct drm_vma_offset_node *node;

	node = drm_vma_offset_lookup_locked(mgr, start, pages);
	return (node && node->vm_node.start == start) ? node : NULL;
}


static inline void drm_vma_offset_lock_lookup(struct drm_vma_offset_manager *mgr)
{
	read_lock(&mgr->vm_lock);
}


static inline void drm_vma_offset_unlock_lookup(struct drm_vma_offset_manager *mgr)
{
	read_unlock(&mgr->vm_lock);
}


static inline void drm_vma_node_reset(struct drm_vma_offset_node *node)
{
	memset(node, 0, sizeof(*node));
	node->vm_files = RB_ROOT;
	rwlock_init(&node->vm_lock);
}


static inline unsigned long drm_vma_node_start(struct drm_vma_offset_node *node)
{
	return node->vm_node.start;
}


static inline unsigned long drm_vma_node_size(struct drm_vma_offset_node *node)
{
	return node->vm_node.size;
}


static inline __u64 drm_vma_node_offset_addr(struct drm_vma_offset_node *node)
{
	return ((__u64)node->vm_node.start) << PAGE_SHIFT;
}


static inline void drm_vma_node_unmap(struct drm_vma_offset_node *node,
				      struct address_space *file_mapping)
{
	if (drm_mm_node_allocated(&node->vm_node))
		unmap_mapping_range(file_mapping,
				    drm_vma_node_offset_addr(node),
				    drm_vma_node_size(node) << PAGE_SHIFT, 1);
}


static inline int drm_vma_node_verify_access(struct drm_vma_offset_node *node,
					     struct drm_file *tag)
{
	return drm_vma_node_is_allowed(node, tag) ? 0 : -EACCES;
}

#endif 
