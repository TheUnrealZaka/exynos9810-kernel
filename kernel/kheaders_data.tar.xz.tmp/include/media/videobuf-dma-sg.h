
#ifndef _VIDEOBUF_DMA_SG_H
#define _VIDEOBUF_DMA_SG_H

#include <media/videobuf-core.h>





struct videobuf_dmabuf {
	u32                 magic;

	
	int                 offset;
	size_t		    size;
	struct page         **pages;

	
	void                *vaddr;
	struct page         **vaddr_pages;
	dma_addr_t          *dma_addr;
	struct device       *dev;

	
	dma_addr_t          bus_addr;

	
	struct scatterlist  *sglist;
	int                 sglen;
	int                 nr_pages;
	int                 direction;
};

struct videobuf_dma_sg_memory {
	u32                 magic;

	
	struct videobuf_dmabuf  dma;
};


int videobuf_dma_free(struct videobuf_dmabuf *dma);

int videobuf_dma_unmap(struct device *dev, struct videobuf_dmabuf *dma);
struct videobuf_dmabuf *videobuf_to_dma(struct videobuf_buffer *buf);

void *videobuf_sg_alloc(size_t size);

void videobuf_queue_sg_init(struct videobuf_queue *q,
			 const struct videobuf_queue_ops *ops,
			 struct device *dev,
			 spinlock_t *irqlock,
			 enum v4l2_buf_type type,
			 enum v4l2_field field,
			 unsigned int msize,
			 void *priv,
			 struct mutex *ext_lock);

#endif 

