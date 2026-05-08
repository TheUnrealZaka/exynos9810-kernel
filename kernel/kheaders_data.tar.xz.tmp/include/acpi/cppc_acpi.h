

#ifndef _CPPC_ACPI_H
#define _CPPC_ACPI_H

#include <linux/acpi.h>
#include <linux/types.h>

#include <acpi/pcc.h>
#include <acpi/processor.h>


#define CPPC_NUM_ENT	21
#define CPPC_REV	2

#define PCC_CMD_COMPLETE_MASK	(1 << 0)
#define PCC_ERROR_MASK		(1 << 2)

#define MAX_CPC_REG_ENT 19


#define	CMD_READ 0
#define	CMD_WRITE 1


struct cpc_reg {
	u8 descriptor;
	u16 length;
	u8 space_id;
	u8 bit_width;
	u8 bit_offset;
	u8 access_width;
	u64 __iomem address;
} __packed;


struct cpc_register_resource {
	acpi_object_type type;
	u64 __iomem *sys_mem_vaddr;
	union {
		struct cpc_reg reg;
		u64 int_value;
	} cpc_entry;
};


struct cpc_desc {
	int num_entries;
	int version;
	int cpu_id;
	int write_cmd_status;
	int write_cmd_id;
	struct cpc_register_resource cpc_regs[MAX_CPC_REG_ENT];
	struct acpi_psd_package domain_info;
	struct kobject kobj;
};


enum cppc_regs {
	HIGHEST_PERF,
	NOMINAL_PERF,
	LOW_NON_LINEAR_PERF,
	LOWEST_PERF,
	GUARANTEED_PERF,
	DESIRED_PERF,
	MIN_PERF,
	MAX_PERF,
	PERF_REDUC_TOLERANCE,
	TIME_WINDOW,
	CTR_WRAP_TIME,
	REFERENCE_CTR,
	DELIVERED_CTR,
	PERF_LIMITED,
	ENABLE,
	AUTO_SEL_ENABLE,
	AUTO_ACT_WINDOW,
	ENERGY_PERF,
	REFERENCE_PERF,
};


struct cppc_perf_caps {
	u32 highest_perf;
	u32 nominal_perf;
	u32 lowest_perf;
};

struct cppc_perf_ctrls {
	u32 max_perf;
	u32 min_perf;
	u32 desired_perf;
};

struct cppc_perf_fb_ctrs {
	u64 reference;
	u64 delivered;
	u64 reference_perf;
	u64 ctr_wrap_time;
};


struct cppc_cpudata {
	int cpu;
	struct cppc_perf_caps perf_caps;
	struct cppc_perf_ctrls perf_ctrls;
	struct cppc_perf_fb_ctrs perf_fb_ctrs;
	struct cpufreq_policy *cur_policy;
	unsigned int shared_type;
	cpumask_var_t shared_cpu_map;
};

extern int cppc_get_perf_ctrs(int cpu, struct cppc_perf_fb_ctrs *perf_fb_ctrs);
extern int cppc_set_perf(int cpu, struct cppc_perf_ctrls *perf_ctrls);
extern int cppc_get_perf_caps(int cpu, struct cppc_perf_caps *caps);
extern int acpi_get_psd_map(struct cppc_cpudata **);
extern unsigned int cppc_get_transition_latency(int cpu);

#endif 
