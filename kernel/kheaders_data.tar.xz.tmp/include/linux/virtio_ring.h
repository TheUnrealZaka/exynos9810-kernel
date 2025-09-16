#ifndef _LINUX_VIRTIO_RING_H
#define _LINUX_VIRTIO_RING_H

#include <asm/barrier.h>
#include <linux/irqreturn.h>
#include <uapi/linux/virtio_ring.h>



static inline void virtio_mb(bool weak_barriers)
{
	if (weak_barriers)
		virt_mb();
	else
		mb();
}

static inline void virtio_rmb(bool weak_barriers)
{
	if (weak_barriers)
		virt_rmb();
	else
		rmb();
}

static inline void virtio_wmb(bool weak_barriers)
{
	if (weak_barriers)
		virt_wmb();
	else
		wmb();
}

static inline void virtio_store_mb(bool weak_barriers,
				   __virtio16 *p, __virtio16 v)
{
	if (weak_barriers) {
		virt_store_mb(*p, v);
	} else {
		WRITE_ONCE(*p, v);
		mb();
	}
}

struct virtio_device;
struct virtqueue;


struct virtqueue *vring_create_virtqueue(unsigned int index,
					 unsigned int num,
					 unsigned int vring_align,
					 struct virtio_device *vdev,
					 bool weak_barriers,
					 bool may_reduce_num,
					 bool (*notify)(struct virtqueue *vq),
					 void (*callback)(struct virtqueue *vq),
					 const char *name);


struct virtqueue *__vring_new_virtqueue(unsigned int index,
					struct vring vring,
					struct virtio_device *vdev,
					bool weak_barriers,
					bool (*notify)(struct virtqueue *),
					void (*callback)(struct virtqueue *),
					const char *name);


struct virtqueue *vring_new_virtqueue(unsigned int index,
				      unsigned int num,
				      unsigned int vring_align,
				      struct virtio_device *vdev,
				      bool weak_barriers,
				      void *pages,
				      bool (*notify)(struct virtqueue *vq),
				      void (*callback)(struct virtqueue *vq),
				      const char *name);


void vring_del_virtqueue(struct virtqueue *vq);


void vring_transport_features(struct virtio_device *vdev);

irqreturn_t vring_interrupt(int irq, void *_vq);
#endif 
