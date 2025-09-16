
#ifndef _LINUX_CLOCKSOURCE_H
#define _LINUX_CLOCKSOURCE_H

#include <linux/types.h>
#include <linux/timex.h>
#include <linux/time.h>
#include <linux/list.h>
#include <linux/cache.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/of.h>
#include <asm/div64.h>
#include <asm/io.h>

struct clocksource;
struct module;

#ifdef CONFIG_ARCH_CLOCKSOURCE_DATA
#include <asm/clocksource.h>
#endif


struct clocksource {
	cycle_t (*read)(struct clocksource *cs);
	cycle_t mask;
	u32 mult;
	u32 shift;
	u64 max_idle_ns;
	u32 maxadj;
#ifdef CONFIG_ARCH_CLOCKSOURCE_DATA
	struct arch_clocksource_data archdata;
#endif
	u64 max_cycles;
	const char *name;
	struct list_head list;
	int rating;
	int (*enable)(struct clocksource *cs);
	void (*disable)(struct clocksource *cs);
	unsigned long flags;
	void (*suspend)(struct clocksource *cs);
	void (*resume)(struct clocksource *cs);

	
#ifdef CONFIG_CLOCKSOURCE_WATCHDOG
	
	struct list_head wd_list;
	cycle_t cs_last;
	cycle_t wd_last;
#endif
	struct module *owner;
};


#define CLOCK_SOURCE_IS_CONTINUOUS		0x01
#define CLOCK_SOURCE_MUST_VERIFY		0x02

#define CLOCK_SOURCE_WATCHDOG			0x10
#define CLOCK_SOURCE_VALID_FOR_HRES		0x20
#define CLOCK_SOURCE_UNSTABLE			0x40
#define CLOCK_SOURCE_SUSPEND_NONSTOP		0x80
#define CLOCK_SOURCE_RESELECT			0x100


#define CLOCKSOURCE_MASK(bits) GENMASK_ULL((bits) - 1, 0)

static inline u32 clocksource_freq2mult(u32 freq, u32 shift_constant, u64 from)
{
	
	u64 tmp = ((u64)from) << shift_constant;

	tmp += freq/2; 
	do_div(tmp, freq);

	return (u32)tmp;
}


static inline u32 clocksource_khz2mult(u32 khz, u32 shift_constant)
{
	return clocksource_freq2mult(khz, shift_constant, NSEC_PER_MSEC);
}


static inline u32 clocksource_hz2mult(u32 hz, u32 shift_constant)
{
	return clocksource_freq2mult(hz, shift_constant, NSEC_PER_SEC);
}


static inline s64 clocksource_cyc2ns(cycle_t cycles, u32 mult, u32 shift)
{
	return ((u64) cycles * mult) >> shift;
}


extern int clocksource_unregister(struct clocksource*);
extern void clocksource_touch_watchdog(void);
extern void clocksource_change_rating(struct clocksource *cs, int rating);
extern void clocksource_suspend(void);
extern void clocksource_resume(void);
extern struct clocksource * __init clocksource_default_clock(void);
extern void clocksource_mark_unstable(struct clocksource *cs);

extern u64
clocks_calc_max_nsecs(u32 mult, u32 shift, u32 maxadj, u64 mask, u64 *max_cycles);
extern void
clocks_calc_mult_shift(u32 *mult, u32 *shift, u32 from, u32 to, u32 minsec);


extern int
__clocksource_register_scale(struct clocksource *cs, u32 scale, u32 freq);
extern void
__clocksource_update_freq_scale(struct clocksource *cs, u32 scale, u32 freq);


static inline int __clocksource_register(struct clocksource *cs)
{
	return __clocksource_register_scale(cs, 1, 0);
}

static inline int clocksource_register_hz(struct clocksource *cs, u32 hz)
{
	return __clocksource_register_scale(cs, 1, hz);
}

static inline int clocksource_register_khz(struct clocksource *cs, u32 khz)
{
	return __clocksource_register_scale(cs, 1000, khz);
}

static inline void __clocksource_update_freq_hz(struct clocksource *cs, u32 hz)
{
	__clocksource_update_freq_scale(cs, 1, hz);
}

static inline void __clocksource_update_freq_khz(struct clocksource *cs, u32 khz)
{
	__clocksource_update_freq_scale(cs, 1000, khz);
}


extern int timekeeping_notify(struct clocksource *clock);

extern cycle_t clocksource_mmio_readl_up(struct clocksource *);
extern cycle_t clocksource_mmio_readl_down(struct clocksource *);
extern cycle_t clocksource_mmio_readw_up(struct clocksource *);
extern cycle_t clocksource_mmio_readw_down(struct clocksource *);

extern int clocksource_mmio_init(void __iomem *, const char *,
	unsigned long, int, unsigned, cycle_t (*)(struct clocksource *));

extern int clocksource_i8253_init(void);

#define CLOCKSOURCE_OF_DECLARE(name, compat, fn) \
	OF_DECLARE_1_RET(clksrc, name, compat, fn)

#ifdef CONFIG_CLKSRC_PROBE
extern void clocksource_probe(void);
#else
static inline void clocksource_probe(void) {}
#endif

#define CLOCKSOURCE_ACPI_DECLARE(name, table_id, fn)		\
	ACPI_DECLARE_PROBE_ENTRY(clksrc, name, table_id, 0, NULL, 0, fn)

#endif 
