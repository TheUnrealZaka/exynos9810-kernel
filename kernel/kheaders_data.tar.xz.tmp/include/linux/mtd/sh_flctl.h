

#ifndef __SH_FLCTL_H__
#define __SH_FLCTL_H__

#include <linux/completion.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/pm_qos.h>


#define FLCMNCR(f)		(f->reg + 0x0)
#define FLCMDCR(f)		(f->reg + 0x4)
#define FLCMCDR(f)		(f->reg + 0x8)
#define FLADR(f)		(f->reg + 0xC)
#define FLADR2(f)		(f->reg + 0x3C)
#define FLDATAR(f)		(f->reg + 0x10)
#define FLDTCNTR(f)		(f->reg + 0x14)
#define FLINTDMACR(f)		(f->reg + 0x18)
#define FLBSYTMR(f)		(f->reg + 0x1C)
#define FLBSYCNT(f)		(f->reg + 0x20)
#define FLDTFIFO(f)		(f->reg + 0x24)
#define FLECFIFO(f)		(f->reg + 0x28)
#define FLTRCR(f)		(f->reg + 0x2C)
#define FLHOLDCR(f)		(f->reg + 0x38)
#define	FL4ECCRESULT0(f)	(f->reg + 0x80)
#define	FL4ECCRESULT1(f)	(f->reg + 0x84)
#define	FL4ECCRESULT2(f)	(f->reg + 0x88)
#define	FL4ECCRESULT3(f)	(f->reg + 0x8C)
#define	FL4ECCCR(f)		(f->reg + 0x90)
#define	FL4ECCCNT(f)		(f->reg + 0x94)
#define	FLERRADR(f)		(f->reg + 0x98)


#define _4ECCCNTEN	(0x1 << 24)
#define _4ECCEN		(0x1 << 23)
#define _4ECCCORRECT	(0x1 << 22)
#define SHBUSSEL	(0x1 << 20)
#define SEL_16BIT	(0x1 << 19)
#define SNAND_E		(0x1 << 18)	
#define QTSEL_E		(0x1 << 17)
#define ENDIAN		(0x1 << 16)	
#define FCKSEL_E	(0x1 << 15)
#define ACM_SACCES_MODE	(0x01 << 10)
#define NANWF_E		(0x1 << 9)
#define SE_D		(0x1 << 8)	
#define	CE1_ENABLE	(0x1 << 4)	
#define	CE0_ENABLE	(0x1 << 3)	
#define	TYPESEL_SET	(0x1 << 0)


#define PULSE3	(0x1 << 27)
#define PULSE2	(0x1 << 17)
#define PULSE1	(0x1 << 15)
#define PULSE0	(0x1 << 9)
#define CLK_8B_0_5			PULSE1
#define CLK_8B_1			0x0
#define CLK_8B_1_5			(PULSE1 | PULSE2)
#define CLK_8B_2			PULSE0
#define CLK_8B_3			(PULSE0 | PULSE1 | PULSE2)
#define CLK_8B_4			(PULSE0 | PULSE2)
#define CLK_16B_6L_2H			PULSE0
#define CLK_16B_9L_3H			(PULSE0 | PULSE1 | PULSE2)
#define CLK_16B_12L_4H			(PULSE0 | PULSE2)


#define ADRCNT2_E	(0x1 << 31)	
#define ADRMD_E		(0x1 << 26)	
#define CDSRC_E		(0x1 << 25)	
#define DOSR_E		(0x1 << 24)	
#define SELRW		(0x1 << 21)	
#define DOADR_E		(0x1 << 20)	
#define ADRCNT_1	(0x00 << 18)	
#define ADRCNT_2	(0x01 << 18)	
#define ADRCNT_3	(0x02 << 18)	
#define ADRCNT_4	(0x03 << 18)	
#define DOCMD2_E	(0x1 << 17)	
#define DOCMD1_E	(0x1 << 16)	


#define ESTERINTE	(0x1 << 24)	
#define AC1CLR		(0x1 << 19)	
#define AC0CLR		(0x1 << 18)	
#define DREQ0EN		(0x1 << 16)	
#define ECERB		(0x1 << 9)	
#define STERB		(0x1 << 8)	
#define STERINTE	(0x1 << 4)	


#define TRSTRT		(0x1 << 0)	
#define TREND		(0x1 << 1)	


#define HOLDEN		(0x1 << 0)


#define	_4ECCFA		(0x1 << 2)	
#define	_4ECCEND	(0x1 << 1)	
#define	_4ECCEXST	(0x1 << 0)	

#define LOOP_TIMEOUT_MAX	0x00010000

enum flctl_ecc_res_t {
	FL_SUCCESS,
	FL_REPAIRABLE,
	FL_ERROR,
	FL_TIMEOUT
};

struct dma_chan;

struct sh_flctl {
	struct nand_chip	chip;
	struct platform_device	*pdev;
	struct dev_pm_qos_request pm_qos;
	void __iomem		*reg;
	resource_size_t		fifo;

	uint8_t	done_buff[2048 + 64];	
	int	read_bytes;
	unsigned int index;
	int	seqin_column;		
	int	seqin_page_addr;	
	uint32_t seqin_read_cmd;		
	int	erase1_page_addr;	
	uint32_t erase_ADRCNT;		
	uint32_t rw_ADRCNT;	
	uint32_t flcmncr_base;	
	uint32_t flintdmacr_base;	

	unsigned page_size:1;	
	unsigned hwecc:1;	
	unsigned holden:1;	
	unsigned qos_request:1;	

	
	struct dma_chan		*chan_fifo0_rx;
	struct dma_chan		*chan_fifo0_tx;
	struct completion	dma_complete;
};

struct sh_flctl_platform_data {
	struct mtd_partition	*parts;
	int			nr_parts;
	unsigned long		flcmncr_val;

	unsigned has_hwecc:1;
	unsigned use_holden:1;

	unsigned int            slave_id_fifo0_tx;
	unsigned int            slave_id_fifo0_rx;
};

static inline struct sh_flctl *mtd_to_flctl(struct mtd_info *mtdinfo)
{
	return container_of(mtd_to_nand(mtdinfo), struct sh_flctl, chip);
}

#endif	
