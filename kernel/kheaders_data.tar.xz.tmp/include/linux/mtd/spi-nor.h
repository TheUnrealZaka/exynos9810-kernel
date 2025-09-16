

#ifndef __LINUX_MTD_SPI_NOR_H
#define __LINUX_MTD_SPI_NOR_H

#include <linux/bitops.h>
#include <linux/mtd/cfi.h>
#include <linux/mtd/mtd.h>


#define SNOR_MFR_ATMEL		CFI_MFR_ATMEL
#define SNOR_MFR_GIGADEVICE	0xc8
#define SNOR_MFR_INTEL		CFI_MFR_INTEL
#define SNOR_MFR_MICRON		CFI_MFR_ST 
#define SNOR_MFR_MACRONIX	CFI_MFR_MACRONIX
#define SNOR_MFR_SPANSION	CFI_MFR_AMD
#define SNOR_MFR_SST		CFI_MFR_SST
#define SNOR_MFR_WINBOND	0xef 




#define SPINOR_OP_WREN		0x06	
#define SPINOR_OP_RDSR		0x05	
#define SPINOR_OP_WRSR		0x01	
#define SPINOR_OP_READ		0x03	
#define SPINOR_OP_READ_FAST	0x0b	
#define SPINOR_OP_READ_1_1_2	0x3b	
#define SPINOR_OP_READ_1_1_4	0x6b	
#define SPINOR_OP_PP		0x02	
#define SPINOR_OP_BE_4K		0x20	
#define SPINOR_OP_BE_4K_PMC	0xd7	
#define SPINOR_OP_BE_32K	0x52	
#define SPINOR_OP_CHIP_ERASE	0xc7	
#define SPINOR_OP_SE		0xd8	
#define SPINOR_OP_RDID		0x9f	
#define SPINOR_OP_RDCR		0x35	
#define SPINOR_OP_RDFSR		0x70	


#define SPINOR_OP_READ4		0x13	
#define SPINOR_OP_READ4_FAST	0x0c	
#define SPINOR_OP_READ4_1_1_2	0x3c	
#define SPINOR_OP_READ4_1_1_4	0x6c	
#define SPINOR_OP_PP_4B		0x12	
#define SPINOR_OP_SE_4B		0xdc	


#define SPINOR_OP_BP		0x02	
#define SPINOR_OP_WRDI		0x04	
#define SPINOR_OP_AAI_WP	0xad	


#define SPINOR_OP_EN4B		0xb7	
#define SPINOR_OP_EX4B		0xe9	


#define SPINOR_OP_BRWR		0x17	


#define SPINOR_OP_RD_EVCR      0x65    
#define SPINOR_OP_WD_EVCR      0x61    


#define SR_WIP			BIT(0)	
#define SR_WEL			BIT(1)	

#define SR_BP0			BIT(2)	
#define SR_BP1			BIT(3)	
#define SR_BP2			BIT(4)	
#define SR_TB			BIT(5)	
#define SR_SRWD			BIT(7)	

#define SR_QUAD_EN_MX		BIT(6)	


#define EVCR_QUAD_EN_MICRON	BIT(7)	


#define FSR_READY		BIT(7)


#define CR_QUAD_EN_SPAN		BIT(1)	

enum read_mode {
	SPI_NOR_NORMAL = 0,
	SPI_NOR_FAST,
	SPI_NOR_DUAL,
	SPI_NOR_QUAD,
};

#define SPI_NOR_MAX_CMD_SIZE	8
enum spi_nor_ops {
	SPI_NOR_OPS_READ = 0,
	SPI_NOR_OPS_WRITE,
	SPI_NOR_OPS_ERASE,
	SPI_NOR_OPS_LOCK,
	SPI_NOR_OPS_UNLOCK,
};

enum spi_nor_option_flags {
	SNOR_F_USE_FSR		= BIT(0),
	SNOR_F_HAS_SR_TB	= BIT(1),
};


struct spi_nor {
	struct mtd_info		mtd;
	struct mutex		lock;
	struct device		*dev;
	u32			page_size;
	u8			addr_width;
	u8			erase_opcode;
	u8			read_opcode;
	u8			read_dummy;
	u8			program_opcode;
	enum read_mode		flash_read;
	bool			sst_write_second;
	u32			flags;
	u8			cmd_buf[SPI_NOR_MAX_CMD_SIZE];

	int (*prepare)(struct spi_nor *nor, enum spi_nor_ops ops);
	void (*unprepare)(struct spi_nor *nor, enum spi_nor_ops ops);
	int (*read_reg)(struct spi_nor *nor, u8 opcode, u8 *buf, int len);
	int (*write_reg)(struct spi_nor *nor, u8 opcode, u8 *buf, int len);

	ssize_t (*read)(struct spi_nor *nor, loff_t from,
			size_t len, u_char *read_buf);
	ssize_t (*write)(struct spi_nor *nor, loff_t to,
			size_t len, const u_char *write_buf);
	int (*erase)(struct spi_nor *nor, loff_t offs);

	int (*flash_lock)(struct spi_nor *nor, loff_t ofs, uint64_t len);
	int (*flash_unlock)(struct spi_nor *nor, loff_t ofs, uint64_t len);
	int (*flash_is_locked)(struct spi_nor *nor, loff_t ofs, uint64_t len);

	void *priv;
};

static inline void spi_nor_set_flash_node(struct spi_nor *nor,
					  struct device_node *np)
{
	mtd_set_of_node(&nor->mtd, np);
}

static inline struct device_node *spi_nor_get_flash_node(struct spi_nor *nor)
{
	return mtd_get_of_node(&nor->mtd);
}


int spi_nor_scan(struct spi_nor *nor, const char *name, enum read_mode mode);

#endif
