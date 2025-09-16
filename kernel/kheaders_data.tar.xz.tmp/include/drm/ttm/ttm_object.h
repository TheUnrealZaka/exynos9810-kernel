



#ifndef _TTM_OBJECT_H_
#define _TTM_OBJECT_H_

#include <linux/list.h>
#include <drm/drm_hashtab.h>
#include <linux/kref.h>
#include <linux/rcupdate.h>
#include <linux/dma-buf.h>
#include <ttm/ttm_memory.h>



enum ttm_ref_type {
	TTM_REF_USAGE,
	TTM_REF_SYNCCPU_READ,
	TTM_REF_SYNCCPU_WRITE,
	TTM_REF_NUM
};



enum ttm_object_type {
	ttm_fence_type,
	ttm_buffer_type,
	ttm_lock_type,
	ttm_prime_type,
	ttm_driver_type0 = 256,
	ttm_driver_type1,
	ttm_driver_type2,
	ttm_driver_type3,
	ttm_driver_type4,
	ttm_driver_type5
};

struct ttm_object_file;
struct ttm_object_device;



struct ttm_base_object {
	struct rcu_head rhead;
	struct drm_hash_item hash;
	enum ttm_object_type object_type;
	bool shareable;
	struct ttm_object_file *tfile;
	struct kref refcount;
	void (*refcount_release) (struct ttm_base_object **base);
	void (*ref_obj_release) (struct ttm_base_object *base,
				 enum ttm_ref_type ref_type);
};




struct ttm_prime_object {
	struct ttm_base_object base;
	struct mutex mutex;
	size_t size;
	enum ttm_object_type real_type;
	struct dma_buf *dma_buf;
	void (*refcount_release) (struct ttm_base_object **);
};



extern int ttm_base_object_init(struct ttm_object_file *tfile,
				struct ttm_base_object *base,
				bool shareable,
				enum ttm_object_type type,
				void (*refcount_release) (struct ttm_base_object
							  **),
				void (*ref_obj_release) (struct ttm_base_object
							 *,
							 enum ttm_ref_type
							 ref_type));



extern struct ttm_base_object *ttm_base_object_lookup(struct ttm_object_file
						      *tfile, uint32_t key);



extern struct ttm_base_object *
ttm_base_object_lookup_for_ref(struct ttm_object_device *tdev, uint32_t key);



extern void ttm_base_object_unref(struct ttm_base_object **p_base);


extern int ttm_ref_object_add(struct ttm_object_file *tfile,
			      struct ttm_base_object *base,
			      enum ttm_ref_type ref_type, bool *existed,
			      bool require_existed);

extern bool ttm_ref_object_exists(struct ttm_object_file *tfile,
				  struct ttm_base_object *base);


extern int ttm_ref_object_base_unref(struct ttm_object_file *tfile,
				     unsigned long key,
				     enum ttm_ref_type ref_type);



extern struct ttm_object_file *ttm_object_file_init(struct ttm_object_device
						    *tdev,
						    unsigned int hash_order);



extern void ttm_object_file_release(struct ttm_object_file **p_tfile);



extern struct ttm_object_device *
ttm_object_device_init(struct ttm_mem_global *mem_glob,
		       unsigned int hash_order,
		       const struct dma_buf_ops *ops);



extern void ttm_object_device_release(struct ttm_object_device **p_tdev);

#define ttm_base_object_kfree(__object, __base)\
	kfree_rcu(__object, __base.rhead)

extern int ttm_prime_object_init(struct ttm_object_file *tfile,
				 size_t size,
				 struct ttm_prime_object *prime,
				 bool shareable,
				 enum ttm_object_type type,
				 void (*refcount_release)
				 (struct ttm_base_object **),
				 void (*ref_obj_release)
				 (struct ttm_base_object *,
				  enum ttm_ref_type ref_type));

static inline enum ttm_object_type
ttm_base_object_type(struct ttm_base_object *base)
{
	return (base->object_type == ttm_prime_type) ?
		container_of(base, struct ttm_prime_object, base)->real_type :
		base->object_type;
}
extern int ttm_prime_fd_to_handle(struct ttm_object_file *tfile,
				  int fd, u32 *handle);
extern int ttm_prime_handle_to_fd(struct ttm_object_file *tfile,
				  uint32_t handle, uint32_t flags,
				  int *prime_fd);

#define ttm_prime_object_kfree(__obj, __prime)		\
	kfree_rcu(__obj, __prime.base.rhead)
#endif
