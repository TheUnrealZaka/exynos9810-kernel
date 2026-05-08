
#ifndef __LINUX_MTD_NAND_H
#define __LINUX_MTD_NAND_H

#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/flashchip.h>
#include <linux/mtd/bbm.h>

struct nand_chip;
struct mtd_info;
struct nand_flash_dev;
struct device_node;


int nand_scan(struct mtd_info *mtd, int max_chips);

int nand_scan_ident(struct mtd_info *mtd, int max_chips,
			   struct nand_flash_dev *table);
int nand_scan_tail(struct mtd_info *mtd);


void nand_release(struct nand_chip *chip);


void nand_wait_ready(struct mtd_info *mtd);


int nand_lock(struct mtd_info *mtd, loff_t ofs, uint64_t len);


int nand_unlock(struct mtd_info *mtd, loff_t ofs, uint64_t len);


#define NAND_MAX_CHIPS		8



#define NAND_NCE		0x01

#define NAND_CLE		0x02

#define NAND_ALE		0x04

#define NAND_CTRL_CLE		(NAND_NCE | NAND_CLE)
#define NAND_CTRL_ALE		(NAND_NCE | NAND_ALE)
#define NAND_CTRL_CHANGE	0x80


#define NAND_CMD_READ0		0
#define NAND_CMD_READ1		1
#define NAND_CMD_RNDOUT		5
#define NAND_CMD_PAGEPROG	0x10
#define NAND_CMD_READOOB	0x50
#define NAND_CMD_ERASE1		0x60
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_SEQIN		0x80
#define NAND_CMD_RNDIN		0x85
#define NAND_CMD_READID		0x90
#define NAND_CMD_ERASE2		0xd0
#define NAND_CMD_PARAM		0xec
#define NAND_CMD_GET_FEATURES	0xee
#define NAND_CMD_SET_FEATURES	0xef
#define NAND_CMD_RESET		0xff

#define NAND_CMD_LOCK		0x2a
#define NAND_CMD_UNLOCK1	0x23
#define NAND_CMD_UNLOCK2	0x24


#define NAND_CMD_READSTART	0x30
#define NAND_CMD_RNDOUTSTART	0xE0
#define NAND_CMD_CACHEDPROG	0x15

#define NAND_CMD_NONE		-1


#define NAND_STATUS_FAIL	0x01
#define NAND_STATUS_FAIL_N1	0x02
#define NAND_STATUS_TRUE_READY	0x20
#define NAND_STATUS_READY	0x40
#define NAND_STATUS_WP		0x80


typedef enum {
	NAND_ECC_NONE,
	NAND_ECC_SOFT,
	NAND_ECC_HW,
	NAND_ECC_HW_SYNDROME,
	NAND_ECC_HW_OOB_FIRST,
} nand_ecc_modes_t;

enum nand_ecc_algo {
	NAND_ECC_UNKNOWN,
	NAND_ECC_HAMMING,
	NAND_ECC_BCH,
};



#define NAND_ECC_READ		0

#define NAND_ECC_WRITE		1

#define NAND_ECC_READSYN	2


#define NAND_ECC_GENERIC_ERASED_CHECK	BIT(0)
#define NAND_ECC_MAXIMIZE		BIT(1)


#define NAND_GET_DEVICE		0x80




#define NAND_BUSWIDTH_16	0x00000002

#define NAND_CACHEPRG		0x00000008

#define NAND_NEED_READRDY	0x00000100


#define NAND_NO_SUBPAGE_WRITE	0x00000200


#define NAND_BROKEN_XD		0x00000400


#define NAND_ROM		0x00000800


#define NAND_SUBPAGE_READ	0x00001000


#define NAND_NEED_SCRAMBLING	0x00002000


#define NAND_SAMSUNG_LP_OPTIONS NAND_CACHEPRG


#define NAND_HAS_CACHEPROG(chip) ((chip->options & NAND_CACHEPRG))
#define NAND_HAS_SUBPAGE_READ(chip) ((chip->options & NAND_SUBPAGE_READ))



#define NAND_SKIP_BBTSCAN	0x00010000

#define NAND_OWN_BUFFERS	0x00020000

#define NAND_SCAN_SILENT_NODEV	0x00040000

#define NAND_BUSWIDTH_AUTO      0x00080000

#define NAND_USE_BOUNCE_BUFFER	0x00100000



#define NAND_CONTROLLER_ALLOC	0x80000000


#define NAND_CI_CHIPNR_MSK	0x03
#define NAND_CI_CELLTYPE_MSK	0x0C
#define NAND_CI_CELLTYPE_SHIFT	2


#define ONFI_FEATURE_16_BIT_BUS		(1 << 0)
#define ONFI_FEATURE_EXT_PARAM_PAGE	(1 << 7)


#define ONFI_TIMING_MODE_0		(1 << 0)
#define ONFI_TIMING_MODE_1		(1 << 1)
#define ONFI_TIMING_MODE_2		(1 << 2)
#define ONFI_TIMING_MODE_3		(1 << 3)
#define ONFI_TIMING_MODE_4		(1 << 4)
#define ONFI_TIMING_MODE_5		(1 << 5)
#define ONFI_TIMING_MODE_UNKNOWN	(1 << 6)


#define ONFI_FEATURE_ADDR_TIMING_MODE	0x1


#define ONFI_FEATURE_ADDR_READ_RETRY	0x89


#define ONFI_SUBFEATURE_PARAM_LEN	4


#define ONFI_OPT_CMD_SET_GET_FEATURES	(1 << 2)

struct nand_onfi_params {
	
	
	u8 sig[4];
	__le16 revision;
	__le16 features;
	__le16 opt_cmd;
	u8 reserved0[2];
	__le16 ext_param_page_length; 
	u8 num_of_param_pages;        
	u8 reserved1[17];

	
	char manufacturer[12];
	char model[20];
	u8 jedec_id;
	__le16 date_code;
	u8 reserved2[13];

	
	__le32 byte_per_page;
	__le16 spare_bytes_per_page;
	__le32 data_bytes_per_ppage;
	__le16 spare_bytes_per_ppage;
	__le32 pages_per_block;
	__le32 blocks_per_lun;
	u8 lun_count;
	u8 addr_cycles;
	u8 bits_per_cell;
	__le16 bb_per_lun;
	__le16 block_endurance;
	u8 guaranteed_good_blocks;
	__le16 guaranteed_block_endurance;
	u8 programs_per_page;
	u8 ppage_attr;
	u8 ecc_bits;
	u8 interleaved_bits;
	u8 interleaved_ops;
	u8 reserved3[13];

	
	u8 io_pin_capacitance_max;
	__le16 async_timing_mode;
	__le16 program_cache_timing_mode;
	__le16 t_prog;
	__le16 t_bers;
	__le16 t_r;
	__le16 t_ccs;
	__le16 src_sync_timing_mode;
	u8 src_ssync_features;
	__le16 clk_pin_capacitance_typ;
	__le16 io_pin_capacitance_typ;
	__le16 input_pin_capacitance_typ;
	u8 input_pin_capacitance_max;
	u8 driver_strength_support;
	__le16 t_int_r;
	__le16 t_adl;
	u8 reserved4[8];

	
	__le16 vendor_revision;
	u8 vendor[88];

	__le16 crc;
} __packed;

#define ONFI_CRC_BASE	0x4F4E


struct onfi_ext_ecc_info {
	u8 ecc_bits;
	u8 codeword_size;
	__le16 bb_per_lun;
	__le16 block_endurance;
	u8 reserved[2];
} __packed;

#define ONFI_SECTION_TYPE_0	0	
#define ONFI_SECTION_TYPE_1	1	
#define ONFI_SECTION_TYPE_2	2	
struct onfi_ext_section {
	u8 type;
	u8 length;
} __packed;

#define ONFI_EXT_SECTION_MAX 8


struct onfi_ext_param_page {
	__le16 crc;
	u8 sig[4];             
	u8 reserved0[10];
	struct onfi_ext_section sections[ONFI_EXT_SECTION_MAX];

	
} __packed;

struct nand_onfi_vendor_micron {
	u8 two_plane_read;
	u8 read_cache;
	u8 read_unique_id;
	u8 dq_imped;
	u8 dq_imped_num_settings;
	u8 dq_imped_feat_addr;
	u8 rb_pulldown_strength;
	u8 rb_pulldown_strength_feat_addr;
	u8 rb_pulldown_strength_num_settings;
	u8 otp_mode;
	u8 otp_page_start;
	u8 otp_data_prot_addr;
	u8 otp_num_pages;
	u8 otp_feat_addr;
	u8 read_retry_options;
	u8 reserved[72];
	u8 param_revision;
} __packed;

struct jedec_ecc_info {
	u8 ecc_bits;
	u8 codeword_size;
	__le16 bb_per_lun;
	__le16 block_endurance;
	u8 reserved[2];
} __packed;


#define JEDEC_FEATURE_16_BIT_BUS	(1 << 0)

struct nand_jedec_params {
	
	
	u8 sig[4];
	__le16 revision;
	__le16 features;
	u8 opt_cmd[3];
	__le16 sec_cmd;
	u8 num_of_param_pages;
	u8 reserved0[18];

	
	char manufacturer[12];
	char model[20];
	u8 jedec_id[6];
	u8 reserved1[10];

	
	__le32 byte_per_page;
	__le16 spare_bytes_per_page;
	u8 reserved2[6];
	__le32 pages_per_block;
	__le32 blocks_per_lun;
	u8 lun_count;
	u8 addr_cycles;
	u8 bits_per_cell;
	u8 programs_per_page;
	u8 multi_plane_addr;
	u8 multi_plane_op_attr;
	u8 reserved3[38];

	
	__le16 async_sdr_speed_grade;
	__le16 toggle_ddr_speed_grade;
	__le16 sync_ddr_speed_grade;
	u8 async_sdr_features;
	u8 toggle_ddr_features;
	u8 sync_ddr_features;
	__le16 t_prog;
	__le16 t_bers;
	__le16 t_r;
	__le16 t_r_multi_plane;
	__le16 t_ccs;
	__le16 io_pin_capacitance_typ;
	__le16 input_pin_capacitance_typ;
	__le16 clk_pin_capacitance_typ;
	u8 driver_strength_support;
	__le16 t_adl;
	u8 reserved4[36];

	
	u8 guaranteed_good_blocks;
	__le16 guaranteed_block_endurance;
	struct jedec_ecc_info ecc_info[4];
	u8 reserved5[29];

	
	u8 reserved6[148];

	
	__le16 vendor_rev_num;
	u8 reserved7[88];

	
	__le16 crc;
} __packed;


struct nand_hw_control {
	spinlock_t lock;
	struct nand_chip *active;
	wait_queue_head_t wq;
};

static inline void nand_hw_control_init(struct nand_hw_control *nfc)
{
	nfc->active = NULL;
	spin_lock_init(&nfc->lock);
	init_waitqueue_head(&nfc->wq);
}


struct nand_ecc_ctrl {
	nand_ecc_modes_t mode;
	enum nand_ecc_algo algo;
	int steps;
	int size;
	int bytes;
	int total;
	int strength;
	int prepad;
	int postpad;
	unsigned int options;
	void *priv;
	void (*hwctl)(struct mtd_info *mtd, int mode);
	int (*calculate)(struct mtd_info *mtd, const uint8_t *dat,
			uint8_t *ecc_code);
	int (*correct)(struct mtd_info *mtd, uint8_t *dat, uint8_t *read_ecc,
			uint8_t *calc_ecc);
	int (*read_page_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			uint8_t *buf, int oob_required, int page);
	int (*write_page_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			const uint8_t *buf, int oob_required, int page);
	int (*read_page)(struct mtd_info *mtd, struct nand_chip *chip,
			uint8_t *buf, int oob_required, int page);
	int (*read_subpage)(struct mtd_info *mtd, struct nand_chip *chip,
			uint32_t offs, uint32_t len, uint8_t *buf, int page);
	int (*write_subpage)(struct mtd_info *mtd, struct nand_chip *chip,
			uint32_t offset, uint32_t data_len,
			const uint8_t *data_buf, int oob_required, int page);
	int (*write_page)(struct mtd_info *mtd, struct nand_chip *chip,
			const uint8_t *buf, int oob_required, int page);
	int (*write_oob_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			int page);
	int (*read_oob_raw)(struct mtd_info *mtd, struct nand_chip *chip,
			int page);
	int (*read_oob)(struct mtd_info *mtd, struct nand_chip *chip, int page);
	int (*write_oob)(struct mtd_info *mtd, struct nand_chip *chip,
			int page);
};


struct nand_buffers {
	uint8_t	*ecccalc;
	uint8_t	*ecccode;
	uint8_t *databuf;
};


struct nand_sdr_timings {
	u32 tALH_min;
	u32 tADL_min;
	u32 tALS_min;
	u32 tAR_min;
	u32 tCEA_max;
	u32 tCEH_min;
	u32 tCH_min;
	u32 tCHZ_max;
	u32 tCLH_min;
	u32 tCLR_min;
	u32 tCLS_min;
	u32 tCOH_min;
	u32 tCS_min;
	u32 tDH_min;
	u32 tDS_min;
	u32 tFEAT_max;
	u32 tIR_min;
	u32 tITC_max;
	u32 tRC_min;
	u32 tREA_max;
	u32 tREH_min;
	u32 tRHOH_min;
	u32 tRHW_min;
	u32 tRHZ_max;
	u32 tRLOH_min;
	u32 tRP_min;
	u32 tRR_min;
	u64 tRST_max;
	u32 tWB_max;
	u32 tWC_min;
	u32 tWH_min;
	u32 tWHR_min;
	u32 tWP_min;
	u32 tWW_min;
};


enum nand_data_interface_type {
	NAND_SDR_IFACE,
};


struct nand_data_interface {
	enum nand_data_interface_type type;
	union {
		struct nand_sdr_timings sdr;
	} timings;
};


static inline const struct nand_sdr_timings *
nand_get_sdr_timings(const struct nand_data_interface *conf)
{
	if (conf->type != NAND_SDR_IFACE)
		return ERR_PTR(-EINVAL);

	return &conf->timings.sdr;
}



struct nand_chip {
	struct mtd_info mtd;
	void __iomem *IO_ADDR_R;
	void __iomem *IO_ADDR_W;

	uint8_t (*read_byte)(struct mtd_info *mtd);
	u16 (*read_word)(struct mtd_info *mtd);
	void (*write_byte)(struct mtd_info *mtd, uint8_t byte);
	void (*write_buf)(struct mtd_info *mtd, const uint8_t *buf, int len);
	void (*read_buf)(struct mtd_info *mtd, uint8_t *buf, int len);
	void (*select_chip)(struct mtd_info *mtd, int chip);
	int (*block_bad)(struct mtd_info *mtd, loff_t ofs);
	int (*block_markbad)(struct mtd_info *mtd, loff_t ofs);
	void (*cmd_ctrl)(struct mtd_info *mtd, int dat, unsigned int ctrl);
	int (*dev_ready)(struct mtd_info *mtd);
	void (*cmdfunc)(struct mtd_info *mtd, unsigned command, int column,
			int page_addr);
	int(*waitfunc)(struct mtd_info *mtd, struct nand_chip *this);
	int (*erase)(struct mtd_info *mtd, int page);
	int (*scan_bbt)(struct mtd_info *mtd);
	int (*errstat)(struct mtd_info *mtd, struct nand_chip *this, int state,
			int status, int page);
	int (*write_page)(struct mtd_info *mtd, struct nand_chip *chip,
			uint32_t offset, int data_len, const uint8_t *buf,
			int oob_required, int page, int cached, int raw);
	int (*onfi_set_features)(struct mtd_info *mtd, struct nand_chip *chip,
			int feature_addr, uint8_t *subfeature_para);
	int (*onfi_get_features)(struct mtd_info *mtd, struct nand_chip *chip,
			int feature_addr, uint8_t *subfeature_para);
	int (*setup_read_retry)(struct mtd_info *mtd, int retry_mode);
	int (*setup_data_interface)(struct mtd_info *mtd,
				    const struct nand_data_interface *conf,
				    bool check_only);


	int chip_delay;
	unsigned int options;
	unsigned int bbt_options;

	int page_shift;
	int phys_erase_shift;
	int bbt_erase_shift;
	int chip_shift;
	int numchips;
	uint64_t chipsize;
	int pagemask;
	int pagebuf;
	unsigned int pagebuf_bitflips;
	int subpagesize;
	uint8_t bits_per_cell;
	uint16_t ecc_strength_ds;
	uint16_t ecc_step_ds;
	int onfi_timing_mode_default;
	int badblockpos;
	int badblockbits;

	int onfi_version;
	int jedec_version;
	union {
		struct nand_onfi_params	onfi_params;
		struct nand_jedec_params jedec_params;
	};

	struct nand_data_interface *data_interface;

	int read_retries;

	flstate_t state;

	uint8_t *oob_poi;
	struct nand_hw_control *controller;

	struct nand_ecc_ctrl ecc;
	struct nand_buffers *buffers;
	struct nand_hw_control hwcontrol;

	uint8_t *bbt;
	struct nand_bbt_descr *bbt_td;
	struct nand_bbt_descr *bbt_md;

	struct nand_bbt_descr *badblock_pattern;

	void *priv;
};

extern const struct mtd_ooblayout_ops nand_ooblayout_sp_ops;
extern const struct mtd_ooblayout_ops nand_ooblayout_lp_ops;

static inline void nand_set_flash_node(struct nand_chip *chip,
				       struct device_node *np)
{
	mtd_set_of_node(&chip->mtd, np);
}

static inline struct device_node *nand_get_flash_node(struct nand_chip *chip)
{
	return mtd_get_of_node(&chip->mtd);
}

static inline struct nand_chip *mtd_to_nand(struct mtd_info *mtd)
{
	return container_of(mtd, struct nand_chip, mtd);
}

static inline struct mtd_info *nand_to_mtd(struct nand_chip *chip)
{
	return &chip->mtd;
}

static inline void *nand_get_controller_data(struct nand_chip *chip)
{
	return chip->priv;
}

static inline void nand_set_controller_data(struct nand_chip *chip, void *priv)
{
	chip->priv = priv;
}


#define NAND_MFR_TOSHIBA	0x98
#define NAND_MFR_ESMT		0xc8
#define NAND_MFR_SAMSUNG	0xec
#define NAND_MFR_FUJITSU	0x04
#define NAND_MFR_NATIONAL	0x8f
#define NAND_MFR_RENESAS	0x07
#define NAND_MFR_STMICRO	0x20
#define NAND_MFR_HYNIX		0xad
#define NAND_MFR_MICRON		0x2c
#define NAND_MFR_AMD		0x01
#define NAND_MFR_MACRONIX	0xc2
#define NAND_MFR_EON		0x92
#define NAND_MFR_SANDISK	0x45
#define NAND_MFR_INTEL		0x89
#define NAND_MFR_ATO		0x9b


#define NAND_MAX_ID_LEN 8


#define LEGACY_ID_NAND(nm, devid, chipsz, erasesz, opts)          \
	{ .name = (nm), {{ .dev_id = (devid) }}, .pagesize = 512, \
	  .chipsize = (chipsz), .erasesize = (erasesz), .options = (opts) }


#define EXTENDED_ID_NAND(nm, devid, chipsz, opts)                      \
	{ .name = (nm), {{ .dev_id = (devid) }}, .chipsize = (chipsz), \
	  .options = (opts) }

#define NAND_ECC_INFO(_strength, _step)	\
			{ .strength_ds = (_strength), .step_ds = (_step) }
#define NAND_ECC_STRENGTH(type)		((type)->ecc.strength_ds)
#define NAND_ECC_STEP(type)		((type)->ecc.step_ds)


struct nand_flash_dev {
	char *name;
	union {
		struct {
			uint8_t mfr_id;
			uint8_t dev_id;
		};
		uint8_t id[NAND_MAX_ID_LEN];
	};
	unsigned int pagesize;
	unsigned int chipsize;
	unsigned int erasesize;
	unsigned int options;
	uint16_t id_len;
	uint16_t oobsize;
	struct {
		uint16_t strength_ds;
		uint16_t step_ds;
	} ecc;
	int onfi_timing_mode_default;
};


struct nand_manufacturers {
	int id;
	char *name;
};

extern struct nand_flash_dev nand_flash_ids[];
extern struct nand_manufacturers nand_manuf_ids[];

int nand_default_bbt(struct mtd_info *mtd);
int nand_markbad_bbt(struct mtd_info *mtd, loff_t offs);
int nand_isreserved_bbt(struct mtd_info *mtd, loff_t offs);
int nand_isbad_bbt(struct mtd_info *mtd, loff_t offs, int allowbbt);
int nand_erase_nand(struct mtd_info *mtd, struct erase_info *instr,
		    int allowbbt);
int nand_do_read(struct mtd_info *mtd, loff_t from, size_t len,
		 size_t *retlen, uint8_t *buf);


struct platform_nand_chip {
	int nr_chips;
	int chip_offset;
	int nr_partitions;
	struct mtd_partition *partitions;
	int chip_delay;
	unsigned int options;
	unsigned int bbt_options;
	const char **part_probe_types;
};


struct platform_device;


struct platform_nand_ctrl {
	int (*probe)(struct platform_device *pdev);
	void (*remove)(struct platform_device *pdev);
	void (*hwcontrol)(struct mtd_info *mtd, int cmd);
	int (*dev_ready)(struct mtd_info *mtd);
	void (*select_chip)(struct mtd_info *mtd, int chip);
	void (*cmd_ctrl)(struct mtd_info *mtd, int dat, unsigned int ctrl);
	void (*write_buf)(struct mtd_info *mtd, const uint8_t *buf, int len);
	void (*read_buf)(struct mtd_info *mtd, uint8_t *buf, int len);
	unsigned char (*read_byte)(struct mtd_info *mtd);
	void *priv;
};


struct platform_nand_data {
	struct platform_nand_chip chip;
	struct platform_nand_ctrl ctrl;
};


static inline int onfi_feature(struct nand_chip *chip)
{
	return chip->onfi_version ? le16_to_cpu(chip->onfi_params.features) : 0;
}


static inline int onfi_get_async_timing_mode(struct nand_chip *chip)
{
	if (!chip->onfi_version)
		return ONFI_TIMING_MODE_UNKNOWN;
	return le16_to_cpu(chip->onfi_params.async_timing_mode);
}


static inline int onfi_get_sync_timing_mode(struct nand_chip *chip)
{
	if (!chip->onfi_version)
		return ONFI_TIMING_MODE_UNKNOWN;
	return le16_to_cpu(chip->onfi_params.src_sync_timing_mode);
}

int onfi_init_data_interface(struct nand_chip *chip,
			     struct nand_data_interface *iface,
			     enum nand_data_interface_type type,
			     int timing_mode);


static inline bool nand_is_slc(struct nand_chip *chip)
{
	return chip->bits_per_cell == 1;
}


static inline int nand_opcode_8bits(unsigned int command)
{
	switch (command) {
	case NAND_CMD_READID:
	case NAND_CMD_PARAM:
	case NAND_CMD_GET_FEATURES:
	case NAND_CMD_SET_FEATURES:
		return 1;
	default:
		break;
	}
	return 0;
}


static inline int jedec_feature(struct nand_chip *chip)
{
	return chip->jedec_version ? le16_to_cpu(chip->jedec_params.features)
		: 0;
}


const struct nand_sdr_timings *onfi_async_timing_mode_to_sdr_timings(int mode);

const struct nand_data_interface *nand_get_default_data_interface(void);

int nand_check_erased_ecc_chunk(void *data, int datalen,
				void *ecc, int ecclen,
				void *extraoob, int extraooblen,
				int threshold);


int nand_write_oob_std(struct mtd_info *mtd, struct nand_chip *chip, int page);


int nand_write_oob_syndrome(struct mtd_info *mtd, struct nand_chip *chip,
			    int page);


int nand_read_oob_std(struct mtd_info *mtd, struct nand_chip *chip, int page);


int nand_read_oob_syndrome(struct mtd_info *mtd, struct nand_chip *chip,
			   int page);


int nand_reset(struct nand_chip *chip, int chipnr);


void nand_cleanup(struct nand_chip *chip);

#endif 
