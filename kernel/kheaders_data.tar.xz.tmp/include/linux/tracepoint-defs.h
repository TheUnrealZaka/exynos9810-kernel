#ifndef TRACEPOINT_DEFS_H
#define TRACEPOINT_DEFS_H 1



#include <linux/atomic.h>
#include <linux/static_key.h>

struct trace_print_flags {
	unsigned long		mask;
	const char		*name;
};

struct trace_print_flags_u64 {
	unsigned long long	mask;
	const char		*name;
};

struct tracepoint_func {
	void *func;
	void *data;
	int prio;
};

struct tracepoint {
	const char *name;		
	struct static_key key;
	void (*regfunc)(void);
	void (*unregfunc)(void);
	struct tracepoint_func __rcu *funcs;
};

#endif
