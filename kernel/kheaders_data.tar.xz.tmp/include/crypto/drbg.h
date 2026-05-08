

#ifndef _DRBG_H
#define _DRBG_H


#include <linux/random.h>
#include <linux/scatterlist.h>
#include <crypto/hash.h>
#include <crypto/skcipher.h>
#include <linux/module.h>
#include <linux/crypto.h>
#include <linux/slab.h>
#include <crypto/internal/rng.h>
#include <crypto/rng.h>
#include <linux/fips.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/workqueue.h>


struct drbg_string {
	const unsigned char *buf;
	size_t len;
	struct list_head list;
};

static inline void drbg_string_fill(struct drbg_string *string,
				    const unsigned char *buf, size_t len)
{
	string->buf = buf;
	string->len = len;
	INIT_LIST_HEAD(&string->list);
}

struct drbg_state;
typedef uint32_t drbg_flag_t;

struct drbg_core {
	drbg_flag_t flags;	
	__u8 statelen;		
	__u8 blocklen_bytes;	
	char cra_name[CRYPTO_MAX_ALG_NAME]; 
	 
	char backend_cra_name[CRYPTO_MAX_ALG_NAME];
};

struct drbg_state_ops {
	int (*update)(struct drbg_state *drbg, struct list_head *seed,
		      int reseed);
	int (*generate)(struct drbg_state *drbg,
			unsigned char *buf, unsigned int buflen,
			struct list_head *addtl);
	int (*crypto_init)(struct drbg_state *drbg);
	int (*crypto_fini)(struct drbg_state *drbg);

};

struct drbg_test_data {
	struct drbg_string *testentropy; 
};

enum drbg_seed_state {
	DRBG_SEED_STATE_UNSEEDED,
	DRBG_SEED_STATE_PARTIAL, 
	DRBG_SEED_STATE_FULL,
};

struct drbg_state {
	struct mutex drbg_mutex;	
	unsigned char *V;	
	unsigned char *Vbuf;
	
	unsigned char *C;
	unsigned char *Cbuf;
	
	size_t reseed_ctr;
	size_t reseed_threshold;
	 
	unsigned char *scratchpad;
	unsigned char *scratchpadbuf;
	void *priv_data;	

	struct crypto_skcipher *ctr_handle;	
	struct skcipher_request *ctr_req;	
	__u8 *ctr_null_value_buf;		
	__u8 *ctr_null_value;			
	__u8 *outscratchpadbuf;			
        __u8 *outscratchpad;			
	struct completion ctr_completion;	
	int ctr_async_err;			

	enum drbg_seed_state seeded;		
	bool pr;		
#ifdef CONFIG_CRYPTO_FIPS
	bool hw_entropy;
#endif
	bool fips_primed;	
	unsigned char *prev;	
	struct crypto_rng *jent;
	const struct drbg_state_ops *d_ops;
	const struct drbg_core *core;
	struct drbg_string test_data;
};

static inline __u8 drbg_statelen(struct drbg_state *drbg)
{
	if (drbg && drbg->core)
		return drbg->core->statelen;
	return 0;
}

static inline __u8 drbg_blocklen(struct drbg_state *drbg)
{
	if (drbg && drbg->core)
		return drbg->core->blocklen_bytes;
	return 0;
}

static inline __u8 drbg_keylen(struct drbg_state *drbg)
{
	if (drbg && drbg->core)
		return (drbg->core->statelen - drbg->core->blocklen_bytes);
	return 0;
}

static inline size_t drbg_max_request_bytes(struct drbg_state *drbg)
{
	
	return (1 << 16);
}

static inline size_t drbg_max_addtl(struct drbg_state *drbg)
{
	
#if (__BITS_PER_LONG == 32)
	
	return (SIZE_MAX - 1);
#else
	return (1UL<<35);
#endif
}

static inline size_t drbg_max_requests(struct drbg_state *drbg)
{
	
	return (1<<20);
}


static inline int crypto_drbg_get_bytes_addtl(struct crypto_rng *drng,
			unsigned char *outbuf, unsigned int outlen,
			struct drbg_string *addtl)
{
	return crypto_rng_generate(drng, addtl->buf, addtl->len,
				   outbuf, outlen);
}


static inline int crypto_drbg_get_bytes_addtl_test(struct crypto_rng *drng,
			unsigned char *outbuf, unsigned int outlen,
			struct drbg_string *addtl,
			struct drbg_test_data *test_data)
{
	crypto_rng_set_entropy(drng, test_data->testentropy->buf,
			       test_data->testentropy->len);
	return crypto_rng_generate(drng, addtl->buf, addtl->len,
				   outbuf, outlen);
}


static inline int crypto_drbg_reset_test(struct crypto_rng *drng,
					 struct drbg_string *pers,
					 struct drbg_test_data *test_data)
{
	crypto_rng_set_entropy(drng, test_data->testentropy->buf,
			       test_data->testentropy->len);
	return crypto_rng_reset(drng, pers->buf, pers->len);
}


#define DRBG_CTR	((drbg_flag_t)1<<0)
#define DRBG_HMAC	((drbg_flag_t)1<<1)
#define DRBG_HASH	((drbg_flag_t)1<<2)
#define DRBG_TYPE_MASK	(DRBG_CTR | DRBG_HMAC | DRBG_HASH)

#define DRBG_STRENGTH128	((drbg_flag_t)1<<3)
#define DRBG_STRENGTH192	((drbg_flag_t)1<<4)
#define DRBG_STRENGTH256	((drbg_flag_t)1<<5)
#define DRBG_STRENGTH_MASK	(DRBG_STRENGTH128 | DRBG_STRENGTH192 | \
				 DRBG_STRENGTH256)

enum drbg_prefixes {
	DRBG_PREFIX0 = 0x00,
	DRBG_PREFIX1,
	DRBG_PREFIX2,
	DRBG_PREFIX3
};

#endif 
