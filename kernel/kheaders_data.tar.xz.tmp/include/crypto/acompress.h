
#ifndef _CRYPTO_ACOMP_H
#define _CRYPTO_ACOMP_H
#include <linux/crypto.h>

#define CRYPTO_ACOMP_ALLOC_OUTPUT	0x00000001


struct acomp_req {
	struct crypto_async_request base;
	struct scatterlist *src;
	struct scatterlist *dst;
	unsigned int slen;
	unsigned int dlen;
	u32 flags;
	void *__ctx[] CRYPTO_MINALIGN_ATTR;
};


struct crypto_acomp {
	int (*compress)(struct acomp_req *req);
	int (*decompress)(struct acomp_req *req);
	void (*dst_free)(struct scatterlist *dst);
	unsigned int reqsize;
	struct crypto_tfm base;
};


struct acomp_alg {
	int (*compress)(struct acomp_req *req);
	int (*decompress)(struct acomp_req *req);
	void (*dst_free)(struct scatterlist *dst);
	int (*init)(struct crypto_acomp *tfm);
	void (*exit)(struct crypto_acomp *tfm);
	unsigned int reqsize;
	struct crypto_alg base;
};




struct crypto_acomp *crypto_alloc_acomp(const char *alg_name, u32 type,
					u32 mask);

static inline struct crypto_tfm *crypto_acomp_tfm(struct crypto_acomp *tfm)
{
	return &tfm->base;
}

static inline struct acomp_alg *__crypto_acomp_alg(struct crypto_alg *alg)
{
	return container_of(alg, struct acomp_alg, base);
}

static inline struct crypto_acomp *__crypto_acomp_tfm(struct crypto_tfm *tfm)
{
	return container_of(tfm, struct crypto_acomp, base);
}

static inline struct acomp_alg *crypto_acomp_alg(struct crypto_acomp *tfm)
{
	return __crypto_acomp_alg(crypto_acomp_tfm(tfm)->__crt_alg);
}

static inline unsigned int crypto_acomp_reqsize(struct crypto_acomp *tfm)
{
	return tfm->reqsize;
}

static inline void acomp_request_set_tfm(struct acomp_req *req,
					 struct crypto_acomp *tfm)
{
	req->base.tfm = crypto_acomp_tfm(tfm);
}

static inline struct crypto_acomp *crypto_acomp_reqtfm(struct acomp_req *req)
{
	return __crypto_acomp_tfm(req->base.tfm);
}


static inline void crypto_free_acomp(struct crypto_acomp *tfm)
{
	crypto_destroy_tfm(tfm, crypto_acomp_tfm(tfm));
}

static inline int crypto_has_acomp(const char *alg_name, u32 type, u32 mask)
{
	type &= ~CRYPTO_ALG_TYPE_MASK;
	type |= CRYPTO_ALG_TYPE_ACOMPRESS;
	mask |= CRYPTO_ALG_TYPE_MASK;

	return crypto_has_alg(alg_name, type, mask);
}


struct acomp_req *acomp_request_alloc(struct crypto_acomp *tfm);


void acomp_request_free(struct acomp_req *req);


static inline void acomp_request_set_callback(struct acomp_req *req,
					      u32 flgs,
					      crypto_completion_t cmpl,
					      void *data)
{
	req->base.complete = cmpl;
	req->base.data = data;
	req->base.flags = flgs;
}


static inline void acomp_request_set_params(struct acomp_req *req,
					    struct scatterlist *src,
					    struct scatterlist *dst,
					    unsigned int slen,
					    unsigned int dlen)
{
	req->src = src;
	req->dst = dst;
	req->slen = slen;
	req->dlen = dlen;

	if (!req->dst)
		req->flags |= CRYPTO_ACOMP_ALLOC_OUTPUT;
}


static inline int crypto_acomp_compress(struct acomp_req *req)
{
	struct crypto_acomp *tfm = crypto_acomp_reqtfm(req);

	return tfm->compress(req);
}


static inline int crypto_acomp_decompress(struct acomp_req *req)
{
	struct crypto_acomp *tfm = crypto_acomp_reqtfm(req);

	return tfm->decompress(req);
}

#endif
