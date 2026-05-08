
#ifndef SH_DMA_H
#define SH_DMA_H

#include <linux/dmaengine.h>
#include <linux/list.h>
#include <linux/shdma-base.h>
#include <linux/types.h>

struct device;


struct sh_dmae_slave {
	struct shdma_slave		shdma_slave;	
};


struct sh_dmae_slave_config {
	int		slave_id;
	dma_addr_t	addr;
	u32		chcr;
	char		mid_rid;
};


struct sh_dmae_channel {
	unsigned int	offset;
	unsigned int	dmars;
	unsigned int	chclr_offset;
	unsigned char	dmars_bit;
	unsigned char	chclr_bit;
};


struct sh_dmae_pdata {
	const struct sh_dmae_slave_config *slave;
	int slave_num;
	const struct sh_dmae_channel *channel;
	int channel_num;
	unsigned int ts_low_shift;
	unsigned int ts_low_mask;
	unsigned int ts_high_shift;
	unsigned int ts_high_mask;
	const unsigned int *ts_shift;
	int ts_shift_num;
	u16 dmaor_init;
	unsigned int chcr_offset;
	u32 chcr_ie_bit;

	unsigned int dmaor_is_32bit:1;
	unsigned int needs_tend_set:1;
	unsigned int no_dmars:1;
	unsigned int chclr_present:1;
	unsigned int chclr_bitwise:1;
	unsigned int slave_only:1;
};


#define DMAOR_AE	0x00000004	
#define DMAOR_NMIF	0x00000002
#define DMAOR_DME	0x00000001	


#define DM_INC	0x00004000	
#define DM_DEC	0x00008000	
#define DM_FIX	0x0000c000	
#define SM_INC	0x00001000	
#define SM_DEC	0x00002000	
#define SM_FIX	0x00003000	
#define RS_AUTO	0x00000400	
#define RS_ERS	0x00000800	
#define CHCR_DE	0x00000001	
#define CHCR_TE	0x00000002	
#define CHCR_IE	0x00000004	

#endif
