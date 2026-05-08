#ifndef _LINUX_VIRTIO_9P_H
#define _LINUX_VIRTIO_9P_H

#include <linux/types.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_config.h>




#define VIRTIO_9P_MOUNT_TAG 0

struct virtio_9p_config {
	
	__u16 tag_len;
	
	__u8 tag[0];
} __attribute__((packed));

#endif 
