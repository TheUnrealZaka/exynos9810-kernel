#undef TRACE_SYSTEM
#define TRACE_SYSTEM gpu

#if !defined(_TRACE_GPU_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_GPU_H

#include <linux/tracepoint.h>
#include <linux/time.h>

#define show_secs_from_ns(ns) \
	({ \
		u64 t = ns + (NSEC_PER_USEC / 2); \
		do_div(t, NSEC_PER_SEC); \
		t; \
	})

#define show_usecs_from_ns(ns) \
	({ \
		u64 t = ns + (NSEC_PER_USEC / 2) ; \
		u32 rem; \
		do_div(t, NSEC_PER_USEC); \
		rem = do_div(t, USEC_PER_SEC); \
	})


TRACE_EVENT(gpu_sched_switch,

	TP_PROTO(const char *gpu_name, u64 timestamp,
		u32 next_ctx_id, s32 next_prio, u32 next_job_id),

	TP_ARGS(gpu_name, timestamp, next_ctx_id, next_prio, next_job_id),

	TP_STRUCT__entry(
		__string(       gpu_name,       gpu_name        )
		__field(        u64,            timestamp       )
		__field(        u32,            next_ctx_id     )
		__field(        s32,            next_prio       )
		__field(        u32,            next_job_id     )
	),

	TP_fast_assign(
		__assign_str(gpu_name, gpu_name);
		__entry->timestamp = timestamp;
		__entry->next_ctx_id = next_ctx_id;
		__entry->next_prio = next_prio;
		__entry->next_job_id = next_job_id;
	),

	TP_printk("gpu_name=%s ts=%llu.%06lu next_ctx_id=%lu next_prio=%ld "
		"next_job_id=%lu",
		__get_str(gpu_name),
		(unsigned long long)show_secs_from_ns(__entry->timestamp),
		(unsigned long)show_usecs_from_ns(__entry->timestamp),
		(unsigned long)__entry->next_ctx_id,
		(long)__entry->next_prio,
		(unsigned long)__entry->next_job_id)
);


TRACE_EVENT(gpu_job_enqueue,

	TP_PROTO(u32 ctx_id, u32 job_id, const char *type),

	TP_ARGS(ctx_id, job_id, type),

	TP_STRUCT__entry(
		__field(        u32,            ctx_id          )
		__field(        u32,            job_id          )
		__string(       type,           type            )
	),

	TP_fast_assign(
		__entry->ctx_id = ctx_id;
		__entry->job_id = job_id;
		__assign_str(type, type);
	),

	TP_printk("ctx_id=%lu job_id=%lu type=%s",
		(unsigned long)__entry->ctx_id,
		(unsigned long)__entry->job_id,
		__get_str(type))
);

#undef show_secs_from_ns
#undef show_usecs_from_ns

#endif 


#include <trace/define_trace.h>
