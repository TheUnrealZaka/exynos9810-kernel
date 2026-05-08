#undef TRACE_SYSTEM
#define TRACE_SYSTEM rcu

#if !defined(_TRACE_RCU_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_RCU_H

#include <linux/tracepoint.h>


TRACE_EVENT(rcu_utilization,

	TP_PROTO(const char *s),

	TP_ARGS(s),

	TP_STRUCT__entry(
		__field(const char *, s)
	),

	TP_fast_assign(
		__entry->s = s;
	),

	TP_printk("%s", __entry->s)
);

#ifdef CONFIG_RCU_TRACE

#if defined(CONFIG_TREE_RCU) || defined(CONFIG_PREEMPT_RCU)


TRACE_EVENT(rcu_grace_period,

	TP_PROTO(const char *rcuname, unsigned long gpnum, const char *gpevent),

	TP_ARGS(rcuname, gpnum, gpevent),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(unsigned long, gpnum)
		__field(const char *, gpevent)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->gpnum = gpnum;
		__entry->gpevent = gpevent;
	),

	TP_printk("%s %lu %s",
		  __entry->rcuname, __entry->gpnum, __entry->gpevent)
);


TRACE_EVENT(rcu_future_grace_period,

	TP_PROTO(const char *rcuname, unsigned long gpnum, unsigned long completed,
		 unsigned long c, u8 level, int grplo, int grphi,
		 const char *gpevent),

	TP_ARGS(rcuname, gpnum, completed, c, level, grplo, grphi, gpevent),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(unsigned long, gpnum)
		__field(unsigned long, completed)
		__field(unsigned long, c)
		__field(u8, level)
		__field(int, grplo)
		__field(int, grphi)
		__field(const char *, gpevent)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->gpnum = gpnum;
		__entry->completed = completed;
		__entry->c = c;
		__entry->level = level;
		__entry->grplo = grplo;
		__entry->grphi = grphi;
		__entry->gpevent = gpevent;
	),

	TP_printk("%s %lu %lu %lu %u %d %d %s",
		  __entry->rcuname, __entry->gpnum, __entry->completed,
		  __entry->c, __entry->level, __entry->grplo, __entry->grphi,
		  __entry->gpevent)
);


TRACE_EVENT(rcu_grace_period_init,

	TP_PROTO(const char *rcuname, unsigned long gpnum, u8 level,
		 int grplo, int grphi, unsigned long qsmask),

	TP_ARGS(rcuname, gpnum, level, grplo, grphi, qsmask),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(unsigned long, gpnum)
		__field(u8, level)
		__field(int, grplo)
		__field(int, grphi)
		__field(unsigned long, qsmask)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->gpnum = gpnum;
		__entry->level = level;
		__entry->grplo = grplo;
		__entry->grphi = grphi;
		__entry->qsmask = qsmask;
	),

	TP_printk("%s %lu %u %d %d %lx",
		  __entry->rcuname, __entry->gpnum, __entry->level,
		  __entry->grplo, __entry->grphi, __entry->qsmask)
);


TRACE_EVENT(rcu_exp_grace_period,

	TP_PROTO(const char *rcuname, unsigned long gpseq, const char *gpevent),

	TP_ARGS(rcuname, gpseq, gpevent),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(unsigned long, gpseq)
		__field(const char *, gpevent)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->gpseq = gpseq;
		__entry->gpevent = gpevent;
	),

	TP_printk("%s %lu %s",
		  __entry->rcuname, __entry->gpseq, __entry->gpevent)
);


TRACE_EVENT(rcu_exp_funnel_lock,

	TP_PROTO(const char *rcuname, u8 level, int grplo, int grphi,
		 const char *gpevent),

	TP_ARGS(rcuname, level, grplo, grphi, gpevent),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(u8, level)
		__field(int, grplo)
		__field(int, grphi)
		__field(const char *, gpevent)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->level = level;
		__entry->grplo = grplo;
		__entry->grphi = grphi;
		__entry->gpevent = gpevent;
	),

	TP_printk("%s %d %d %d %s",
		  __entry->rcuname, __entry->level, __entry->grplo,
		  __entry->grphi, __entry->gpevent)
);


TRACE_EVENT(rcu_nocb_wake,

	TP_PROTO(const char *rcuname, int cpu, const char *reason),

	TP_ARGS(rcuname, cpu, reason),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(int, cpu)
		__field(const char *, reason)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->cpu = cpu;
		__entry->reason = reason;
	),

	TP_printk("%s %d %s", __entry->rcuname, __entry->cpu, __entry->reason)
);


TRACE_EVENT(rcu_preempt_task,

	TP_PROTO(const char *rcuname, int pid, unsigned long gpnum),

	TP_ARGS(rcuname, pid, gpnum),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(unsigned long, gpnum)
		__field(int, pid)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->gpnum = gpnum;
		__entry->pid = pid;
	),

	TP_printk("%s %lu %d",
		  __entry->rcuname, __entry->gpnum, __entry->pid)
);


TRACE_EVENT(rcu_unlock_preempted_task,

	TP_PROTO(const char *rcuname, unsigned long gpnum, int pid),

	TP_ARGS(rcuname, gpnum, pid),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(unsigned long, gpnum)
		__field(int, pid)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->gpnum = gpnum;
		__entry->pid = pid;
	),

	TP_printk("%s %lu %d", __entry->rcuname, __entry->gpnum, __entry->pid)
);


TRACE_EVENT(rcu_quiescent_state_report,

	TP_PROTO(const char *rcuname, unsigned long gpnum,
		 unsigned long mask, unsigned long qsmask,
		 u8 level, int grplo, int grphi, int gp_tasks),

	TP_ARGS(rcuname, gpnum, mask, qsmask, level, grplo, grphi, gp_tasks),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(unsigned long, gpnum)
		__field(unsigned long, mask)
		__field(unsigned long, qsmask)
		__field(u8, level)
		__field(int, grplo)
		__field(int, grphi)
		__field(u8, gp_tasks)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->gpnum = gpnum;
		__entry->mask = mask;
		__entry->qsmask = qsmask;
		__entry->level = level;
		__entry->grplo = grplo;
		__entry->grphi = grphi;
		__entry->gp_tasks = gp_tasks;
	),

	TP_printk("%s %lu %lx>%lx %u %d %d %u",
		  __entry->rcuname, __entry->gpnum,
		  __entry->mask, __entry->qsmask, __entry->level,
		  __entry->grplo, __entry->grphi, __entry->gp_tasks)
);


TRACE_EVENT(rcu_fqs,

	TP_PROTO(const char *rcuname, unsigned long gpnum, int cpu, const char *qsevent),

	TP_ARGS(rcuname, gpnum, cpu, qsevent),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(unsigned long, gpnum)
		__field(int, cpu)
		__field(const char *, qsevent)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->gpnum = gpnum;
		__entry->cpu = cpu;
		__entry->qsevent = qsevent;
	),

	TP_printk("%s %lu %d %s",
		  __entry->rcuname, __entry->gpnum,
		  __entry->cpu, __entry->qsevent)
);

#endif 


TRACE_EVENT(rcu_dyntick,

	TP_PROTO(const char *polarity, long long oldnesting, long long newnesting),

	TP_ARGS(polarity, oldnesting, newnesting),

	TP_STRUCT__entry(
		__field(const char *, polarity)
		__field(long long, oldnesting)
		__field(long long, newnesting)
	),

	TP_fast_assign(
		__entry->polarity = polarity;
		__entry->oldnesting = oldnesting;
		__entry->newnesting = newnesting;
	),

	TP_printk("%s %llx %llx", __entry->polarity,
		  __entry->oldnesting, __entry->newnesting)
);


TRACE_EVENT(rcu_prep_idle,

	TP_PROTO(const char *reason),

	TP_ARGS(reason),

	TP_STRUCT__entry(
		__field(const char *, reason)
	),

	TP_fast_assign(
		__entry->reason = reason;
	),

	TP_printk("%s", __entry->reason)
);


TRACE_EVENT(rcu_callback,

	TP_PROTO(const char *rcuname, struct rcu_head *rhp, long qlen_lazy,
		 long qlen),

	TP_ARGS(rcuname, rhp, qlen_lazy, qlen),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(void *, rhp)
		__field(void *, func)
		__field(long, qlen_lazy)
		__field(long, qlen)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->rhp = rhp;
		__entry->func = rhp->func;
		__entry->qlen_lazy = qlen_lazy;
		__entry->qlen = qlen;
	),

	TP_printk("%s rhp=%p func=%pf %ld/%ld",
		  __entry->rcuname, __entry->rhp, __entry->func,
		  __entry->qlen_lazy, __entry->qlen)
);


TRACE_EVENT(rcu_kfree_callback,

	TP_PROTO(const char *rcuname, struct rcu_head *rhp, unsigned long offset,
		 long qlen_lazy, long qlen),

	TP_ARGS(rcuname, rhp, offset, qlen_lazy, qlen),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(void *, rhp)
		__field(unsigned long, offset)
		__field(long, qlen_lazy)
		__field(long, qlen)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->rhp = rhp;
		__entry->offset = offset;
		__entry->qlen_lazy = qlen_lazy;
		__entry->qlen = qlen;
	),

	TP_printk("%s rhp=%p func=%ld %ld/%ld",
		  __entry->rcuname, __entry->rhp, __entry->offset,
		  __entry->qlen_lazy, __entry->qlen)
);


TRACE_EVENT(rcu_batch_start,

	TP_PROTO(const char *rcuname, long qlen_lazy, long qlen, long blimit),

	TP_ARGS(rcuname, qlen_lazy, qlen, blimit),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(long, qlen_lazy)
		__field(long, qlen)
		__field(long, blimit)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->qlen_lazy = qlen_lazy;
		__entry->qlen = qlen;
		__entry->blimit = blimit;
	),

	TP_printk("%s CBs=%ld/%ld bl=%ld",
		  __entry->rcuname, __entry->qlen_lazy, __entry->qlen,
		  __entry->blimit)
);


TRACE_EVENT(rcu_invoke_callback,

	TP_PROTO(const char *rcuname, struct rcu_head *rhp),

	TP_ARGS(rcuname, rhp),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(void *, rhp)
		__field(void *, func)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->rhp = rhp;
		__entry->func = rhp->func;
	),

	TP_printk("%s rhp=%p func=%pf",
		  __entry->rcuname, __entry->rhp, __entry->func)
);


TRACE_EVENT(rcu_invoke_kfree_callback,

	TP_PROTO(const char *rcuname, struct rcu_head *rhp, unsigned long offset),

	TP_ARGS(rcuname, rhp, offset),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(void *, rhp)
		__field(unsigned long, offset)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->rhp = rhp;
		__entry->offset	= offset;
	),

	TP_printk("%s rhp=%p func=%ld",
		  __entry->rcuname, __entry->rhp, __entry->offset)
);


TRACE_EVENT(rcu_batch_end,

	TP_PROTO(const char *rcuname, int callbacks_invoked,
		 char cb, char nr, char iit, char risk),

	TP_ARGS(rcuname, callbacks_invoked, cb, nr, iit, risk),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(int, callbacks_invoked)
		__field(char, cb)
		__field(char, nr)
		__field(char, iit)
		__field(char, risk)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->callbacks_invoked = callbacks_invoked;
		__entry->cb = cb;
		__entry->nr = nr;
		__entry->iit = iit;
		__entry->risk = risk;
	),

	TP_printk("%s CBs-invoked=%d idle=%c%c%c%c",
		  __entry->rcuname, __entry->callbacks_invoked,
		  __entry->cb ? 'C' : '.',
		  __entry->nr ? 'S' : '.',
		  __entry->iit ? 'I' : '.',
		  __entry->risk ? 'R' : '.')
);


TRACE_EVENT(rcu_torture_read,

	TP_PROTO(const char *rcutorturename, struct rcu_head *rhp,
		 unsigned long secs, unsigned long c_old, unsigned long c),

	TP_ARGS(rcutorturename, rhp, secs, c_old, c),

	TP_STRUCT__entry(
		__field(const char *, rcutorturename)
		__field(struct rcu_head *, rhp)
		__field(unsigned long, secs)
		__field(unsigned long, c_old)
		__field(unsigned long, c)
	),

	TP_fast_assign(
		__entry->rcutorturename = rcutorturename;
		__entry->rhp = rhp;
		__entry->secs = secs;
		__entry->c_old = c_old;
		__entry->c = c;
	),

	TP_printk("%s torture read %p %luus c: %lu %lu",
		  __entry->rcutorturename, __entry->rhp,
		  __entry->secs, __entry->c_old, __entry->c)
);


TRACE_EVENT(rcu_barrier,

	TP_PROTO(const char *rcuname, const char *s, int cpu, int cnt, unsigned long done),

	TP_ARGS(rcuname, s, cpu, cnt, done),

	TP_STRUCT__entry(
		__field(const char *, rcuname)
		__field(const char *, s)
		__field(int, cpu)
		__field(int, cnt)
		__field(unsigned long, done)
	),

	TP_fast_assign(
		__entry->rcuname = rcuname;
		__entry->s = s;
		__entry->cpu = cpu;
		__entry->cnt = cnt;
		__entry->done = done;
	),

	TP_printk("%s %s cpu %d remaining %d # %lu",
		  __entry->rcuname, __entry->s, __entry->cpu, __entry->cnt,
		  __entry->done)
);

#else 

#define trace_rcu_grace_period(rcuname, gpnum, gpevent) do { } while (0)
#define trace_rcu_future_grace_period(rcuname, gpnum, completed, c, \
				      level, grplo, grphi, event) \
				      do { } while (0)
#define trace_rcu_grace_period_init(rcuname, gpnum, level, grplo, grphi, \
				    qsmask) do { } while (0)
#define trace_rcu_exp_grace_period(rcuname, gqseq, gpevent) \
	do { } while (0)
#define trace_rcu_exp_funnel_lock(rcuname, level, grplo, grphi, gpevent) \
	do { } while (0)
#define trace_rcu_nocb_wake(rcuname, cpu, reason) do { } while (0)
#define trace_rcu_preempt_task(rcuname, pid, gpnum) do { } while (0)
#define trace_rcu_unlock_preempted_task(rcuname, gpnum, pid) do { } while (0)
#define trace_rcu_quiescent_state_report(rcuname, gpnum, mask, qsmask, level, \
					 grplo, grphi, gp_tasks) do { } \
	while (0)
#define trace_rcu_fqs(rcuname, gpnum, cpu, qsevent) do { } while (0)
#define trace_rcu_dyntick(polarity, oldnesting, newnesting) do { } while (0)
#define trace_rcu_prep_idle(reason) do { } while (0)
#define trace_rcu_callback(rcuname, rhp, qlen_lazy, qlen) do { } while (0)
#define trace_rcu_kfree_callback(rcuname, rhp, offset, qlen_lazy, qlen) \
	do { } while (0)
#define trace_rcu_batch_start(rcuname, qlen_lazy, qlen, blimit) \
	do { } while (0)
#define trace_rcu_invoke_callback(rcuname, rhp) do { } while (0)
#define trace_rcu_invoke_kfree_callback(rcuname, rhp, offset) do { } while (0)
#define trace_rcu_batch_end(rcuname, callbacks_invoked, cb, nr, iit, risk) \
	do { } while (0)
#define trace_rcu_torture_read(rcutorturename, rhp, secs, c_old, c) \
	do { } while (0)
#define trace_rcu_barrier(name, s, cpu, cnt, done) do { } while (0)

#endif 

#endif 


#include <trace/define_trace.h>
