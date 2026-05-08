#undef TRACE_SYSTEM
#define TRACE_SYSTEM context_tracking

#if !defined(_TRACE_CONTEXT_TRACKING_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_CONTEXT_TRACKING_H

#include <linux/tracepoint.h>

DECLARE_EVENT_CLASS(context_tracking_user,

	TP_PROTO(int dummy),

	TP_ARGS(dummy),

	TP_STRUCT__entry(
		__field( int,	dummy	)
	),

	TP_fast_assign(
		__entry->dummy		= dummy;
	),

	TP_printk("%s", "")
);


DEFINE_EVENT(context_tracking_user, user_enter,

	TP_PROTO(int dummy),

	TP_ARGS(dummy)
);


DEFINE_EVENT(context_tracking_user, user_exit,

	TP_PROTO(int dummy),

	TP_ARGS(dummy)
);


#endif 


#include <trace/define_trace.h>
