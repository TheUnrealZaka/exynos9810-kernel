

#ifndef _CRYPTO_AEAD_H
#define _CRYPTO_AEAD_H

#include <linux/crypto.h>
#include <linux/kernel.h>
#include <linux/slab.h>



struct crypto_aead;


struct aead_request {
	struct crypto_async_request base;

	unsigned int assoclen;
	unsigned int cryptlen;

	u8 *iv;

	struct scatterlist *src;
	struct scatterlist *dst;

	void *__ctx[] CRYPTO_MINALIGN_ATTR;
};


struct aead_alg {
	int (*setkey)(struct crypto_aead *tfm, const u8 *key,
	              unsigned int keylen);
	int (*setauthsize)(struct crypto_aead *tfm, unsigned int authsize);
	int (*encrypt)(struct aead_request *req);
	int (*decrypt)(struct aead_request *req);
	int (*init)(struct crypto_aead *tfm);
	void (*exit)(struct crypto_aead *tfm);

	const char *geniv;

	unsigned int ivsize;
	unsigned int maxauthsize;
	unsigned int chunksize;

	struct crypto_alg base;
};

struct crypto_aead {
	unsigned int authsize;
	unsigned int reqsize;

	struct crypto_tfm base;
};

static inline struct crypto_aead *__crypto_aead_cast(struct crypto_tfm *tfm)
{
	return container_of(tfm, struct crypto_aead, base);
}


struct crypto_aead *crypto_alloc_aead(const char *alg_name, u32 type, u32 mask);

static inline struct crypto_tfm *crypto_aead_tfm(struct crypto_aead *tfm)
{
	return &tfm->base;
}


static inline void crypto_free_aead(struct crypto_aead *tfm)
{
	crypto_destroy_tfm(tfm, crypto_aead_tfm(tfm));
}

static inline struct aead_alg *crypto_aead_alg(struct crypto_aead *tfm)
{
	return container_of(crypto_aead_tfm(tfm)->__crt_alg,
			    struct aead_alg, base);
}

static inline unsigned int crypto_aead_alg_ivsize(struct aead_alg *alg)
{
	return alg->ivsize;
}


static inline unsigned int crypto_aead_ivsize(struct crypto_aead *tfm)
{
	return crypto_aead_alg_ivsize(crypto_aead_alg(tfm));
}


static inline unsigned int crypto_aead_authsize(struct crypto_aead *tfm)
{
	return tfm->authsize;
}


static inline unsigned int crypto_aead_blocksize(struct crypto_aead *tfm)
{
	return crypto_tfm_alg_blocksize(crypto_aead_tfm(tfm));
}

static inline unsigned int crypto_aead_alignmask(struct crypto_aead *tfm)
{
	return crypto_tfm_alg_alignmask(crypto_aead_tfm(tfm));
}

static inline u32 crypto_aead_get_flags(struct crypto_aead *tfm)
{
	return crypto_tfm_get_flags(crypto_aead_tfm(tfm));
}

static inline void crypto_aead_set_flags(struct crypto_aead *tfm, u32 flags)
{
	crypto_tfm_set_flags(crypto_aead_tfm(tfm), flags);
}

static inline void crypto_aead_clear_flags(struct crypto_aead *tfm, u32 flags)
{
	crypto_tfm_clear_flags(crypto_aead_tfm(tfm), flags);
}


int crypto_aead_setkey(struct crypto_aead *tfm,
		       const u8 *key, unsigned int keylen);


int crypto_aead_setauthsize(struct crypto_aead *tfm, unsigned int authsize);

static inline struct crypto_aead *crypto_aead_reqtfm(struct aead_request *req)
{
	return __crypto_aead_cast(req->base.tfm);
}


static inline int crypto_aead_encrypt(struct aead_request *req)
{
	return crypto_aead_alg(crypto_aead_reqtfm(req))->encrypt(req);
}


static inline int crypto_aead_decrypt(struct aead_request *req)
{
	struct crypto_aead *aead = crypto_aead_reqtfm(req);

	if (req->cryptlen < crypto_aead_authsize(aead))
		return -EINVAL;

	return crypto_aead_alg(aead)->decrypt(req);
}




static inline unsigned int crypto_aead_reqsize(struct crypto_aead *tfm)
{
	return tfm->reqsize;
}


static inline void aead_request_set_tfm(struct aead_request *req,
					struct crypto_aead *tfm)
{
	req->base.tfm = crypto_aead_tfm(tfm);
}


static inline struct aead_request *aead_request_alloc(struct crypto_aead *tfm,
						      gfp_t gfp)
{
	struct aead_request *req;

	req = kmalloc(sizeof(*req) + crypto_aead_reqsize(tfm), gfp);

	if (likely(req))
		aead_request_set_tfm(req, tfm);

	return req;
}


static inline void aead_request_free(struct aead_request *req)
{
	kzfree(req);
}


static inline void aead_request_set_callback(struct aead_request *req,
					     u32 flags,
					     crypto_completion_t compl,
					     void *data)
{
	req->base.complete = compl;
	req->base.data = data;
	req->base.flags = flags;
}


static inline void aead_request_set_crypt(struct aead_request *req,
					  struct scatterlist *src,
					  struct scatterlist *dst,
					  unsigned int cryptlen, u8 *iv)
{
	req->src = src;
	req->dst = dst;
	req->cryptlen = cryptlen;
	req->iv = iv;
}


static inline void aead_request_set_ad(struct aead_request *req,
				       unsigned int assoclen)
{
	req->assoclen = assoclen;
}

#endif	
