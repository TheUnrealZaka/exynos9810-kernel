
#ifndef _MEDIA_VIDEOBUF2_V4L2_H
#define _MEDIA_VIDEOBUF2_V4L2_H

#include <linux/videodev2.h>
#include <media/videobuf2-core.h>

#if VB2_MAX_FRAME != VIDEO_MAX_FRAME
#error VB2_MAX_FRAME != VIDEO_MAX_FRAME
#endif

#if VB2_MAX_PLANES != VIDEO_MAX_PLANES
#error VB2_MAX_PLANES != VIDEO_MAX_PLANES
#endif


struct vb2_v4l2_buffer {
	struct vb2_buffer	vb2_buf;

	__u32			flags;
	__u32			field;
	struct v4l2_timecode	timecode;
	__u32			sequence;
	__u32			reserved2;
};


#define to_vb2_v4l2_buffer(vb) \
	container_of(vb, struct vb2_v4l2_buffer, vb2_buf)

int vb2_querybuf(struct vb2_queue *q, struct v4l2_buffer *b);


int vb2_reqbufs(struct vb2_queue *q, struct v4l2_requestbuffers *req);


int vb2_create_bufs(struct vb2_queue *q, struct v4l2_create_buffers *create);


int vb2_prepare_buf(struct vb2_queue *q, struct v4l2_buffer *b);


int vb2_qbuf(struct vb2_queue *q, struct v4l2_buffer *b);


int vb2_expbuf(struct vb2_queue *q, struct v4l2_exportbuffer *eb);


int vb2_dqbuf(struct vb2_queue *q, struct v4l2_buffer *b, bool nonblocking);


int vb2_streamon(struct vb2_queue *q, enum v4l2_buf_type type);


int vb2_streamoff(struct vb2_queue *q, enum v4l2_buf_type type);


int __must_check vb2_queue_init(struct vb2_queue *q);


void vb2_queue_release(struct vb2_queue *q);


unsigned int vb2_poll(struct vb2_queue *q, struct file *file,
		      poll_table *wait);





int vb2_ioctl_reqbufs(struct file *file, void *priv,
			  struct v4l2_requestbuffers *p);
int vb2_ioctl_create_bufs(struct file *file, void *priv,
			  struct v4l2_create_buffers *p);
int vb2_ioctl_prepare_buf(struct file *file, void *priv,
			  struct v4l2_buffer *p);
int vb2_ioctl_querybuf(struct file *file, void *priv, struct v4l2_buffer *p);
int vb2_ioctl_qbuf(struct file *file, void *priv, struct v4l2_buffer *p);
int vb2_ioctl_dqbuf(struct file *file, void *priv, struct v4l2_buffer *p);
int vb2_ioctl_streamon(struct file *file, void *priv, enum v4l2_buf_type i);
int vb2_ioctl_streamoff(struct file *file, void *priv, enum v4l2_buf_type i);
int vb2_ioctl_expbuf(struct file *file, void *priv,
	struct v4l2_exportbuffer *p);



int vb2_fop_mmap(struct file *file, struct vm_area_struct *vma);
int vb2_fop_release(struct file *file);
int _vb2_fop_release(struct file *file, struct mutex *lock);
ssize_t vb2_fop_write(struct file *file, const char __user *buf,
		size_t count, loff_t *ppos);
ssize_t vb2_fop_read(struct file *file, char __user *buf,
		size_t count, loff_t *ppos);
unsigned int vb2_fop_poll(struct file *file, poll_table *wait);
#ifndef CONFIG_MMU
unsigned long vb2_fop_get_unmapped_area(struct file *file, unsigned long addr,
		unsigned long len, unsigned long pgoff, unsigned long flags);
#endif


void vb2_ops_wait_prepare(struct vb2_queue *vq);


void vb2_ops_wait_finish(struct vb2_queue *vq);

#endif 
