
#ifndef LINUX_DMAENGINE_H
#define LINUX_DMAENGINE_H

#include <linux/device.h>
#include <linux/err.h>
#include <linux/uio.h>
#include <linux/bug.h>
#include <linux/scatterlist.h>
#include <linux/bitmap.h>
#include <linux/types.h>
#include <asm/page.h>


typedef s32 dma_cookie_t;
#define DMA_MIN_COOKIE	1

static inline int dma_submit_error(dma_cookie_t cookie)
{
	return cookie < 0 ? cookie : 0;
}


enum dma_status {
	DMA_COMPLETE,
	DMA_IN_PROGRESS,
	DMA_PAUSED,
	DMA_ERROR,
};


enum dma_transaction_type {
	DMA_MEMCPY,
	DMA_XOR,
	DMA_PQ,
	DMA_XOR_VAL,
	DMA_PQ_VAL,
	DMA_MEMSET,
	DMA_MEMSET_SG,
	DMA_INTERRUPT,
	DMA_SG,
	DMA_PRIVATE,
	DMA_ASYNC_TX,
	DMA_SLAVE,
	DMA_CYCLIC,
	DMA_INTERLEAVE,

	DMA_TX_TYPE_END,
};


enum dma_transfer_direction {
	DMA_MEM_TO_MEM,
	DMA_MEM_TO_DEV,
	DMA_DEV_TO_MEM,
	DMA_DEV_TO_DEV,
	DMA_TRANS_NONE,
};




struct data_chunk {
	size_t size;
	size_t icg;
	size_t dst_icg;
	size_t src_icg;
};


struct dma_interleaved_template {
	dma_addr_t src_start;
	dma_addr_t dst_start;
	enum dma_transfer_direction dir;
	bool src_inc;
	bool dst_inc;
	bool src_sgl;
	bool dst_sgl;
	size_t numf;
	size_t frame_size;
	struct data_chunk sgl[0];
};


enum dma_ctrl_flags {
	DMA_PREP_INTERRUPT = (1 << 0),
	DMA_CTRL_ACK = (1 << 1),
	DMA_PREP_PQ_DISABLE_P = (1 << 2),
	DMA_PREP_PQ_DISABLE_Q = (1 << 3),
	DMA_PREP_CONTINUE = (1 << 4),
	DMA_PREP_FENCE = (1 << 5),
	DMA_CTRL_REUSE = (1 << 6),
};


enum sum_check_bits {
	SUM_CHECK_P = 0,
	SUM_CHECK_Q = 1,
};


enum sum_check_flags {
	SUM_CHECK_P_RESULT = (1 << SUM_CHECK_P),
	SUM_CHECK_Q_RESULT = (1 << SUM_CHECK_Q),
};



typedef struct { DECLARE_BITMAP(bits, DMA_TX_TYPE_END); } dma_cap_mask_t;



struct dma_chan_percpu {
	
	unsigned long memcpy_count;
	unsigned long bytes_transferred;
};


struct dma_router {
	struct device *dev;
	void (*route_free)(struct device *dev, void *route_data);
};


struct dma_chan {
	struct dma_device *device;
	dma_cookie_t cookie;
	dma_cookie_t completed_cookie;

	
	int chan_id;
	struct dma_chan_dev *dev;

	struct list_head device_node;
	struct dma_chan_percpu __percpu *local;
	int client_count;
	int table_count;

	
	struct dma_router *router;
	void *route_data;

	void *private;
};


struct dma_chan_dev {
	struct dma_chan *chan;
	struct device device;
	int dev_id;
	atomic_t *idr_ref;
};


enum dma_slave_buswidth {
	DMA_SLAVE_BUSWIDTH_UNDEFINED = 0,
	DMA_SLAVE_BUSWIDTH_1_BYTE = 1,
	DMA_SLAVE_BUSWIDTH_2_BYTES = 2,
	DMA_SLAVE_BUSWIDTH_3_BYTES = 3,
	DMA_SLAVE_BUSWIDTH_4_BYTES = 4,
	DMA_SLAVE_BUSWIDTH_8_BYTES = 8,
	DMA_SLAVE_BUSWIDTH_16_BYTES = 16,
	DMA_SLAVE_BUSWIDTH_32_BYTES = 32,
	DMA_SLAVE_BUSWIDTH_64_BYTES = 64,
};


struct dma_slave_config {
	enum dma_transfer_direction direction;
	phys_addr_t src_addr;
	phys_addr_t dst_addr;
	enum dma_slave_buswidth src_addr_width;
	enum dma_slave_buswidth dst_addr_width;
	u32 src_maxburst;
	u32 dst_maxburst;
	bool device_fc;
	unsigned int slave_id;
};


enum dma_residue_granularity {
	DMA_RESIDUE_GRANULARITY_DESCRIPTOR = 0,
	DMA_RESIDUE_GRANULARITY_SEGMENT = 1,
	DMA_RESIDUE_GRANULARITY_BURST = 2,
};


struct dma_slave_caps {
	u32 src_addr_widths;
	u32 dst_addr_widths;
	u32 directions;
	u32 max_burst;
	bool cmd_pause;
	bool cmd_terminate;
	enum dma_residue_granularity residue_granularity;
	bool descriptor_reuse;
};

static inline const char *dma_chan_name(struct dma_chan *chan)
{
	return dev_name(&chan->dev->device);
}

void dma_chan_cleanup(struct kref *kref);


typedef bool (*dma_filter_fn)(struct dma_chan *chan, void *filter_param);

typedef void (*dma_async_tx_callback)(void *dma_async_param);

enum dmaengine_tx_result {
	DMA_TRANS_NOERROR = 0,		
	DMA_TRANS_READ_FAILED,		
	DMA_TRANS_WRITE_FAILED,		
	DMA_TRANS_ABORTED,		
};

struct dmaengine_result {
	enum dmaengine_tx_result result;
	u32 residue;
};

typedef void (*dma_async_tx_callback_result)(void *dma_async_param,
				const struct dmaengine_result *result);

struct dmaengine_unmap_data {
	u8 map_cnt;
	u8 to_cnt;
	u8 from_cnt;
	u8 bidi_cnt;
	struct device *dev;
	struct kref kref;
	size_t len;
	dma_addr_t addr[0];
};


struct dma_async_tx_descriptor {
	dma_cookie_t cookie;
	enum dma_ctrl_flags flags; 
	dma_addr_t phys;
	struct dma_chan *chan;
	dma_cookie_t (*tx_submit)(struct dma_async_tx_descriptor *tx);
	int (*desc_free)(struct dma_async_tx_descriptor *tx);
	dma_async_tx_callback callback;
	dma_async_tx_callback_result callback_result;
	void *callback_param;
	struct dmaengine_unmap_data *unmap;
#ifdef CONFIG_ASYNC_TX_ENABLE_CHANNEL_SWITCH
	struct dma_async_tx_descriptor *next;
	struct dma_async_tx_descriptor *parent;
	spinlock_t lock;
#endif
};

#ifdef CONFIG_DMA_ENGINE
static inline void dma_set_unmap(struct dma_async_tx_descriptor *tx,
				 struct dmaengine_unmap_data *unmap)
{
	kref_get(&unmap->kref);
	tx->unmap = unmap;
}

struct dmaengine_unmap_data *
dmaengine_get_unmap_data(struct device *dev, int nr, gfp_t flags);
void dmaengine_unmap_put(struct dmaengine_unmap_data *unmap);
#else
static inline void dma_set_unmap(struct dma_async_tx_descriptor *tx,
				 struct dmaengine_unmap_data *unmap)
{
}
static inline struct dmaengine_unmap_data *
dmaengine_get_unmap_data(struct device *dev, int nr, gfp_t flags)
{
	return NULL;
}
static inline void dmaengine_unmap_put(struct dmaengine_unmap_data *unmap)
{
}
#endif

static inline void dma_descriptor_unmap(struct dma_async_tx_descriptor *tx)
{
	if (tx->unmap) {
		dmaengine_unmap_put(tx->unmap);
		tx->unmap = NULL;
	}
}

#ifndef CONFIG_ASYNC_TX_ENABLE_CHANNEL_SWITCH
static inline void txd_lock(struct dma_async_tx_descriptor *txd)
{
}
static inline void txd_unlock(struct dma_async_tx_descriptor *txd)
{
}
static inline void txd_chain(struct dma_async_tx_descriptor *txd, struct dma_async_tx_descriptor *next)
{
	BUG();
}
static inline void txd_clear_parent(struct dma_async_tx_descriptor *txd)
{
}
static inline void txd_clear_next(struct dma_async_tx_descriptor *txd)
{
}
static inline struct dma_async_tx_descriptor *txd_next(struct dma_async_tx_descriptor *txd)
{
	return NULL;
}
static inline struct dma_async_tx_descriptor *txd_parent(struct dma_async_tx_descriptor *txd)
{
	return NULL;
}

#else
static inline void txd_lock(struct dma_async_tx_descriptor *txd)
{
	spin_lock_bh(&txd->lock);
}
static inline void txd_unlock(struct dma_async_tx_descriptor *txd)
{
	spin_unlock_bh(&txd->lock);
}
static inline void txd_chain(struct dma_async_tx_descriptor *txd, struct dma_async_tx_descriptor *next)
{
	txd->next = next;
	next->parent = txd;
}
static inline void txd_clear_parent(struct dma_async_tx_descriptor *txd)
{
	txd->parent = NULL;
}
static inline void txd_clear_next(struct dma_async_tx_descriptor *txd)
{
	txd->next = NULL;
}
static inline struct dma_async_tx_descriptor *txd_parent(struct dma_async_tx_descriptor *txd)
{
	return txd->parent;
}
static inline struct dma_async_tx_descriptor *txd_next(struct dma_async_tx_descriptor *txd)
{
	return txd->next;
}
#endif


struct dma_tx_state {
	dma_cookie_t last;
	dma_cookie_t used;
	u32 residue;
};


enum dmaengine_alignment {
	DMAENGINE_ALIGN_1_BYTE = 0,
	DMAENGINE_ALIGN_2_BYTES = 1,
	DMAENGINE_ALIGN_4_BYTES = 2,
	DMAENGINE_ALIGN_8_BYTES = 3,
	DMAENGINE_ALIGN_16_BYTES = 4,
	DMAENGINE_ALIGN_32_BYTES = 5,
	DMAENGINE_ALIGN_64_BYTES = 6,
};


struct dma_slave_map {
	const char *devname;
	const char *slave;
	void *param;
};


struct dma_filter {
	dma_filter_fn fn;
	int mapcnt;
	const struct dma_slave_map *map;
};


struct dma_device {

	unsigned int chancnt;
	unsigned int privatecnt;
	struct list_head channels;
	struct list_head global_node;
	struct dma_filter filter;
	dma_cap_mask_t  cap_mask;
	unsigned short max_xor;
	unsigned short max_pq;
	enum dmaengine_alignment copy_align;
	enum dmaengine_alignment xor_align;
	enum dmaengine_alignment pq_align;
	enum dmaengine_alignment fill_align;
	#define DMA_HAS_PQ_CONTINUE (1 << 15)

	int dev_id;
	struct device *dev;

	u32 src_addr_widths;
	u32 dst_addr_widths;
	u32 directions;
	u32 max_burst;
	bool descriptor_reuse;
	enum dma_residue_granularity residue_granularity;

	int (*device_alloc_chan_resources)(struct dma_chan *chan);
	void (*device_free_chan_resources)(struct dma_chan *chan);

	struct dma_async_tx_descriptor *(*device_prep_dma_memcpy)(
		struct dma_chan *chan, dma_addr_t dst, dma_addr_t src,
		size_t len, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_xor)(
		struct dma_chan *chan, dma_addr_t dst, dma_addr_t *src,
		unsigned int src_cnt, size_t len, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_xor_val)(
		struct dma_chan *chan, dma_addr_t *src,	unsigned int src_cnt,
		size_t len, enum sum_check_flags *result, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_pq)(
		struct dma_chan *chan, dma_addr_t *dst, dma_addr_t *src,
		unsigned int src_cnt, const unsigned char *scf,
		size_t len, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_pq_val)(
		struct dma_chan *chan, dma_addr_t *pq, dma_addr_t *src,
		unsigned int src_cnt, const unsigned char *scf, size_t len,
		enum sum_check_flags *pqres, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_memset)(
		struct dma_chan *chan, dma_addr_t dest, int value, size_t len,
		unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_memset_sg)(
		struct dma_chan *chan, struct scatterlist *sg,
		unsigned int nents, int value, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_interrupt)(
		struct dma_chan *chan, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_sg)(
		struct dma_chan *chan,
		struct scatterlist *dst_sg, unsigned int dst_nents,
		struct scatterlist *src_sg, unsigned int src_nents,
		unsigned long flags);

	struct dma_async_tx_descriptor *(*device_prep_slave_sg)(
		struct dma_chan *chan, struct scatterlist *sgl,
		unsigned int sg_len, enum dma_transfer_direction direction,
		unsigned long flags, void *context);
	struct dma_async_tx_descriptor *(*device_prep_dma_cyclic)(
		struct dma_chan *chan, dma_addr_t buf_addr, size_t buf_len,
		size_t period_len, enum dma_transfer_direction direction,
		unsigned long flags, void *context);
	struct dma_async_tx_descriptor *(*device_prep_interleaved_dma)(
		struct dma_chan *chan, struct dma_interleaved_template *xt,
		unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_imm_data)(
		struct dma_chan *chan, dma_addr_t dst, u64 data,
		unsigned long flags);

	int (*device_config)(struct dma_chan *chan,
			     struct dma_slave_config *config);
	int (*device_pause)(struct dma_chan *chan);
	int (*device_resume)(struct dma_chan *chan);
	int (*device_terminate_all)(struct dma_chan *chan);
	void (*device_synchronize)(struct dma_chan *chan);

	enum dma_status (*device_tx_status)(struct dma_chan *chan,
					    dma_cookie_t cookie,
					    struct dma_tx_state *txstate);
	void (*device_issue_pending)(struct dma_chan *chan);
};

static inline int dmaengine_slave_config(struct dma_chan *chan,
					  struct dma_slave_config *config)
{
	if (chan->device->device_config)
		return chan->device->device_config(chan, config);

	return -ENOSYS;
}

static inline bool is_slave_direction(enum dma_transfer_direction direction)
{
	return (direction == DMA_MEM_TO_DEV) || (direction == DMA_DEV_TO_MEM);
}

static inline struct dma_async_tx_descriptor *dmaengine_prep_slave_single(
	struct dma_chan *chan, dma_addr_t buf, size_t len,
	enum dma_transfer_direction dir, unsigned long flags)
{
	struct scatterlist sg;
	sg_init_table(&sg, 1);
	sg_dma_address(&sg) = buf;
	sg_dma_len(&sg) = len;

	if (!chan || !chan->device || !chan->device->device_prep_slave_sg)
		return NULL;

	return chan->device->device_prep_slave_sg(chan, &sg, 1,
						  dir, flags, NULL);
}

static inline struct dma_async_tx_descriptor *dmaengine_prep_slave_sg(
	struct dma_chan *chan, struct scatterlist *sgl,	unsigned int sg_len,
	enum dma_transfer_direction dir, unsigned long flags)
{
	if (!chan || !chan->device || !chan->device->device_prep_slave_sg)
		return NULL;

	return chan->device->device_prep_slave_sg(chan, sgl, sg_len,
						  dir, flags, NULL);
}

#ifdef CONFIG_RAPIDIO_DMA_ENGINE
struct rio_dma_ext;
static inline struct dma_async_tx_descriptor *dmaengine_prep_rio_sg(
	struct dma_chan *chan, struct scatterlist *sgl,	unsigned int sg_len,
	enum dma_transfer_direction dir, unsigned long flags,
	struct rio_dma_ext *rio_ext)
{
	if (!chan || !chan->device || !chan->device->device_prep_slave_sg)
		return NULL;

	return chan->device->device_prep_slave_sg(chan, sgl, sg_len,
						  dir, flags, rio_ext);
}
#endif

static inline struct dma_async_tx_descriptor *dmaengine_prep_dma_cyclic(
		struct dma_chan *chan, dma_addr_t buf_addr, size_t buf_len,
		size_t period_len, enum dma_transfer_direction dir,
		unsigned long flags)
{
	if (!chan || !chan->device || !chan->device->device_prep_dma_cyclic)
		return NULL;

	return chan->device->device_prep_dma_cyclic(chan, buf_addr, buf_len,
						period_len, dir, flags, NULL);
}

static inline struct dma_async_tx_descriptor *dmaengine_prep_interleaved_dma(
		struct dma_chan *chan, struct dma_interleaved_template *xt,
		unsigned long flags)
{
	if (!chan || !chan->device || !chan->device->device_prep_interleaved_dma)
		return NULL;

	return chan->device->device_prep_interleaved_dma(chan, xt, flags);
}

static inline struct dma_async_tx_descriptor *dmaengine_prep_dma_memset(
		struct dma_chan *chan, dma_addr_t dest, int value, size_t len,
		unsigned long flags)
{
	if (!chan || !chan->device || !chan->device->device_prep_dma_memset)
		return NULL;

	return chan->device->device_prep_dma_memset(chan, dest, value,
						    len, flags);
}

static inline struct dma_async_tx_descriptor *dmaengine_prep_dma_sg(
		struct dma_chan *chan,
		struct scatterlist *dst_sg, unsigned int dst_nents,
		struct scatterlist *src_sg, unsigned int src_nents,
		unsigned long flags)
{
	if (!chan || !chan->device || !chan->device->device_prep_dma_sg)
		return NULL;

	return chan->device->device_prep_dma_sg(chan, dst_sg, dst_nents,
			src_sg, src_nents, flags);
}


static inline int dmaengine_terminate_all(struct dma_chan *chan)
{
	if (chan->device->device_terminate_all)
		return chan->device->device_terminate_all(chan);

	return -ENOSYS;
}


static inline int dmaengine_terminate_async(struct dma_chan *chan)
{
	if (chan->device->device_terminate_all)
		return chan->device->device_terminate_all(chan);

	return -EINVAL;
}


static inline void dmaengine_synchronize(struct dma_chan *chan)
{
	might_sleep();

	if (chan->device->device_synchronize)
		chan->device->device_synchronize(chan);
}


static inline int dmaengine_terminate_sync(struct dma_chan *chan)
{
	int ret;

	ret = dmaengine_terminate_async(chan);
	if (ret)
		return ret;

	dmaengine_synchronize(chan);

	return 0;
}

static inline int dmaengine_pause(struct dma_chan *chan)
{
	if (chan->device->device_pause)
		return chan->device->device_pause(chan);

	return -ENOSYS;
}

static inline int dmaengine_resume(struct dma_chan *chan)
{
	if (chan->device->device_resume)
		return chan->device->device_resume(chan);

	return -ENOSYS;
}

static inline enum dma_status dmaengine_tx_status(struct dma_chan *chan,
	dma_cookie_t cookie, struct dma_tx_state *state)
{
	return chan->device->device_tx_status(chan, cookie, state);
}

static inline dma_cookie_t dmaengine_submit(struct dma_async_tx_descriptor *desc)
{
	return desc->tx_submit(desc);
}

static inline bool dmaengine_check_align(enum dmaengine_alignment align,
					 size_t off1, size_t off2, size_t len)
{
	size_t mask;

	if (!align)
		return true;
	mask = (1 << align) - 1;
	if (mask & (off1 | off2 | len))
		return false;
	return true;
}

static inline bool is_dma_copy_aligned(struct dma_device *dev, size_t off1,
				       size_t off2, size_t len)
{
	return dmaengine_check_align(dev->copy_align, off1, off2, len);
}

static inline bool is_dma_xor_aligned(struct dma_device *dev, size_t off1,
				      size_t off2, size_t len)
{
	return dmaengine_check_align(dev->xor_align, off1, off2, len);
}

static inline bool is_dma_pq_aligned(struct dma_device *dev, size_t off1,
				     size_t off2, size_t len)
{
	return dmaengine_check_align(dev->pq_align, off1, off2, len);
}

static inline bool is_dma_fill_aligned(struct dma_device *dev, size_t off1,
				       size_t off2, size_t len)
{
	return dmaengine_check_align(dev->fill_align, off1, off2, len);
}

static inline void
dma_set_maxpq(struct dma_device *dma, int maxpq, int has_pq_continue)
{
	dma->max_pq = maxpq;
	if (has_pq_continue)
		dma->max_pq |= DMA_HAS_PQ_CONTINUE;
}

static inline bool dmaf_continue(enum dma_ctrl_flags flags)
{
	return (flags & DMA_PREP_CONTINUE) == DMA_PREP_CONTINUE;
}

static inline bool dmaf_p_disabled_continue(enum dma_ctrl_flags flags)
{
	enum dma_ctrl_flags mask = DMA_PREP_CONTINUE | DMA_PREP_PQ_DISABLE_P;

	return (flags & mask) == mask;
}

static inline bool dma_dev_has_pq_continue(struct dma_device *dma)
{
	return (dma->max_pq & DMA_HAS_PQ_CONTINUE) == DMA_HAS_PQ_CONTINUE;
}

static inline unsigned short dma_dev_to_maxpq(struct dma_device *dma)
{
	return dma->max_pq & ~DMA_HAS_PQ_CONTINUE;
}


static inline int dma_maxpq(struct dma_device *dma, enum dma_ctrl_flags flags)
{
	if (dma_dev_has_pq_continue(dma) || !dmaf_continue(flags))
		return dma_dev_to_maxpq(dma);
	else if (dmaf_p_disabled_continue(flags))
		return dma_dev_to_maxpq(dma) - 1;
	else if (dmaf_continue(flags))
		return dma_dev_to_maxpq(dma) - 3;
	BUG();
}

static inline size_t dmaengine_get_icg(bool inc, bool sgl, size_t icg,
				      size_t dir_icg)
{
	if (inc) {
		if (dir_icg)
			return dir_icg;
		else if (sgl)
			return icg;
	}

	return 0;
}

static inline size_t dmaengine_get_dst_icg(struct dma_interleaved_template *xt,
					   struct data_chunk *chunk)
{
	return dmaengine_get_icg(xt->dst_inc, xt->dst_sgl,
				 chunk->icg, chunk->dst_icg);
}

static inline size_t dmaengine_get_src_icg(struct dma_interleaved_template *xt,
					   struct data_chunk *chunk)
{
	return dmaengine_get_icg(xt->src_inc, xt->src_sgl,
				 chunk->icg, chunk->src_icg);
}



#ifdef CONFIG_DMA_ENGINE
void dmaengine_get(void);
void dmaengine_put(void);
#else
static inline void dmaengine_get(void)
{
}
static inline void dmaengine_put(void)
{
}
#endif

#ifdef CONFIG_ASYNC_TX_DMA
#define async_dmaengine_get()	dmaengine_get()
#define async_dmaengine_put()	dmaengine_put()
#ifndef CONFIG_ASYNC_TX_ENABLE_CHANNEL_SWITCH
#define async_dma_find_channel(type) dma_find_channel(DMA_ASYNC_TX)
#else
#define async_dma_find_channel(type) dma_find_channel(type)
#endif 
#else
static inline void async_dmaengine_get(void)
{
}
static inline void async_dmaengine_put(void)
{
}
static inline struct dma_chan *
async_dma_find_channel(enum dma_transaction_type type)
{
	return NULL;
}
#endif 
void dma_async_tx_descriptor_init(struct dma_async_tx_descriptor *tx,
				  struct dma_chan *chan);

static inline void async_tx_ack(struct dma_async_tx_descriptor *tx)
{
	tx->flags |= DMA_CTRL_ACK;
}

static inline void async_tx_clear_ack(struct dma_async_tx_descriptor *tx)
{
	tx->flags &= ~DMA_CTRL_ACK;
}

static inline bool async_tx_test_ack(struct dma_async_tx_descriptor *tx)
{
	return (tx->flags & DMA_CTRL_ACK) == DMA_CTRL_ACK;
}

#define dma_cap_set(tx, mask) __dma_cap_set((tx), &(mask))
static inline void
__dma_cap_set(enum dma_transaction_type tx_type, dma_cap_mask_t *dstp)
{
	set_bit(tx_type, dstp->bits);
}

#define dma_cap_clear(tx, mask) __dma_cap_clear((tx), &(mask))
static inline void
__dma_cap_clear(enum dma_transaction_type tx_type, dma_cap_mask_t *dstp)
{
	clear_bit(tx_type, dstp->bits);
}

#define dma_cap_zero(mask) __dma_cap_zero(&(mask))
static inline void __dma_cap_zero(dma_cap_mask_t *dstp)
{
	bitmap_zero(dstp->bits, DMA_TX_TYPE_END);
}

#define dma_has_cap(tx, mask) __dma_has_cap((tx), &(mask))
static inline int
__dma_has_cap(enum dma_transaction_type tx_type, dma_cap_mask_t *srcp)
{
	return test_bit(tx_type, srcp->bits);
}

#define for_each_dma_cap_mask(cap, mask) \
	for_each_set_bit(cap, mask.bits, DMA_TX_TYPE_END)


static inline void dma_async_issue_pending(struct dma_chan *chan)
{
	chan->device->device_issue_pending(chan);
}


static inline enum dma_status dma_async_is_tx_complete(struct dma_chan *chan,
	dma_cookie_t cookie, dma_cookie_t *last, dma_cookie_t *used)
{
	struct dma_tx_state state;
	enum dma_status status;

	status = chan->device->device_tx_status(chan, cookie, &state);
	if (last)
		*last = state.last;
	if (used)
		*used = state.used;
	return status;
}


static inline enum dma_status dma_async_is_complete(dma_cookie_t cookie,
			dma_cookie_t last_complete, dma_cookie_t last_used)
{
	if (last_complete <= last_used) {
		if ((cookie <= last_complete) || (cookie > last_used))
			return DMA_COMPLETE;
	} else {
		if ((cookie <= last_complete) && (cookie > last_used))
			return DMA_COMPLETE;
	}
	return DMA_IN_PROGRESS;
}

static inline void
dma_set_tx_state(struct dma_tx_state *st, dma_cookie_t last, dma_cookie_t used, u32 residue)
{
	if (st) {
		st->last = last;
		st->used = used;
		st->residue = residue;
	}
}

#ifdef CONFIG_DMA_ENGINE
struct dma_chan *dma_find_channel(enum dma_transaction_type tx_type);
enum dma_status dma_sync_wait(struct dma_chan *chan, dma_cookie_t cookie);
enum dma_status dma_wait_for_async_tx(struct dma_async_tx_descriptor *tx);
void dma_issue_pending_all(void);
struct dma_chan *__dma_request_channel(const dma_cap_mask_t *mask,
					dma_filter_fn fn, void *fn_param);
struct dma_chan *dma_request_slave_channel(struct device *dev, const char *name);

struct dma_chan *dma_request_chan(struct device *dev, const char *name);
struct dma_chan *dma_request_chan_by_mask(const dma_cap_mask_t *mask);

void dma_release_channel(struct dma_chan *chan);
int dma_get_slave_caps(struct dma_chan *chan, struct dma_slave_caps *caps);
#else
static inline struct dma_chan *dma_find_channel(enum dma_transaction_type tx_type)
{
	return NULL;
}
static inline enum dma_status dma_sync_wait(struct dma_chan *chan, dma_cookie_t cookie)
{
	return DMA_COMPLETE;
}
static inline enum dma_status dma_wait_for_async_tx(struct dma_async_tx_descriptor *tx)
{
	return DMA_COMPLETE;
}
static inline void dma_issue_pending_all(void)
{
}
static inline struct dma_chan *__dma_request_channel(const dma_cap_mask_t *mask,
					      dma_filter_fn fn, void *fn_param)
{
	return NULL;
}
static inline struct dma_chan *dma_request_slave_channel(struct device *dev,
							 const char *name)
{
	return NULL;
}
static inline struct dma_chan *dma_request_chan(struct device *dev,
						const char *name)
{
	return ERR_PTR(-ENODEV);
}
static inline struct dma_chan *dma_request_chan_by_mask(
						const dma_cap_mask_t *mask)
{
	return ERR_PTR(-ENODEV);
}
static inline void dma_release_channel(struct dma_chan *chan)
{
}
static inline int dma_get_slave_caps(struct dma_chan *chan,
				     struct dma_slave_caps *caps)
{
	return -ENXIO;
}
#endif

#define dma_request_slave_channel_reason(dev, name) dma_request_chan(dev, name)

static inline int dmaengine_desc_set_reuse(struct dma_async_tx_descriptor *tx)
{
	struct dma_slave_caps caps;
	int ret;

	ret = dma_get_slave_caps(tx->chan, &caps);
	if (ret)
		return ret;

	if (caps.descriptor_reuse) {
		tx->flags |= DMA_CTRL_REUSE;
		return 0;
	} else {
		return -EPERM;
	}
}

static inline void dmaengine_desc_clear_reuse(struct dma_async_tx_descriptor *tx)
{
	tx->flags &= ~DMA_CTRL_REUSE;
}

static inline bool dmaengine_desc_test_reuse(struct dma_async_tx_descriptor *tx)
{
	return (tx->flags & DMA_CTRL_REUSE) == DMA_CTRL_REUSE;
}

static inline int dmaengine_desc_free(struct dma_async_tx_descriptor *desc)
{
	
	if (dmaengine_desc_test_reuse(desc))
		return desc->desc_free(desc);
	else
		return -EPERM;
}



int dma_async_device_register(struct dma_device *device);
void dma_async_device_unregister(struct dma_device *device);
void dma_run_dependencies(struct dma_async_tx_descriptor *tx);
struct dma_chan *dma_get_slave_channel(struct dma_chan *chan);
struct dma_chan *dma_get_any_slave_channel(struct dma_device *device);
#define dma_request_channel(mask, x, y) __dma_request_channel(&(mask), x, y)
#define dma_request_slave_channel_compat(mask, x, y, dev, name) \
	__dma_request_slave_channel_compat(&(mask), x, y, dev, name)

static inline struct dma_chan
*__dma_request_slave_channel_compat(const dma_cap_mask_t *mask,
				  dma_filter_fn fn, void *fn_param,
				  struct device *dev, const char *name)
{
	struct dma_chan *chan;

	chan = dma_request_slave_channel(dev, name);
	if (chan)
		return chan;

	if (!fn || !fn_param)
		return NULL;

	return __dma_request_channel(mask, fn, fn_param);
}
#endif 
