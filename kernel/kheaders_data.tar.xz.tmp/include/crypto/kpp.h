

#ifndef _CRYPTO_KPP_
#define _CRYPTO_KPP_
#include <linux/crypto.h>


struct kpp_request {
	struct crypto_async_request base;
	struct scatterlist *src;
	struct scatterlist *dst;
	unsigned int src_len;
	unsigned int dst_len;
	void *__ctx[] CRYPTO_MINALIGN_ATTR;
};


struct crypto_kpp {
	struct crypto_tfm base;
};


struct kpp_alg {
	int (*set_secret)(struct crypto_kpp *tfm, void *buffer,
			  unsigned int len);
	int (*generate_public_key)(struct kpp_request *req);
	int (*compute_shared_secret)(struct kpp_request *req);

	int (*max_size)(struct crypto_kpp *tfm);

	int (*init)(struct crypto_kpp *tfm);
	void (*exit)(struct crypto_kpp *tfm);

	unsigned int reqsize;
	struct crypto_alg base;
};




struct crypto_kpp *crypto_alloc_kpp(const char *alg_name, u32 type, u32 mask);

static inline struct crypto_tfm *crypto_kpp_tfm(struct crypto_kpp *tfm)
{
	return &tfm->base;
}

static inline struct kpp_alg *__crypto_kpp_alg(struct crypto_alg *alg)
{
	return container_of(alg, struct kpp_alg, base);
}

static inline struct crypto_kpp *__crypto_kpp_tfm(struct crypto_tfm *tfm)
{
	return container_of(tfm, struct crypto_kpp, base);
}

static inline struct kpp_alg *crypto_kpp_alg(struct crypto_kpp *tfm)
{
	return __crypto_kpp_alg(crypto_kpp_tfm(tfm)->__crt_alg);
}

static inline unsigned int crypto_kpp_reqsize(struct crypto_kpp *tfm)
{
	return crypto_kpp_alg(tfm)->reqsize;
}

static inline void kpp_request_set_tfm(struct kpp_request *req,
				       struct crypto_kpp *tfm)
{
	req->base.tfm = crypto_kpp_tfm(tfm);
}

static inline struct crypto_kpp *crypto_kpp_reqtfm(struct kpp_request *req)
{
	return __crypto_kpp_tfm(req->base.tfm);
}


static inline void crypto_free_kpp(struct crypto_kpp *tfm)
{
	crypto_destroy_tfm(tfm, crypto_kpp_tfm(tfm));
}


static inline struct kpp_request *kpp_request_alloc(struct crypto_kpp *tfm,
						    gfp_t gfp)
{
	struct kpp_request *req;

	req = kmalloc(sizeof(*req) + crypto_kpp_reqsize(tfm), gfp);
	if (likely(req))
		kpp_request_set_tfm(req, tfm);

	return req;
}


static inline void kpp_request_free(struct kpp_request *req)
{
	kzfree(req);
}


static inline void kpp_request_set_callback(struct kpp_request *req,
					    u32 flgs,
					    crypto_completion_t cmpl,
					    void *data)
{
	req->base.complete = cmpl;
	req->base.data = data;
	req->base.flags = flgs;
}


static inline void kpp_request_set_input(struct kpp_request *req,
					 struct scatterlist *input,
					 unsigned int input_len)
{
	req->src = input;
	req->src_len = input_len;
}


static inline void kpp_request_set_output(struct kpp_request *req,
					  struct scatterlist *output,
					  unsigned int output_len)
{
	req->dst = output;
	req->dst_len = output_len;
}

enum {
	CRYPTO_KPP_SECRET_TYPE_UNKNOWN,
	CRYPTO_KPP_SECRET_TYPE_DH,
	CRYPTO_KPP_SECRET_TYPE_ECDH,
};


struct kpp_secret {
	unsigned short type;
	unsigned short len;
};


static inline int crypto_kpp_set_secret(struct crypto_kpp *tfm, void *buffer,
					unsigned int len)
{
	struct kpp_alg *alg = crypto_kpp_alg(tfm);

	return alg->set_secret(tfm, buffer, len);
}


static inline int crypto_kpp_generate_public_key(struct kpp_request *req)
{
	struct crypto_kpp *tfm = crypto_kpp_reqtfm(req);
	struct kpp_alg *alg = crypto_kpp_alg(tfm);

	return alg->generate_public_key(req);
}


static inline int crypto_kpp_compute_shared_secret(struct kpp_request *req)
{
	struct crypto_kpp *tfm = crypto_kpp_reqtfm(req);
	struct kpp_alg *alg = crypto_kpp_alg(tfm);

	return alg->compute_shared_secret(req);
}


static inline int crypto_kpp_maxsize(struct crypto_kpp *tfm)
{
	struct kpp_alg *alg = crypto_kpp_alg(tfm);

	return alg->max_size(tfm);
}

#endif
