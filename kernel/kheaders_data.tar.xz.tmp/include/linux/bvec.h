
#ifndef __LINUX_BVEC_ITER_H
#define __LINUX_BVEC_ITER_H

#include <linux/kernel.h>
#include <linux/bug.h>


struct bio_vec {
	struct page	*bv_page;
	unsigned int	bv_len;
	unsigned int	bv_offset;
};

struct bvec_iter {
	sector_t		bi_sector;	
	unsigned int		bi_size;	

	unsigned int		bi_idx;		

	unsigned int            bi_bvec_done;	
#ifdef CONFIG_CRYPTO_DISKCIPHER
	u64                     bi_dun;
#endif
};


#define __bvec_iter_bvec(bvec, iter)	(&(bvec)[(iter).bi_idx])

#define bvec_iter_page(bvec, iter)				\
	(__bvec_iter_bvec((bvec), (iter))->bv_page)

#define bvec_iter_len(bvec, iter)				\
	min((iter).bi_size,					\
	    __bvec_iter_bvec((bvec), (iter))->bv_len - (iter).bi_bvec_done)

#define bvec_iter_offset(bvec, iter)				\
	(__bvec_iter_bvec((bvec), (iter))->bv_offset + (iter).bi_bvec_done)

#define bvec_iter_bvec(bvec, iter)				\
((struct bio_vec) {						\
	.bv_page	= bvec_iter_page((bvec), (iter)),	\
	.bv_len		= bvec_iter_len((bvec), (iter)),	\
	.bv_offset	= bvec_iter_offset((bvec), (iter)),	\
})

static inline void bvec_iter_advance(const struct bio_vec *bv,
				     struct bvec_iter *iter,
				     unsigned bytes)
{
	WARN_ONCE(bytes > iter->bi_size,
		  "Attempted to advance past end of bvec iter\n");

	while (bytes) {
		unsigned iter_len = bvec_iter_len(bv, *iter);
		unsigned len = min(bytes, iter_len);

		bytes -= len;
		iter->bi_size -= len;
		iter->bi_bvec_done += len;

		if (iter->bi_bvec_done == __bvec_iter_bvec(bv, *iter)->bv_len) {
			iter->bi_bvec_done = 0;
			iter->bi_idx++;
		}
	}
}

static inline void bvec_iter_skip_zero_bvec(struct bvec_iter *iter)
{
	iter->bi_bvec_done = 0;
	iter->bi_idx++;
}

#define for_each_bvec(bvl, bio_vec, iter, start)			\
	for (iter = (start);						\
	     (iter).bi_size &&						\
		((bvl = bvec_iter_bvec((bio_vec), (iter))), 1);	\
	     (bvl).bv_len ? (void)bvec_iter_advance((bio_vec), &(iter),	\
		     (bvl).bv_len) : bvec_iter_skip_zero_bvec(&(iter)))

#endif 
