

#ifndef _M2M1SHOT_H_
#define _M2M1SHOT_H_

#include <linux/kernel.h>
#include <linux/videodev2.h>
#include <linux/list.h>
#include <linux/miscdevice.h>
#include <linux/dma-direction.h>
#include <linux/kref.h>
#include <linux/completion.h>

#include <uapi/linux/m2m1shot.h>

struct m2m1shot_devops;
struct m2m1shot_task;


struct m2m1shot_device {
	struct miscdevice misc;
	struct list_head tasks;		
	struct list_head contexts;	
	struct device *dev;
	spinlock_t lock_task;		
	spinlock_t lock_ctx;		
	unsigned long timeout_jiffies;	
	struct m2m1shot_task *current_task; 
	const struct m2m1shot_devops *ops;
};


struct m2m1shot_context {
	struct list_head node;
	struct mutex mutex;
	struct kref kref;
	struct m2m1shot_device *m21dev;
	void *priv;
};


enum m2m1shot_state {
	M2M1SHOT_BUFSTATE_READY,
	M2M1SHOT_BUFSTATE_PROCESSING,
	M2M1SHOT_BUFSTATE_DONE,
	M2M1SHOT_BUFSTATE_TIMEDOUT,
	M2M1SHOT_BUFSTATE_ERROR,
};


struct m2m1shot_buffer_plane_dma {
	size_t bytes_used;
	struct dma_buf *dmabuf;
	struct dma_buf_attachment *attachment;
	struct sg_table *sgt;
	dma_addr_t dma_addr;
	void *priv;
	off_t offset;
};


struct m2m1shot_buffer_dma {
	 
	const struct m2m1shot_buffer *buffer;
	struct m2m1shot_buffer_plane_dma plane[M2M1SHOT_MAX_PLANES];
};


struct m2m1shot_task {
	struct m2m1shot task;
	struct list_head task_node;
	struct m2m1shot_context *ctx;
	struct completion complete;
	struct m2m1shot_buffer_dma dma_buf_out;
	struct m2m1shot_buffer_dma dma_buf_cap;
	enum m2m1shot_state state;
};


struct m2m1shot_devops {
	int (*init_context)(struct m2m1shot_context *ctx);
	int (*free_context)(struct m2m1shot_context *ctx);
	
	int (*prepare_format)(struct m2m1shot_context *ctx,
				struct m2m1shot_pix_format *fmt,
				enum dma_data_direction dir,
				size_t bytes_used[]);
	int (*prepare_operation)(struct m2m1shot_context *ctx,
				struct m2m1shot_task *task);
	int (*prepare_buffer)(struct m2m1shot_context *ctx,
				struct m2m1shot_buffer_dma *dma_buffer,
				int plane,
				enum dma_data_direction dir);
	void (*finish_buffer)(struct m2m1shot_context *ctx,
				struct m2m1shot_buffer_dma *dma_buffer,
				int plane,
				enum dma_data_direction dir);
	int (*device_run)(struct m2m1shot_context *ctx,
			struct m2m1shot_task *task);
	void (*timeout_task)(struct m2m1shot_context *ctx,
			struct m2m1shot_task *task);
	
	long (*custom_ioctl)(struct m2m1shot_context *ctx,
			unsigned int cmd, unsigned long arg);
};


void m2m1shot_task_finish(struct m2m1shot_device *m21dev,
				struct m2m1shot_task *task, bool error);


void m2m1shot_task_cancel(struct m2m1shot_device *m21dev,
				struct m2m1shot_task *task,
				enum m2m1shot_state reason);


struct m2m1shot_device *m2m1shot_create_device(struct device *dev,
					const struct m2m1shot_devops *ops,
					const char *suffix, int id,
					unsigned long timeout_jiffies);


void m2m1shot_destroy_device(struct m2m1shot_device *m21dev);


static inline struct m2m1shot_task *m2m1shot_get_current_task(
					struct m2m1shot_device *m21dev)
{
	return m21dev->current_task;
}


static inline void m2m1shot_set_dma_address(
		struct m2m1shot_buffer_dma *buffer_dma,
		int plane, dma_addr_t dma_addr)
{
	buffer_dma->plane[plane].dma_addr = dma_addr;
}

#endif 
