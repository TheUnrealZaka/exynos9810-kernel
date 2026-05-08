

#ifndef _MEDIA_VIDEOBUF2_ION_H
#define _MEDIA_VIDEOBUF2_ION_H

#include <linux/scatterlist.h>
#include <linux/dma-mapping.h>
#include <linux/ion.h>
#include <linux/exynos_ion.h>
#include <linux/err.h>
#include <linux/dma-buf.h>




#define VB2ION_CTX_COHERENT_DMA	(1 << (ION_NUM_HEAPS + 3))

#define VB2ION_CTX_UNCACHED_READ_DMA (1 << (ION_NUM_HEAPS + 4))

#define VB2ION_CONTIG_ID_NUM	16
#define VB2ION_NUM_HEAPS	8

#if (BITS_PER_INT <= (VB2ION_CONTIG_ID_NUM + VB2ION_NUM_HEAPS + 6))
#error "Bits are too small to express all flags"
#endif

struct device;
struct vb2_buffer;


void vb2_ion_set_noncoherent_dma_read(struct device *dev, bool noncoherent);


struct vb2_ion_cookie {
	dma_addr_t ioaddr;
	dma_addr_t paddr;
	struct sg_table	*sgt;
	off_t offset;
};



static inline off_t vb2_ion_buffer_offset(void *cookie)
{
	return IS_ERR_OR_NULL(cookie) ?
		-EINVAL : ((struct vb2_ion_cookie *)cookie)->offset;
}


static inline int vb2_ion_phys_address(void *cookie, phys_addr_t *phys_addr)
{
	struct vb2_ion_cookie *vb2cookie = cookie;

	if (WARN_ON(!phys_addr || IS_ERR_OR_NULL(cookie)))
		return -EINVAL;

	if (vb2cookie->paddr) {
		*phys_addr = vb2cookie->paddr;
	} else {
		if (vb2cookie->sgt && vb2cookie->sgt->nents == 1) {
			*phys_addr = sg_phys(vb2cookie->sgt->sgl) +
						vb2cookie->offset;
		} else {
			*phys_addr = 0;
			return -EINVAL;
		}
	}

	return 0;
}


static inline int vb2_ion_dma_address(void *cookie, dma_addr_t *dma_addr)
{
	struct vb2_ion_cookie *vb2cookie = cookie;

	if (WARN_ON(!dma_addr || IS_ERR_OR_NULL(cookie)))
		return -EINVAL;

	if (vb2cookie->ioaddr == 0)
		return vb2_ion_phys_address(cookie, (phys_addr_t *)dma_addr);

	*dma_addr = vb2cookie->ioaddr;

	return 0;
}


static inline struct scatterlist *vb2_ion_get_sg(void *cookie, int *nents)
{
	struct vb2_ion_cookie *vb2cookie = cookie;

	if (WARN_ON(!nents || IS_ERR_OR_NULL(cookie)))
		return NULL;

	*nents = vb2cookie->sgt->nents;
	return vb2cookie->sgt->sgl;
}


struct dma_buf *vb2_ion_get_dmabuf(void *cookie);


void vb2_ion_sync_for_device(void *cookie, off_t offset, size_t size,
						enum dma_data_direction dir);
void vb2_ion_sync_for_cpu(void *cookie, off_t offset, size_t size,
						enum dma_data_direction dir);
int vb2_ion_buf_prepare(struct vb2_buffer *vb);
void vb2_ion_buf_finish(struct vb2_buffer *vb);
int vb2_ion_buf_prepare_exact(struct vb2_buffer *vb);
int vb2_ion_buf_finish_exact(struct vb2_buffer *vb);

extern const struct vb2_mem_ops vb2_ion_memops;
extern struct ion_device *ion_exynos; 

#endif 
