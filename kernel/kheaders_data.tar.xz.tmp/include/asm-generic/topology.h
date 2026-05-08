
#ifndef _ASM_GENERIC_TOPOLOGY_H
#define _ASM_GENERIC_TOPOLOGY_H

#ifndef	CONFIG_NUMA


#ifndef cpu_to_node
#define cpu_to_node(cpu)	((void)(cpu),0)
#endif
#ifndef set_numa_node
#define set_numa_node(node)
#endif
#ifndef set_cpu_numa_node
#define set_cpu_numa_node(cpu, node)
#endif
#ifndef cpu_to_mem
#define cpu_to_mem(cpu)		((void)(cpu),0)
#endif

#ifndef parent_node
#define parent_node(node)	((void)(node),0)
#endif
#ifndef cpumask_of_node
  #ifdef CONFIG_NEED_MULTIPLE_NODES
    #define cpumask_of_node(node)	((node) == 0 ? cpu_online_mask : cpu_none_mask)
  #else
    #define cpumask_of_node(node)	((void)(node), cpu_online_mask)
  #endif
#endif
#ifndef pcibus_to_node
#define pcibus_to_node(bus)	((void)(bus), -1)
#endif

#ifndef cpumask_of_pcibus
#define cpumask_of_pcibus(bus)	(pcibus_to_node(bus) == -1 ?		\
				 cpu_all_mask :				\
				 cpumask_of_node(pcibus_to_node(bus)))
#endif

#endif	

#if !defined(CONFIG_NUMA) || !defined(CONFIG_HAVE_MEMORYLESS_NODES)

#ifndef set_numa_mem
#define set_numa_mem(node)
#endif
#ifndef set_cpu_numa_mem
#define set_cpu_numa_mem(cpu, node)
#endif

#endif	

#endif 
