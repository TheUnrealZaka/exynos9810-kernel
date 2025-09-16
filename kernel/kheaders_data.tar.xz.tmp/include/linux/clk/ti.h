
#ifndef __LINUX_CLK_TI_H__
#define __LINUX_CLK_TI_H__

#include <linux/clk-provider.h>
#include <linux/clkdev.h>


struct dpll_data {
	void __iomem		*mult_div1_reg;
	u32			mult_mask;
	u32			div1_mask;
	struct clk_hw		*clk_bypass;
	struct clk_hw		*clk_ref;
	void __iomem		*control_reg;
	u32			enable_mask;
	unsigned long		last_rounded_rate;
	u16			last_rounded_m;
	u8			last_rounded_m4xen;
	u8			last_rounded_lpmode;
	u16			max_multiplier;
	u8			last_rounded_n;
	u8			min_divider;
	u16			max_divider;
	unsigned long		max_rate;
	u8			modes;
	void __iomem		*autoidle_reg;
	void __iomem		*idlest_reg;
	u32			autoidle_mask;
	u32			freqsel_mask;
	u32			idlest_mask;
	u32			dco_mask;
	u32			sddiv_mask;
	u32			dcc_mask;
	unsigned long		dcc_rate;
	u32			lpmode_mask;
	u32			m4xen_mask;
	u8			auto_recal_bit;
	u8			recal_en_bit;
	u8			recal_st_bit;
	u8			flags;
};

struct clk_hw_omap;


struct clk_hw_omap_ops {
	void	(*find_idlest)(struct clk_hw_omap *oclk,
			       void __iomem **idlest_reg,
			       u8 *idlest_bit, u8 *idlest_val);
	void	(*find_companion)(struct clk_hw_omap *oclk,
				  void __iomem **other_reg,
				  u8 *other_bit);
	void	(*allow_idle)(struct clk_hw_omap *oclk);
	void	(*deny_idle)(struct clk_hw_omap *oclk);
};


struct clk_hw_omap {
	struct clk_hw		hw;
	struct list_head	node;
	unsigned long		fixed_rate;
	u8			fixed_div;
	void __iomem		*enable_reg;
	u8			enable_bit;
	u8			flags;
	void __iomem		*clksel_reg;
	u32			clksel_mask;
	const struct clksel	*clksel;
	struct dpll_data	*dpll_data;
	const char		*clkdm_name;
	struct clockdomain	*clkdm;
	const struct clk_hw_omap_ops	*ops;
};


#define ENABLE_REG_32BIT	(1 << 0)	
#define CLOCK_IDLE_CONTROL	(1 << 1)
#define CLOCK_NO_IDLE_PARENT	(1 << 2)
#define ENABLE_ON_INIT		(1 << 3)	
#define INVERT_ENABLE		(1 << 4)	
#define CLOCK_CLKOUTX2		(1 << 5)
#define MEMMAP_ADDRESSING	(1 << 6)


#define DPLL_LOW_POWER_STOP	0x1
#define DPLL_LOW_POWER_BYPASS	0x5
#define DPLL_LOCKED		0x7


#define DPLL_J_TYPE		0x1


enum {
	TI_CLKM_CM = 0,
	TI_CLKM_CM2,
	TI_CLKM_PRM,
	TI_CLKM_SCRM,
	TI_CLKM_CTRL,
	TI_CLKM_PLLSS,
	CLK_MAX_MEMMAPS
};


struct clk_omap_reg {
	u16 offset;
	u16 index;
};


struct ti_clk_ll_ops {
	u32	(*clk_readl)(void __iomem *reg);
	void	(*clk_writel)(u32 val, void __iomem *reg);
	int	(*clkdm_clk_enable)(struct clockdomain *clkdm, struct clk *clk);
	int	(*clkdm_clk_disable)(struct clockdomain *clkdm,
				     struct clk *clk);
	int	(*cm_wait_module_ready)(u8 part, s16 prcm_mod, u16 idlest_reg,
					u8 idlest_shift);
	int	(*cm_split_idlest_reg)(void __iomem *idlest_reg, s16 *prcm_inst,
				       u8 *idlest_reg_id);
};

#define to_clk_hw_omap(_hw) container_of(_hw, struct clk_hw_omap, hw)

void omap2_init_clk_clkdm(struct clk_hw *clk);
int omap2_clk_disable_autoidle_all(void);
int omap2_clk_enable_autoidle_all(void);
int omap2_clk_allow_idle(struct clk *clk);
int omap2_clk_deny_idle(struct clk *clk);
unsigned long omap2_dpllcore_recalc(struct clk_hw *hw,
				    unsigned long parent_rate);
int omap2_reprogram_dpllcore(struct clk_hw *clk, unsigned long rate,
			     unsigned long parent_rate);
void omap2xxx_clkt_dpllcore_init(struct clk_hw *hw);
void omap2xxx_clkt_vps_init(void);
unsigned long omap2_get_dpll_rate(struct clk_hw_omap *clk);

void ti_dt_clk_init_retry_clks(void);
void ti_dt_clockdomains_setup(void);
int ti_clk_setup_ll_ops(struct ti_clk_ll_ops *ops);

struct regmap;

int omap2_clk_provider_init(struct device_node *parent, int index,
			    struct regmap *syscon, void __iomem *mem);
void omap2_clk_legacy_provider_init(int index, void __iomem *mem);

int omap3430_dt_clk_init(void);
int omap3630_dt_clk_init(void);
int am35xx_dt_clk_init(void);
int dm814x_dt_clk_init(void);
int dm816x_dt_clk_init(void);
int omap4xxx_dt_clk_init(void);
int omap5xxx_dt_clk_init(void);
int dra7xx_dt_clk_init(void);
int am33xx_dt_clk_init(void);
int am43xx_dt_clk_init(void);
int omap2420_dt_clk_init(void);
int omap2430_dt_clk_init(void);

struct ti_clk_features {
	u32 flags;
	long fint_min;
	long fint_max;
	long fint_band1_max;
	long fint_band2_min;
	u8 dpll_bypass_vals;
	u8 cm_idlest_val;
};

#define TI_CLK_DPLL_HAS_FREQSEL			BIT(0)
#define TI_CLK_DPLL4_DENY_REPROGRAM		BIT(1)
#define TI_CLK_DISABLE_CLKDM_CONTROL		BIT(2)
#define TI_CLK_ERRATA_I810			BIT(3)

void ti_clk_setup_features(struct ti_clk_features *features);
const struct ti_clk_features *ti_clk_get_features(void);

extern const struct clk_hw_omap_ops clkhwops_omap2xxx_dpll;

#ifdef CONFIG_ATAGS
int omap3430_clk_legacy_init(void);
int omap3430es1_clk_legacy_init(void);
int omap36xx_clk_legacy_init(void);
int am35xx_clk_legacy_init(void);
#else
static inline int omap3430_clk_legacy_init(void) { return -ENXIO; }
static inline int omap3430es1_clk_legacy_init(void) { return -ENXIO; }
static inline int omap36xx_clk_legacy_init(void) { return -ENXIO; }
static inline int am35xx_clk_legacy_init(void) { return -ENXIO; }
#endif


#endif
