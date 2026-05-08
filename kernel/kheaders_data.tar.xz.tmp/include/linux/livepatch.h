

#ifndef _LINUX_LIVEPATCH_H_
#define _LINUX_LIVEPATCH_H_

#include <linux/module.h>
#include <linux/ftrace.h>

#if IS_ENABLED(CONFIG_LIVEPATCH)

#include <asm/livepatch.h>

enum klp_state {
	KLP_DISABLED,
	KLP_ENABLED
};


struct klp_func {
	
	const char *old_name;
	void *new_func;
	
	unsigned long old_sympos;

	
	unsigned long old_addr;
	struct kobject kobj;
	enum klp_state state;
	struct list_head stack_node;
};


struct klp_object {
	
	const char *name;
	struct klp_func *funcs;

	
	struct kobject kobj;
	struct module *mod;
	enum klp_state state;
};


struct klp_patch {
	
	struct module *mod;
	struct klp_object *objs;

	
	struct list_head list;
	struct kobject kobj;
	enum klp_state state;
};

#define klp_for_each_object(patch, obj) \
	for (obj = patch->objs; obj->funcs || obj->name; obj++)

#define klp_for_each_func(obj, func) \
	for (func = obj->funcs; \
	     func->old_name || func->new_func || func->old_sympos; \
	     func++)

int klp_register_patch(struct klp_patch *);
int klp_unregister_patch(struct klp_patch *);
int klp_enable_patch(struct klp_patch *);
int klp_disable_patch(struct klp_patch *);

void arch_klp_init_object_loaded(struct klp_patch *patch,
				 struct klp_object *obj);


int klp_module_coming(struct module *mod);
void klp_module_going(struct module *mod);

#else 

static inline int klp_module_coming(struct module *mod) { return 0; }
static inline void klp_module_going(struct module *mod) { }

#endif 

#endif 
