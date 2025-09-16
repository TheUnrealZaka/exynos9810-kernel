
#ifndef _NET_PPP_COMP_H
#define _NET_PPP_COMP_H

#include <uapi/linux/ppp-comp.h>


struct module;



#ifndef DO_BSD_COMPRESS
#define DO_BSD_COMPRESS	1	
#endif
#ifndef DO_DEFLATE
#define DO_DEFLATE	1	
#endif
#define DO_PREDICTOR_1	0
#define DO_PREDICTOR_2	0



struct compressor {
	int	compress_proto;	

	
	void	*(*comp_alloc) (unsigned char *options, int opt_len);

	
	void	(*comp_free) (void *state);

	
	int	(*comp_init) (void *state, unsigned char *options,
			      int opt_len, int unit, int opthdr, int debug);

	
	void	(*comp_reset) (void *state);

	
	int     (*compress) (void *state, unsigned char *rptr,
			      unsigned char *obuf, int isize, int osize);

	
	void	(*comp_stat) (void *state, struct compstat *stats);

	
	void	*(*decomp_alloc) (unsigned char *options, int opt_len);

	
	void	(*decomp_free) (void *state);

	
	int	(*decomp_init) (void *state, unsigned char *options,
				int opt_len, int unit, int opthdr, int mru,
				int debug);

	
	void	(*decomp_reset) (void *state);

	
	int	(*decompress) (void *state, unsigned char *ibuf, int isize,
				unsigned char *obuf, int osize);

	
	void	(*incomp) (void *state, unsigned char *ibuf, int icnt);

	
	void	(*decomp_stat) (void *state, struct compstat *stats);

	
	struct module *owner;
	
	unsigned int comp_extra;
};



#define DECOMP_ERROR		-1	
#define DECOMP_FATALERROR	-2	

extern int ppp_register_compressor(struct compressor *);
extern void ppp_unregister_compressor(struct compressor *);
#endif 
