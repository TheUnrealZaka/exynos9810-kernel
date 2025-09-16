

#ifndef _UIO_PRUSS_H_
#define _UIO_PRUSS_H_


struct uio_pruss_pdata {
	u32		pintc_base;
	struct gen_pool *sram_pool;
};
#endif 
