
#ifndef _CRYPTO_AKCIPHER_H
#define _CRYPTO_AKCIPHER_H
#include <linux/crypto.h>


struct akcipher_request {
	struct crypto_async_request base;
	struct scatterlist *src;
	struct scatterlist *dst;
	unsigned int src_len;
	unsigned int dst_len;
	void *__ctx[] CRYPTO_MINALIGN_ATTR;
};


struct crypto_akcipher {
	struct crypto_tfm base;
};


struct akcipher_alg {
	int (*sign)(struct akcipher_request *req);
	int (*verify)(struct akcipher_request *req);
	int (*encrypt)(struct akcipher_request *req);
	int (*decrypt)(struct akcipher_request *req);
	int (*set_pub_key)(struct crypto_akcipher *tfm, const void *key,
			   unsigned int keylen);
	int (*set_priv_key)(struct crypto_akcipher *tfm, const void *key,
			    unsigned int keylen);
	int (*max_size)(struct crypto_akcipher *tfm);
	int (*init)(struct crypto_akcipher *tfm);
	void (*exit)(struct crypto_akcipher *tfm);

	unsigned int reqsize;
	struct crypto_alg base;
};




struct crypto_akcipher *crypto_alloc_akcipher(const char *alg_name, u32 type,
					      u32 mask);

static inline struct crypto_tfm *crypto_akcipher_tfm(
	struct crypto_akcipher *tfm)
{
	return &tfm->base;
}

static inline struct akcipher_alg *__crypto_akcipher_alg(struct crypto_alg *alg)
{
	return container_of(alg, struct akcipher_alg, base);
}

static inline struct crypto_akcipher *__crypto_akcipher_tfm(
	struct crypto_tfm *tfm)
{
	return container_of(tfm, struct crypto_akcipher, base);
}

static inline struct akcipher_alg *crypto_akcipher_alg(
	struct crypto_akcipher *tfm)
{
	return __crypto_akcipher_alg(crypto_akcipher_tfm(tfm)->__crt_alg);
}

static inline unsigned int crypto_akcipher_reqsize(struct crypto_akcipher *tfm)
{
	return crypto_akcipher_alg(tfm)->reqsize;
}

static inline void akcipher_request_set_tfm(struct akcipher_request *req,
					    struct crypto_akcipher *tfm)
{
	req->base.tfm = crypto_akcipher_tfm(tfm);
}

static inline struct crypto_akcipher *crypto_akcipher_reqtfm(
	struct akcipher_request *req)
{
	return __crypto_akcipher_tfm(req->base.tfm);
}


static inline void crypto_free_akcipher(struct crypto_akcipher *tfm)
{
	crypto_destroy_tfm(tfm, crypto_akcipher_tfm(tfm));
}


static inline struct akcipher_request *akcipher_request_alloc(
	struct crypto_akcipher *tfm, gfp_t gfp)
{
	struct akcipher_request *req;

	req = kmalloc(sizeof(*req) + crypto_akcipher_reqsize(tfm), gfp);
	if (likely(req))
		akcipher_request_set_tfm(req, tfm);

	return req;
}


static inline void akcipher_request_free(struct akcipher_request *req)
{
	kzfree(req);
}


static inline void akcipher_request_set_callback(struct akcipher_request *req,
						 u32 flgs,
						 crypto_completion_t cmpl,
						 void *data)
{
	req->base.complete = cmpl;
	req->base.data = data;
	req->base.flags = flgs;
}


static inline void akcipher_request_set_crypt(struct akcipher_request *req,
					      struct scatterlist *src,
					      struct scatterlist *dst,
					      unsigned int src_len,
					      unsigned int dst_len)
{
	req->src = src;
	req->dst = dst;
	req->src_len = src_len;
	req->dst_len = dst_len;
}


static inline int crypto_akcipher_maxsize(struct crypto_akcipher *tfm)
{
	struct akcipher_alg *alg = crypto_akcipher_alg(tfm);

	return alg->max_size(tfm);
}


static inline int crypto_akcipher_encrypt(struct akcipher_request *req)
{
	struct crypto_akcipher *tfm = crypto_akcipher_reqtfm(req);
	struct akcipher_alg *alg = crypto_akcipher_alg(tfm);

	return alg->encrypt(req);
}


static inline int crypto_akcipher_decrypt(struct akcipher_request *req)
{
	struct crypto_akcipher *tfm = crypto_akcipher_reqtfm(req);
	struct akcipher_alg *alg = crypto_akcipher_alg(tfm);

	return alg->decrypt(req);
}


static inline int crypto_akcipher_sign(struct akcipher_request *req)
{
	struct crypto_akcipher *tfm = crypto_akcipher_reqtfm(req);
	struct akcipher_alg *alg = crypto_akcipher_alg(tfm);

	return alg->sign(req);
}


static inline int crypto_akcipher_verify(struct akcipher_request *req)
{
	struct crypto_akcipher *tfm = crypto_akcipher_reqtfm(req);
	struct akcipher_alg *alg = crypto_akcipher_alg(tfm);

	return alg->verify(req);
}


static inline int crypto_akcipher_set_pub_key(struct crypto_akcipher *tfm,
					      const void *key,
					      unsigned int keylen)
{
	struct akcipher_alg *alg = crypto_akcipher_alg(tfm);

	return alg->set_pub_key(tfm, key, keylen);
}


static inline int crypto_akcipher_set_priv_key(struct crypto_akcipher *tfm,
					       const void *key,
					       unsigned int keylen)
{
	struct akcipher_alg *alg = crypto_akcipher_alg(tfm);

	return alg->set_priv_key(tfm, key, keylen);
}
#endif
