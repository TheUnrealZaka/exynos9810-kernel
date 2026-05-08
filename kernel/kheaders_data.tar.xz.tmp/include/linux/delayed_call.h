#ifndef _DELAYED_CALL_H
#define _DELAYED_CALL_H



struct delayed_call {
	void (*fn)(void *);
	void *arg;
};

#define DEFINE_DELAYED_CALL(name) struct delayed_call name = {NULL, NULL}


static inline void set_delayed_call(struct delayed_call *call,
		void (*fn)(void *), void *arg)
{
	call->fn = fn;
	call->arg = arg;
}

static inline void do_delayed_call(struct delayed_call *call)
{
	if (call->fn)
		call->fn(call->arg);
}

static inline void clear_delayed_call(struct delayed_call *call)
{
	call->fn = NULL;
}
#endif
