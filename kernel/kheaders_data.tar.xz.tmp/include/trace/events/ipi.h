#undef TRACE_SYSTEM
#define TRACE_SYSTEM ipi

#if !defined(_TRACE_IPI_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_IPI_H

#include <linux/tracepoint.h>


TRACE_EVENT(ipi_raise,

	TP_PROTO(const struct cpumask *mask, const char *reason),

	TP_ARGS(mask, reason),

	TP_STRUCT__entry(
		__bitmask(target_cpus, nr_cpumask_bits)
		__field(const char *, reason)
	),

	TP_fast_assign(
		__assign_bitmask(target_cpus, cpumask_bits(mask), nr_cpumask_bits);
		__entry->reason = reason;
	),

	TP_printk("target_mask=%s (%s)", __get_bitmask(target_cpus), __entry->reason)
);

DECLARE_EVENT_CLASS(ipi_handler,

	TP_PROTO(const char *reason),

	TP_ARGS(reason),

	TP_STRUCT__entry(
		__field(const char *, reason)
	),

	TP_fast_assign(
		__entry->reason = reason;
	),

	TP_printk("(%s)", __entry->reason)
);


DEFINE_EVENT(ipi_handler, ipi_entry,

	TP_PROTO(const char *reason),

	TP_ARGS(reason)
);


DEFINE_EVENT(ipi_handler, ipi_exit,

	TP_PROTO(const char *reason),

	TP_ARGS(reason)
);

#endif 


#include <trace/define_trace.h>
