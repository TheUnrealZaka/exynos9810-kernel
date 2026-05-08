
#ifndef _RSA_HELPER_
#define _RSA_HELPER_
#include <linux/types.h>


struct rsa_key {
	const u8 *n;
	const u8 *e;
	const u8 *d;
	const u8 *p;
	const u8 *q;
	const u8 *dp;
	const u8 *dq;
	const u8 *qinv;
	size_t n_sz;
	size_t e_sz;
	size_t d_sz;
	size_t p_sz;
	size_t q_sz;
	size_t dp_sz;
	size_t dq_sz;
	size_t qinv_sz;
};

int rsa_parse_pub_key(struct rsa_key *rsa_key, const void *key,
		      unsigned int key_len);

int rsa_parse_priv_key(struct rsa_key *rsa_key, const void *key,
		       unsigned int key_len);

extern struct crypto_template rsa_pkcs1pad_tmpl;
#endif
