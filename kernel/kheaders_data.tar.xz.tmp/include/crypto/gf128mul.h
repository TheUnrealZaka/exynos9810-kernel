


#ifndef _CRYPTO_GF128MUL_H
#define _CRYPTO_GF128MUL_H

#include <crypto/b128ops.h>
#include <linux/slab.h>





void gf128mul_lle(be128 *a, const be128 *b);

void gf128mul_bbe(be128 *a, const be128 *b);


void gf128mul_x_ble(be128 *a, const be128 *b);



struct gf128mul_4k {
	be128 t[256];
};

struct gf128mul_4k *gf128mul_init_4k_lle(const be128 *g);
struct gf128mul_4k *gf128mul_init_4k_bbe(const be128 *g);
void gf128mul_4k_lle(be128 *a, struct gf128mul_4k *t);
void gf128mul_4k_bbe(be128 *a, struct gf128mul_4k *t);

static inline void gf128mul_free_4k(struct gf128mul_4k *t)
{
	kfree(t);
}




struct gf128mul_64k {
	struct gf128mul_4k *t[16];
};


struct gf128mul_64k *gf128mul_init_64k_lle(const be128 *g);
struct gf128mul_64k *gf128mul_init_64k_bbe(const be128 *g);
void gf128mul_free_64k(struct gf128mul_64k *t);
void gf128mul_64k_lle(be128 *a, struct gf128mul_64k *t);
void gf128mul_64k_bbe(be128 *a, struct gf128mul_64k *t);

#endif 
