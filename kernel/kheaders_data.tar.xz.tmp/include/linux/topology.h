
#ifndef _LINUX_TOPOLOGY_H
#define _LINUX_TOPOLOGY_H

#include <linux/cpumask.h>
#include <linux/bitops.h>
#include <linux/mmzone.h>
#include <linux/smp.h>
#include <linux/percpu.h>
#include <asm/topology.h>

#ifndef nr_cpus_node
#define nr_cpus_node(node) cpumask_weight(cpumask_of_node(node))
#endif

#define for_each_node_with_cpus(node)			\
	for_each_online_node(node)			\
		if (nr_cpus_node(node))

int arch_update_cpu_topology(void);


#define LOCAL_DISTANCE		10
#define REMOTE_DISTANCE		20
#ifndef node_distance
#define node_distance(from,to)	((from) == (to) ? LOCAL_DISTANCE : REMOTE_DISTANCE)
#endif
#ifndef RECLAIM_DISTANCE

#define RECLAIM_DISTANCE 30
#endif
#ifndef PENALTY_FOR_NODE_WITH_CPUS
#define PENALTY_FOR_NODE_WITH_CPUS	(1)
#endif

#ifdef CONFIG_USE_PERCPU_NUMA_NODE_ID
DECLARE_PER_CPU(int, numa_node);

#ifndef numa_node_id

static inline int numa_node_id(void)
{
	return raw_cpu_read(numa_node);
}
#endif

#ifndef cpu_to_node
static inline int cpu_to_node(int cpu)
{
	return per_cpu(numa_node, cpu);
}
#endif

#ifndef set_numa_node
static inline void set_numa_node(int node)
{
	this_cpu_write(numa_node, node);
}
#endif

#ifndef set_cpu_numa_node
static inline void set_cpu_numa_node(int cpu, int node)
{
	per_cpu(numa_node, cpu) = node;
}
#endif

#else	


#ifndef numa_node_id
static inline int numa_node_id(void)
{
	return cpu_to_node(raw_smp_processor_id());
}
#endif

#endif	

#ifdef CONFIG_HAVE_MEMORYLESS_NODES


DECLARE_PER_CPU(int, _numa_mem_);
extern int _node_numa_mem_[MAX_NUMNODES];

#ifndef set_numa_mem
static inline void set_numa_mem(int node)
{
	this_cpu_write(_numa_mem_, node);
	_node_numa_mem_[numa_node_id()] = node;
}
#endif

#ifndef node_to_mem_node
static inline int node_to_mem_node(int node)
{
	return _node_numa_mem_[node];
}
#endif

#ifndef numa_mem_id

static inline int numa_mem_id(void)
{
	return raw_cpu_read(_numa_mem_);
}
#endif

#ifndef cpu_to_mem
static inline int cpu_to_mem(int cpu)
{
	return per_cpu(_numa_mem_, cpu);
}
#endif

#ifndef set_cpu_numa_mem
static inline void set_cpu_numa_mem(int cpu, int node)
{
	per_cpu(_numa_mem_, cpu) = node;
	_node_numa_mem_[cpu_to_node(cpu)] = node;
}
#endif

#else	

#ifndef numa_mem_id

static inline int numa_mem_id(void)
{
	return numa_node_id();
}
#endif

#ifndef node_to_mem_node
static inline int node_to_mem_node(int node)
{
	return node;
}
#endif

#ifndef cpu_to_mem
static inline int cpu_to_mem(int cpu)
{
	return cpu_to_node(cpu);
}
#endif

#endif	

#ifndef topology_physical_package_id
#define topology_physical_package_id(cpu)	((void)(cpu), -1)
#endif
#ifndef topology_core_id
#define topology_core_id(cpu)			((void)(cpu), 0)
#endif
#ifndef topology_sibling_cpumask
#define topology_sibling_cpumask(cpu)		cpumask_of(cpu)
#endif
#ifndef topology_core_cpumask
#define topology_core_cpumask(cpu)		cpumask_of(cpu)
#endif

#ifdef CONFIG_SCHED_SMT
static inline const struct cpumask *cpu_smt_mask(int cpu)
{
	return topology_sibling_cpumask(cpu);
}
#endif

static inline const struct cpumask *cpu_cpu_mask(int cpu)
{
	return cpumask_of_node(cpu_to_node(cpu));
}


#endif 
