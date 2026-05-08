

#ifndef __SOUND_MEMALLOC_H
#define __SOUND_MEMALLOC_H

struct device;


struct snd_dma_device {
	int type;			
	struct device *dev;		
};

#ifndef snd_dma_pci_data
#define snd_dma_pci_data(pci)	(&(pci)->dev)
#define snd_dma_isa_data()	NULL
#define snd_dma_continuous_data(x)	((struct device *)(__force unsigned long)(x))
#endif



#define SNDRV_DMA_TYPE_UNKNOWN		0	
#define SNDRV_DMA_TYPE_CONTINUOUS	1	
#define SNDRV_DMA_TYPE_DEV		2	
#ifdef CONFIG_SND_DMA_SGBUF
#define SNDRV_DMA_TYPE_DEV_SG		3	
#else
#define SNDRV_DMA_TYPE_DEV_SG	SNDRV_DMA_TYPE_DEV 
#endif
#ifdef CONFIG_GENERIC_ALLOCATOR
#define SNDRV_DMA_TYPE_DEV_IRAM		4	
#else
#define SNDRV_DMA_TYPE_DEV_IRAM	SNDRV_DMA_TYPE_DEV
#endif


struct snd_dma_buffer {
	struct snd_dma_device dev;	
	unsigned char *area;	
	dma_addr_t addr;	
	size_t bytes;		
	void *private_data;	
};

#ifdef CONFIG_SND_DMA_SGBUF

void *snd_malloc_sgbuf_pages(struct device *device,
			     size_t size, struct snd_dma_buffer *dmab,
			     size_t *res_size);
int snd_free_sgbuf_pages(struct snd_dma_buffer *dmab);

struct snd_sg_page {
	void *buf;
	dma_addr_t addr;
};

struct snd_sg_buf {
	int size;	
	int pages;	
	int tblsize;	
	struct snd_sg_page *table;	
	struct page **page_table;	
	struct device *dev;
};


static inline unsigned int snd_sgbuf_aligned_pages(size_t size)
{
	return (size + PAGE_SIZE - 1) >> PAGE_SHIFT;
}


static inline dma_addr_t snd_sgbuf_get_addr(struct snd_dma_buffer *dmab,
					   size_t offset)
{
	struct snd_sg_buf *sgbuf = dmab->private_data;
	dma_addr_t addr = sgbuf->table[offset >> PAGE_SHIFT].addr;
	addr &= ~((dma_addr_t)PAGE_SIZE - 1);
	return addr + offset % PAGE_SIZE;
}


static inline void *snd_sgbuf_get_ptr(struct snd_dma_buffer *dmab,
				     size_t offset)
{
	struct snd_sg_buf *sgbuf = dmab->private_data;
	return sgbuf->table[offset >> PAGE_SHIFT].buf + offset % PAGE_SIZE;
}

unsigned int snd_sgbuf_get_chunk_size(struct snd_dma_buffer *dmab,
				      unsigned int ofs, unsigned int size);
#else

static inline dma_addr_t snd_sgbuf_get_addr(struct snd_dma_buffer *dmab,
					    size_t offset)
{
	return dmab->addr + offset;
}

static inline void *snd_sgbuf_get_ptr(struct snd_dma_buffer *dmab,
				      size_t offset)
{
	return dmab->area + offset;
}

#define snd_sgbuf_get_chunk_size(dmab, ofs, size)	(size)

#endif 


int snd_dma_alloc_pages(int type, struct device *dev, size_t size,
			struct snd_dma_buffer *dmab);
int snd_dma_alloc_pages_fallback(int type, struct device *dev, size_t size,
                                 struct snd_dma_buffer *dmab);
void snd_dma_free_pages(struct snd_dma_buffer *dmab);


void *snd_malloc_pages(size_t size, gfp_t gfp_flags);
void snd_free_pages(void *ptr, size_t size);

#endif 

