#ifndef _LINUX_VIRTIO_BALLOON_H
#define _LINUX_VIRTIO_BALLOON_H

#include <linux/types.h>
#include <linux/virtio_types.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_config.h>


#define VIRTIO_BALLOON_F_MUST_TELL_HOST	0 
#define VIRTIO_BALLOON_F_STATS_VQ	1 
#define VIRTIO_BALLOON_F_DEFLATE_ON_OOM	2 


#define VIRTIO_BALLOON_PFN_SHIFT 12

struct virtio_balloon_config {
	
	__u32 num_pages;
	
	__u32 actual;
};

#define VIRTIO_BALLOON_S_SWAP_IN  0   
#define VIRTIO_BALLOON_S_SWAP_OUT 1   
#define VIRTIO_BALLOON_S_MAJFLT   2   
#define VIRTIO_BALLOON_S_MINFLT   3   
#define VIRTIO_BALLOON_S_MEMFREE  4   
#define VIRTIO_BALLOON_S_MEMTOT   5   
#define VIRTIO_BALLOON_S_AVAIL    6   
#define VIRTIO_BALLOON_S_NR       7


struct virtio_balloon_stat {
	__virtio16 tag;
	__virtio64 val;
} __attribute__((packed));

#endif 
