#ifndef _LINUX_MODULE_PARAMS_H
#define _LINUX_MODULE_PARAMS_H

#include <linux/init.h>
#include <linux/stringify.h>
#include <linux/kernel.h>


#ifdef MODULE
#define MODULE_PARAM_PREFIX 
#else
#define MODULE_PARAM_PREFIX KBUILD_MODNAME "."
#endif


#define MAX_PARAM_PREFIX_LEN (64 - sizeof(unsigned long))

#ifdef MODULE
#define __MODULE_INFO(tag, name, info)					  \
static const char __UNIQUE_ID(name)[]					  \
  __used __attribute__((section(".modinfo"), unused, aligned(1)))	  \
  = __stringify(tag) "=" info
#else  

#define __MODULE_INFO(tag, name, info)					  \
  struct __UNIQUE_ID(name) {}
#endif
#define __MODULE_PARM_TYPE(name, _type)					  \
  __MODULE_INFO(parmtype, name##type, #name ":" _type)


#define MODULE_PARM_DESC(_parm, desc) \
	__MODULE_INFO(parm, _parm, #_parm ":" desc)

struct kernel_param;


enum {
	KERNEL_PARAM_OPS_FL_NOARG = (1 << 0)
};

struct kernel_param_ops {
	
	unsigned int flags;
	
	int (*set)(const char *val, const struct kernel_param *kp);
	
	int (*get)(char *buffer, const struct kernel_param *kp);
	
	void (*free)(void *arg);
};


enum {
	KERNEL_PARAM_FL_UNSAFE = (1 << 0)
};

struct kernel_param {
	const char *name;
	struct module *mod;
	const struct kernel_param_ops *ops;
	const u16 perm;
	s8 level;
	u8 flags;
	union {
		void *arg;
		const struct kparam_string *str;
		const struct kparam_array *arr;
	};
};

extern const struct kernel_param __start___param[], __stop___param[];


struct kparam_string {
	unsigned int maxlen;
	char *string;
};


struct kparam_array
{
	unsigned int max;
	unsigned int elemsize;
	unsigned int *num;
	const struct kernel_param_ops *ops;
	void *elem;
};


#define module_param(name, type, perm)				\
	module_param_named(name, name, type, perm)


#define module_param_unsafe(name, type, perm)			\
	module_param_named_unsafe(name, name, type, perm)


#define module_param_named(name, value, type, perm)			   \
	param_check_##type(name, &(value));				   \
	module_param_cb(name, &param_ops_##type, &value, perm);		   \
	__MODULE_PARM_TYPE(name, #type)


#define module_param_named_unsafe(name, value, type, perm)		\
	param_check_##type(name, &(value));				\
	module_param_cb_unsafe(name, &param_ops_##type, &value, perm);	\
	__MODULE_PARM_TYPE(name, #type)


#define module_param_cb(name, ops, arg, perm)				      \
	__module_param_call(MODULE_PARAM_PREFIX, name, ops, arg, perm, -1, 0)

#define module_param_cb_unsafe(name, ops, arg, perm)			      \
	__module_param_call(MODULE_PARAM_PREFIX, name, ops, arg, perm, -1,    \
			    KERNEL_PARAM_FL_UNSAFE)


#define __level_param_cb(name, ops, arg, perm, level)			\
	__module_param_call(MODULE_PARAM_PREFIX, name, ops, arg, perm, level, 0)

#define core_param_cb(name, ops, arg, perm)		\
	__level_param_cb(name, ops, arg, perm, 1)

#define postcore_param_cb(name, ops, arg, perm)		\
	__level_param_cb(name, ops, arg, perm, 2)

#define arch_param_cb(name, ops, arg, perm)		\
	__level_param_cb(name, ops, arg, perm, 3)

#define subsys_param_cb(name, ops, arg, perm)		\
	__level_param_cb(name, ops, arg, perm, 4)

#define fs_param_cb(name, ops, arg, perm)		\
	__level_param_cb(name, ops, arg, perm, 5)

#define device_param_cb(name, ops, arg, perm)		\
	__level_param_cb(name, ops, arg, perm, 6)

#define late_param_cb(name, ops, arg, perm)		\
	__level_param_cb(name, ops, arg, perm, 7)


#if defined(CONFIG_ALPHA) || defined(CONFIG_IA64) || defined(CONFIG_PPC64)
#define __moduleparam_const
#else
#define __moduleparam_const const
#endif


#define __module_param_call(prefix, name, ops, arg, perm, level, flags)	\
				\
	static const char __param_str_##name[] = prefix #name;		\
	static struct kernel_param __moduleparam_const __param_##name	\
	__used								\
    __attribute__ ((unused,__section__ ("__param"),aligned(sizeof(void *)))) \
	= { __param_str_##name, THIS_MODULE, ops,			\
	    VERIFY_OCTAL_PERMISSIONS(perm), level, flags, { arg } }


#define module_param_call(name, _set, _get, arg, perm)			\
	static const struct kernel_param_ops __param_ops_##name =	\
		{ .flags = 0, .set = _set, .get = _get };		\
	__module_param_call(MODULE_PARAM_PREFIX,			\
			    name, &__param_ops_##name, arg, perm, -1, 0)

#ifdef CONFIG_SYSFS
extern void kernel_param_lock(struct module *mod);
extern void kernel_param_unlock(struct module *mod);
#else
static inline void kernel_param_lock(struct module *mod)
{
}
static inline void kernel_param_unlock(struct module *mod)
{
}
#endif

#ifndef MODULE

#define core_param(name, var, type, perm)				\
	param_check_##type(name, &(var));				\
	__module_param_call("", name, &param_ops_##type, &var, perm, -1, 0)


#define core_param_unsafe(name, var, type, perm)		\
	param_check_##type(name, &(var));				\
	__module_param_call("", name, &param_ops_##type, &var, perm,	\
			    -1, KERNEL_PARAM_FL_UNSAFE)

#endif 


#define module_param_string(name, string, len, perm)			\
	static const struct kparam_string __param_string_##name		\
		= { len, string };					\
	__module_param_call(MODULE_PARAM_PREFIX, name,			\
			    &param_ops_string,				\
			    .str = &__param_string_##name, perm, -1, 0);\
	__MODULE_PARM_TYPE(name, "string")


extern bool parameq(const char *name1, const char *name2);


extern bool parameqn(const char *name1, const char *name2, size_t n);


extern char *parse_args(const char *name,
		      char *args,
		      const struct kernel_param *params,
		      unsigned num,
		      s16 level_min,
		      s16 level_max,
		      void *arg,
		      int (*unknown)(char *param, char *val,
				     const char *doing, void *arg));


#ifdef CONFIG_SYSFS
extern void destroy_params(const struct kernel_param *params, unsigned num);
#else
static inline void destroy_params(const struct kernel_param *params,
				  unsigned num)
{
}
#endif 



#define __param_check(name, p, type) \
	static inline type __always_unused *__check_##name(void) { return(p); }

extern const struct kernel_param_ops param_ops_byte;
extern int param_set_byte(const char *val, const struct kernel_param *kp);
extern int param_get_byte(char *buffer, const struct kernel_param *kp);
#define param_check_byte(name, p) __param_check(name, p, unsigned char)

extern const struct kernel_param_ops param_ops_short;
extern int param_set_short(const char *val, const struct kernel_param *kp);
extern int param_get_short(char *buffer, const struct kernel_param *kp);
#define param_check_short(name, p) __param_check(name, p, short)

extern const struct kernel_param_ops param_ops_ushort;
extern int param_set_ushort(const char *val, const struct kernel_param *kp);
extern int param_get_ushort(char *buffer, const struct kernel_param *kp);
#define param_check_ushort(name, p) __param_check(name, p, unsigned short)

extern const struct kernel_param_ops param_ops_int;
extern int param_set_int(const char *val, const struct kernel_param *kp);
extern int param_get_int(char *buffer, const struct kernel_param *kp);
#define param_check_int(name, p) __param_check(name, p, int)

extern const struct kernel_param_ops param_ops_uint;
extern int param_set_uint(const char *val, const struct kernel_param *kp);
extern int param_get_uint(char *buffer, const struct kernel_param *kp);
#define param_check_uint(name, p) __param_check(name, p, unsigned int)

extern const struct kernel_param_ops param_ops_long;
extern int param_set_long(const char *val, const struct kernel_param *kp);
extern int param_get_long(char *buffer, const struct kernel_param *kp);
#define param_check_long(name, p) __param_check(name, p, long)

extern const struct kernel_param_ops param_ops_ulong;
extern int param_set_ulong(const char *val, const struct kernel_param *kp);
extern int param_get_ulong(char *buffer, const struct kernel_param *kp);
#define param_check_ulong(name, p) __param_check(name, p, unsigned long)

extern const struct kernel_param_ops param_ops_ullong;
extern int param_set_ullong(const char *val, const struct kernel_param *kp);
extern int param_get_ullong(char *buffer, const struct kernel_param *kp);
#define param_check_ullong(name, p) __param_check(name, p, unsigned long long)

extern const struct kernel_param_ops param_ops_charp;
extern int param_set_charp(const char *val, const struct kernel_param *kp);
extern int param_get_charp(char *buffer, const struct kernel_param *kp);
extern void param_free_charp(void *arg);
#define param_check_charp(name, p) __param_check(name, p, char *)


extern const struct kernel_param_ops param_ops_bool;
extern int param_set_bool(const char *val, const struct kernel_param *kp);
extern int param_get_bool(char *buffer, const struct kernel_param *kp);
#define param_check_bool(name, p) __param_check(name, p, bool)

extern const struct kernel_param_ops param_ops_bool_enable_only;
extern int param_set_bool_enable_only(const char *val,
				      const struct kernel_param *kp);

#define param_check_bool_enable_only param_check_bool

extern const struct kernel_param_ops param_ops_invbool;
extern int param_set_invbool(const char *val, const struct kernel_param *kp);
extern int param_get_invbool(char *buffer, const struct kernel_param *kp);
#define param_check_invbool(name, p) __param_check(name, p, bool)


extern const struct kernel_param_ops param_ops_bint;
extern int param_set_bint(const char *val, const struct kernel_param *kp);
#define param_get_bint param_get_int
#define param_check_bint param_check_int


#define module_param_array(name, type, nump, perm)		\
	module_param_array_named(name, name, type, nump, perm)


#define module_param_array_named(name, array, type, nump, perm)		\
	param_check_##type(name, &(array)[0]);				\
	static const struct kparam_array __param_arr_##name		\
	= { .max = ARRAY_SIZE(array), .num = nump,                      \
	    .ops = &param_ops_##type,					\
	    .elemsize = sizeof(array[0]), .elem = array };		\
	__module_param_call(MODULE_PARAM_PREFIX, name,			\
			    &param_array_ops,				\
			    .arr = &__param_arr_##name,			\
			    perm, -1, 0);				\
	__MODULE_PARM_TYPE(name, "array of " #type)

extern const struct kernel_param_ops param_array_ops;

extern const struct kernel_param_ops param_ops_string;
extern int param_set_copystring(const char *val, const struct kernel_param *);
extern int param_get_string(char *buffer, const struct kernel_param *kp);



struct module;

#if defined(CONFIG_SYSFS) && defined(CONFIG_MODULES)
extern int module_param_sysfs_setup(struct module *mod,
				    const struct kernel_param *kparam,
				    unsigned int num_params);

extern void module_param_sysfs_remove(struct module *mod);
#else
static inline int module_param_sysfs_setup(struct module *mod,
			     const struct kernel_param *kparam,
			     unsigned int num_params)
{
	return 0;
}

static inline void module_param_sysfs_remove(struct module *mod)
{ }
#endif

#endif 
