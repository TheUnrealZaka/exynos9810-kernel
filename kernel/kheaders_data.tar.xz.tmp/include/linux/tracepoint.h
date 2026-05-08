#ifndef _LINUX_TRACEPOINT_H
#define _LINUX_TRACEPOINT_H



#include <linux/smp.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/cpumask.h>
#include <linux/rcupdate.h>
#include <linux/tracepoint-defs.h>

struct module;
struct tracepoint;
struct notifier_block;

struct trace_enum_map {
	const char		*system;
	const char		*enum_string;
	unsigned long		enum_value;
};

struct bpf_raw_event_map {
	struct tracepoint       *tp;
 	void                    *bpf_func;
 	u32                     num_args;
 	u32			writable_size;
} __aligned(32);

#define TRACEPOINT_DEFAULT_PRIO	10

extern int
tracepoint_probe_register(struct tracepoint *tp, void *probe, void *data);
extern int
tracepoint_probe_register_prio(struct tracepoint *tp, void *probe, void *data,
			       int prio);
extern int
tracepoint_probe_unregister(struct tracepoint *tp, void *probe, void *data);
extern void
for_each_kernel_tracepoint(void (*fct)(struct tracepoint *tp, void *priv),
		void *priv);

#ifdef CONFIG_MODULES
struct tp_module {
	struct list_head list;
	struct module *mod;
};

bool trace_module_has_bad_taint(struct module *mod);
extern int register_tracepoint_module_notifier(struct notifier_block *nb);
extern int unregister_tracepoint_module_notifier(struct notifier_block *nb);
#else
static inline bool trace_module_has_bad_taint(struct module *mod)
{
	return false;
}
static inline
int register_tracepoint_module_notifier(struct notifier_block *nb)
{
	return 0;
}
static inline
int unregister_tracepoint_module_notifier(struct notifier_block *nb)
{
	return 0;
}
#endif 


static inline void tracepoint_synchronize_unregister(void)
{
	synchronize_sched();
}

#ifdef CONFIG_HAVE_SYSCALL_TRACEPOINTS
extern void syscall_regfunc(void);
extern void syscall_unregfunc(void);
#endif 

#define PARAMS(args...) args

#define TRACE_DEFINE_ENUM(x)

#endif 



#ifndef DECLARE_TRACE

#define TP_PROTO(args...)	args
#define TP_ARGS(args...)	args
#define TP_CONDITION(args...)	args


#if defined(CONFIG_TRACEPOINTS) && !defined(NOTRACE)
#define TRACEPOINTS_ENABLED
#endif

#ifdef TRACEPOINTS_ENABLED


#define __DO_TRACE(tp, proto, args, cond, prercu, postrcu)		\
	do {								\
		struct tracepoint_func *it_func_ptr;			\
		void *it_func;						\
		void *__data;						\
									\
		if (!(cond))						\
			return;						\
		prercu;							\
		rcu_read_lock_sched_notrace();				\
		it_func_ptr = rcu_dereference_sched((tp)->funcs);	\
		if (it_func_ptr) {					\
			do {						\
				it_func = (it_func_ptr)->func;		\
				__data = (it_func_ptr)->data;		\
				((void(*)(proto))(it_func))(args);	\
			} while ((++it_func_ptr)->func);		\
		}							\
		rcu_read_unlock_sched_notrace();			\
		postrcu;						\
	} while (0)

#ifndef MODULE
#define __DECLARE_TRACE_RCU(name, proto, args, cond, data_proto, data_args)	\
	static inline void trace_##name##_rcuidle(proto)		\
	{								\
		if (static_key_false(&__tracepoint_##name.key))		\
			__DO_TRACE(&__tracepoint_##name,		\
				TP_PROTO(data_proto),			\
				TP_ARGS(data_args),			\
				TP_CONDITION(cond),			\
				rcu_irq_enter_irqson(),			\
				rcu_irq_exit_irqson());			\
	}
#else
#define __DECLARE_TRACE_RCU(name, proto, args, cond, data_proto, data_args)
#endif


#define __DECLARE_TRACE(name, proto, args, cond, data_proto, data_args) \
	extern struct tracepoint __tracepoint_##name;			\
	static inline void trace_##name(proto)				\
	{								\
		if (static_key_false(&__tracepoint_##name.key))		\
			__DO_TRACE(&__tracepoint_##name,		\
				TP_PROTO(data_proto),			\
				TP_ARGS(data_args),			\
				TP_CONDITION(cond),,);			\
		if (IS_ENABLED(CONFIG_LOCKDEP) && (cond)) {		\
			rcu_read_lock_sched_notrace();			\
			rcu_dereference_sched(__tracepoint_##name.funcs);\
			rcu_read_unlock_sched_notrace();		\
		}							\
	}								\
	__DECLARE_TRACE_RCU(name, PARAMS(proto), PARAMS(args),		\
		PARAMS(cond), PARAMS(data_proto), PARAMS(data_args))	\
	static inline int						\
	register_trace_##name(void (*probe)(data_proto), void *data)	\
	{								\
		return tracepoint_probe_register(&__tracepoint_##name,	\
						(void *)probe, data);	\
	}								\
	static inline int						\
	register_trace_prio_##name(void (*probe)(data_proto), void *data,\
				   int prio)				\
	{								\
		return tracepoint_probe_register_prio(&__tracepoint_##name, \
					      (void *)probe, data, prio); \
	}								\
	static inline int						\
	unregister_trace_##name(void (*probe)(data_proto), void *data)	\
	{								\
		return tracepoint_probe_unregister(&__tracepoint_##name,\
						(void *)probe, data);	\
	}								\
	static inline void						\
	check_trace_callback_type_##name(void (*cb)(data_proto))	\
	{								\
	}								\
	static inline bool						\
	trace_##name##_enabled(void)					\
	{								\
		return static_key_false(&__tracepoint_##name.key);	\
	}


#define DEFINE_TRACE_FN(name, reg, unreg)				 \
	static const char __tpstrtab_##name[]				 \
	__attribute__((section("__tracepoints_strings"))) = #name;	 \
	struct tracepoint __tracepoint_##name				 \
	__attribute__((section("__tracepoints"))) =			 \
		{ __tpstrtab_##name, STATIC_KEY_INIT_FALSE, reg, unreg, NULL };\
	static struct tracepoint * const __tracepoint_ptr_##name __used	 \
	__attribute__((section("__tracepoints_ptrs"))) =		 \
		&__tracepoint_##name;

#define DEFINE_TRACE(name)						\
	DEFINE_TRACE_FN(name, NULL, NULL);

#define EXPORT_TRACEPOINT_SYMBOL_GPL(name)				\
	EXPORT_SYMBOL_GPL(__tracepoint_##name)
#define EXPORT_TRACEPOINT_SYMBOL(name)					\
	EXPORT_SYMBOL(__tracepoint_##name)

#else 
#define __DECLARE_TRACE(name, proto, args, cond, data_proto, data_args) \
	static inline void trace_##name(proto)				\
	{ }								\
	static inline void trace_##name##_rcuidle(proto)		\
	{ }								\
	static inline int						\
	register_trace_##name(void (*probe)(data_proto),		\
			      void *data)				\
	{								\
		return -ENOSYS;						\
	}								\
	static inline int						\
	unregister_trace_##name(void (*probe)(data_proto),		\
				void *data)				\
	{								\
		return -ENOSYS;						\
	}								\
	static inline void check_trace_callback_type_##name(void (*cb)(data_proto)) \
	{								\
	}								\
	static inline bool						\
	trace_##name##_enabled(void)					\
	{								\
		return false;						\
	}

#define DEFINE_TRACE_FN(name, reg, unreg)
#define DEFINE_TRACE(name)
#define EXPORT_TRACEPOINT_SYMBOL_GPL(name)
#define EXPORT_TRACEPOINT_SYMBOL(name)

#endif 

#ifdef CONFIG_TRACING

#define tracepoint_string(str)						\
	({								\
		static const char *___tp_str __tracepoint_string = str; \
		___tp_str;						\
	})
#define __tracepoint_string	__attribute__((section("__tracepoint_str"), used))
#else

# define tracepoint_string(str) str
# define __tracepoint_string
#endif


#define DECLARE_TRACE_NOARGS(name)					\
	__DECLARE_TRACE(name, void, ,					\
			cpu_online(raw_smp_processor_id()),		\
			void *__data, __data)

#define DECLARE_TRACE(name, proto, args)				\
	__DECLARE_TRACE(name, PARAMS(proto), PARAMS(args),		\
			cpu_online(raw_smp_processor_id()),		\
			PARAMS(void *__data, proto),			\
			PARAMS(__data, args))

#define DECLARE_TRACE_CONDITION(name, proto, args, cond)		\
	__DECLARE_TRACE(name, PARAMS(proto), PARAMS(args),		\
			cpu_online(raw_smp_processor_id()) && (PARAMS(cond)), \
			PARAMS(void *__data, proto),			\
			PARAMS(__data, args))

#define TRACE_EVENT_FLAGS(event, flag)

#define TRACE_EVENT_PERF_PERM(event, expr...)

#endif 

#ifndef TRACE_EVENT


#define DECLARE_EVENT_CLASS(name, proto, args, tstruct, assign, print)
#define DEFINE_EVENT(template, name, proto, args)		\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
#define DEFINE_EVENT_FN(template, name, proto, args, reg, unreg)\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
#define DEFINE_EVENT_PRINT(template, name, proto, args, print)	\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
#define DEFINE_EVENT_CONDITION(template, name, proto,		\
			       args, cond)			\
	DECLARE_TRACE_CONDITION(name, PARAMS(proto),		\
				PARAMS(args), PARAMS(cond))

#define TRACE_EVENT(name, proto, args, struct, assign, print)	\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
#define TRACE_EVENT_FN(name, proto, args, struct,		\
		assign, print, reg, unreg)			\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
#define TRACE_EVENT_FN_COND(name, proto, args, cond, struct,		\
		assign, print, reg, unreg)			\
	DECLARE_TRACE_CONDITION(name, PARAMS(proto),	\
			PARAMS(args), PARAMS(cond))
#define TRACE_EVENT_CONDITION(name, proto, args, cond,		\
			      struct, assign, print)		\
	DECLARE_TRACE_CONDITION(name, PARAMS(proto),		\
				PARAMS(args), PARAMS(cond))

#define TRACE_EVENT_FLAGS(event, flag)

#define TRACE_EVENT_PERF_PERM(event, expr...)

#endif 
