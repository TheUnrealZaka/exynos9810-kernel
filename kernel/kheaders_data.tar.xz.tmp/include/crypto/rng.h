

#ifndef _CRYPTO_RNG_H
#define _CRYPTO_RNG_H

#include <linux/crypto.h>

struct crypto_rng;


struct rng_alg {
	int (*generate)(struct crypto_rng *tfm,
			const u8 *src, unsigned int slen,
			u8 *dst, unsigned int dlen);
	int (*seed)(struct crypto_rng *tfm, const u8 *seed, unsigned int slen);
	void (*set_ent)(struct crypto_rng *tfm, const u8 *data,
			unsigned int len);

	unsigned int seedsize;

	struct crypto_alg base;
};

struct crypto_rng {
	struct crypto_tfm base;
};

extern struct crypto_rng *crypto_default_rng;

int crypto_get_default_rng(void);
void crypto_put_default_rng(void);




struct crypto_rng *crypto_alloc_rng(const char *alg_name, u32 type, u32 mask);

static inline struct crypto_tfm *crypto_rng_tfm(struct crypto_rng *tfm)
{
	return &tfm->base;
}


static inline struct rng_alg *crypto_rng_alg(struct crypto_rng *tfm)
{
	return container_of(crypto_rng_tfm(tfm)->__crt_alg,
			    struct rng_alg, base);
}


static inline void crypto_free_rng(struct crypto_rng *tfm)
{
	crypto_destroy_tfm(tfm, crypto_rng_tfm(tfm));
}


static inline int crypto_rng_generate(struct crypto_rng *tfm,
				      const u8 *src, unsigned int slen,
				      u8 *dst, unsigned int dlen)
{
	return crypto_rng_alg(tfm)->generate(tfm, src, slen, dst, dlen);
}


static inline int crypto_rng_get_bytes(struct crypto_rng *tfm,
				       u8 *rdata, unsigned int dlen)
{
	return crypto_rng_generate(tfm, NULL, 0, rdata, dlen);
}


int crypto_rng_reset(struct crypto_rng *tfm, const u8 *seed,
		     unsigned int slen);


static inline int crypto_rng_seedsize(struct crypto_rng *tfm)
{
	return crypto_rng_alg(tfm)->seedsize;
}


int crypto_rng_check_entropy(struct crypto_rng *rng);

#endif
