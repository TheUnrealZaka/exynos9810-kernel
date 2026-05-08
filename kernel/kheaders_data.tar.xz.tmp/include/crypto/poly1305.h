

#ifndef _CRYPTO_POLY1305_H
#define _CRYPTO_POLY1305_H

#include <linux/types.h>
#include <linux/crypto.h>

#define POLY1305_BLOCK_SIZE	16
#define POLY1305_KEY_SIZE	32
#define POLY1305_DIGEST_SIZE	16

struct poly1305_key {
	u32 r[5];	
};

struct poly1305_state {
	u32 h[5];	
};

struct poly1305_desc_ctx {
	
	struct poly1305_key r;
	
	u32 s[4];
	
	struct poly1305_state h;
	
	u8 buf[POLY1305_BLOCK_SIZE];
	
	unsigned int buflen;
	
	bool rset;
	
	bool sset;
};


void poly1305_core_setkey(struct poly1305_key *key, const u8 *raw_key);
static inline void poly1305_core_init(struct poly1305_state *state)
{
	memset(state->h, 0, sizeof(state->h));
}
void poly1305_core_blocks(struct poly1305_state *state,
			  const struct poly1305_key *key,
			  const void *src, unsigned int nblocks);
void poly1305_core_emit(const struct poly1305_state *state, void *dst);


int crypto_poly1305_init(struct shash_desc *desc);
unsigned int crypto_poly1305_setdesckey(struct poly1305_desc_ctx *dctx,
					const u8 *src, unsigned int srclen);
int crypto_poly1305_update(struct shash_desc *desc,
			   const u8 *src, unsigned int srclen);
int crypto_poly1305_final(struct shash_desc *desc, u8 *dst);

#endif
