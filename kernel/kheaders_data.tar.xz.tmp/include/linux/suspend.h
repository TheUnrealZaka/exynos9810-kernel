#ifndef _LINUX_SUSPEND_H
#define _LINUX_SUSPEND_H

#include <linux/swap.h>
#include <linux/notifier.h>
#include <linux/init.h>
#include <linux/pm.h>
#include <linux/mm.h>
#include <linux/freezer.h>
#include <asm/errno.h>

#ifdef CONFIG_VT
extern void pm_set_vt_switch(int);
#else
static inline void pm_set_vt_switch(int do_switch)
{
}
#endif

#ifdef CONFIG_VT_CONSOLE_SLEEP
extern void pm_prepare_console(void);
extern void pm_restore_console(void);
#else
static inline void pm_prepare_console(void)
{
}

static inline void pm_restore_console(void)
{
}
#endif

typedef int __bitwise suspend_state_t;

#define PM_SUSPEND_ON		((__force suspend_state_t) 0)
#define PM_SUSPEND_FREEZE	((__force suspend_state_t) 1)
#define PM_SUSPEND_STANDBY	((__force suspend_state_t) 2)
#define PM_SUSPEND_MEM		((__force suspend_state_t) 3)
#define PM_SUSPEND_MIN		PM_SUSPEND_FREEZE
#define PM_SUSPEND_MAX		((__force suspend_state_t) 4)

enum suspend_stat_step {
	SUSPEND_FREEZE = 1,
	SUSPEND_PREPARE,
	SUSPEND_SUSPEND,
	SUSPEND_SUSPEND_LATE,
	SUSPEND_SUSPEND_NOIRQ,
	SUSPEND_RESUME_NOIRQ,
	SUSPEND_RESUME_EARLY,
	SUSPEND_RESUME
};

struct suspend_stats {
	int	success;
	int	fail;
	int	failed_freeze;
	int	failed_prepare;
	int	failed_suspend;
	int	failed_suspend_late;
	int	failed_suspend_noirq;
	int	failed_resume;
	int	failed_resume_early;
	int	failed_resume_noirq;
#define	REC_FAILED_NUM	2
	int	last_failed_dev;
	char	failed_devs[REC_FAILED_NUM][40];
	int	last_failed_errno;
	int	errno[REC_FAILED_NUM];
	int	last_failed_step;
	enum suspend_stat_step	failed_steps[REC_FAILED_NUM];
};

extern struct suspend_stats suspend_stats;

static inline void dpm_save_failed_dev(const char *name)
{
	strlcpy(suspend_stats.failed_devs[suspend_stats.last_failed_dev],
		name,
		sizeof(suspend_stats.failed_devs[0]));
	suspend_stats.last_failed_dev++;
	suspend_stats.last_failed_dev %= REC_FAILED_NUM;
}

static inline void dpm_save_failed_errno(int err)
{
	suspend_stats.errno[suspend_stats.last_failed_errno] = err;
	suspend_stats.last_failed_errno++;
	suspend_stats.last_failed_errno %= REC_FAILED_NUM;
}

static inline void dpm_save_failed_step(enum suspend_stat_step step)
{
	suspend_stats.failed_steps[suspend_stats.last_failed_step] = step;
	suspend_stats.last_failed_step++;
	suspend_stats.last_failed_step %= REC_FAILED_NUM;
}


struct platform_suspend_ops {
	int (*valid)(suspend_state_t state);
	int (*begin)(suspend_state_t state);
	int (*prepare)(void);
	int (*prepare_late)(void);
	int (*enter)(suspend_state_t state);
	void (*wake)(void);
	void (*finish)(void);
	bool (*suspend_again)(void);
	void (*end)(void);
	void (*recover)(void);
};

struct platform_freeze_ops {
	int (*begin)(void);
	int (*prepare)(void);
	void (*restore)(void);
	void (*end)(void);
};

#ifdef CONFIG_SUSPEND

extern void suspend_set_ops(const struct platform_suspend_ops *ops);
extern int suspend_valid_only_mem(suspend_state_t state);

extern unsigned int pm_suspend_global_flags;

#define PM_SUSPEND_FLAG_FW_SUSPEND	(1 << 0)
#define PM_SUSPEND_FLAG_FW_RESUME	(1 << 1)

static inline void pm_suspend_clear_flags(void)
{
	pm_suspend_global_flags = 0;
}

static inline void pm_set_suspend_via_firmware(void)
{
	pm_suspend_global_flags |= PM_SUSPEND_FLAG_FW_SUSPEND;
}

static inline void pm_set_resume_via_firmware(void)
{
	pm_suspend_global_flags |= PM_SUSPEND_FLAG_FW_RESUME;
}

static inline bool pm_suspend_via_firmware(void)
{
	return !!(pm_suspend_global_flags & PM_SUSPEND_FLAG_FW_SUSPEND);
}

static inline bool pm_resume_via_firmware(void)
{
	return !!(pm_suspend_global_flags & PM_SUSPEND_FLAG_FW_RESUME);
}


enum freeze_state {
	FREEZE_STATE_NONE,      
	FREEZE_STATE_ENTER,     
	FREEZE_STATE_WAKE,      
};

extern enum freeze_state __read_mostly suspend_freeze_state;

static inline bool idle_should_freeze(void)
{
	return unlikely(suspend_freeze_state == FREEZE_STATE_ENTER);
}

extern void __init pm_states_init(void);
extern void freeze_set_ops(const struct platform_freeze_ops *ops);
extern void freeze_wake(void);


extern void arch_suspend_disable_irqs(void);


extern void arch_suspend_enable_irqs(void);

extern int pm_suspend(suspend_state_t state);
#else 
#define suspend_valid_only_mem	NULL

static inline void pm_suspend_clear_flags(void) {}
static inline void pm_set_suspend_via_firmware(void) {}
static inline void pm_set_resume_via_firmware(void) {}
static inline bool pm_suspend_via_firmware(void) { return false; }
static inline bool pm_resume_via_firmware(void) { return false; }

static inline void suspend_set_ops(const struct platform_suspend_ops *ops) {}
static inline int pm_suspend(suspend_state_t state) { return -ENOSYS; }
static inline bool idle_should_freeze(void) { return false; }
static inline void __init pm_states_init(void) {}
static inline void freeze_set_ops(const struct platform_freeze_ops *ops) {}
static inline void freeze_wake(void) {}
#endif 


struct pbe {
	void *address;		
	void *orig_address;	
	struct pbe *next;
};


extern void mark_free_pages(struct zone *zone);


struct platform_hibernation_ops {
	int (*begin)(void);
	void (*end)(void);
	int (*pre_snapshot)(void);
	void (*finish)(void);
	int (*prepare)(void);
	int (*enter)(void);
	void (*leave)(void);
	int (*pre_restore)(void);
	void (*restore_cleanup)(void);
	void (*recover)(void);
};

#ifdef CONFIG_HIBERNATION

extern void __register_nosave_region(unsigned long b, unsigned long e, int km);
static inline void __init register_nosave_region(unsigned long b, unsigned long e)
{
	__register_nosave_region(b, e, 0);
}
static inline void __init register_nosave_region_late(unsigned long b, unsigned long e)
{
	__register_nosave_region(b, e, 1);
}
extern int swsusp_page_is_forbidden(struct page *);
extern void swsusp_set_page_free(struct page *);
extern void swsusp_unset_page_free(struct page *);
extern unsigned long get_safe_page(gfp_t gfp_mask);
extern asmlinkage int swsusp_arch_suspend(void);
extern asmlinkage int swsusp_arch_resume(void);

extern void hibernation_set_ops(const struct platform_hibernation_ops *ops);
extern int hibernate(void);
extern bool system_entering_hibernation(void);
extern bool hibernation_available(void);
asmlinkage int swsusp_save(void);
extern struct pbe *restore_pblist;
#else 
static inline void register_nosave_region(unsigned long b, unsigned long e) {}
static inline void register_nosave_region_late(unsigned long b, unsigned long e) {}
static inline int swsusp_page_is_forbidden(struct page *p) { return 0; }
static inline void swsusp_set_page_free(struct page *p) {}
static inline void swsusp_unset_page_free(struct page *p) {}

static inline void hibernation_set_ops(const struct platform_hibernation_ops *ops) {}
static inline int hibernate(void) { return -ENOSYS; }
static inline bool system_entering_hibernation(void) { return false; }
static inline bool hibernation_available(void) { return false; }
#endif 


#define PM_HIBERNATION_PREPARE	0x0001 
#define PM_POST_HIBERNATION	0x0002 
#define PM_SUSPEND_PREPARE	0x0003 
#define PM_POST_SUSPEND		0x0004 
#define PM_RESTORE_PREPARE	0x0005 
#define PM_POST_RESTORE		0x0006 

extern struct mutex pm_mutex;

#ifdef CONFIG_PM_SLEEP
void save_processor_state(void);
void restore_processor_state(void);


extern int register_pm_notifier(struct notifier_block *nb);
extern int unregister_pm_notifier(struct notifier_block *nb);
extern bool is_pm_chain_notifier(struct notifier_block **notifier_block, unsigned long val);

#define pm_notifier(fn, pri) {				\
	static struct notifier_block fn##_nb =			\
		{ .notifier_call = fn, .priority = pri };	\
	register_pm_notifier(&fn##_nb);			\
}


extern bool events_check_enabled;
extern unsigned int pm_wakeup_irq;

extern bool pm_wakeup_pending(void);
extern void pm_system_wakeup(void);
extern void pm_wakeup_clear(void);
extern void pm_system_irq_wakeup(unsigned int irq_number);
extern bool pm_get_wakeup_count(unsigned int *count, bool block);
extern bool pm_save_wakeup_count(unsigned int count);
extern void pm_wakep_autosleep_enabled(bool set);
extern void pm_print_active_wakeup_sources(void);
extern void pm_get_active_wakeup_sources(char *pending_sources, size_t max);

static inline void lock_system_sleep(void)
{
	current->flags |= PF_FREEZER_SKIP;
	mutex_lock(&pm_mutex);
}

static inline void unlock_system_sleep(void)
{
	
	current->flags &= ~PF_FREEZER_SKIP;
	mutex_unlock(&pm_mutex);
}

#else 

static inline int register_pm_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int unregister_pm_notifier(struct notifier_block *nb)
{
	return 0;
}

#define pm_notifier(fn, pri)	do { (void)(fn); } while (0)

static inline bool pm_wakeup_pending(void) { return false; }
static inline void pm_system_wakeup(void) {}
static inline void pm_wakeup_clear(void) {}
static inline void pm_system_irq_wakeup(unsigned int irq_number) {}

static inline void lock_system_sleep(void) {}
static inline void unlock_system_sleep(void) {}

#endif 

#ifdef CONFIG_PM_SLEEP_DEBUG
extern bool pm_print_times_enabled;
#else
#define pm_print_times_enabled	(false)
#endif

#ifdef CONFIG_PM_AUTOSLEEP


void queue_up_suspend_work(void);

#else 

static inline void queue_up_suspend_work(void) {}

#endif 

#ifdef CONFIG_ARCH_SAVE_PAGE_KEYS

unsigned long page_key_additional_pages(unsigned long pages);
int page_key_alloc(unsigned long pages);
void page_key_free(void);
void page_key_read(unsigned long *pfn);
void page_key_memorize(unsigned long *pfn);
void page_key_write(void *address);

#else 

static inline unsigned long page_key_additional_pages(unsigned long pages)
{
	return 0;
}

static inline int  page_key_alloc(unsigned long pages)
{
	return 0;
}

static inline void page_key_free(void) {}
static inline void page_key_read(unsigned long *pfn) {}
static inline void page_key_memorize(unsigned long *pfn) {}
static inline void page_key_write(void *address) {}

#endif 

#endif 
