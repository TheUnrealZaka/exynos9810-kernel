

#ifndef __LINUX_CPUFEATURE_H
#define __LINUX_CPUFEATURE_H

#ifdef CONFIG_GENERIC_CPU_AUTOPROBE

#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <asm/cpufeature.h>



#ifndef CPU_FEATURE_TYPEFMT
#define CPU_FEATURE_TYPEFMT	"%s"
#endif

#ifndef CPU_FEATURE_TYPEVAL
#define CPU_FEATURE_TYPEVAL	ELF_PLATFORM
#endif


#define module_cpu_feature_match(x, __initfunc)			\
static struct cpu_feature const __maybe_unused cpu_feature_match_ ## x[] = \
	{ { .feature = cpu_feature(x) }, { } };			\
MODULE_DEVICE_TABLE(cpu, cpu_feature_match_ ## x);		\
								\
static int __init cpu_feature_match_ ## x ## _init(void)	\
{								\
	if (!cpu_have_feature(cpu_feature(x)))			\
		return -ENODEV;					\
	return __initfunc();					\
}								\
module_init(cpu_feature_match_ ## x ## _init)

#endif
#endif
