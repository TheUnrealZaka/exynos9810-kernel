

#ifndef _M2M1SHOT2_H_
#define _M2M1SHOT2_H_

#include <linux/kernel.h>
#include <linux/videodev2.h>
#include <linux/list.h>
#include <linux/miscdevice.h>
#include <linux/dma-direction.h>
#include <linux/kref.h>
#include <linux/completion.h>
#include <linux/dma-buf.h>

#include <uapi/linux/m2m1shot2.h>

#include "../../drivers/dma-buf/sync_debug.h"

struct m2m1shot2_device;
struct m2m1shot2_context;


struct m2m1shot2_dma_buffer {
	u32				payload;
	union {
		struct {
			u32				offset;
			struct dma_buf			*dmabuf;
			struct dma_buf_attachment	*attachment;
		} dmabuf;
		struct {
			u32				length;
			unsigned long			addr;
			struct vm_area_struct		*vma;
		} userptr;
	};
	struct sg_table			*sgt;
	dma_addr_t			dma_addr;
	void				*priv;
};


struct m2m1shot2_context_format {
	struct m2m1shot2_format fmt;
	void *priv;
	u32 colorspace;
};


struct m2m1shot2_context_image {
	unsigned int			index;
	u32				flags;
	struct m2m1shot2_context_format	fmt;

	u32				memory;
	unsigned int			num_planes;
	struct m2m1shot2_dma_buffer	plane[M2M1SHOT2_MAX_PLANES];

	struct sync_file_waiter	waiter;
	struct sync_file		*fence;
};


struct m2m1shot2_source_image {
	struct m2m1shot2_context_image	img;
	struct m2m1shot2_extra		ext;
};



struct m2m1shot2_devops {
	int (*init_context)(struct m2m1shot2_context *ctx);
	int (*free_context)(struct m2m1shot2_context *ctx);
	int (*prepare_format)(struct m2m1shot2_context_format *fmt,
				unsigned int index,
				enum dma_data_direction dir,
				size_t payload[],
				unsigned int *num_planes);
	int (*prepare_source)(struct m2m1shot2_context *ctx,
				unsigned int index,
				struct m2m1shot2_source_image *img);
	int (*prepare_target)(struct m2m1shot2_context *ctx,
				struct m2m1shot2_context_image *img);
	int (*prepare_perf)(struct m2m1shot2_context *ctx,
				struct m2m1shot2_performance_data *data);
	int (*device_run)(struct m2m1shot2_context *ctx);
	int (*custom_ioctl)(struct m2m1shot2_context *ctx,
				unsigned int cmd,
				unsigned int arg);
};




#define M2M1S2_CTXSTATE_PROCESSING		0

#define M2M1S2_CTXSTATE_PENDING			1

#define M2M1S2_CTXSTATE_WAITING			2

#define M2M1S2_CTXSTATE_PROCESSED		8

#define M2M1S2_CTXSTATE_ERROR			9


#define M2M1S2_CTXSTATE_CACHECLEANALL		16

#define M2M1S2_CTXSTATE_CACHEINVALALL		17

#define M2M1S2_CTXSTATE_CACHEFLUSH		18
#define M2M1S2_CTXSTATE_CACHEFLUSHALL		19

#define M2M1S2_CTXSTATE_IDLE(ctx)		(((ctx)->state & 0xFF) == 0)

#define M2M1S2_CTXSTATE_BUSY(ctx)		(((ctx)->state & 0x3) != 0)

#define BTS_PEAK_FPS_RATIO 1667

#define M2M1S2_TIMEOUT_INTERVAL		1000

struct m2m1shot2_context {
	struct list_head		node;
	unsigned long			state;
	struct kref			starter;
	struct m2m1shot2_device		*m21dev;
	void				*priv;
	struct mutex			mutex;
	struct completion		complete;
	struct work_struct		work;
	struct work_struct		dwork;

	unsigned int			flags;
	unsigned int			num_sources;
	struct m2m1shot2_source_image	source[M2M1SHOT2_MAX_IMAGES];
	struct m2m1shot2_context_image	target;

	struct sync_timeline		*timeline;
	enum m2m1shot2_priority		priority;
	u32				timeline_max;
	u32				ctx_private;
	u32				ctx_private2;
	u64				work_delay_in_nsec;
	struct timer_list		timer;
	struct sync_file		*release_fence;
	spinlock_t			fence_timeout_lock;
};

#define M2M1SHOT2_DEVATTR_COHERENT		(1 << 0)

struct m2m1shot2_device {
	struct miscdevice		misc;
	unsigned long			attr;
	struct device			*dev;
	struct list_head		contexts;
	struct list_head		active_contexts;
	struct m2m1shot2_context	*current_ctx;
	spinlock_t			lock_ctx;
	spinlock_t			lock_priority;
	const struct m2m1shot2_devops	*ops;
	struct workqueue_struct		*schedule_workqueue;
	struct workqueue_struct		*destroy_workqueue;
	unsigned char			prior_stats[M2M1SHOT2_PRIORITY_END];
	struct mutex			lock_qos;
	struct list_head		qos_contexts;
};

#ifdef CONFIG_MEDIA_M2M1SHOT2
struct m2m1shot2_context *m2m1shot2_current_context(
				const struct m2m1shot2_device *m21dev);
struct m2m1shot2_device *m2m1shot2_create_device(struct device *dev,
				const struct m2m1shot2_devops *ops,
			const char *nodename, int id, unsigned long attr);
void m2m1shot2_destroy_device(struct m2m1shot2_device *m21dev);
void m2m1shot2_finish_context(struct m2m1shot2_context *ctx, bool success);

static inline u32 m2m1shot2_get_payload(struct m2m1shot2_context *ctx,
					unsigned int index, unsigned int plane)
{
	BUG_ON(index >= M2M1SHOT2_MAX_IMAGES);
	BUG_ON(plane >= ctx->source[index].img.num_planes);

	return ctx->source[index].img.plane[plane].payload;
}

static inline void m2m1shot2_set_payload(struct m2m1shot2_context *ctx,
					unsigned int plane, u32 payload)
{
	BUG_ON(plane >= ctx->target.num_planes);
	BUG_ON((ctx->target.memory == M2M1SHOT2_BUFTYPE_DMABUF)
		&& (payload > ctx->target.plane[plane].dmabuf.dmabuf->size));
	BUG_ON((ctx->target.memory == M2M1SHOT2_BUFTYPE_USERPTR)
		&& (payload > ctx->target.plane[plane].userptr.length));

	ctx->target.plane[plane].payload = payload;
}

static inline dma_addr_t m2m1shot2_src_dma_addr(struct m2m1shot2_context *ctx,
					unsigned int index, unsigned int plane)
{
	BUG_ON(index >= M2M1SHOT2_MAX_IMAGES);
	BUG_ON(plane >= ctx->source[index].img.num_planes);

	return ctx->source[index].img.plane[plane].dma_addr;
}

static inline dma_addr_t m2m1shot2_dst_dma_addr(struct m2m1shot2_context *ctx,
						unsigned int plane)
{
	BUG_ON(plane >= ctx->target.num_planes);

	return ctx->target.plane[plane].dma_addr;
}

static inline struct m2m1shot2_context_format *m2m1shot2_src_format(
			struct m2m1shot2_context *ctx, unsigned int index)
{
	BUG_ON(index >= M2M1SHOT2_MAX_IMAGES);

	return &ctx->source[index].img.fmt;
}

static inline struct m2m1shot2_context_format *m2m1shot2_dst_format(
					struct m2m1shot2_context *ctx)
{
	return &ctx->target.fmt;
}

#else 
static inline struct m2m1shot2_context *m2m1shot2_current_context(
				const struct m2m1shot2_device *m21dev)
{
	return NULL;
}
static inline struct m2m1shot2_device *m2m1shot2_create_device(
		struct device *dev, const struct m2m1shot2_devops *ops,
		const char *nodename, int id, unsigned long attr)
{
	return NULL;
}
#define m2m1shot2_destroy_device(m21dev)		do { } while (0)
#define m2m1shot2_finish_context(ctx, success)		do { } while (0)
static inline u32 m2m1shot2_get_payload(struct m2m1shot2_context *ctx,
					unsigned int index, unsigned int plane)
{
	return 0;
}

#define m2m1shot2_set_payload(ctx, plane, payload)	do { } while (0)
static inline dma_addr_t m2m1shot2_src_dma_addr(struct m2m1shot2_context *ctx,
					unsigned int index, unsigned int plane)
{
	return 0;
}

static inline dma_addr_t m2m1shot2_dst_dma_addr(struct m2m1shot2_context *ctx,
						unsigned int plane)
{
	return 0;
}
#endif 

#endif 
