

#ifndef __ARM_PMU_H__
#define __ARM_PMU_H__

#include <linux/interrupt.h>
#include <linux/perf_event.h>
#include <linux/sysfs.h>
#include <asm/cputype.h>


struct arm_pmu_platdata {
	irqreturn_t (*handle_irq)(int irq, void *dev,
				  irq_handler_t pmu_handler);
};

#ifdef CONFIG_ARM_PMU


#define ARMPMU_MAX_HWEVENTS		32

#define HW_OP_UNSUPPORTED		0xFFFF
#define C(_x)				PERF_COUNT_HW_CACHE_##_x
#define CACHE_OP_UNSUPPORTED		0xFFFF

#define PERF_MAP_ALL_UNSUPPORTED					\
	[0 ... PERF_COUNT_HW_MAX - 1] = HW_OP_UNSUPPORTED

#define PERF_CACHE_MAP_ALL_UNSUPPORTED					\
[0 ... C(MAX) - 1] = {							\
	[0 ... C(OP_MAX) - 1] = {					\
		[0 ... C(RESULT_MAX) - 1] = CACHE_OP_UNSUPPORTED,	\
	},								\
}


struct pmu_hw_events {
	
	struct perf_event	*events[ARMPMU_MAX_HWEVENTS];

	
	DECLARE_BITMAP(used_mask, ARMPMU_MAX_HWEVENTS);

	
	raw_spinlock_t		pmu_lock;

	
	struct arm_pmu		*percpu_pmu;
};

enum armpmu_attr_groups {
	ARMPMU_ATTR_GROUP_COMMON,
	ARMPMU_ATTR_GROUP_EVENTS,
	ARMPMU_ATTR_GROUP_FORMATS,
	ARMPMU_NR_ATTR_GROUPS
};

struct arm_pmu {
	struct pmu	pmu;
	cpumask_t	active_irqs;
	cpumask_t	supported_cpus;
	int		*irq_affinity;
	char		*name;
	irqreturn_t	(*handle_irq)(int irq_num, void *dev);
	void		(*enable)(struct perf_event *event);
	void		(*disable)(struct perf_event *event);
	int		(*get_event_idx)(struct pmu_hw_events *hw_events,
					 struct perf_event *event);
	void		(*clear_event_idx)(struct pmu_hw_events *hw_events,
					 struct perf_event *event);
	int		(*set_event_filter)(struct hw_perf_event *evt,
					    struct perf_event_attr *attr);
	u32		(*read_counter)(struct perf_event *event);
	void		(*write_counter)(struct perf_event *event, u32 val);
	void		(*start)(struct arm_pmu *);
	void		(*stop)(struct arm_pmu *);
	void		(*reset)(void *);
	int		(*request_irq)(struct arm_pmu *, irq_handler_t handler);
	void		(*free_irq)(struct arm_pmu *);
	int		(*map_event)(struct perf_event *event);
	int		num_events;
	atomic_t	active_events;
	struct mutex	reserve_mutex;
	u64		max_period;
	bool		secure_access; 
#define ARMV8_PMUV3_MAX_COMMON_EVENTS 0x40
	DECLARE_BITMAP(pmceid_bitmap, ARMV8_PMUV3_MAX_COMMON_EVENTS);
	struct platform_device	*plat_device;
	struct pmu_hw_events	__percpu *hw_events;
	struct hlist_node	node;
	struct notifier_block	cpu_pm_nb;
	
	const struct attribute_group *attr_groups[ARMPMU_NR_ATTR_GROUPS + 1];
};

#define to_arm_pmu(p) (container_of(p, struct arm_pmu, pmu))

u64 armpmu_event_update(struct perf_event *event);

int armpmu_event_set_period(struct perf_event *event);

int armpmu_map_event(struct perf_event *event,
		     const unsigned (*event_map)[PERF_COUNT_HW_MAX],
		     const unsigned (*cache_map)[PERF_COUNT_HW_CACHE_MAX]
						[PERF_COUNT_HW_CACHE_OP_MAX]
						[PERF_COUNT_HW_CACHE_RESULT_MAX],
		     u32 raw_event_mask);

struct pmu_probe_info {
	unsigned int cpuid;
	unsigned int mask;
	int (*init)(struct arm_pmu *);
};

#define PMU_PROBE(_cpuid, _mask, _fn)	\
{					\
	.cpuid = (_cpuid),		\
	.mask = (_mask),		\
	.init = (_fn),			\
}

#define ARM_PMU_PROBE(_cpuid, _fn) \
	PMU_PROBE(_cpuid, ARM_CPU_PART_MASK, _fn)

#define ARM_PMU_XSCALE_MASK	((0xff << 24) | ARM_CPU_XSCALE_ARCH_MASK)

#define XSCALE_PMU_PROBE(_version, _fn) \
	PMU_PROBE(ARM_CPU_IMP_INTEL << 24 | _version, ARM_PMU_XSCALE_MASK, _fn)

int arm_pmu_device_probe(struct platform_device *pdev,
			 const struct of_device_id *of_table,
			 const struct pmu_probe_info *probe_table);

#define ARMV8_PMU_PDEV_NAME "armv8-pmu"

#endif 

#endif 
