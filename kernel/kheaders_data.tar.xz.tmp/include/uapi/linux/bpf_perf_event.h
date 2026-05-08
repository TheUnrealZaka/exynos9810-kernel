
#ifndef _UAPI__LINUX_BPF_PERF_EVENT_H__
#define _UAPI__LINUX_BPF_PERF_EVENT_H__

#include <linux/types.h>
#include <linux/ptrace.h>

struct bpf_perf_event_data {
	struct pt_regs regs;
	__u64 sample_period;
};

#endif 
