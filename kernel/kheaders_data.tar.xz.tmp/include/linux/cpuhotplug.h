#ifndef __CPUHOTPLUG_H
#define __CPUHOTPLUG_H

#include <linux/types.h>

enum cpuhp_state {
	CPUHP_OFFLINE,
	CPUHP_CREATE_THREADS,
	CPUHP_PERF_PREPARE,
	CPUHP_PERF_X86_PREPARE,
	CPUHP_PERF_X86_UNCORE_PREP,
	CPUHP_PERF_X86_AMD_UNCORE_PREP,
	CPUHP_PERF_BFIN,
	CPUHP_PERF_POWER,
	CPUHP_PERF_SUPERH,
	CPUHP_X86_HPET_DEAD,
	CPUHP_X86_APB_DEAD,
	CPUHP_VIRT_NET_DEAD,
	CPUHP_SLUB_DEAD,
	CPUHP_MM_WRITEBACK_DEAD,
	CPUHP_SOFTIRQ_DEAD,
	CPUHP_NET_MVNETA_DEAD,
	CPUHP_CPUIDLE_DEAD,
	CPUHP_ARM64_FPSIMD_DEAD,
	CPUHP_ARM_OMAP_WAKE_DEAD,
	CPUHP_IRQ_POLL_DEAD,
	CPUHP_BLOCK_SOFTIRQ_DEAD,
	CPUHP_VIRT_SCSI_DEAD,
	CPUHP_ACPI_CPUDRV_DEAD,
	CPUHP_S390_PFAULT_DEAD,
	CPUHP_BLK_MQ_DEAD,
	CPUHP_RANDOM_PREPARE,
	CPUHP_WORKQUEUE_PREP,
	CPUHP_POWER_NUMA_PREPARE,
	CPUHP_HRTIMERS_PREPARE,
	CPUHP_PROFILE_PREPARE,
	CPUHP_X2APIC_PREPARE,
	CPUHP_SMPCFD_PREPARE,
	CPUHP_RELAY_PREPARE,
	CPUHP_SLAB_PREPARE,
	CPUHP_MD_RAID5_PREPARE,
	CPUHP_RCUTREE_PREP,
	CPUHP_CPUIDLE_COUPLED_PREPARE,
	CPUHP_POWERPC_PMAC_PREPARE,
	CPUHP_POWERPC_MMU_CTX_PREPARE,
	CPUHP_XEN_PREPARE,
	CPUHP_XEN_EVTCHN_PREPARE,
	CPUHP_NOTIFY_PREPARE,
	CPUHP_ARM_SHMOBILE_SCU_PREPARE,
	CPUHP_SH_SH3X_PREPARE,
#ifndef CONFIG_EXYNOS_HOTPLUG_GOVERNOR
	CPUHP_BLK_MQ_PREPARE,
#endif
	CPUHP_ZCOMP_PREPARE,
	CPUHP_TIMERS_PREPARE,
	CPUHP_NOTF_ERR_INJ_PREPARE,
	CPUHP_MIPS_SOC_PREPARE,
	CPUHP_BRINGUP_CPU,
	CPUHP_AP_IDLE_DEAD,
	CPUHP_AP_OFFLINE,
	CPUHP_AP_EXYNOS_CPU_DOWN_POWER_CONTROL,
	CPUHP_AP_SCHED_STARTING,
	CPUHP_AP_RCUTREE_DYING,
	CPUHP_AP_IRQ_GIC_STARTING,
	CPUHP_AP_IRQ_GICV3_STARTING,
	CPUHP_AP_IRQ_HIP04_STARTING,
	CPUHP_AP_IRQ_ARMADA_XP_STARTING,
	CPUHP_AP_IRQ_ARMADA_CASC_STARTING,
	CPUHP_AP_IRQ_BCM2836_STARTING,
	CPUHP_AP_ARM_MVEBU_COHERENCY,
	CPUHP_AP_PERF_X86_UNCORE_STARTING,
	CPUHP_AP_PERF_X86_AMD_UNCORE_STARTING,
	CPUHP_AP_PERF_X86_STARTING,
	CPUHP_AP_PERF_X86_AMD_IBS_STARTING,
	CPUHP_AP_PERF_X86_CQM_STARTING,
	CPUHP_AP_PERF_X86_CSTATE_STARTING,
	CPUHP_AP_PERF_XTENSA_STARTING,
	CPUHP_AP_PERF_METAG_STARTING,
	CPUHP_AP_MIPS_OP_LOONGSON3_STARTING,
	CPUHP_AP_ARM_VFP_STARTING,
	CPUHP_AP_ARM64_DEBUG_MONITORS_STARTING,
	CPUHP_AP_PERF_ARM_HW_BREAKPOINT_STARTING,
	CPUHP_AP_PERF_ARM_STARTING,
	CPUHP_AP_ARM_L2X0_STARTING,
	CPUHP_AP_EXYNOS4_MCT_TIMER_STARTING,
	CPUHP_AP_EXYNOS_CPU_UP_POWER_CONTROL,	
	CPUHP_AP_ARM_ARCH_TIMER_STARTING,
	CPUHP_AP_ARM_GLOBAL_TIMER_STARTING,
	CPUHP_AP_JCORE_TIMER_STARTING,
	CPUHP_AP_ARM_TWD_STARTING,
	CPUHP_AP_METAG_TIMER_STARTING,
	CPUHP_AP_QCOM_TIMER_STARTING,
	CPUHP_AP_ARMADA_TIMER_STARTING,
	CPUHP_AP_MARCO_TIMER_STARTING,
	CPUHP_AP_MIPS_GIC_TIMER_STARTING,
	CPUHP_AP_ARC_TIMER_STARTING,
	CPUHP_AP_KVM_STARTING,
	CPUHP_AP_KVM_ARM_VGIC_INIT_STARTING,
	CPUHP_AP_KVM_ARM_VGIC_STARTING,
	CPUHP_AP_KVM_ARM_TIMER_STARTING,
	
	CPUHP_AP_DUMMY_TIMER_STARTING,
	CPUHP_AP_ARM_XEN_STARTING,
	CPUHP_AP_ARM_CORESIGHT_STARTING,
	CPUHP_AP_ARM_CORESIGHT4_STARTING,
	CPUHP_AP_ARM64_ISNDEP_STARTING,
	CPUHP_AP_ECD_STARTING,
	CPUHP_AP_SMPCFD_DYING,
	CPUHP_AP_X86_TBOOT_DYING,
	CPUHP_AP_ETM_DYING,
	CPUHP_AP_ONLINE,
	CPUHP_TEARDOWN_CPU,
	CPUHP_AP_ONLINE_IDLE,
	CPUHP_AP_SMPBOOT_THREADS,
	CPUHP_AP_X86_VDSO_VMA_ONLINE,
	CPUHP_AP_PERF_ONLINE,
	CPUHP_AP_PERF_X86_ONLINE,
	CPUHP_AP_PERF_X86_UNCORE_ONLINE,
	CPUHP_AP_PERF_X86_AMD_UNCORE_ONLINE,
	CPUHP_AP_PERF_X86_AMD_POWER_ONLINE,
	CPUHP_AP_PERF_X86_RAPL_ONLINE,
	CPUHP_AP_PERF_X86_CQM_ONLINE,
	CPUHP_AP_PERF_X86_CSTATE_ONLINE,
	CPUHP_AP_PERF_S390_CF_ONLINE,
	CPUHP_AP_PERF_S390_SF_ONLINE,
	CPUHP_AP_PERF_ARM_CCI_ONLINE,
	CPUHP_AP_PERF_ARM_CCN_ONLINE,
	CPUHP_AP_PERF_ARM_L2X0_ONLINE,
	CPUHP_AP_WORKQUEUE_ONLINE,
	CPUHP_AP_RANDOM_ONLINE,
	CPUHP_AP_RCUTREE_ONLINE,
	CPUHP_AP_NOTIFY_ONLINE,
	CPUHP_AP_ONLINE_DYN,
	CPUHP_AP_ONLINE_DYN_END		= CPUHP_AP_ONLINE_DYN + 30,
	CPUHP_AP_X86_HPET_ONLINE,
	CPUHP_AP_X86_KVM_CLK_ONLINE,
	CPUHP_AP_ACTIVE,
	CPUHP_ONLINE,
};

int __cpuhp_setup_state(enum cpuhp_state state,	const char *name, bool invoke,
			int (*startup)(unsigned int cpu),
			int (*teardown)(unsigned int cpu), bool multi_instance);


static inline int cpuhp_setup_state(enum cpuhp_state state,
				    const char *name,
				    int (*startup)(unsigned int cpu),
				    int (*teardown)(unsigned int cpu))
{
	return __cpuhp_setup_state(state, name, true, startup, teardown, false);
}


static inline int cpuhp_setup_state_nocalls(enum cpuhp_state state,
					    const char *name,
					    int (*startup)(unsigned int cpu),
					    int (*teardown)(unsigned int cpu))
{
	return __cpuhp_setup_state(state, name, false, startup, teardown,
				   false);
}


static inline int cpuhp_setup_state_multi(enum cpuhp_state state,
					  const char *name,
					  int (*startup)(unsigned int cpu,
							 struct hlist_node *node),
					  int (*teardown)(unsigned int cpu,
							  struct hlist_node *node))
{
	return __cpuhp_setup_state(state, name, false,
				   (void *) startup,
				   (void *) teardown, true);
}

int __cpuhp_state_add_instance(enum cpuhp_state state, struct hlist_node *node,
			       bool invoke);


static inline int cpuhp_state_add_instance(enum cpuhp_state state,
					   struct hlist_node *node)
{
	return __cpuhp_state_add_instance(state, node, true);
}


static inline int cpuhp_state_add_instance_nocalls(enum cpuhp_state state,
						   struct hlist_node *node)
{
	return __cpuhp_state_add_instance(state, node, false);
}

void __cpuhp_remove_state(enum cpuhp_state state, bool invoke);


static inline void cpuhp_remove_state(enum cpuhp_state state)
{
	__cpuhp_remove_state(state, true);
}


static inline void cpuhp_remove_state_nocalls(enum cpuhp_state state)
{
	__cpuhp_remove_state(state, false);
}


static inline void cpuhp_remove_multi_state(enum cpuhp_state state)
{
	__cpuhp_remove_state(state, false);
}

int __cpuhp_state_remove_instance(enum cpuhp_state state,
				  struct hlist_node *node, bool invoke);


static inline int cpuhp_state_remove_instance(enum cpuhp_state state,
					      struct hlist_node *node)
{
	return __cpuhp_state_remove_instance(state, node, true);
}


static inline int cpuhp_state_remove_instance_nocalls(enum cpuhp_state state,
						      struct hlist_node *node)
{
	return __cpuhp_state_remove_instance(state, node, false);
}

#ifdef CONFIG_SMP
void cpuhp_online_idle(enum cpuhp_state state);
#else
static inline void cpuhp_online_idle(enum cpuhp_state state) { }
#endif

#endif
