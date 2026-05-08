#ifndef _UAPI_LINUX_VIRTIO_RING_H
#define _UAPI_LINUX_VIRTIO_RING_H

#ifndef __KERNEL__
#include <stdint.h>
#endif
#include <linux/types.h>
#include <linux/virtio_types.h>


#define VRING_DESC_F_NEXT	1

#define VRING_DESC_F_WRITE	2

#define VRING_DESC_F_INDIRECT	4


#define VRING_USED_F_NO_NOTIFY	1

#define VRING_AVAIL_F_NO_INTERRUPT	1


#define VIRTIO_RING_F_INDIRECT_DESC	28



#define VIRTIO_RING_F_EVENT_IDX		29


struct vring_desc {
	
	__virtio64 addr;
	
	__virtio32 len;
	
	__virtio16 flags;
	
	__virtio16 next;
};

struct vring_avail {
	__virtio16 flags;
	__virtio16 idx;
	__virtio16 ring[];
};


struct vring_used_elem {
	
	__virtio32 id;
	
	__virtio32 len;
};

struct vring_used {
	__virtio16 flags;
	__virtio16 idx;
	struct vring_used_elem ring[];
};

struct vring {
	unsigned int num;

	struct vring_desc *desc;

	struct vring_avail *avail;

	struct vring_used *used;
};


#define VRING_AVAIL_ALIGN_SIZE 2
#define VRING_USED_ALIGN_SIZE 4
#define VRING_DESC_ALIGN_SIZE 16



#define vring_used_event(vr) ((vr)->avail->ring[(vr)->num])
#define vring_avail_event(vr) (*(__virtio16 *)&(vr)->used->ring[(vr)->num])

static inline void vring_init(struct vring *vr, unsigned int num, void *p,
			      unsigned long align)
{
	vr->num = num;
	vr->desc = p;
	vr->avail = p + num*sizeof(struct vring_desc);
	vr->used = (void *)(((uintptr_t)&vr->avail->ring[num] + sizeof(__virtio16)
		+ align-1) & ~(align - 1));
}

static inline unsigned vring_size(unsigned int num, unsigned long align)
{
	return ((sizeof(struct vring_desc) * num + sizeof(__virtio16) * (3 + num)
		 + align - 1) & ~(align - 1))
		+ sizeof(__virtio16) * 3 + sizeof(struct vring_used_elem) * num;
}



static inline int vring_need_event(__u16 event_idx, __u16 new_idx, __u16 old)
{
	
	return (__u16)(new_idx - event_idx - 1) < (__u16)(new_idx - old);
}

#endif 
