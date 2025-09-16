
#ifndef __LINUX_MTD_BBM_H
#define __LINUX_MTD_BBM_H


#define NAND_MAX_CHIPS		8


struct nand_bbt_descr {
	int options;
	int pages[NAND_MAX_CHIPS];
	int offs;
	int veroffs;
	uint8_t version[NAND_MAX_CHIPS];
	int len;
	int maxblocks;
	int reserved_block_code;
	uint8_t *pattern;
};




#define NAND_BBT_NRBITS_MSK	0x0000000F
#define NAND_BBT_1BIT		0x00000001
#define NAND_BBT_2BIT		0x00000002
#define NAND_BBT_4BIT		0x00000004
#define NAND_BBT_8BIT		0x00000008

#define NAND_BBT_LASTBLOCK	0x00000010

#define NAND_BBT_ABSPAGE	0x00000020

#define NAND_BBT_PERCHIP	0x00000080

#define NAND_BBT_VERSION	0x00000100

#define NAND_BBT_CREATE		0x00000200

#define NAND_BBT_CREATE_EMPTY	0x00000400

#define NAND_BBT_WRITE		0x00002000

#define NAND_BBT_SAVECONTENT	0x00004000

#define NAND_BBT_SCAN2NDPAGE	0x00008000

#define NAND_BBT_SCANLASTPAGE	0x00010000

#define NAND_BBT_USE_FLASH	0x00020000

#define NAND_BBT_NO_OOB		0x00040000

#define NAND_BBT_NO_OOB_BBM	0x00080000


#define NAND_BBT_DYNAMICSTRUCT	0x80000000


#define NAND_BBT_SCAN_MAXBLOCKS	4


#define NAND_SMALL_BADBLOCK_POS		5
#define NAND_LARGE_BADBLOCK_POS		0
#define ONENAND_BADBLOCK_POS		0


#define ONENAND_BBT_READ_ERROR		1
#define ONENAND_BBT_READ_ECC_ERROR	2
#define ONENAND_BBT_READ_FATAL_ERROR	4


struct bbm_info {
	int bbt_erase_shift;
	int badblockpos;
	int options;

	uint8_t *bbt;

	int (*isbad_bbt)(struct mtd_info *mtd, loff_t ofs, int allowbbt);

	
	struct nand_bbt_descr *badblock_pattern;

	void *priv;
};


extern int onenand_default_bbt(struct mtd_info *mtd);

#endif	
