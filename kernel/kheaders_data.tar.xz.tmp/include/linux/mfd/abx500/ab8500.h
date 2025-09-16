
#ifndef MFD_AB8500_H
#define MFD_AB8500_H

#include <linux/atomic.h>
#include <linux/mutex.h>
#include <linux/irqdomain.h>

struct device;


enum ab8500_version {
	AB8500_VERSION_AB8500 = 0x0,
	AB8500_VERSION_AB8505 = 0x1,
	AB8500_VERSION_AB9540 = 0x2,
	AB8500_VERSION_AB8540 = 0x4,
	AB8500_VERSION_UNDEFINED,
};


#define AB8500_CUTEARLY	0x00
#define AB8500_CUT1P0	0x10
#define AB8500_CUT1P1	0x11
#define AB8500_CUT1P2	0x12 
#define AB8500_CUT2P0	0x20
#define AB8500_CUT3P0	0x30
#define AB8500_CUT3P3	0x33


#define AB8500_M_FSM_RANK	0x0
#define AB8500_SYS_CTRL1_BLOCK	0x1
#define AB8500_SYS_CTRL2_BLOCK	0x2
#define AB8500_REGU_CTRL1	0x3
#define AB8500_REGU_CTRL2	0x4
#define AB8500_USB		0x5
#define AB8500_TVOUT		0x6
#define AB8500_DBI		0x7
#define AB8500_ECI_AV_ACC	0x8
#define AB8500_RESERVED		0x9
#define AB8500_GPADC		0xA
#define AB8500_CHARGER		0xB
#define AB8500_GAS_GAUGE	0xC
#define AB8500_AUDIO		0xD
#define AB8500_INTERRUPT	0xE
#define AB8500_RTC		0xF
#define AB8500_MISC		0x10
#define AB8500_DEVELOPMENT	0x11
#define AB8500_DEBUG		0x12
#define AB8500_PROD_TEST	0x13
#define AB8500_STE_TEST		0x14
#define AB8500_OTP_EMUL		0x15

#define AB8500_DEBUG_FIELD_LAST	0x16




#define AB8500_INT_MAIN_EXT_CH_NOT_OK	0 
#define AB8500_INT_UN_PLUG_TV_DET	1 
#define AB8500_INT_PLUG_TV_DET		2 
#define AB8500_INT_TEMP_WARM		3
#define AB8500_INT_PON_KEY2DB_F		4
#define AB8500_INT_PON_KEY2DB_R		5
#define AB8500_INT_PON_KEY1DB_F		6
#define AB8500_INT_PON_KEY1DB_R		7

#define AB8500_INT_BATT_OVV		8
#define AB8500_INT_MAIN_CH_UNPLUG_DET	10 
#define AB8500_INT_MAIN_CH_PLUG_DET	11 
#define AB8500_INT_VBUS_DET_F		14
#define AB8500_INT_VBUS_DET_R		15

#define AB8500_INT_VBUS_CH_DROP_END	16
#define AB8500_INT_RTC_60S		17
#define AB8500_INT_RTC_ALARM		18
#define AB8540_INT_BIF_INT		19
#define AB8500_INT_BAT_CTRL_INDB	20
#define AB8500_INT_CH_WD_EXP		21
#define AB8500_INT_VBUS_OVV		22
#define AB8500_INT_MAIN_CH_DROP_END	23 

#define AB8500_INT_CCN_CONV_ACC		24
#define AB8500_INT_INT_AUD		25
#define AB8500_INT_CCEOC		26
#define AB8500_INT_CC_INT_CALIB		27
#define AB8500_INT_LOW_BAT_F		28
#define AB8500_INT_LOW_BAT_R		29
#define AB8500_INT_BUP_CHG_NOT_OK	30
#define AB8500_INT_BUP_CHG_OK		31

#define AB8500_INT_GP_HW_ADC_CONV_END	32 
#define AB8500_INT_ACC_DETECT_1DB_F	33
#define AB8500_INT_ACC_DETECT_1DB_R	34
#define AB8500_INT_ACC_DETECT_22DB_F	35
#define AB8500_INT_ACC_DETECT_22DB_R	36
#define AB8500_INT_ACC_DETECT_21DB_F	37
#define AB8500_INT_ACC_DETECT_21DB_R	38
#define AB8500_INT_GP_SW_ADC_CONV_END	39

#define AB8500_INT_GPIO6R		40 
#define AB8500_INT_GPIO7R		41 
#define AB8500_INT_GPIO8R		42 
#define AB8500_INT_GPIO9R		43 
#define AB8500_INT_GPIO10R		44 
#define AB8500_INT_GPIO11R		45 
#define AB8500_INT_GPIO12R		46 
#define AB8500_INT_GPIO13R		47 

#define AB8500_INT_GPIO24R		48 
#define AB8500_INT_GPIO25R		49 
#define AB8500_INT_GPIO36R		50 
#define AB8500_INT_GPIO37R		51 
#define AB8500_INT_GPIO38R		52 
#define AB8500_INT_GPIO39R		53 
#define AB8500_INT_GPIO40R		54 
#define AB8500_INT_GPIO41R		55 

#define AB8500_INT_GPIO6F		56 
#define AB8500_INT_GPIO7F		57 
#define AB8500_INT_GPIO8F		58 
#define AB8500_INT_GPIO9F		59 
#define AB8500_INT_GPIO10F		60
#define AB8500_INT_GPIO11F		61
#define AB8500_INT_GPIO12F		62 
#define AB8500_INT_GPIO13F		63

#define AB8500_INT_GPIO24F		64 
#define AB8500_INT_GPIO25F		65 
#define AB8500_INT_GPIO36F		66 
#define AB8500_INT_GPIO37F		67 
#define AB8500_INT_GPIO38F		68 
#define AB8500_INT_GPIO39F		69 
#define AB8500_INT_GPIO40F		70 
#define AB8500_INT_GPIO41F		71 

#define AB8500_INT_ADP_SOURCE_ERROR	72
#define AB8500_INT_ADP_SINK_ERROR	73
#define AB8500_INT_ADP_PROBE_PLUG	74
#define AB8500_INT_ADP_PROBE_UNPLUG	75
#define AB8500_INT_ADP_SENSE_OFF	76
#define AB8500_INT_USB_PHY_POWER_ERR	78
#define AB8500_INT_USB_LINK_STATUS	79

#define AB8500_INT_BTEMP_LOW		80
#define AB8500_INT_BTEMP_LOW_MEDIUM	81
#define AB8500_INT_BTEMP_MEDIUM_HIGH	82
#define AB8500_INT_BTEMP_HIGH		83

#define AB8500_INT_SRP_DETECT		88
#define AB8500_INT_USB_CHARGER_NOT_OKR	89
#define AB8500_INT_ID_WAKEUP_R		90
#define AB8500_INT_ID_DET_PLUGR         91 
#define AB8500_INT_ID_DET_R1R		92
#define AB8500_INT_ID_DET_R2R		93
#define AB8500_INT_ID_DET_R3R		94
#define AB8500_INT_ID_DET_R4R		95

#define AB8500_INT_ID_WAKEUP_F		96 
#define AB8500_INT_ID_DET_PLUGF		97 
#define AB8500_INT_ID_DET_R1F		98 
#define AB8500_INT_ID_DET_R2F		99 
#define AB8500_INT_ID_DET_R3F		100 
#define AB8500_INT_ID_DET_R4F		101 
#define AB8500_INT_CHAUTORESTARTAFTSEC	102 
#define AB8500_INT_CHSTOPBYSEC		103

#define AB8500_INT_USB_CH_TH_PROT_F	104
#define AB8500_INT_USB_CH_TH_PROT_R	105
#define AB8500_INT_MAIN_CH_TH_PROT_F	106 
#define AB8500_INT_MAIN_CH_TH_PROT_R	107 
#define AB8500_INT_CHCURLIMNOHSCHIRP	109
#define AB8500_INT_CHCURLIMHSCHIRP	110
#define AB8500_INT_XTAL32K_KO		111



#define AB9540_INT_GPIO50R		113 
#define AB9540_INT_GPIO51R		114 
#define AB9540_INT_GPIO52R		115 
#define AB9540_INT_GPIO53R		116 
#define AB9540_INT_GPIO54R		117 
#define AB9540_INT_IEXT_CH_RF_BFN_R	118

#define AB9540_INT_GPIO50F		121 
#define AB9540_INT_GPIO51F		122 
#define AB9540_INT_GPIO52F		123 
#define AB9540_INT_GPIO53F		124 
#define AB9540_INT_GPIO54F		125 
#define AB9540_INT_IEXT_CH_RF_BFN_F	126

#define AB8505_INT_KEYSTUCK		128
#define AB8505_INT_IKR			129
#define AB8505_INT_IKP			130
#define AB8505_INT_KP			131
#define AB8505_INT_KEYDEGLITCH		132
#define AB8505_INT_MODPWRSTATUSF	134
#define AB8505_INT_MODPWRSTATUSR	135

#define AB8500_INT_HOOK_DET_NEG_F	138
#define AB8500_INT_HOOK_DET_NEG_R	139
#define AB8500_INT_HOOK_DET_POS_F	140
#define AB8500_INT_HOOK_DET_POS_R	141
#define AB8500_INT_PLUG_DET_COMP_F	142
#define AB8500_INT_PLUG_DET_COMP_R	143

#define AB8505_INT_COLL			144
#define AB8505_INT_RESERR		145
#define AB8505_INT_FRAERR		146
#define AB8505_INT_COMERR		147
#define AB8505_INT_SPDSET		148
#define AB8505_INT_DSENT		149
#define AB8505_INT_DREC			150
#define AB8505_INT_ACC_INT		151

#define AB8505_INT_NOPINT		152

#define AB8540_INT_IDPLUGDETCOMPF	160
#define AB8540_INT_IDPLUGDETCOMPR	161
#define AB8540_INT_FMDETCOMPLOF		162
#define AB8540_INT_FMDETCOMPLOR		163
#define AB8540_INT_FMDETCOMPHIF		164
#define AB8540_INT_FMDETCOMPHIR		165
#define AB8540_INT_ID5VDETCOMPF		166
#define AB8540_INT_ID5VDETCOMPR		167

#define AB8540_INT_GPIO43F		168
#define AB8540_INT_GPIO43R		169
#define AB8540_INT_GPIO44F		170
#define AB8540_INT_GPIO44R		171
#define AB8540_INT_KEYPOSDETCOMPF	172
#define AB8540_INT_KEYPOSDETCOMPR	173
#define AB8540_INT_KEYNEGDETCOMPF	174
#define AB8540_INT_KEYNEGDETCOMPR	175

#define AB8540_INT_GPIO1VBATF		176
#define AB8540_INT_GPIO1VBATR		177
#define AB8540_INT_GPIO2VBATF		178
#define AB8540_INT_GPIO2VBATR		179
#define AB8540_INT_GPIO3VBATF		180
#define AB8540_INT_GPIO3VBATR		181
#define AB8540_INT_GPIO4VBATF		182
#define AB8540_INT_GPIO4VBATR		183

#define AB8540_INT_SYSCLKREQ2F		184
#define AB8540_INT_SYSCLKREQ2R		185
#define AB8540_INT_SYSCLKREQ3F		186
#define AB8540_INT_SYSCLKREQ3R		187
#define AB8540_INT_SYSCLKREQ4F		188
#define AB8540_INT_SYSCLKREQ4R		189
#define AB8540_INT_SYSCLKREQ5F		190
#define AB8540_INT_SYSCLKREQ5R		191

#define AB8540_INT_PWMOUT1F		192
#define AB8540_INT_PWMOUT1R		193
#define AB8540_INT_PWMCTRL0F		194
#define AB8540_INT_PWMCTRL0R		195
#define AB8540_INT_PWMCTRL1F		196
#define AB8540_INT_PWMCTRL1R		197
#define AB8540_INT_SYSCLKREQ6F		198
#define AB8540_INT_SYSCLKREQ6R		199

#define AB8540_INT_PWMEXTVIBRA1F	200
#define AB8540_INT_PWMEXTVIBRA1R	201
#define AB8540_INT_PWMEXTVIBRA2F	202
#define AB8540_INT_PWMEXTVIBRA2R	203
#define AB8540_INT_PWMOUT2F		204
#define AB8540_INT_PWMOUT2R		205
#define AB8540_INT_PWMOUT3F		206
#define AB8540_INT_PWMOUT3R		207

#define AB8540_INT_ADDATA2F		208
#define AB8540_INT_ADDATA2R		209
#define AB8540_INT_DADATA2F		210
#define AB8540_INT_DADATA2R		211
#define AB8540_INT_FSYNC2F		212
#define AB8540_INT_FSYNC2R		213
#define AB8540_INT_BITCLK2F		214
#define AB8540_INT_BITCLK2R		215

#define AB8540_INT_RTC_1S		216


#define AB8500_NR_IRQS			112
#define AB8505_NR_IRQS			153
#define AB9540_NR_IRQS			153
#define AB8540_NR_IRQS			216

#define AB8500_MAX_NR_IRQS		AB8540_NR_IRQS

#define AB8500_NUM_IRQ_REGS		14
#define AB9540_NUM_IRQ_REGS		20
#define AB8540_NUM_IRQ_REGS		27


#define AB8500_POR_ON_VBAT		0x01
#define AB8500_POW_KEY_1_ON		0x02
#define AB8500_POW_KEY_2_ON		0x04
#define AB8500_RTC_ALARM		0x08
#define AB8500_MAIN_CH_DET		0x10
#define AB8500_VBUS_DET			0x20
#define AB8500_USB_ID_DET		0x40


struct ab8500 {
	struct device	*dev;
	struct mutex	lock;
	struct mutex	irq_lock;
	atomic_t	transfer_ongoing;
	int		irq;
	struct irq_domain  *domain;
	enum ab8500_version version;
	u8		chip_id;

	int (*write)(struct ab8500 *ab8500, u16 addr, u8 data);
	int (*write_masked)(struct ab8500 *ab8500, u16 addr, u8 mask, u8 data);
	int (*read)(struct ab8500 *ab8500, u16 addr);

	unsigned long	tx_buf[4];
	unsigned long	rx_buf[4];

	u8 *mask;
	u8 *oldmask;
	int mask_size;
	const int *irq_reg_offset;
	int it_latchhier_num;
};

struct ab8500_regulator_platform_data;
struct ab8500_codec_platform_data;
struct ab8500_sysctrl_platform_data;


struct ab8500_platform_data {
	void (*init) (struct ab8500 *);
	struct ab8500_regulator_platform_data *regulator;
	struct ab8500_codec_platform_data *codec;
	struct ab8500_sysctrl_platform_data *sysctrl;
};

extern int ab8500_init(struct ab8500 *ab8500,
				 enum ab8500_version version);
extern int ab8500_exit(struct ab8500 *ab8500);

extern int ab8500_suspend(struct ab8500 *ab8500);

static inline int is_ab8500(struct ab8500 *ab)
{
	return ab->version == AB8500_VERSION_AB8500;
}

static inline int is_ab8505(struct ab8500 *ab)
{
	return ab->version == AB8500_VERSION_AB8505;
}

static inline int is_ab9540(struct ab8500 *ab)
{
	return ab->version == AB8500_VERSION_AB9540;
}

static inline int is_ab8540(struct ab8500 *ab)
{
	return ab->version == AB8500_VERSION_AB8540;
}


static inline int is_ab8500_1p0_or_earlier(struct ab8500 *ab)
{
	return (is_ab8500(ab) && (ab->chip_id <= AB8500_CUT1P0));
}


static inline int is_ab8500_1p1_or_earlier(struct ab8500 *ab)
{
	return (is_ab8500(ab) && (ab->chip_id <= AB8500_CUT1P1));
}


static inline int is_ab8500_2p0_or_earlier(struct ab8500 *ab)
{
	return (is_ab8500(ab) && (ab->chip_id <= AB8500_CUT2P0));
}

static inline int is_ab8500_3p3_or_earlier(struct ab8500 *ab)
{
	return (is_ab8500(ab) && (ab->chip_id <= AB8500_CUT3P3));
}


static inline int is_ab8500_2p0(struct ab8500 *ab)
{
	return (is_ab8500(ab) && (ab->chip_id == AB8500_CUT2P0));
}

static inline int is_ab8505_1p0_or_earlier(struct ab8500 *ab)
{
	return (is_ab8505(ab) && (ab->chip_id <= AB8500_CUT1P0));
}

static inline int is_ab8505_2p0(struct ab8500 *ab)
{
	return (is_ab8505(ab) && (ab->chip_id == AB8500_CUT2P0));
}

static inline int is_ab9540_1p0_or_earlier(struct ab8500 *ab)
{
	return (is_ab9540(ab) && (ab->chip_id <= AB8500_CUT1P0));
}

static inline int is_ab9540_2p0(struct ab8500 *ab)
{
	return (is_ab9540(ab) && (ab->chip_id == AB8500_CUT2P0));
}


static inline int is_ab9540_3p0(struct ab8500 *ab)
{
	return (is_ab9540(ab) && (ab->chip_id == AB8500_CUT3P0));
}

static inline int is_ab8540_1p0_or_earlier(struct ab8500 *ab)
{
	return is_ab8540(ab) && (ab->chip_id <= AB8500_CUT1P0);
}

static inline int is_ab8540_1p1_or_earlier(struct ab8500 *ab)
{
	return is_ab8540(ab) && (ab->chip_id <= AB8500_CUT1P1);
}

static inline int is_ab8540_1p2_or_earlier(struct ab8500 *ab)
{
	return is_ab8540(ab) && (ab->chip_id <= AB8500_CUT1P2);
}

static inline int is_ab8540_2p0_or_earlier(struct ab8500 *ab)
{
	return is_ab8540(ab) && (ab->chip_id <= AB8500_CUT2P0);
}

static inline int is_ab8540_2p0(struct ab8500 *ab)
{
	return is_ab8540(ab) && (ab->chip_id == AB8500_CUT2P0);
}

static inline int is_ab8505_2p0_earlier(struct ab8500 *ab)
{
	return (is_ab8505(ab) && (ab->chip_id < AB8500_CUT2P0));
}

static inline int is_ab9540_2p0_or_earlier(struct ab8500 *ab)
{
	return (is_ab9540(ab) && (ab->chip_id < AB8500_CUT2P0));
}

void ab8500_override_turn_on_stat(u8 mask, u8 set);

#ifdef CONFIG_AB8500_DEBUG
extern int prcmu_abb_read(u8 slave, u8 reg, u8 *value, u8 size);
void ab8500_dump_all_banks(struct device *dev);
void ab8500_debug_register_interrupt(int line);
#else
static inline void ab8500_dump_all_banks(struct device *dev) {}
static inline void ab8500_debug_register_interrupt(int line) {}
#endif

#endif 
