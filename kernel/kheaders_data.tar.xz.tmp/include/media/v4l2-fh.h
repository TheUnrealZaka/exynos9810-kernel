

#ifndef V4L2_FH_H
#define V4L2_FH_H

#include <linux/fs.h>
#include <linux/list.h>
#include <linux/videodev2.h>

struct video_device;
struct v4l2_ctrl_handler;


struct v4l2_fh {
	struct list_head	list;
	struct video_device	*vdev;
	struct v4l2_ctrl_handler *ctrl_handler;
	enum v4l2_priority	prio;

	
	wait_queue_head_t	wait;
	struct mutex		subscribe_lock;
	struct list_head	subscribed;
	struct list_head	available;
	unsigned int		navailable;
	u32			sequence;

#if IS_ENABLED(CONFIG_V4L2_MEM2MEM_DEV)
	struct v4l2_m2m_ctx	*m2m_ctx;
#endif
};


void v4l2_fh_init(struct v4l2_fh *fh, struct video_device *vdev);


void v4l2_fh_add(struct v4l2_fh *fh);


int v4l2_fh_open(struct file *filp);


void v4l2_fh_del(struct v4l2_fh *fh);


void v4l2_fh_exit(struct v4l2_fh *fh);


int v4l2_fh_release(struct file *filp);


int v4l2_fh_is_singular(struct v4l2_fh *fh);


static inline int v4l2_fh_is_singular_file(struct file *filp)
{
	return v4l2_fh_is_singular(filp->private_data);
}

#endif 
