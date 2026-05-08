#undef TRACE_SYSTEM
#define TRACE_SYSTEM workqueue

#if !defined(_TRACE_WORKQUEUE_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_WORKQUEUE_H

#include <linux/tracepoint.h>
#include <linux/workqueue.h>

DECLARE_EVENT_CLASS(workqueue_work,

	TP_PROTO(struct work_struct *work),

	TP_ARGS(work),

	TP_STRUCT__entry(
		__field( void *,	work	)
	),

	TP_fast_assign(
		__entry->work		= work;
	),

	TP_printk("work struct %p", __entry->work)
);


TRACE_EVENT(workqueue_queue_work,

	TP_PROTO(unsigned int req_cpu, struct pool_workqueue *pwq,
		 struct work_struct *work),

	TP_ARGS(req_cpu, pwq, work),

	TP_STRUCT__entry(
		__field( void *,	work	)
		__field( void *,	function)
		__field( void *,	workqueue)
		__field( unsigned int,	req_cpu	)
		__field( unsigned int,	cpu	)
	),

	TP_fast_assign(
		__entry->work		= work;
		__entry->function	= work->func;
		__entry->workqueue	= pwq->wq;
		__entry->req_cpu	= req_cpu;
		__entry->cpu		= pwq->pool->cpu;
	),

	TP_printk("work struct=%p function=%pf workqueue=%p req_cpu=%u cpu=%u",
		  __entry->work, __entry->function, __entry->workqueue,
		  __entry->req_cpu, __entry->cpu)
);


DEFINE_EVENT(workqueue_work, workqueue_activate_work,

	TP_PROTO(struct work_struct *work),

	TP_ARGS(work)
);


TRACE_EVENT(workqueue_execute_start,

	TP_PROTO(struct work_struct *work),

	TP_ARGS(work),

	TP_STRUCT__entry(
		__field( void *,	work	)
		__field( void *,	function)
	),

	TP_fast_assign(
		__entry->work		= work;
		__entry->function	= work->func;
	),

	TP_printk("work struct %p: function %pf", __entry->work, __entry->function)
);


DEFINE_EVENT(workqueue_work, workqueue_execute_end,

	TP_PROTO(struct work_struct *work),

	TP_ARGS(work)
);

#endif 


#include <trace/define_trace.h>
