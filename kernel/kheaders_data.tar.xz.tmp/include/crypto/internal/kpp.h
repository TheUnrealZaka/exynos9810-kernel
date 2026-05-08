
#ifndef _CRYPTO_KPP_INT_H
#define _CRYPTO_KPP_INT_H
#include <crypto/kpp.h>
#include <crypto/algapi.h>


static inline void *kpp_request_ctx(struct kpp_request *req)
{
	return req->__ctx;
}

static inline void *kpp_tfm_ctx(struct crypto_kpp *tfm)
{
	return tfm->base.__crt_ctx;
}

static inline void kpp_request_complete(struct kpp_request *req, int err)
{
	req->base.complete(&req->base, err);
}

static inline const char *kpp_alg_name(struct crypto_kpp *tfm)
{
	return crypto_kpp_tfm(tfm)->__crt_alg->cra_name;
}


int crypto_register_kpp(struct kpp_alg *alg);


void crypto_unregister_kpp(struct kpp_alg *alg);

#endif
