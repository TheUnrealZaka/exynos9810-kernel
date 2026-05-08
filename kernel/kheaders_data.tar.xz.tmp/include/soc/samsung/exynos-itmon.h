

#ifndef EXYNOS_ITMON__H
#define EXYNOS_ITMON__H

#ifdef CONFIG_EXYNOS_ITMON
struct itmon_notifier {
	char *port;			
	char *master;			
	char *dest;			
	bool read;			
	unsigned long target_addr;	
	unsigned int errcode;		
};

extern void itmon_notifier_chain_register(struct notifier_block *n);
extern void itmon_enable(bool enabled);
extern void itmon_set_errcnt(int cnt);
#else
static inline void itmon_enable(bool enabled) {}
#define itmon_notifier_chain_register(x)		do { } while (0)
#define itmon_set_errcnt(x)				do { } while (0)
#define itmon_enable(x)					do { } while (0)
#endif

#endif
