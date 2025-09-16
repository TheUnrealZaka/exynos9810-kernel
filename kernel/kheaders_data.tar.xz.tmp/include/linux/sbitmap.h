

#ifndef __LINUX_SCALE_BITMAP_H
#define __LINUX_SCALE_BITMAP_H

#include <linux/kernel.h>
#include <linux/slab.h>


struct sbitmap_word {
	
	unsigned long word;

	
	unsigned long depth;
} ____cacheline_aligned_in_smp;


struct sbitmap {
	
	unsigned int depth;

	
	unsigned int shift;

	
	unsigned int map_nr;

	
	struct sbitmap_word *map;
};

#define SBQ_WAIT_QUEUES 8
#define SBQ_WAKE_BATCH 8


struct sbq_wait_state {
	
	atomic_t wait_cnt;

	
	wait_queue_head_t wait;
} ____cacheline_aligned_in_smp;


struct sbitmap_queue {
	
	struct sbitmap sb;

	
	unsigned int __percpu *alloc_hint;

	
	unsigned int wake_batch;

	
	atomic_t wake_index;

	
	struct sbq_wait_state *ws;

	
	bool round_robin;
};


int sbitmap_init_node(struct sbitmap *sb, unsigned int depth, int shift,
		      gfp_t flags, int node);


static inline void sbitmap_free(struct sbitmap *sb)
{
	kfree(sb->map);
	sb->map = NULL;
}


void sbitmap_resize(struct sbitmap *sb, unsigned int depth);


int sbitmap_get(struct sbitmap *sb, unsigned int alloc_hint, bool round_robin);


bool sbitmap_any_bit_set(const struct sbitmap *sb);


bool sbitmap_any_bit_clear(const struct sbitmap *sb);

typedef bool (*sb_for_each_fn)(struct sbitmap *, unsigned int, void *);


static inline void sbitmap_for_each_set(struct sbitmap *sb, sb_for_each_fn fn,
					void *data)
{
	unsigned int i;

	for (i = 0; i < sb->map_nr; i++) {
		struct sbitmap_word *word = &sb->map[i];
		unsigned int off, nr;

		if (!word->word)
			continue;

		nr = 0;
		off = i << sb->shift;
		while (1) {
			nr = find_next_bit(&word->word, word->depth, nr);
			if (nr >= word->depth)
				break;

			if (!fn(sb, off + nr, data))
				return;

			nr++;
		}
	}
}

#define SB_NR_TO_INDEX(sb, bitnr) ((bitnr) >> (sb)->shift)
#define SB_NR_TO_BIT(sb, bitnr) ((bitnr) & ((1U << (sb)->shift) - 1U))

static inline unsigned long *__sbitmap_word(struct sbitmap *sb,
					    unsigned int bitnr)
{
	return &sb->map[SB_NR_TO_INDEX(sb, bitnr)].word;
}



static inline void sbitmap_set_bit(struct sbitmap *sb, unsigned int bitnr)
{
	set_bit(SB_NR_TO_BIT(sb, bitnr), __sbitmap_word(sb, bitnr));
}

static inline void sbitmap_clear_bit(struct sbitmap *sb, unsigned int bitnr)
{
	clear_bit(SB_NR_TO_BIT(sb, bitnr), __sbitmap_word(sb, bitnr));
}

static inline int sbitmap_test_bit(struct sbitmap *sb, unsigned int bitnr)
{
	return test_bit(SB_NR_TO_BIT(sb, bitnr), __sbitmap_word(sb, bitnr));
}

unsigned int sbitmap_weight(const struct sbitmap *sb);


int sbitmap_queue_init_node(struct sbitmap_queue *sbq, unsigned int depth,
			    int shift, bool round_robin, gfp_t flags, int node);


static inline void sbitmap_queue_free(struct sbitmap_queue *sbq)
{
	kfree(sbq->ws);
	free_percpu(sbq->alloc_hint);
	sbitmap_free(&sbq->sb);
}


void sbitmap_queue_resize(struct sbitmap_queue *sbq, unsigned int depth);


int __sbitmap_queue_get(struct sbitmap_queue *sbq);


static inline int sbitmap_queue_get(struct sbitmap_queue *sbq,
				    unsigned int *cpu)
{
	int nr;

	*cpu = get_cpu();
	nr = __sbitmap_queue_get(sbq);
	put_cpu();
	return nr;
}


void sbitmap_queue_clear(struct sbitmap_queue *sbq, unsigned int nr,
			 unsigned int cpu);

static inline int sbq_index_inc(int index)
{
	return (index + 1) & (SBQ_WAIT_QUEUES - 1);
}

static inline void sbq_index_atomic_inc(atomic_t *index)
{
	int old = atomic_read(index);
	int new = sbq_index_inc(old);
	atomic_cmpxchg(index, old, new);
}


static inline struct sbq_wait_state *sbq_wait_ptr(struct sbitmap_queue *sbq,
						  atomic_t *wait_index)
{
	struct sbq_wait_state *ws;

	ws = &sbq->ws[atomic_read(wait_index)];
	sbq_index_atomic_inc(wait_index);
	return ws;
}


void sbitmap_queue_wake_all(struct sbitmap_queue *sbq);

#endif 
