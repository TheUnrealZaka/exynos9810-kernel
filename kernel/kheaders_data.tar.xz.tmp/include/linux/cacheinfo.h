#ifndef _LINUX_CACHEINFO_H
#define _LINUX_CACHEINFO_H

#include <linux/bitops.h>
#include <linux/cpumask.h>
#include <linux/smp.h>

struct device_node;
struct attribute;

enum cache_type {
	CACHE_TYPE_NOCACHE = 0,
	CACHE_TYPE_INST = BIT(0),
	CACHE_TYPE_DATA = BIT(1),
	CACHE_TYPE_SEPARATE = CACHE_TYPE_INST | CACHE_TYPE_DATA,
	CACHE_TYPE_UNIFIED = BIT(2),
};


struct cacheinfo {
	enum cache_type type;
	unsigned int level;
	unsigned int coherency_line_size;
	unsigned int number_of_sets;
	unsigned int ways_of_associativity;
	unsigned int physical_line_partition;
	unsigned int size;
	cpumask_t shared_cpu_map;
	unsigned int attributes;
#define CACHE_WRITE_THROUGH	BIT(0)
#define CACHE_WRITE_BACK	BIT(1)
#define CACHE_WRITE_POLICY_MASK		\
	(CACHE_WRITE_THROUGH | CACHE_WRITE_BACK)
#define CACHE_READ_ALLOCATE	BIT(2)
#define CACHE_WRITE_ALLOCATE	BIT(3)
#define CACHE_ALLOCATE_POLICY_MASK	\
	(CACHE_READ_ALLOCATE | CACHE_WRITE_ALLOCATE)

	struct device_node *of_node;
	bool disable_sysfs;
	void *priv;
};

struct cpu_cacheinfo {
	struct cacheinfo *info_list;
	unsigned int num_levels;
	unsigned int num_leaves;
	bool cpu_map_populated;
};


#define DEFINE_SMP_CALL_CACHE_FUNCTION(func)			\
static inline void _##func(void *ret)				\
{								\
	int cpu = smp_processor_id();				\
	*(int *)ret = __##func(cpu);				\
}								\
								\
int func(unsigned int cpu)					\
{								\
	int ret;						\
	smp_call_function_single(cpu, _##func, &ret, true);	\
	return ret;						\
}

struct cpu_cacheinfo *get_cpu_cacheinfo(unsigned int cpu);
int init_cache_level(unsigned int cpu);
int populate_cache_leaves(unsigned int cpu);

const struct attribute_group *cache_get_priv_group(struct cacheinfo *this_leaf);

#endif 
