/* SPDX-License-Identifier: GPL-2.0-or-later */


#ifndef _DISKCIPHER_H_
#define _DISKCIPHER_H_

#include <linux/crypto.h>
#include <linux/blk_types.h>

struct diskcipher_alg;

enum iv_mode {
	IV_MODE_LBA, 
	IV_MODE_DUN, 
};

struct crypto_diskcipher {
	u32 algo;
	unsigned int ivsize;
	struct inode *inode;
	
	atomic_t status;
	struct crypto_tfm base;
	enum iv_mode ivmode;
};

struct diskcipher_test_request {
	unsigned int cryptlen;
	const u8 *iv;
	struct scatterlist *src;
	struct scatterlist *dst;
	bool enc;
};


struct diskcipher_alg {
	int (*init)(struct crypto_diskcipher *tfm);
	int (*exit)(struct crypto_diskcipher *tfm);
	int (*setkey)(struct crypto_diskcipher *tfm, const char *key,
			u32 keylen, bool persistent);
	int (*clearkey)(struct crypto_diskcipher *tfm);
	int (*crypt)(struct crypto_diskcipher *tfm, void *req);
	int (*clear)(struct crypto_diskcipher *tfm, void *req);
	int (*do_crypt)(struct crypto_diskcipher *tfm,
			struct diskcipher_test_request *req);
	struct device *dev;
	struct crypto_alg base;
};

static inline unsigned int crypto_diskcipher_ivsize(
		struct crypto_diskcipher *tfm)
{
	return tfm->ivsize;
}

static inline struct crypto_tfm *crypto_diskcipher_tfm(
		struct crypto_diskcipher *tfm)
{
	return &tfm->base;
}

static inline struct diskcipher_alg *__crypto_diskcipher_alg(
		struct crypto_alg *alg)
{
	return container_of(alg, struct diskcipher_alg, base);
}
static inline struct diskcipher_alg *crypto_diskcipher_alg(
		struct crypto_diskcipher *tfm)
{
	return __crypto_diskcipher_alg(crypto_diskcipher_tfm(tfm)->__crt_alg);
}

static inline struct crypto_diskcipher *__crypto_diskcipher_cast(
	struct crypto_tfm *tfm)
{
	return container_of(tfm, struct crypto_diskcipher, base);
}

int crypto_register_diskcipher(struct diskcipher_alg *alg);
void crypto_unregister_diskcipher(struct diskcipher_alg *alg);
int crypto_register_diskciphers(struct diskcipher_alg *algs, int count);
void crypto_unregister_diskciphers(struct diskcipher_alg *algs, int count);

#if defined(CONFIG_CRYPTO_DISKCIPHER)

struct crypto_diskcipher *crypto_alloc_diskcipher(const char *alg_name,
				  u32 type, u32 mask, bool force);


void crypto_free_diskcipher(struct crypto_diskcipher *tfm);


struct crypto_diskcipher *crypto_diskcipher_get(struct bio *bio);


void crypto_diskcipher_set(struct bio *bio, struct crypto_diskcipher *tfm,
				u64 dun);


int crypto_diskcipher_setkey(struct crypto_diskcipher *tfm, const char *key,
				u32 keylen, bool persistent,
				const struct inode *inode);


int crypto_diskcipher_clearkey(struct crypto_diskcipher *tfm);


int crypto_diskcipher_set_crypt(struct crypto_diskcipher *tfm, void *req);


int crypto_diskcipher_clear_crypt(struct crypto_diskcipher *tfm, void *req);


int diskcipher_do_crypt(struct crypto_diskcipher *tfm,
				struct diskcipher_test_request *req);


static inline void diskcipher_request_set_crypt(
	struct diskcipher_test_request *req,
	struct scatterlist *src, struct scatterlist *dst,
	unsigned int cryptlen, void *iv, bool enc)
{
	req->src = src;
	req->dst = dst;
	req->cryptlen = cryptlen;
	req->iv = iv;
	req->enc = enc;
}


bool crypto_diskcipher_blk_mergeble(struct bio *bio1, struct bio *bio2);

#else

#define crypto_alloc_diskcipher(a, b, c, d) ((void *)NULL)
#define crypto_free_diskcipher(a) ((void)0)
#define crypto_diskcipher_get(a) ((void *)NULL)
#define crypto_diskcipher_set(a, b, d) ((void)0)
#define crypto_diskcipher_clearkey(a) ((void)0)
#define crypto_diskcipher_setkey(a, b, c, d, e) (-EINVAL)
#define crypto_diskcipher_blk_mergeble(a, b) (0)
#endif
#endif	
