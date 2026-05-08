#ifndef _SLHC_H
#define _SLHC_H







#include <linux/ip.h>
#include <linux/tcp.h>


#define SL_TYPE_IP 0x40
#define SL_TYPE_UNCOMPRESSED_TCP 0x70
#define SL_TYPE_COMPRESSED_TCP 0x80
#define SL_TYPE_ERROR 0x00


#define NEW_C	0x40	
#define NEW_I	0x20
#define NEW_S	0x08
#define NEW_A	0x04
#define NEW_W	0x02
#define NEW_U	0x01


#define SPECIAL_I (NEW_S|NEW_W|NEW_U)		
#define SPECIAL_D (NEW_S|NEW_A|NEW_W|NEW_U)	
#define SPECIALS_MASK (NEW_S|NEW_A|NEW_W|NEW_U)

#define TCP_PUSH_BIT 0x10



typedef __u8 byte_t;
typedef __u32 int32;


struct cstate {
	byte_t	cs_this;	
	bool	initialized;	
	struct cstate *next;	
	struct iphdr cs_ip;	
	struct tcphdr cs_tcp;
	unsigned char cs_ipopt[64];
	unsigned char cs_tcpopt[64];
	int cs_hsize;
};
#define NULLSLSTATE	(struct cstate *)0


struct slcompress {
	struct cstate *tstate;	
	struct cstate *rstate;	

	byte_t tslot_limit;	
	byte_t rslot_limit;	

	byte_t xmit_oldest;	
	byte_t xmit_current;	
	byte_t recv_current;	

	byte_t flags;
#define SLF_TOSS	0x01	

	int32 sls_o_nontcp;	
	int32 sls_o_tcp;	
	int32 sls_o_uncompressed;	
	int32 sls_o_compressed;	
	int32 sls_o_searches;	
	int32 sls_o_misses;	

	int32 sls_i_uncompressed;	
	int32 sls_i_compressed;	
	int32 sls_i_error;	
	int32 sls_i_tossed;	

	int32 sls_i_runt;
	int32 sls_i_badcheck;
};
#define NULLSLCOMPR	(struct slcompress *)0


struct slcompress *slhc_init(int rslots, int tslots);
void slhc_free(struct slcompress *comp);

int slhc_compress(struct slcompress *comp, unsigned char *icp, int isize,
		  unsigned char *ocp, unsigned char **cpp, int compress_cid);
int slhc_uncompress(struct slcompress *comp, unsigned char *icp, int isize);
int slhc_remember(struct slcompress *comp, unsigned char *icp, int isize);
int slhc_toss(struct slcompress *comp);

#endif	
