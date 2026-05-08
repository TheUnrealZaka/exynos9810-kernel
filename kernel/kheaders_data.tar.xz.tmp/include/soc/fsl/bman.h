

#ifndef __FSL_BMAN_H
#define __FSL_BMAN_H


struct bm_buffer {
	union {
		struct {
			__be16 bpid; 
			__be16 hi; 
			__be32 lo; 
		};
		__be64 data;
	};
} __aligned(8);

static inline dma_addr_t bm_buf_addr(const struct bm_buffer *buf)
{
	return be64_to_cpu(buf->data) & 0xffffffffffffLLU;
}

static inline u64 bm_buffer_get64(const struct bm_buffer *buf)
{
	return be64_to_cpu(buf->data) & 0xffffffffffffLLU;
}

static inline void bm_buffer_set64(struct bm_buffer *buf, u64 addr)
{
	buf->hi = cpu_to_be16(upper_32_bits(addr));
	buf->lo = cpu_to_be32(lower_32_bits(addr));
}

static inline u8 bm_buffer_get_bpid(const struct bm_buffer *buf)
{
	return be16_to_cpu(buf->bpid) & 0xff;
}

static inline void bm_buffer_set_bpid(struct bm_buffer *buf, int bpid)
{
	buf->bpid = cpu_to_be16(bpid & 0xff);
}




struct bman_portal;
struct bman_pool;

#define BM_POOL_MAX		64 


struct bman_pool *bman_new_pool(void);


void bman_free_pool(struct bman_pool *pool);


int bman_get_bpid(const struct bman_pool *pool);


int bman_release(struct bman_pool *pool, const struct bm_buffer *bufs, u8 num);


int bman_acquire(struct bman_pool *pool, struct bm_buffer *bufs, u8 num);

#endif	
