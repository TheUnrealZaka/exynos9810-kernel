
#ifndef _MEDIA_VIDEOBUF2_CORE_H
#define _MEDIA_VIDEOBUF2_CORE_H

#include <linux/mm_types.h>
#include <linux/mutex.h>
#include <linux/poll.h>
#include <linux/dma-buf.h>
#include <linux/sync_file.h>

#include "../../drivers/dma-buf/sync_debug.h"

#define VB2_MAX_FRAME	(32)
#define VB2_MAX_PLANES	(8)


enum vb2_memory {
	VB2_MEMORY_UNKNOWN	= 0,
	VB2_MEMORY_MMAP		= 1,
	VB2_MEMORY_USERPTR	= 2,
	VB2_MEMORY_DMABUF	= 4,
};

struct vb2_fileio_data;
struct vb2_threadio_data;


struct vb2_mem_ops {
	void		*(*alloc)(struct device *dev, unsigned long attrs,
				  unsigned long size,
				  enum dma_data_direction dma_dir,
				  gfp_t gfp_flags);
	void		(*put)(void *buf_priv);
	struct dma_buf *(*get_dmabuf)(void *buf_priv, unsigned long flags);

	void		*(*get_userptr)(struct device *dev, unsigned long vaddr,
					unsigned long size,
					enum dma_data_direction dma_dir);
	void		(*put_userptr)(void *buf_priv);

	void		(*prepare)(void *buf_priv);
	void		(*finish)(void *buf_priv);

	void		*(*attach_dmabuf)(struct device *dev,
					  struct dma_buf *dbuf,
					  unsigned long size,
					  enum dma_data_direction dma_dir);
	void		(*detach_dmabuf)(void *buf_priv);
	int		(*map_dmabuf)(void *buf_priv);
	void		(*unmap_dmabuf)(void *buf_priv);

	void		*(*vaddr)(void *buf_priv);
	void		*(*cookie)(void *buf_priv);

	unsigned int	(*num_users)(void *buf_priv);

	int		(*mmap)(void *buf_priv, struct vm_area_struct *vma);
};


struct vb2_plane {
	void			*mem_priv;
	struct dma_buf		*dbuf;
	unsigned int		dbuf_mapped;
	unsigned int		bytesused;
	unsigned int		length;
	unsigned int		min_length;
	union {
		unsigned int	offset;
		unsigned long	userptr;
		int		fd;
	} m;
	unsigned int		data_offset;
};


enum vb2_io_modes {
	VB2_MMAP	= (1 << 0),
	VB2_USERPTR	= (1 << 1),
	VB2_READ	= (1 << 2),
	VB2_WRITE	= (1 << 3),
	VB2_DMABUF	= (1 << 4),
};


enum vb2_buffer_state {
	VB2_BUF_STATE_DEQUEUED,
	VB2_BUF_STATE_PREPARING,
	VB2_BUF_STATE_PREPARED,
	VB2_BUF_STATE_QUEUED,
	VB2_BUF_STATE_REQUEUEING,
	VB2_BUF_STATE_ACTIVE,
	VB2_BUF_STATE_DONE,
	VB2_BUF_STATE_ERROR,
};

struct vb2_queue;


struct vb2_buffer {
	struct vb2_queue	*vb2_queue;
	unsigned int		index;
	unsigned int		type;
	unsigned int		memory;
	unsigned int		num_planes;
	struct vb2_plane	planes[VB2_MAX_PLANES];
	u64			timestamp;

	struct sync_file	*acquire_fence;

	
	enum vb2_buffer_state	state;

	struct list_head	queued_entry;
	struct list_head	done_entry;
#ifdef CONFIG_VIDEO_ADV_DEBUG
	
	u32		cnt_mem_alloc;
	u32		cnt_mem_put;
	u32		cnt_mem_get_dmabuf;
	u32		cnt_mem_get_userptr;
	u32		cnt_mem_put_userptr;
	u32		cnt_mem_prepare;
	u32		cnt_mem_finish;
	u32		cnt_mem_attach_dmabuf;
	u32		cnt_mem_detach_dmabuf;
	u32		cnt_mem_map_dmabuf;
	u32		cnt_mem_unmap_dmabuf;
	u32		cnt_mem_vaddr;
	u32		cnt_mem_cookie;
	u32		cnt_mem_num_users;
	u32		cnt_mem_mmap;

	u32		cnt_buf_init;
	u32		cnt_buf_prepare;
	u32		cnt_buf_finish;
	u32		cnt_buf_cleanup;
	u32		cnt_buf_queue;

	
	u32		cnt_buf_done;
#endif
};


struct vb2_ops {
	int (*queue_setup)(struct vb2_queue *q,
			   unsigned int *num_buffers, unsigned int *num_planes,
			   unsigned int sizes[], struct device *alloc_devs[]);

	void (*wait_prepare)(struct vb2_queue *q);
	void (*wait_finish)(struct vb2_queue *q);

	int (*buf_init)(struct vb2_buffer *vb);
	int (*buf_prepare)(struct vb2_buffer *vb);
	void (*buf_finish)(struct vb2_buffer *vb);
	void (*buf_cleanup)(struct vb2_buffer *vb);

	int (*start_streaming)(struct vb2_queue *q, unsigned int count);
	void (*stop_streaming)(struct vb2_queue *q);

	void (*buf_queue)(struct vb2_buffer *vb);
};


struct vb2_buf_ops {
	int (*verify_planes_array)(struct vb2_buffer *vb, const void *pb);
	void (*fill_user_buffer)(struct vb2_buffer *vb, void *pb);
	int (*fill_vb2_buffer)(struct vb2_buffer *vb, const void *pb,
				struct vb2_plane *planes);
	void (*copy_timestamp)(struct vb2_buffer *vb, void *pb);
};



struct vb2_queue {
	unsigned int			type;
	unsigned int			io_modes;
	struct device			*dev;
	unsigned long			dma_attrs;
	unsigned			fileio_read_once:1;
	unsigned			fileio_write_immediately:1;
	unsigned			allow_zero_bytesused:1;
	unsigned		   quirk_poll_must_check_waiting_for_buffers:1;

	struct mutex			*lock;
	void				*owner;

	const struct vb2_ops		*ops;
	const struct vb2_mem_ops	*mem_ops;
	const struct vb2_buf_ops	*buf_ops;

	void				*drv_priv;
	unsigned int			buf_struct_size;
	u32				timestamp_flags;
	gfp_t				gfp_flags;
	u32				min_buffers_needed;

	
	struct mutex			mmap_lock;
	unsigned int			memory;
	struct vb2_buffer		*bufs[VB2_MAX_FRAME];
	unsigned int			num_buffers;

	struct list_head		queued_list;
	unsigned int			queued_count;

	atomic_t			owned_by_drv_count;
	struct list_head		done_list;
	spinlock_t			done_lock;
	wait_queue_head_t		done_wq;

	struct device			*alloc_devs[VB2_MAX_PLANES];

	unsigned int			streaming:1;
	unsigned int			start_streaming_called:1;
	unsigned int			error:1;
	unsigned int			waiting_for_buffers:1;
	unsigned int			is_multiplanar:1;
	unsigned int			is_output:1;
	unsigned int			copy_timestamp:1;
	unsigned int			last_buffer_dequeued:1;

	struct vb2_fileio_data		*fileio;
	struct vb2_threadio_data	*threadio;

	struct sync_timeline		*timeline;
	u32				timeline_max;

#ifdef CONFIG_VIDEO_ADV_DEBUG
	
	u32				cnt_queue_setup;
	u32				cnt_wait_prepare;
	u32				cnt_wait_finish;
	u32				cnt_start_streaming;
	u32				cnt_stop_streaming;
#endif
};


void *vb2_plane_vaddr(struct vb2_buffer *vb, unsigned int plane_no);


void *vb2_plane_cookie(struct vb2_buffer *vb, unsigned int plane_no);


void vb2_buffer_done(struct vb2_buffer *vb, enum vb2_buffer_state state);


void vb2_discard_done(struct vb2_queue *q);


int vb2_wait_for_all_buffers(struct vb2_queue *q);


void vb2_core_querybuf(struct vb2_queue *q, unsigned int index, void *pb);


int vb2_core_reqbufs(struct vb2_queue *q, enum vb2_memory memory,
		unsigned int *count);


int vb2_core_create_bufs(struct vb2_queue *q, enum vb2_memory memory,
			 unsigned int *count, unsigned int requested_planes,
			 const unsigned int requested_sizes[]);


int vb2_core_prepare_buf(struct vb2_queue *q, unsigned int index, void *pb);


int vb2_core_qbuf(struct vb2_queue *q, unsigned int index, void *pb);


int vb2_core_dqbuf(struct vb2_queue *q, unsigned int *pindex, void *pb,
		   bool nonblocking);

int vb2_core_streamon(struct vb2_queue *q, unsigned int type);
int vb2_core_streamoff(struct vb2_queue *q, unsigned int type);


int vb2_core_expbuf(struct vb2_queue *q, int *fd, unsigned int type,
		unsigned int index, unsigned int plane, unsigned int flags);


int vb2_core_queue_init(struct vb2_queue *q);


void vb2_core_queue_release(struct vb2_queue *q);


void vb2_queue_error(struct vb2_queue *q);


int vb2_mmap(struct vb2_queue *q, struct vm_area_struct *vma);

#ifndef CONFIG_MMU
unsigned long vb2_get_unmapped_area(struct vb2_queue *q,
				    unsigned long addr,
				    unsigned long len,
				    unsigned long pgoff,
				    unsigned long flags);
#endif


unsigned int vb2_core_poll(struct vb2_queue *q, struct file *file,
			   poll_table *wait);

size_t vb2_read(struct vb2_queue *q, char __user *data, size_t count,
		loff_t *ppos, int nonblock);
size_t vb2_write(struct vb2_queue *q, const char __user *data, size_t count,
		loff_t *ppos, int nonblock);


typedef int (*vb2_thread_fnc)(struct vb2_buffer *vb, void *priv);


int vb2_thread_start(struct vb2_queue *q, vb2_thread_fnc fnc, void *priv,
		     const char *thread_name);


int vb2_thread_stop(struct vb2_queue *q);


static inline bool vb2_is_streaming(struct vb2_queue *q)
{
	return q->streaming;
}


static inline bool vb2_fileio_is_active(struct vb2_queue *q)
{
	return q->fileio;
}


static inline bool vb2_is_busy(struct vb2_queue *q)
{
	return (q->num_buffers > 0);
}


static inline void *vb2_get_drv_priv(struct vb2_queue *q)
{
	return q->drv_priv;
}


static inline void vb2_set_plane_payload(struct vb2_buffer *vb,
				 unsigned int plane_no, unsigned long size)
{
	if (plane_no < vb->num_planes)
		vb->planes[plane_no].bytesused = size;
}


static inline unsigned long vb2_get_plane_payload(struct vb2_buffer *vb,
				 unsigned int plane_no)
{
	if (plane_no < vb->num_planes)
		return vb->planes[plane_no].bytesused;
	return 0;
}


static inline unsigned long
vb2_plane_size(struct vb2_buffer *vb, unsigned int plane_no)
{
	if (plane_no < vb->num_planes)
		return vb->planes[plane_no].length;
	return 0;
}


static inline bool vb2_start_streaming_called(struct vb2_queue *q)
{
	return q->start_streaming_called;
}


static inline void vb2_clear_last_buffer_dequeued(struct vb2_queue *q)
{
	q->last_buffer_dequeued = false;
}




bool vb2_buffer_in_use(struct vb2_queue *q, struct vb2_buffer *vb);


int vb2_verify_memory_type(struct vb2_queue *q,
		enum vb2_memory memory, unsigned int type);
#endif 
