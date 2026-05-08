

#ifndef __POWER_SMARTREFLEX_H
#define __POWER_SMARTREFLEX_H

#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/platform_data/voltage-omap.h>


#define SR_TYPE_V1	1
#define SR_TYPE_V2	2


#define SRCONFIG		0x00
#define SRSTATUS		0x04
#define SENVAL			0x08
#define SENMIN			0x0C
#define SENMAX			0x10
#define SENAVG			0x14
#define AVGWEIGHT		0x18
#define NVALUERECIPROCAL	0x1c
#define SENERROR_V1		0x20
#define ERRCONFIG_V1		0x24
#define IRQ_EOI			0x20
#define IRQSTATUS_RAW		0x24
#define IRQSTATUS		0x28
#define IRQENABLE_SET		0x2C
#define IRQENABLE_CLR		0x30
#define SENERROR_V2		0x34
#define ERRCONFIG_V2		0x38




#define SRCONFIG_ACCUMDATA_SHIFT	22
#define SRCONFIG_SRCLKLENGTH_SHIFT	12
#define SRCONFIG_SENNENABLE_V1_SHIFT	5
#define SRCONFIG_SENPENABLE_V1_SHIFT	3
#define SRCONFIG_SENNENABLE_V2_SHIFT	1
#define SRCONFIG_SENPENABLE_V2_SHIFT	0
#define SRCONFIG_CLKCTRL_SHIFT		0

#define SRCONFIG_ACCUMDATA_MASK		(0x3ff << 22)

#define SRCONFIG_SRENABLE		BIT(11)
#define SRCONFIG_SENENABLE		BIT(10)
#define SRCONFIG_ERRGEN_EN		BIT(9)
#define SRCONFIG_MINMAXAVG_EN		BIT(8)
#define SRCONFIG_DELAYCTRL		BIT(2)


#define AVGWEIGHT_SENPAVGWEIGHT_SHIFT	2
#define AVGWEIGHT_SENNAVGWEIGHT_SHIFT	0


#define NVALUERECIPROCAL_SENPGAIN_SHIFT	20
#define NVALUERECIPROCAL_SENNGAIN_SHIFT	16
#define NVALUERECIPROCAL_RNSENP_SHIFT	8
#define NVALUERECIPROCAL_RNSENN_SHIFT	0


#define ERRCONFIG_ERRWEIGHT_SHIFT	16
#define ERRCONFIG_ERRMAXLIMIT_SHIFT	8
#define ERRCONFIG_ERRMINLIMIT_SHIFT	0

#define SR_ERRWEIGHT_MASK		(0x07 << 16)
#define SR_ERRMAXLIMIT_MASK		(0xff << 8)
#define SR_ERRMINLIMIT_MASK		(0xff << 0)

#define ERRCONFIG_VPBOUNDINTEN_V1	BIT(31)
#define ERRCONFIG_VPBOUNDINTST_V1	BIT(30)
#define	ERRCONFIG_MCUACCUMINTEN		BIT(29)
#define ERRCONFIG_MCUACCUMINTST		BIT(28)
#define	ERRCONFIG_MCUVALIDINTEN		BIT(27)
#define ERRCONFIG_MCUVALIDINTST		BIT(26)
#define ERRCONFIG_MCUBOUNDINTEN		BIT(25)
#define	ERRCONFIG_MCUBOUNDINTST		BIT(24)
#define	ERRCONFIG_MCUDISACKINTEN	BIT(23)
#define ERRCONFIG_VPBOUNDINTST_V2	BIT(23)
#define ERRCONFIG_MCUDISACKINTST	BIT(22)
#define ERRCONFIG_VPBOUNDINTEN_V2	BIT(22)

#define ERRCONFIG_STATUS_V1_MASK	(ERRCONFIG_VPBOUNDINTST_V1 | \
					ERRCONFIG_MCUACCUMINTST | \
					ERRCONFIG_MCUVALIDINTST | \
					ERRCONFIG_MCUBOUNDINTST | \
					ERRCONFIG_MCUDISACKINTST)

#define IRQSTATUS_MCUACCUMINT		BIT(3)
#define IRQSTATUS_MCVALIDINT		BIT(2)
#define IRQSTATUS_MCBOUNDSINT		BIT(1)
#define IRQSTATUS_MCUDISABLEACKINT	BIT(0)


#define IRQENABLE_MCUACCUMINT		BIT(3)
#define IRQENABLE_MCUVALIDINT		BIT(2)
#define IRQENABLE_MCUBOUNDSINT		BIT(1)
#define IRQENABLE_MCUDISABLEACKINT	BIT(0)



#define SRCLKLENGTH_12MHZ_SYSCLK	0x3c
#define SRCLKLENGTH_13MHZ_SYSCLK	0x41
#define SRCLKLENGTH_19MHZ_SYSCLK	0x60
#define SRCLKLENGTH_26MHZ_SYSCLK	0x82
#define SRCLKLENGTH_38MHZ_SYSCLK	0xC0


#define OMAP3430_SR_ACCUMDATA		0x1f4

#define OMAP3430_SR1_SENPAVGWEIGHT	0x03
#define OMAP3430_SR1_SENNAVGWEIGHT	0x03

#define OMAP3430_SR2_SENPAVGWEIGHT	0x01
#define OMAP3430_SR2_SENNAVGWEIGHT	0x01

#define OMAP3430_SR_ERRWEIGHT		0x04
#define OMAP3430_SR_ERRMAXLIMIT		0x02

struct omap_sr {
	char				*name;
	struct list_head		node;
	struct platform_device		*pdev;
	struct omap_sr_nvalue_table	*nvalue_table;
	struct voltagedomain		*voltdm;
	struct dentry			*dbg_dir;
	unsigned int			irq;
	int				srid;
	int				ip_type;
	int				nvalue_count;
	bool				autocomp_active;
	u32				clk_length;
	u32				err_weight;
	u32				err_minlimit;
	u32				err_maxlimit;
	u32				accum_data;
	u32				senn_avgweight;
	u32				senp_avgweight;
	u32				senp_mod;
	u32				senn_mod;
	void __iomem			*base;
};


#define sr_test_cond_timeout(cond, timeout, index)		\
({								\
	for (index = 0; index < timeout; index++) {		\
		if (cond)					\
			break;					\
		udelay(1);					\
	}							\
})


struct omap_sr_pmic_data {
	void (*sr_pmic_init) (void);
};


struct omap_smartreflex_dev_attr {
	const char      *sensor_voltdm_name;
};

#ifdef CONFIG_POWER_AVS_OMAP

#define SR_CLASS1	0x1
#define SR_CLASS2	0x2
#define SR_CLASS3	0x3


struct omap_sr_class_data {
	int (*enable)(struct omap_sr *sr);
	int (*disable)(struct omap_sr *sr, int is_volt_reset);
	int (*configure)(struct omap_sr *sr);
	int (*notify)(struct omap_sr *sr, u32 status);
	u8 notify_flags;
	u8 class_type;
};


struct omap_sr_nvalue_table {
	u32 efuse_offs;
	u32 nvalue;
	u32 errminlimit;
	unsigned long volt_nominal;
};


struct omap_sr_data {
	const char			*name;
	int				ip_type;
	u32				senp_mod;
	u32				senn_mod;
	u32				err_weight;
	u32				err_maxlimit;
	u32				accum_data;
	u32				senn_avgweight;
	u32				senp_avgweight;
	int				nvalue_count;
	bool				enable_on_init;
	struct omap_sr_nvalue_table	*nvalue_table;
	struct voltagedomain		*voltdm;
};


void omap_sr_enable(struct voltagedomain *voltdm);
void omap_sr_disable(struct voltagedomain *voltdm);
void omap_sr_disable_reset_volt(struct voltagedomain *voltdm);


void omap_sr_register_pmic(struct omap_sr_pmic_data *pmic_data);


int sr_enable(struct omap_sr *sr, unsigned long volt);
void sr_disable(struct omap_sr *sr);
int sr_configure_errgen(struct omap_sr *sr);
int sr_disable_errgen(struct omap_sr *sr);
int sr_configure_minmax(struct omap_sr *sr);


int sr_register_class(struct omap_sr_class_data *class_data);
#else
static inline void omap_sr_enable(struct voltagedomain *voltdm) {}
static inline void omap_sr_disable(struct voltagedomain *voltdm) {}
static inline void omap_sr_disable_reset_volt(
		struct voltagedomain *voltdm) {}
static inline void omap_sr_register_pmic(
		struct omap_sr_pmic_data *pmic_data) {}
#endif
#endif
