

#ifndef IB_UMEM_H
#define IB_UMEM_H

#include <linux/list.h>
#include <linux/scatterlist.h>
#include <linux/workqueue.h>

struct ib_ucontext;
struct ib_umem_odp;

struct ib_umem {
	struct ib_ucontext     *context;
	size_t			length;
	unsigned long		address;
	int			page_size;
	int                     writable;
	int                     hugetlb;
	struct work_struct	work;
	struct pid             *pid;
	struct mm_struct       *mm;
	unsigned long		diff;
	struct ib_umem_odp     *odp_data;
	struct sg_table sg_head;
	int             nmap;
	int             npages;
};


static inline int ib_umem_offset(struct ib_umem *umem)
{
	return umem->address & ((unsigned long)umem->page_size - 1);
}


static inline unsigned long ib_umem_start(struct ib_umem *umem)
{
	return umem->address - ib_umem_offset(umem);
}


static inline unsigned long ib_umem_end(struct ib_umem *umem)
{
	return PAGE_ALIGN(umem->address + umem->length);
}

static inline size_t ib_umem_num_pages(struct ib_umem *umem)
{
	return (ib_umem_end(umem) - ib_umem_start(umem)) >> PAGE_SHIFT;
}

#ifdef CONFIG_INFINIBAND_USER_MEM

struct ib_umem *ib_umem_get(struct ib_ucontext *context, unsigned long addr,
			    size_t size, int access, int dmasync);
void ib_umem_release(struct ib_umem *umem);
int ib_umem_page_count(struct ib_umem *umem);
int ib_umem_copy_from(void *dst, struct ib_umem *umem, size_t offset,
		      size_t length);

#else 

#include <linux/err.h>

static inline struct ib_umem *ib_umem_get(struct ib_ucontext *context,
					  unsigned long addr, size_t size,
					  int access, int dmasync) {
	return ERR_PTR(-EINVAL);
}
static inline void ib_umem_release(struct ib_umem *umem) { }
static inline int ib_umem_page_count(struct ib_umem *umem) { return 0; }
static inline int ib_umem_copy_from(void *dst, struct ib_umem *umem, size_t offset,
		      		    size_t length) {
	return -EINVAL;
}
#endif 

#endif 
