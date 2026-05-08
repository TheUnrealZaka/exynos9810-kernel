

#ifndef _MEDIA_VIDEOBUF2_MEMOPS_H
#define _MEDIA_VIDEOBUF2_MEMOPS_H

#include <media/videobuf2-v4l2.h>
#include <linux/mm.h>


struct vb2_vmarea_handler {
	atomic_t		*refcount;
	void			(*put)(void *arg);
	void			*arg;
};

extern const struct vm_operations_struct vb2_common_vm_ops;

struct frame_vector *vb2_create_framevec(unsigned long start,
					 unsigned long length,
					 bool write);
void vb2_destroy_framevec(struct frame_vector *vec);

#endif
