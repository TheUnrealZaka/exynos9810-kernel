

#ifndef _LINUX_KOBJECT_NS_H
#define _LINUX_KOBJECT_NS_H

struct sock;
struct kobject;


enum kobj_ns_type {
	KOBJ_NS_TYPE_NONE = 0,
	KOBJ_NS_TYPE_NET,
	KOBJ_NS_TYPES
};


struct kobj_ns_type_operations {
	enum kobj_ns_type type;
	bool (*current_may_mount)(void);
	void *(*grab_current_ns)(void);
	const void *(*netlink_ns)(struct sock *sk);
	const void *(*initial_ns)(void);
	void (*drop_ns)(void *);
};

int kobj_ns_type_register(const struct kobj_ns_type_operations *ops);
int kobj_ns_type_registered(enum kobj_ns_type type);
const struct kobj_ns_type_operations *kobj_child_ns_ops(struct kobject *parent);
const struct kobj_ns_type_operations *kobj_ns_ops(struct kobject *kobj);

bool kobj_ns_current_may_mount(enum kobj_ns_type type);
void *kobj_ns_grab_current(enum kobj_ns_type type);
const void *kobj_ns_netlink(enum kobj_ns_type type, struct sock *sk);
const void *kobj_ns_initial(enum kobj_ns_type type);
void kobj_ns_drop(enum kobj_ns_type type, void *ns);

#endif 
