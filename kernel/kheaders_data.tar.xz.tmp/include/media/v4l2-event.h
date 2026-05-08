

#ifndef V4L2_EVENT_H
#define V4L2_EVENT_H

#include <linux/types.h>
#include <linux/videodev2.h>
#include <linux/wait.h>



struct v4l2_fh;
struct v4l2_subdev;
struct v4l2_subscribed_event;
struct video_device;


struct v4l2_kevent {
	struct list_head	list;
	struct v4l2_subscribed_event *sev;
	struct v4l2_event	event;
};


struct v4l2_subscribed_event_ops {
	int  (*add)(struct v4l2_subscribed_event *sev, unsigned int elems);
	void (*del)(struct v4l2_subscribed_event *sev);
	void (*replace)(struct v4l2_event *old, const struct v4l2_event *new);
	void (*merge)(const struct v4l2_event *old, struct v4l2_event *new);
};


struct v4l2_subscribed_event {
	struct list_head	list;
	u32			type;
	u32			id;
	u32			flags;
	struct v4l2_fh		*fh;
	struct list_head	node;
	const struct v4l2_subscribed_event_ops *ops;
	unsigned int		elems;
	unsigned int		first;
	unsigned int		in_use;
	struct v4l2_kevent	events[];
};


int v4l2_event_dequeue(struct v4l2_fh *fh, struct v4l2_event *event,
		       int nonblocking);


void v4l2_event_queue(struct video_device *vdev, const struct v4l2_event *ev);


void v4l2_event_queue_fh(struct v4l2_fh *fh, const struct v4l2_event *ev);


int v4l2_event_pending(struct v4l2_fh *fh);


int v4l2_event_subscribe(struct v4l2_fh *fh,
			 const struct v4l2_event_subscription *sub,
			 unsigned int elems,
			 const struct v4l2_subscribed_event_ops *ops);

int v4l2_event_unsubscribe(struct v4l2_fh *fh,
			   const struct v4l2_event_subscription *sub);

void v4l2_event_unsubscribe_all(struct v4l2_fh *fh);


int v4l2_event_subdev_unsubscribe(struct v4l2_subdev *sd,
				  struct v4l2_fh *fh,
				  struct v4l2_event_subscription *sub);

int v4l2_src_change_event_subscribe(struct v4l2_fh *fh,
				    const struct v4l2_event_subscription *sub);

int v4l2_src_change_event_subdev_subscribe(struct v4l2_subdev *sd,
					   struct v4l2_fh *fh,
					   struct v4l2_event_subscription *sub);
#endif 
