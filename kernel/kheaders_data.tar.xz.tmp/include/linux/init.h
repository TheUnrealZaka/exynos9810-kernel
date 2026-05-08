#ifndef _LINUX_INIT_H
#define _LINUX_INIT_H

#include <linux/compiler.h>
#include <linux/types.h>


#if defined(__noretpoline) && !defined(MODULE)
#define __noinitretpoline __noretpoline
#else
#define __noinitretpoline
#endif




#define __init		__section(.init.text) __cold notrace __latent_entropy __noinitretpoline __nocfi
#define __initdata	__section(.init.data)
#define __initconst	__section(.init.rodata)
#define __exitdata	__section(.exit.data)
#define __exit_call	__used __section(.exitcall.exit)


#define __ref            __section(.ref.text) noinline
#define __refdata        __section(.ref.data)
#define __refconst       __section(.ref.rodata)

#ifdef MODULE
#define __exitused
#else
#define __exitused  __used
#endif

#define __exit          __section(.exit.text) __exitused __cold notrace


#define __meminit        __section(.meminit.text) __cold notrace \
						  __latent_entropy
#define __meminitdata    __section(.meminit.data)
#define __meminitconst   __section(.meminit.rodata)
#define __memexit        __section(.memexit.text) __exitused __cold notrace
#define __memexitdata    __section(.memexit.data)
#define __memexitconst   __section(.memexit.rodata)


#define __HEAD		.section	".head.text","ax"
#define __INIT		.section	".init.text","ax"
#define __FINIT		.previous

#define __INITDATA	.section	".init.data","aw",%progbits
#define __INITRODATA	.section	".init.rodata","a",%progbits
#define __FINITDATA	.previous

#define __MEMINIT        .section	".meminit.text", "ax"
#define __MEMINITDATA    .section	".meminit.data", "aw"
#define __MEMINITRODATA  .section	".meminit.rodata", "a"


#define __REF            .section       ".ref.text", "ax"
#define __REFDATA        .section       ".ref.data", "aw"
#define __REFCONST       .section       ".ref.rodata", "a"

#ifndef __ASSEMBLY__

typedef int (*initcall_t)(void);
typedef void (*exitcall_t)(void);

extern initcall_t __con_initcall_start[], __con_initcall_end[];
extern initcall_t __security_initcall_start[], __security_initcall_end[];


typedef void (*ctor_fn_t)(void);


extern int do_one_initcall(initcall_t fn);
extern char __initdata boot_command_line[];
extern char *saved_command_line;
extern unsigned int reset_devices;


void setup_arch(char **);
void prepare_namespace(void);
void __init load_default_modules(void);
int __init init_rootfs(void);

#if defined(CONFIG_DEBUG_RODATA) || defined(CONFIG_DEBUG_SET_MODULE_RONX)
extern bool rodata_enabled;
#endif
#ifdef CONFIG_DEBUG_RODATA
void mark_rodata_ro(void);
#endif

extern void (*late_time_init)(void);

extern bool initcall_debug;

#endif
  
#ifndef MODULE

#ifndef __ASSEMBLY__

#ifdef CONFIG_LTO_CLANG
  
  #define ___initcall_name2(c, fn, id) 	__initcall_##c##_##fn##id
  #define ___initcall_name1(c, fn, id)	___initcall_name2(c, fn, id)
  #define __initcall_name(fn, id) 	___initcall_name1(__COUNTER__, fn, id)
#else
  #define __initcall_name(fn, id) 	__initcall_##fn##id
#endif



#define __define_initcall(fn, id) \
	static initcall_t __initcall_name(fn, id) __used \
	__attribute__((__section__(".initcall" #id ".init"))) = fn;


#define early_initcall(fn)		__define_initcall(fn, early)


#define pure_initcall(fn)		__define_initcall(fn, 0)

#define core_initcall(fn)		__define_initcall(fn, 1)
#define core_initcall_sync(fn)		__define_initcall(fn, 1s)
#define postcore_initcall(fn)		__define_initcall(fn, 2)
#define postcore_initcall_sync(fn)	__define_initcall(fn, 2s)
#define arch_initcall(fn)		__define_initcall(fn, 3)
#define arch_initcall_sync(fn)		__define_initcall(fn, 3s)
#define subsys_initcall(fn)		__define_initcall(fn, 4)
#define subsys_initcall_sync(fn)	__define_initcall(fn, 4s)
#define fs_initcall(fn)			__define_initcall(fn, 5)
#define fs_initcall_sync(fn)		__define_initcall(fn, 5s)
#define rootfs_initcall(fn)		__define_initcall(fn, rootfs)
#define device_initcall(fn)		__define_initcall(fn, 6)
#define device_initcall_sync(fn)	__define_initcall(fn, 6s)
#define late_initcall(fn)		__define_initcall(fn, 7)
#define late_initcall_sync(fn)		__define_initcall(fn, 7s)

#define __initcall(fn) device_initcall(fn)

#define __exitcall(fn)						\
	static exitcall_t __exitcall_##fn __exit_call = fn

#define console_initcall(fn)					\
	static initcall_t __initcall_##fn			\
	__used __section(.con_initcall.init) = fn

#define security_initcall(fn)					\
	static initcall_t __initcall_##fn			\
	__used __section(.security_initcall.init) = fn

#ifdef CONFIG_DEFERRED_INITCALLS
#define deferred_initcall(fn, id) \
	static initcall_t __initcall_##fn##id __used \
	__attribute__((__section__(".deferred_initcall" #id ".init"))) = fn
#endif

struct obs_kernel_param {
	const char *str;
	int (*setup_func)(char *);
	int early;
};


#define __setup_param(str, unique_id, fn, early)			\
	static const char __setup_str_##unique_id[] __initconst		\
		__aligned(1) = str; 					\
	static struct obs_kernel_param __setup_##unique_id		\
		__used __section(.init.setup)				\
		__attribute__((aligned((sizeof(long)))))		\
		= { __setup_str_##unique_id, fn, early }

#define __setup(str, fn)						\
	__setup_param(str, fn, fn, 0)


#define early_param(str, fn)						\
	__setup_param(str, fn, fn, 1)

#define early_param_on_off(str_on, str_off, var, config)		\
									\
	int var = IS_ENABLED(config);					\
									\
	static int __init parse_##var##_on(char *arg)			\
	{								\
		var = 1;						\
		return 0;						\
	}								\
	__setup_param(str_on, parse_##var##_on, parse_##var##_on, 1);	\
									\
	static int __init parse_##var##_off(char *arg)			\
	{								\
		var = 0;						\
		return 0;						\
	}								\
	__setup_param(str_off, parse_##var##_off, parse_##var##_off, 1)


void __init parse_early_param(void);
void __init parse_early_options(char *cmdline);
#endif 

#ifdef CONFIG_DEFERRED_INITCALLS
#define deferred_module_init(fn) deferred_initcall(fn, 0)
#define deferred_module_init_sync(fn) deferred_initcall(fn, 0s)
#endif
#else 

#ifdef CONFIG_DEFERRED_INITCALLS
#define deferred_module_init(fn) module_init(fn)
#endif

#define __setup_param(str, unique_id, fn)	
#define __setup(str, func) 			
#endif


#define __nosavedata __section(.data..nosave)

#ifdef MODULE
#define __exit_p(x) x
#else
#define __exit_p(x) NULL
#endif

#endif 
