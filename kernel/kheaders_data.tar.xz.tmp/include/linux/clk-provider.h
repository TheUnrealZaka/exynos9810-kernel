
#ifndef __LINUX_CLK_PROVIDER_H
#define __LINUX_CLK_PROVIDER_H

#include <linux/io.h>
#include <linux/of.h>

#ifdef CONFIG_COMMON_CLK


#define CLK_SET_RATE_GATE	BIT(0) 
#define CLK_SET_PARENT_GATE	BIT(1) 
#define CLK_SET_RATE_PARENT	BIT(2) 
#define CLK_IGNORE_UNUSED	BIT(3) 
				
#define CLK_IS_BASIC		BIT(5) 
#define CLK_GET_RATE_NOCACHE	BIT(6) 
#define CLK_SET_RATE_NO_REPARENT BIT(7) 
#define CLK_GET_ACCURACY_NOCACHE BIT(8) 
#define CLK_RECALC_NEW_RATES	BIT(9) 
#define CLK_SET_RATE_UNGATE	BIT(10) 
#define CLK_IS_CRITICAL		BIT(11) 

#define CLK_OPS_PARENT_ENABLE	BIT(12)

struct clk;
struct clk_hw;
struct clk_core;
struct dentry;


struct clk_rate_request {
	unsigned long rate;
	unsigned long min_rate;
	unsigned long max_rate;
	unsigned long best_parent_rate;
	struct clk_hw *best_parent_hw;
};


struct clk_ops {
	int		(*prepare)(struct clk_hw *hw);
	void		(*unprepare)(struct clk_hw *hw);
	int		(*is_prepared)(struct clk_hw *hw);
	void		(*unprepare_unused)(struct clk_hw *hw);
	int		(*enable)(struct clk_hw *hw);
	void		(*disable)(struct clk_hw *hw);
	int		(*is_enabled)(struct clk_hw *hw);
	void		(*disable_unused)(struct clk_hw *hw);
	unsigned long	(*recalc_rate)(struct clk_hw *hw,
					unsigned long parent_rate);
	long		(*round_rate)(struct clk_hw *hw, unsigned long rate,
					unsigned long *parent_rate);
	int		(*determine_rate)(struct clk_hw *hw,
					  struct clk_rate_request *req);
	int		(*set_parent)(struct clk_hw *hw, u8 index);
	u8		(*get_parent)(struct clk_hw *hw);
	int		(*set_rate)(struct clk_hw *hw, unsigned long rate,
				    unsigned long parent_rate);
	int		(*set_rate_and_parent)(struct clk_hw *hw,
				    unsigned long rate,
				    unsigned long parent_rate, u8 index);
	unsigned long	(*recalc_accuracy)(struct clk_hw *hw,
					   unsigned long parent_accuracy);
	int		(*get_phase)(struct clk_hw *hw);
	int		(*set_phase)(struct clk_hw *hw, int degrees);
	void		(*init)(struct clk_hw *hw);
	int		(*debug_init)(struct clk_hw *hw, struct dentry *dentry);
};


struct clk_init_data {
	const char		*name;
	const struct clk_ops	*ops;
	const char		* const *parent_names;
	u8			num_parents;
	unsigned long		flags;
};


struct clk_hw {
	struct clk_core *core;
	struct clk *clk;
	const struct clk_init_data *init;
};




struct clk_fixed_rate {
	struct		clk_hw hw;
	unsigned long	fixed_rate;
	unsigned long	fixed_accuracy;
	u8		flags;
};

#define to_clk_fixed_rate(_hw) container_of(_hw, struct clk_fixed_rate, hw)

extern const struct clk_ops clk_fixed_rate_ops;
struct clk *clk_register_fixed_rate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		unsigned long fixed_rate);
struct clk_hw *clk_hw_register_fixed_rate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		unsigned long fixed_rate);
struct clk *clk_register_fixed_rate_with_accuracy(struct device *dev,
		const char *name, const char *parent_name, unsigned long flags,
		unsigned long fixed_rate, unsigned long fixed_accuracy);
void clk_unregister_fixed_rate(struct clk *clk);
struct clk_hw *clk_hw_register_fixed_rate_with_accuracy(struct device *dev,
		const char *name, const char *parent_name, unsigned long flags,
		unsigned long fixed_rate, unsigned long fixed_accuracy);
void clk_hw_unregister_fixed_rate(struct clk_hw *hw);

void of_fixed_clk_setup(struct device_node *np);


struct clk_gate {
	struct clk_hw hw;
	void __iomem	*reg;
	u8		bit_idx;
	u8		flags;
	spinlock_t	*lock;
};

#define to_clk_gate(_hw) container_of(_hw, struct clk_gate, hw)

#define CLK_GATE_SET_TO_DISABLE		BIT(0)
#define CLK_GATE_HIWORD_MASK		BIT(1)

extern const struct clk_ops clk_gate_ops;
struct clk *clk_register_gate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 bit_idx,
		u8 clk_gate_flags, spinlock_t *lock);
struct clk_hw *clk_hw_register_gate(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 bit_idx,
		u8 clk_gate_flags, spinlock_t *lock);
void clk_unregister_gate(struct clk *clk);
void clk_hw_unregister_gate(struct clk_hw *hw);

struct clk_div_table {
	unsigned int	val;
	unsigned int	div;
};


struct clk_divider {
	struct clk_hw	hw;
	void __iomem	*reg;
	u8		shift;
	u8		width;
	u8		flags;
	const struct clk_div_table	*table;
	spinlock_t	*lock;
};

#define to_clk_divider(_hw) container_of(_hw, struct clk_divider, hw)

#define CLK_DIVIDER_ONE_BASED		BIT(0)
#define CLK_DIVIDER_POWER_OF_TWO	BIT(1)
#define CLK_DIVIDER_ALLOW_ZERO		BIT(2)
#define CLK_DIVIDER_HIWORD_MASK		BIT(3)
#define CLK_DIVIDER_ROUND_CLOSEST	BIT(4)
#define CLK_DIVIDER_READ_ONLY		BIT(5)
#define CLK_DIVIDER_MAX_AT_ZERO		BIT(6)

extern const struct clk_ops clk_divider_ops;
extern const struct clk_ops clk_divider_ro_ops;

unsigned long divider_recalc_rate(struct clk_hw *hw, unsigned long parent_rate,
		unsigned int val, const struct clk_div_table *table,
		unsigned long flags);
long divider_round_rate(struct clk_hw *hw, unsigned long rate,
		unsigned long *prate, const struct clk_div_table *table,
		u8 width, unsigned long flags);
int divider_get_val(unsigned long rate, unsigned long parent_rate,
		const struct clk_div_table *table, u8 width,
		unsigned long flags);

struct clk *clk_register_divider(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 shift, u8 width,
		u8 clk_divider_flags, spinlock_t *lock);
struct clk_hw *clk_hw_register_divider(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 shift, u8 width,
		u8 clk_divider_flags, spinlock_t *lock);
struct clk *clk_register_divider_table(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 shift, u8 width,
		u8 clk_divider_flags, const struct clk_div_table *table,
		spinlock_t *lock);
struct clk_hw *clk_hw_register_divider_table(struct device *dev,
		const char *name, const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 shift, u8 width,
		u8 clk_divider_flags, const struct clk_div_table *table,
		spinlock_t *lock);
void clk_unregister_divider(struct clk *clk);
void clk_hw_unregister_divider(struct clk_hw *hw);


struct clk_mux {
	struct clk_hw	hw;
	void __iomem	*reg;
	u32		*table;
	u32		mask;
	u8		shift;
	u8		flags;
	spinlock_t	*lock;
};

#define to_clk_mux(_hw) container_of(_hw, struct clk_mux, hw)

#define CLK_MUX_INDEX_ONE		BIT(0)
#define CLK_MUX_INDEX_BIT		BIT(1)
#define CLK_MUX_HIWORD_MASK		BIT(2)
#define CLK_MUX_READ_ONLY		BIT(3) 
#define CLK_MUX_ROUND_CLOSEST		BIT(4)

extern const struct clk_ops clk_mux_ops;
extern const struct clk_ops clk_mux_ro_ops;

struct clk *clk_register_mux(struct device *dev, const char *name,
		const char * const *parent_names, u8 num_parents,
		unsigned long flags,
		void __iomem *reg, u8 shift, u8 width,
		u8 clk_mux_flags, spinlock_t *lock);
struct clk_hw *clk_hw_register_mux(struct device *dev, const char *name,
		const char * const *parent_names, u8 num_parents,
		unsigned long flags,
		void __iomem *reg, u8 shift, u8 width,
		u8 clk_mux_flags, spinlock_t *lock);

struct clk *clk_register_mux_table(struct device *dev, const char *name,
		const char * const *parent_names, u8 num_parents,
		unsigned long flags,
		void __iomem *reg, u8 shift, u32 mask,
		u8 clk_mux_flags, u32 *table, spinlock_t *lock);
struct clk_hw *clk_hw_register_mux_table(struct device *dev, const char *name,
		const char * const *parent_names, u8 num_parents,
		unsigned long flags,
		void __iomem *reg, u8 shift, u32 mask,
		u8 clk_mux_flags, u32 *table, spinlock_t *lock);

void clk_unregister_mux(struct clk *clk);
void clk_hw_unregister_mux(struct clk_hw *hw);

void of_fixed_factor_clk_setup(struct device_node *node);



struct clk_fixed_factor {
	struct clk_hw	hw;
	unsigned int	mult;
	unsigned int	div;
};

#define to_clk_fixed_factor(_hw) container_of(_hw, struct clk_fixed_factor, hw)

extern const struct clk_ops clk_fixed_factor_ops;
struct clk *clk_register_fixed_factor(struct device *dev, const char *name,
		const char *parent_name, unsigned long flags,
		unsigned int mult, unsigned int div);
void clk_unregister_fixed_factor(struct clk *clk);
struct clk_hw *clk_hw_register_fixed_factor(struct device *dev,
		const char *name, const char *parent_name, unsigned long flags,
		unsigned int mult, unsigned int div);
void clk_hw_unregister_fixed_factor(struct clk_hw *hw);


struct clk_fractional_divider {
	struct clk_hw	hw;
	void __iomem	*reg;
	u8		mshift;
	u8		mwidth;
	u32		mmask;
	u8		nshift;
	u8		nwidth;
	u32		nmask;
	u8		flags;
	spinlock_t	*lock;
};

#define to_clk_fd(_hw) container_of(_hw, struct clk_fractional_divider, hw)

extern const struct clk_ops clk_fractional_divider_ops;
struct clk *clk_register_fractional_divider(struct device *dev,
		const char *name, const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 mshift, u8 mwidth, u8 nshift, u8 nwidth,
		u8 clk_divider_flags, spinlock_t *lock);
struct clk_hw *clk_hw_register_fractional_divider(struct device *dev,
		const char *name, const char *parent_name, unsigned long flags,
		void __iomem *reg, u8 mshift, u8 mwidth, u8 nshift, u8 nwidth,
		u8 clk_divider_flags, spinlock_t *lock);
void clk_hw_unregister_fractional_divider(struct clk_hw *hw);


struct clk_multiplier {
	struct clk_hw	hw;
	void __iomem	*reg;
	u8		shift;
	u8		width;
	u8		flags;
	spinlock_t	*lock;
};

#define to_clk_multiplier(_hw) container_of(_hw, struct clk_multiplier, hw)

#define CLK_MULTIPLIER_ZERO_BYPASS		BIT(0)
#define CLK_MULTIPLIER_ROUND_CLOSEST	BIT(1)

extern const struct clk_ops clk_multiplier_ops;


struct clk_composite {
	struct clk_hw	hw;
	struct clk_ops	ops;

	struct clk_hw	*mux_hw;
	struct clk_hw	*rate_hw;
	struct clk_hw	*gate_hw;

	const struct clk_ops	*mux_ops;
	const struct clk_ops	*rate_ops;
	const struct clk_ops	*gate_ops;
};

#define to_clk_composite(_hw) container_of(_hw, struct clk_composite, hw)

struct clk *clk_register_composite(struct device *dev, const char *name,
		const char * const *parent_names, int num_parents,
		struct clk_hw *mux_hw, const struct clk_ops *mux_ops,
		struct clk_hw *rate_hw, const struct clk_ops *rate_ops,
		struct clk_hw *gate_hw, const struct clk_ops *gate_ops,
		unsigned long flags);
void clk_unregister_composite(struct clk *clk);
struct clk_hw *clk_hw_register_composite(struct device *dev, const char *name,
		const char * const *parent_names, int num_parents,
		struct clk_hw *mux_hw, const struct clk_ops *mux_ops,
		struct clk_hw *rate_hw, const struct clk_ops *rate_ops,
		struct clk_hw *gate_hw, const struct clk_ops *gate_ops,
		unsigned long flags);
void clk_hw_unregister_composite(struct clk_hw *hw);



struct clk_gpio {
	struct clk_hw	hw;
	struct gpio_desc *gpiod;
};

#define to_clk_gpio(_hw) container_of(_hw, struct clk_gpio, hw)

extern const struct clk_ops clk_gpio_gate_ops;
struct clk *clk_register_gpio_gate(struct device *dev, const char *name,
		const char *parent_name, unsigned gpio, bool active_low,
		unsigned long flags);
struct clk_hw *clk_hw_register_gpio_gate(struct device *dev, const char *name,
		const char *parent_name, unsigned gpio, bool active_low,
		unsigned long flags);
void clk_hw_unregister_gpio_gate(struct clk_hw *hw);



extern const struct clk_ops clk_gpio_mux_ops;
struct clk *clk_register_gpio_mux(struct device *dev, const char *name,
		const char * const *parent_names, u8 num_parents, unsigned gpio,
		bool active_low, unsigned long flags);
struct clk_hw *clk_hw_register_gpio_mux(struct device *dev, const char *name,
		const char * const *parent_names, u8 num_parents, unsigned gpio,
		bool active_low, unsigned long flags);
void clk_hw_unregister_gpio_mux(struct clk_hw *hw);


struct clk *clk_register(struct device *dev, struct clk_hw *hw);
struct clk *devm_clk_register(struct device *dev, struct clk_hw *hw);

int __must_check clk_hw_register(struct device *dev, struct clk_hw *hw);
int __must_check devm_clk_hw_register(struct device *dev, struct clk_hw *hw);

void clk_unregister(struct clk *clk);
void devm_clk_unregister(struct device *dev, struct clk *clk);

void clk_hw_unregister(struct clk_hw *hw);
void devm_clk_hw_unregister(struct device *dev, struct clk_hw *hw);


const char *__clk_get_name(const struct clk *clk);
const char *clk_hw_get_name(const struct clk_hw *hw);
struct clk_hw *__clk_get_hw(struct clk *clk);
unsigned int clk_hw_get_num_parents(const struct clk_hw *hw);
struct clk_hw *clk_hw_get_parent(const struct clk_hw *hw);
struct clk_hw *clk_hw_get_parent_by_index(const struct clk_hw *hw,
					  unsigned int index);
unsigned int __clk_get_enable_count(struct clk *clk);
unsigned long clk_hw_get_rate(const struct clk_hw *hw);
unsigned long __clk_get_flags(struct clk *clk);
unsigned long clk_hw_get_flags(const struct clk_hw *hw);
bool clk_hw_is_prepared(const struct clk_hw *hw);
bool clk_hw_is_enabled(const struct clk_hw *hw);
bool __clk_is_enabled(struct clk *clk);
struct clk *__clk_lookup(const char *name);
int __clk_mux_determine_rate(struct clk_hw *hw,
			     struct clk_rate_request *req);
int __clk_determine_rate(struct clk_hw *core, struct clk_rate_request *req);
int __clk_mux_determine_rate_closest(struct clk_hw *hw,
				     struct clk_rate_request *req);
void clk_hw_reparent(struct clk_hw *hw, struct clk_hw *new_parent);
void clk_hw_set_rate_range(struct clk_hw *hw, unsigned long min_rate,
			   unsigned long max_rate);

static inline void __clk_hw_set_clk(struct clk_hw *dst, struct clk_hw *src)
{
	dst->clk = src->clk;
	dst->core = src->core;
}


unsigned long clk_hw_round_rate(struct clk_hw *hw, unsigned long rate);

struct of_device_id;

typedef void (*of_clk_init_cb_t)(struct device_node *);

struct clk_onecell_data {
	struct clk **clks;
	unsigned int clk_num;
};

struct clk_hw_onecell_data {
	unsigned int num;
	struct clk_hw *hws[];
};

extern struct of_device_id __clk_of_table;

#define CLK_OF_DECLARE(name, compat, fn) OF_DECLARE_1(clk, name, compat, fn)


#define CLK_OF_DECLARE_DRIVER(name, compat, fn) \
	static void __init name##_of_clk_init_driver(struct device_node *np) \
	{								\
		of_node_clear_flag(np, OF_POPULATED);			\
		fn(np);							\
	}								\
	OF_DECLARE_1(clk, name, compat, name##_of_clk_init_driver)

#ifdef CONFIG_OF
int of_clk_add_provider(struct device_node *np,
			struct clk *(*clk_src_get)(struct of_phandle_args *args,
						   void *data),
			void *data);
int of_clk_add_hw_provider(struct device_node *np,
			   struct clk_hw *(*get)(struct of_phandle_args *clkspec,
						 void *data),
			   void *data);
void of_clk_del_provider(struct device_node *np);
struct clk *of_clk_src_simple_get(struct of_phandle_args *clkspec,
				  void *data);
struct clk_hw *of_clk_hw_simple_get(struct of_phandle_args *clkspec,
				    void *data);
struct clk *of_clk_src_onecell_get(struct of_phandle_args *clkspec, void *data);
struct clk_hw *of_clk_hw_onecell_get(struct of_phandle_args *clkspec,
				     void *data);
unsigned int of_clk_get_parent_count(struct device_node *np);
int of_clk_parent_fill(struct device_node *np, const char **parents,
		       unsigned int size);
const char *of_clk_get_parent_name(struct device_node *np, int index);
int of_clk_detect_critical(struct device_node *np, int index,
			    unsigned long *flags);
void of_clk_init(const struct of_device_id *matches);

#else 

static inline int of_clk_add_provider(struct device_node *np,
			struct clk *(*clk_src_get)(struct of_phandle_args *args,
						   void *data),
			void *data)
{
	return 0;
}
static inline int of_clk_add_hw_provider(struct device_node *np,
			struct clk_hw *(*get)(struct of_phandle_args *clkspec,
					      void *data),
			void *data)
{
	return 0;
}
static inline void of_clk_del_provider(struct device_node *np) {}
static inline struct clk *of_clk_src_simple_get(
	struct of_phandle_args *clkspec, void *data)
{
	return ERR_PTR(-ENOENT);
}
static inline struct clk_hw *
of_clk_hw_simple_get(struct of_phandle_args *clkspec, void *data)
{
	return ERR_PTR(-ENOENT);
}
static inline struct clk *of_clk_src_onecell_get(
	struct of_phandle_args *clkspec, void *data)
{
	return ERR_PTR(-ENOENT);
}
static inline struct clk_hw *
of_clk_hw_onecell_get(struct of_phandle_args *clkspec, void *data)
{
	return ERR_PTR(-ENOENT);
}
static inline unsigned int of_clk_get_parent_count(struct device_node *np)
{
	return 0;
}
static inline int of_clk_parent_fill(struct device_node *np,
				     const char **parents, unsigned int size)
{
	return 0;
}
static inline const char *of_clk_get_parent_name(struct device_node *np,
						 int index)
{
	return NULL;
}
static inline int of_clk_detect_critical(struct device_node *np, int index,
					  unsigned long *flags)
{
	return 0;
}
static inline void of_clk_init(const struct of_device_id *matches) {}
#endif 



#if IS_ENABLED(CONFIG_PPC)

static inline u32 clk_readl(u32 __iomem *reg)
{
	return ioread32be(reg);
}

static inline void clk_writel(u32 val, u32 __iomem *reg)
{
	iowrite32be(val, reg);
}

#else	

static inline u32 clk_readl(u32 __iomem *reg)
{
	return readl(reg);
}

static inline void clk_writel(u32 val, u32 __iomem *reg)
{
	writel(val, reg);
}

#endif	

#ifdef CONFIG_DEBUG_FS
struct dentry *clk_debugfs_add_file(struct clk_hw *hw, char *name, umode_t mode,
				void *data, const struct file_operations *fops);
#endif

#endif 
#endif 
