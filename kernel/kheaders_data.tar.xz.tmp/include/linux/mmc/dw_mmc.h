

#ifndef LINUX_MMC_DW_MMC_H
#define LINUX_MMC_DW_MMC_H

#include <linux/scatterlist.h>
#include <linux/mmc/core.h>
#include <linux/dmaengine.h>
#include <linux/reset.h>
#include <linux/pm_qos.h>

#define MAX_MCI_SLOTS	2

enum dw_mci_state {
	STATE_IDLE = 0,
	STATE_SENDING_CMD,
	STATE_SENDING_DATA,
	STATE_DATA_BUSY,
	STATE_SENDING_STOP,
	STATE_DATA_ERROR,
	STATE_SENDING_CMD11,
	STATE_WAITING_CMD11_DONE,
};

enum {
	EVENT_CMD_COMPLETE = 0,
	EVENT_XFER_COMPLETE,
	EVENT_DATA_COMPLETE,
	EVENT_DATA_ERROR,
};

struct mmc_data;

enum {
	TRANS_MODE_PIO = 0,
	TRANS_MODE_IDMAC,
	TRANS_MODE_EDMAC
};

struct dw_mci_dma_slave {
	struct dma_chan *ch;
	enum dma_transfer_direction direction;
};


struct dw_mci {
	spinlock_t		lock;
	spinlock_t		irq_lock;
	void __iomem		*regs;
	void __iomem		*fifo_reg;

	struct scatterlist	*sg;
	struct sg_mapping_iter	sg_miter;

	struct dw_mci_slot	*cur_slot;
	struct mmc_request	*mrq;
	struct mmc_command	*cmd;
	struct mmc_data		*data;
	struct mmc_command	stop_abort;
	unsigned int		prev_blksz;
	unsigned char		timing;
	struct workqueue_struct	*card_workqueue;

	
	int			use_dma;
	int			using_dma;
	int			dma_64bit_address;

	dma_addr_t		sg_dma;
	void			*sg_cpu;
	const struct dw_mci_dma_ops	*dma_ops;
	
	unsigned int		ring_size;

	
	struct dw_mci_dma_slave *dms;
	
	resource_size_t		phy_regs;

	unsigned int            desc_sz;
	struct pm_qos_request   pm_qos_lock;
	struct delayed_work	qos_work;
	bool			qos_cntrl;
	u32			cmd_status;
	u32			data_status;
	u32			stop_cmdr;
	u32			dir_status;
	struct tasklet_struct	tasklet;
	u32			tasklet_state;
	struct work_struct	card_work;
	u32			card_detect_cnt;
	unsigned long		pending_events;
	unsigned long		completed_events;
	enum dw_mci_state	state;
	struct list_head	queue;

	u32			bus_hz;
	u32			current_speed;
	u32			num_slots;
	u32			fifoth_val;
	u32			cd_rd_thr;
	u16			verid;
	u16			data_offset;
	struct device		*dev;
	struct dw_mci_board	*pdata;
	const struct dw_mci_drv_data	*drv_data;
	void			*priv;
	struct clk		*biu_clk;
	struct clk		*ciu_clk;
	struct clk		*ciu_gate;
	atomic_t		biu_clk_cnt;
	atomic_t		ciu_clk_cnt;
	atomic_t		biu_en_win;
	atomic_t		ciu_en_win;
	struct dw_mci_slot	*slot[MAX_MCI_SLOTS];

	
	int			fifo_depth;
	int			data_shift;
	u8			part_buf_start;
	u8			part_buf_count;
	union {
		u16		part_buf16;
		u32		part_buf32;
		u64		part_buf;
	};
	void (*push_data)(struct dw_mci *host, void *buf, int cnt);
	void (*pull_data)(struct dw_mci *host, void *buf, int cnt);

	
	u32			quirks;

	
	struct timer_list       timer;
	bool			vqmmc_enabled;
	unsigned long		irq_flags; 
	int			irq;

	
#define DW_MMC_REQ_IDLE		0
#define DW_MMC_REQ_BUSY		1
	unsigned int		req_state;
	struct dw_mci_debug_info        *debug_info;    

	
	unsigned int qactive_check;

	
	int idle_ip_index;

	
	unsigned int transferred_cnt;

	
	struct dw_mci_sfr_ram_dump      *sfr_dump;

	
	bool sw_timeout_chk;

	
	u32			cclk_in;

	int			sdio_id0;

	struct timer_list       cmd11_timer;
	struct timer_list       dto_timer;
};


struct dw_mci_dma_ops {
	
	int (*init)(struct dw_mci *host);
	int (*start)(struct dw_mci *host, unsigned int sg_len);
	void (*complete)(void *host);
	void (*stop)(struct dw_mci *host);
	void (*reset)(struct dw_mci *host);
	void (*cleanup)(struct dw_mci *host);
	void (*exit)(struct dw_mci *host);
};



#define DW_MCI_QUIRK_HIGHSPEED                  BIT(0)

#define DW_MCI_QUIRK_BROKEN_CARD_DETECTION      BIT(1)

#define DW_MCI_QUIRK_NO_WRITE_PROTECT           BIT(2)

#define DW_MCI_QUIRK_NO_DETECT_EBIT             BIT(3)

#define DW_MMC_QUIRK_FIXED_VOLTAGE              BIT(4)

#define DW_MCI_QUIRK_HWACG_CTRL                 BIT(5)

#define DW_MCI_QUIRK_ENABLE_ULP                 BIT(6)

#define DW_MCI_QUIRK_USE_SMU                    BIT(7)

#define DW_MCI_QUIRK_USE_SSC			BIT(8)

#define DW_MCI_QUIRK_BROKEN_DTO			BIT(9)



#define DW_MCI_SLOT_QUIRK_NO_WRITE_PROTECT	BIT(0)
enum dw_mci_cd_types {
	DW_MCI_CD_INTERNAL = 1, 
	DW_MCI_CD_EXTERNAL,     
	DW_MCI_CD_GPIO,         
	DW_MCI_CD_NONE,         
	DW_MCI_CD_PERMANENT,    
};
struct dma_pdata;


struct dw_mci_board {
	u32 num_slots;

	u32 quirks; 
	unsigned int bus_hz; 

	u32 caps;	
	u32 caps2;	
	u32 pm_caps;	
	
	unsigned int fifo_depth;

	
	u32 detect_delay_ms;
	u8 clk_smpl;
	bool is_fine_tuned;
	bool tuned;
	bool extra_tuning;
	bool only_once_tune;

	
	unsigned int qos_dvfs_level;
	unsigned char io_mode;

	
	unsigned int ssc_rate;

	enum dw_mci_cd_types cd_type;
	struct reset_control *rstc;
	struct dw_mci_dma_ops *dma_ops;
	struct dma_pdata *data;
	struct block_settings *blk_settings;
	unsigned int sw_timeout;

	
	u32 data_timeout;
	u32 hto_timeout;
	bool use_gate_clock;
	bool use_biu_gate_clock;
	bool use_gpio_invert;
	bool enable_cclk_on_suspend;
	bool on_suspend;

	
	unsigned int desc_sz;
};

#ifdef CONFIG_MMC_DW_IDMAC
#define IDMAC_INT_CLR		(SDMMC_IDMAC_INT_AI | SDMMC_IDMAC_INT_NI | \
				 SDMMC_IDMAC_INT_CES | SDMMC_IDMAC_INT_DU | \
				 SDMMC_IDMAC_INT_FBE | SDMMC_IDMAC_INT_RI | \
				 SDMMC_IDMAC_INT_TI)

#if defined(CONFIG_MMC_DW_EXYNOS_FMP)
struct idmac_desc_64addr {
	u32		des0;	
#define IDMAC_DES0_DIC	BIT(1)
#define IDMAC_DES0_LD	BIT(2)
#define IDMAC_DES0_FD	BIT(3)
#define IDMAC_DES0_CH	BIT(4)
#define IDMAC_DES0_ER	BIT(5)
#define IDMAC_DES0_CES	BIT(30)
#define IDMAC_DES0_OWN	BIT(31)
	u32		des1;	
#define IDMAC_64ADDR_SET_BUFFER1_SIZE(d, s) \
	((d)->des2 = ((d)->des2 & cpu_to_le32(0x03ffe000)) | \
	 ((cpu_to_le32(s)) & cpu_to_le32(0x1fff)))
	u32		des2;	
	u32		des3;	
	u32		des4;	
	u32		des5;	
	u32		des6;	
	u32		des7;	
	u32		des8;	
	u32		des9;	
	u32		des10;	
	u32		des11;	
	u32		des12;	
	u32		des13;	
	u32		des14;	
	u32		des15;	
	u32		des16;	
	u32		des17;	
	u32		des18;	
	u32		des19;	
	u32		des20;	
	u32		des21;	
	u32		des22;	
	u32		des23;	
	u32		des24;	
	u32		des25;	
	u32		des26;	
	u32		des27;	
	u32		des28;	
	u32		des29;	
	u32		des30;	
	u32		des31;	
#if defined(CONFIG_EXYNOS_FMP_DUAL_FILE_ENCRYPTION)
	u32		des32;	
	u32		des33;	
	u32		des34;	
	u32		des35;	
	u32		des36;	
	u32		des37;	
	u32		des38;	
	u32		des39;	
	u32		des40;	
	u32		des41;	
	u32		des42;	
	u32		des43;	
	u32		des44;	
	u32		des45;	
	u32		des46;	
	u32		des47;	
	u32		des48;	
	u32		des49;	
	u32		des50;	
	u32		des51;	
	u32		des52;	
	u32		des53;	
	u32		des54;	
	u32		des55;	
	u32		des56;	
	u32		des57;	
	u32		des58;	
	u32		des59;	
	u32		des60;	
	u32		des61;	
	u32		des62;	
	u32		des63;	
#endif 
#define IDMAC_64ADDR_SET_DESC_CLEAR(d) \
do {			\
	(d)->des1 = 0;	\
	(d)->des2 = 0;	\
	(d)->des3 = 0;	\
} while(0)
#define IDMAC_64ADDR_SET_DESC_ADDR(d, a) \
do {			\
	(d)->des6 = ((u32)(a) & 0xffffffff); \
	(d)->des7 = ((u32)((a) >> 32));	\
} while(0)
};
#else
struct idmac_desc_64addr {
	u32		des0;	
#define IDMAC_OWN_CLR64(x) \
	!((x) & cpu_to_le32(IDMAC_DES0_OWN))

	u32		des1;	

	u32		des2;	
#define IDMAC_64ADDR_SET_BUFFER1_SIZE(d, s) \
	((d)->des2 = ((d)->des2 & 0x03ffe000) | ((s) & 0x1fff))

	u32		des3;	

	u32		des4;	
	u32		des5;	

	u32		des6;	
	u32		des7;	
#define IDMAC_64ADDR_SET_DESC_CLEAR(d) \
do {			\
	(d)->des1 = 0;	\
	(d)->des2 = 0;	\
	(d)->des3 = 0;	\
} while(0)
#define IDMAC_64ADDR_SET_DESC_ADDR(d, a) \
do {			\
	(d)->des6 = ((u32)(a) & 0xffffffff); \
	(d)->des7 = ((u32)((a) >> 32));	\
} while(0)
};
#endif

struct idmac_desc {
	u32		des0;	
#define IDMAC_DES0_DIC	BIT(1)
#define IDMAC_DES0_LD	BIT(2)
#define IDMAC_DES0_FD	BIT(3)
#define IDMAC_DES0_CH	BIT(4)
#define IDMAC_DES0_ER	BIT(5)
#define IDMAC_DES0_CES	BIT(30)
#define IDMAC_DES0_OWN	BIT(31)

	u32		des1;	
#define IDMAC_SET_BUFFER1_SIZE(d, s) \
	((d)->des1 = ((d)->des1 & 0x03ffe000) | ((s) & 0x1fff))

	u32		des2;	

	u32		des3;	
#define IDMAC_SET_DESC_ADDR(d, a) \
do {	\
	(d)->des3 = (u32)(a);	\
} while(0)
};
#endif 


#define CLEAR		0
#define AES_CBC		1
#define AES_XTS		2

#endif 
