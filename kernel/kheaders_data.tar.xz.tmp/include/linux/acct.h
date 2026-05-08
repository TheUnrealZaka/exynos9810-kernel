
#ifndef _LINUX_ACCT_H
#define _LINUX_ACCT_H

#include <uapi/linux/acct.h>



#ifdef CONFIG_BSD_PROCESS_ACCT
struct vfsmount;
struct super_block;
struct pacct_struct;
struct pid_namespace;
extern int acct_parm[]; 
extern void acct_collect(long exitcode, int group_dead);
extern void acct_process(void);
extern void acct_exit_ns(struct pid_namespace *);
#else
#define acct_collect(x,y)	do { } while (0)
#define acct_process()		do { } while (0)
#define acct_exit_ns(ns)	do { } while (0)
#endif



#undef ACCT_VERSION
#undef AHZ

#ifdef CONFIG_BSD_PROCESS_ACCT_V3
#define ACCT_VERSION	3
#define AHZ		100
typedef struct acct_v3 acct_t;
#else
#ifdef CONFIG_M68K
#define ACCT_VERSION	1
#else
#define ACCT_VERSION	2
#endif
#define AHZ		(USER_HZ)
typedef struct acct acct_t;
#endif

#include <linux/jiffies.h>


static inline u32 jiffies_to_AHZ(unsigned long x)
{
#if (TICK_NSEC % (NSEC_PER_SEC / AHZ)) == 0
# if HZ < AHZ
	return x * (AHZ / HZ);
# else
	return x / (HZ / AHZ);
# endif
#else
        u64 tmp = (u64)x * TICK_NSEC;
        do_div(tmp, (NSEC_PER_SEC / AHZ));
        return (long)tmp;
#endif
}

static inline u64 nsec_to_AHZ(u64 x)
{
#if (NSEC_PER_SEC % AHZ) == 0
	do_div(x, (NSEC_PER_SEC / AHZ));
#elif (AHZ % 512) == 0
	x *= AHZ/512;
	do_div(x, (NSEC_PER_SEC / 512));
#else
	
	x *= 9;
	do_div(x, (unsigned long)((9ull * NSEC_PER_SEC + (AHZ/2))
	                          / AHZ));
#endif
	return x;
}

#endif	
