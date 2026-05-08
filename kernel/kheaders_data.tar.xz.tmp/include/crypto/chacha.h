/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _CRYPTO_CHACHA_H
#define _CRYPTO_CHACHA_H

#include <linux/types.h>
#include <linux/crypto.h>


#define CHACHA_IV_SIZE		16

#define CHACHA_KEY_SIZE		32
#define CHACHA20_KEY_SIZE	32
#define CHACHA_BLOCK_SIZE	64
#define CHACHA20_BLOCK_SIZE	64


#define XCHACHA_IV_SIZE		32

struct chacha_ctx {
	u32 key[8];
	int nrounds;
};

void chacha_block(u32 *state, u8 *stream, int nrounds);
static inline void chacha20_block(u32 *state, u8 *stream)
{
	chacha_block(state, stream, 20);
}
void hchacha_block(const u32 *in, u32 *out, int nrounds);

void crypto_chacha_init(u32 *state, struct chacha_ctx *ctx, u8 *iv);

int crypto_chacha20_setkey(struct crypto_tfm *tfm, const u8 *key,
			   unsigned int keysize);
int crypto_chacha12_setkey(struct crypto_tfm *tfm, const u8 *key,
			   unsigned int keysize);

int crypto_chacha_crypt(struct blkcipher_desc *desc, struct scatterlist *dst,
			struct scatterlist *src, unsigned int nbytes);
int crypto_xchacha_crypt(struct blkcipher_desc *desc, struct scatterlist *dst,
			 struct scatterlist *src, unsigned int nbytes);

enum chacha_constants { 
	CHACHA_CONSTANT_EXPA = 0x61707865U,
	CHACHA_CONSTANT_ND_3 = 0x3320646eU,
	CHACHA_CONSTANT_2_BY = 0x79622d32U,
	CHACHA_CONSTANT_TE_K = 0x6b206574U
};

static inline void chacha_init_consts(u32 *state)
{
	state[0]  = CHACHA_CONSTANT_EXPA;
	state[1]  = CHACHA_CONSTANT_ND_3;
	state[2]  = CHACHA_CONSTANT_2_BY;
	state[3]  = CHACHA_CONSTANT_TE_K;
}

#endif 
