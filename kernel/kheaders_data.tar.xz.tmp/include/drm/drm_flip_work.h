

#ifndef DRM_FLIP_WORK_H
#define DRM_FLIP_WORK_H

#include <linux/kfifo.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>



struct drm_flip_work;


typedef void (*drm_flip_func_t)(struct drm_flip_work *work, void *val);


struct drm_flip_task {
	struct list_head node;
	void *data;
};


struct drm_flip_work {
	const char *name;
	drm_flip_func_t func;
	struct work_struct worker;
	struct list_head queued;
	struct list_head commited;
	spinlock_t lock;
};

struct drm_flip_task *drm_flip_work_allocate_task(void *data, gfp_t flags);
void drm_flip_work_queue_task(struct drm_flip_work *work,
			      struct drm_flip_task *task);
void drm_flip_work_queue(struct drm_flip_work *work, void *val);
void drm_flip_work_commit(struct drm_flip_work *work,
		struct workqueue_struct *wq);
void drm_flip_work_init(struct drm_flip_work *work,
		const char *name, drm_flip_func_t func);
void drm_flip_work_cleanup(struct drm_flip_work *work);

#endif  
