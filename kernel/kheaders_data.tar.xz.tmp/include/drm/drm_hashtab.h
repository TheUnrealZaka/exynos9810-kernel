


#ifndef DRM_HASHTAB_H
#define DRM_HASHTAB_H

#include <linux/list.h>

#define drm_hash_entry(_ptr, _type, _member) container_of(_ptr, _type, _member)

struct drm_hash_item {
	struct hlist_node head;
	unsigned long key;
};

struct drm_open_hash {
	struct hlist_head *table;
	u8 order;
};

extern int drm_ht_create(struct drm_open_hash *ht, unsigned int order);
extern int drm_ht_insert_item(struct drm_open_hash *ht, struct drm_hash_item *item);
extern int drm_ht_just_insert_please(struct drm_open_hash *ht, struct drm_hash_item *item,
				     unsigned long seed, int bits, int shift,
				     unsigned long add);
extern int drm_ht_find_item(struct drm_open_hash *ht, unsigned long key, struct drm_hash_item **item);

extern void drm_ht_verbose_list(struct drm_open_hash *ht, unsigned long key);
extern int drm_ht_remove_key(struct drm_open_hash *ht, unsigned long key);
extern int drm_ht_remove_item(struct drm_open_hash *ht, struct drm_hash_item *item);
extern void drm_ht_remove(struct drm_open_hash *ht);


#define drm_ht_insert_item_rcu drm_ht_insert_item
#define drm_ht_just_insert_please_rcu drm_ht_just_insert_please
#define drm_ht_remove_key_rcu drm_ht_remove_key
#define drm_ht_remove_item_rcu drm_ht_remove_item
#define drm_ht_find_item_rcu drm_ht_find_item

#endif
