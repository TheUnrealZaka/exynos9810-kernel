

#ifndef _LINUX_STACKDEPOT_H
#define _LINUX_STACKDEPOT_H

typedef u32 depot_stack_handle_t;

struct stack_trace;

depot_stack_handle_t depot_save_stack(struct stack_trace *trace, gfp_t flags);

void depot_fetch_stack(depot_stack_handle_t handle, struct stack_trace *trace);

#endif
