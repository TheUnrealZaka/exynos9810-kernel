#ifndef _LINUX_MODULE_H
#define _LINUX_MODULE_H

#include <linux/list.h>
#include <linux/stat.h>
#include <linux/compiler.h>
#include <linux/cache.h>
#include <linux/kmod.h>
#include <linux/init.h>
#include <linux/elf.h>
#include <linux/stringify.h>
#include <linux/kobject.h>
#include <linux/moduleparam.h>
#include <linux/jump_label.h>
#include <linux/export.h>
#include <linux/extable.h>	
#include <linux/rbtree_latch.h>
#include <linux/cfi.h>

#include <linux/percpu.h>
#include <asm/module.h>


#define MODULE_SIG_STRING "~Module signature appended~\n"


#define MODULE_SUPPORTED_DEVICE(name)

#define MODULE_NAME_LEN MAX_PARAM_PREFIX_LEN

struct modversion_info {
	unsigned long crc;
	char name[MODULE_NAME_LEN];
};

struct module;
struct exception_table_entry;

struct module_kobject {
	struct kobject kobj;
	struct module *mod;
	struct kobject *drivers_dir;
	struct module_param_attrs *mp;
	struct completion *kobj_completion;
};

struct module_attribute {
	struct attribute attr;
	ssize_t (*show)(struct module_attribute *, struct module_kobject *,
			char *);
	ssize_t (*store)(struct module_attribute *, struct module_kobject *,
			 const char *, size_t count);
	void (*setup)(struct module *, const char *);
	int (*test)(struct module *);
	void (*free)(struct module *);
};

struct module_version_attribute {
	struct module_attribute mattr;
	const char *module_name;
	const char *version;
} __attribute__ ((__aligned__(sizeof(void *))));

extern ssize_t __modver_version_show(struct module_attribute *,
				     struct module_kobject *, char *);

extern struct module_attribute module_uevent;


extern int init_module(void);
extern void cleanup_module(void);

#ifndef MODULE

#define module_init(x)	__initcall(x);


#define module_exit(x)	__exitcall(x);

#else 


#define early_initcall(fn)		module_init(fn)
#define core_initcall(fn)		module_init(fn)
#define core_initcall_sync(fn)		module_init(fn)
#define postcore_initcall(fn)		module_init(fn)
#define postcore_initcall_sync(fn)	module_init(fn)
#define arch_initcall(fn)		module_init(fn)
#define subsys_initcall(fn)		module_init(fn)
#define subsys_initcall_sync(fn)	module_init(fn)
#define fs_initcall(fn)			module_init(fn)
#define fs_initcall_sync(fn)		module_init(fn)
#define rootfs_initcall(fn)		module_init(fn)
#define device_initcall(fn)		module_init(fn)
#define device_initcall_sync(fn)	module_init(fn)
#define late_initcall(fn)		module_init(fn)
#define late_initcall_sync(fn)		module_init(fn)

#define console_initcall(fn)		module_init(fn)
#define security_initcall(fn)		module_init(fn)


#define module_init(initfn)					\
	static inline initcall_t __maybe_unused __inittest(void)		\
	{ return initfn; }					\
	int init_module(void) __copy(initfn) __attribute__((alias(#initfn)));


#define module_exit(exitfn)					\
	static inline exitcall_t __maybe_unused __exittest(void)		\
	{ return exitfn; }					\
	void cleanup_module(void) __copy(exitfn) __attribute__((alias(#exitfn)));

#endif


#ifdef CONFIG_MODULES
#define __init_or_module
#define __initdata_or_module
#define __initconst_or_module
#define __INIT_OR_MODULE	.text
#define __INITDATA_OR_MODULE	.data
#define __INITRODATA_OR_MODULE	.section ".rodata","a",%progbits
#else
#define __init_or_module __init
#define __initdata_or_module __initdata
#define __initconst_or_module __initconst
#define __INIT_OR_MODULE __INIT
#define __INITDATA_OR_MODULE __INITDATA
#define __INITRODATA_OR_MODULE __INITRODATA
#endif 


#define MODULE_INFO(tag, info) __MODULE_INFO(tag, tag, info)


#define MODULE_ALIAS(_alias) MODULE_INFO(alias, _alias)


#define MODULE_SOFTDEP(_softdep) MODULE_INFO(softdep, _softdep)


#define MODULE_LICENSE(_license) MODULE_INFO(license, _license)


#define MODULE_AUTHOR(_author) MODULE_INFO(author, _author)


#define MODULE_DESCRIPTION(_description) MODULE_INFO(description, _description)

#ifdef MODULE

#define MODULE_DEVICE_TABLE(type, name)					\
extern const typeof(name) __mod_##type##__##name##_device_table		\
  __attribute__ ((unused, alias(__stringify(name))))
#else  
#define MODULE_DEVICE_TABLE(type, name)
#endif



#if defined(MODULE) || !defined(CONFIG_SYSFS)
#define MODULE_VERSION(_version) MODULE_INFO(version, _version)
#else
#define MODULE_VERSION(_version)					\
	static struct module_version_attribute ___modver_attr = {	\
		.mattr	= {						\
			.attr	= {					\
				.name	= "version",			\
				.mode	= S_IRUGO,			\
			},						\
			.show	= __modver_version_show,		\
		},							\
		.module_name	= KBUILD_MODNAME,			\
		.version	= _version,				\
	};								\
	static const struct module_version_attribute			\
	__used __attribute__ ((__section__ ("__modver")))		\
	* __moduleparam_const __modver_attr = &___modver_attr
#endif


#define MODULE_FIRMWARE(_firmware) MODULE_INFO(firmware, _firmware)

struct notifier_block;

#ifdef CONFIG_MODULES

extern int modules_disabled; 

void *__symbol_get(const char *symbol);
void *__symbol_get_gpl(const char *symbol);
#define symbol_get(x) ((typeof(&x))(__symbol_get(VMLINUX_SYMBOL_STR(x))))


struct module_use {
	struct list_head source_list;
	struct list_head target_list;
	struct module *source, *target;
};

enum module_state {
	MODULE_STATE_LIVE,	
	MODULE_STATE_COMING,	
	MODULE_STATE_GOING,	
	MODULE_STATE_UNFORMED,	
};

struct module;

struct mod_tree_node {
	struct module *mod;
	struct latch_tree_node node;
};

struct module_layout {
	
	void *base;
	
	unsigned int size;
	
	unsigned int text_size;
	
	unsigned int ro_size;
	
	unsigned int ro_after_init_size;

#ifdef CONFIG_MODULES_TREE_LOOKUP
	struct mod_tree_node mtn;
#endif
};

#ifdef CONFIG_MODULES_TREE_LOOKUP

#define __module_layout_align ____cacheline_aligned
#else
#define __module_layout_align
#endif

struct mod_kallsyms {
	Elf_Sym *symtab;
	unsigned int num_symtab;
	char *strtab;
};

#ifdef CONFIG_LIVEPATCH
struct klp_modinfo {
	Elf_Ehdr hdr;
	Elf_Shdr *sechdrs;
	char *secstrings;
	unsigned int symndx;
};
#endif

struct module {
	enum module_state state;

	
	struct list_head list;

	
	char name[MODULE_NAME_LEN];

	
	struct module_kobject mkobj;
	struct module_attribute *modinfo_attrs;
	const char *version;
	const char *srcversion;
	struct kobject *holders_dir;

	
	const struct kernel_symbol *syms;
	const unsigned long *crcs;
	unsigned int num_syms;

#ifdef CONFIG_CFI_CLANG
	cfi_check_fn cfi_check;
#endif

	
#ifdef CONFIG_SYSFS
	struct mutex param_lock;
#endif
	struct kernel_param *kp;
	unsigned int num_kp;

	
	unsigned int num_gpl_syms;
	const struct kernel_symbol *gpl_syms;
	const unsigned long *gpl_crcs;

#ifdef CONFIG_UNUSED_SYMBOLS
	
	const struct kernel_symbol *unused_syms;
	const unsigned long *unused_crcs;
	unsigned int num_unused_syms;

	
	unsigned int num_unused_gpl_syms;
	const struct kernel_symbol *unused_gpl_syms;
	const unsigned long *unused_gpl_crcs;
#endif

#ifdef CONFIG_MODULE_SIG
	
	bool sig_ok;
#endif

	bool async_probe_requested;

	
	const struct kernel_symbol *gpl_future_syms;
	const unsigned long *gpl_future_crcs;
	unsigned int num_gpl_future_syms;

	
	unsigned int num_exentries;
	struct exception_table_entry *extable;

	
	int (*init)(void);

	
	struct module_layout core_layout __module_layout_align;
	struct module_layout init_layout;

	
	struct mod_arch_specific arch;

	unsigned int taints;	

#ifdef CONFIG_GENERIC_BUG
	
	unsigned num_bugs;
	struct list_head bug_list;
	struct bug_entry *bug_table;
#endif

#ifdef CONFIG_KALLSYMS
	
	struct mod_kallsyms *kallsyms;
	struct mod_kallsyms core_kallsyms;
	
	
	struct module_sect_attrs *sect_attrs;

	
	struct module_notes_attrs *notes_attrs;
#endif

	
	char *args;

#ifdef CONFIG_SMP
	
	void __percpu *percpu;
	unsigned int percpu_size;
#endif

#ifdef CONFIG_TRACEPOINTS
	unsigned int num_tracepoints;
	struct tracepoint * const *tracepoints_ptrs;
#endif
#ifdef CONFIG_BPF_EVENTS
	unsigned int num_bpf_raw_events;
	struct bpf_raw_event_map *bpf_raw_events;
#endif
#ifdef HAVE_JUMP_LABEL
	struct jump_entry *jump_entries;
	unsigned int num_jump_entries;
#endif
#ifdef CONFIG_TRACING
	unsigned int num_trace_bprintk_fmt;
	const char **trace_bprintk_fmt_start;
#endif
#ifdef CONFIG_EVENT_TRACING
	struct trace_event_call **trace_events;
	unsigned int num_trace_events;
	struct trace_enum_map **trace_enums;
	unsigned int num_trace_enums;
#endif
#ifdef CONFIG_FTRACE_MCOUNT_RECORD
	unsigned int num_ftrace_callsites;
	unsigned long *ftrace_callsites;
#endif

#ifdef CONFIG_LIVEPATCH
	bool klp; 
	bool klp_alive;

	
	struct klp_modinfo *klp_info;
#endif

#ifdef CONFIG_MODULE_UNLOAD
	
	struct list_head source_list;
	
	struct list_head target_list;

	
	void (*exit)(void);

	atomic_t refcnt;
#endif

#ifdef CONFIG_CONSTRUCTORS
	
	ctor_fn_t *ctors;
	unsigned int num_ctors;
#endif
} ____cacheline_aligned;
#ifndef MODULE_ARCH_INIT
#define MODULE_ARCH_INIT {}
#endif

extern struct mutex module_mutex;


static inline int module_is_live(struct module *mod)
{
	return mod->state != MODULE_STATE_GOING;
}

struct module *__module_text_address(unsigned long addr);
struct module *__module_address(unsigned long addr);
bool is_module_address(unsigned long addr);
bool is_module_percpu_address(unsigned long addr);
bool is_module_text_address(unsigned long addr);

static inline bool within_module_core(unsigned long addr,
				      const struct module *mod)
{
	return (unsigned long)mod->core_layout.base <= addr &&
	       addr < (unsigned long)mod->core_layout.base + mod->core_layout.size;
}

static inline bool within_module_init(unsigned long addr,
				      const struct module *mod)
{
	return (unsigned long)mod->init_layout.base <= addr &&
	       addr < (unsigned long)mod->init_layout.base + mod->init_layout.size;
}

static inline bool within_module(unsigned long addr, const struct module *mod)
{
	return within_module_init(addr, mod) || within_module_core(addr, mod);
}


struct module *find_module(const char *name);

struct symsearch {
	const struct kernel_symbol *start, *stop;
	const unsigned long *crcs;
	enum {
		NOT_GPL_ONLY,
		GPL_ONLY,
		WILL_BE_GPL_ONLY,
	} licence;
	bool unused;
};


const struct kernel_symbol *find_symbol(const char *name,
					struct module **owner,
					const unsigned long **crc,
					bool gplok,
					bool warn);


bool each_symbol_section(bool (*fn)(const struct symsearch *arr,
				    struct module *owner,
				    void *data), void *data);


int module_get_kallsym(unsigned int symnum, unsigned long *value, char *type,
			char *name, char *module_name, int *exported);


unsigned long module_kallsyms_lookup_name(const char *name);

int module_kallsyms_on_each_symbol(int (*fn)(void *, const char *,
					     struct module *, unsigned long),
				   void *data);

extern void __noreturn __module_put_and_exit(struct module *mod,
			long code);
#define module_put_and_exit(code) __module_put_and_exit(THIS_MODULE, code)

#ifdef CONFIG_MODULE_UNLOAD
int module_refcount(struct module *mod);
void __symbol_put(const char *symbol);
#define symbol_put(x) __symbol_put(VMLINUX_SYMBOL_STR(x))
void symbol_put_addr(void *addr);


extern void __module_get(struct module *module);


extern bool try_module_get(struct module *module);

extern void module_put(struct module *module);

#else 
static inline int try_module_get(struct module *module)
{
	return !module || module_is_live(module);
}
static inline void module_put(struct module *module)
{
}
static inline void __module_get(struct module *module)
{
}
#define symbol_put(x) do { } while (0)
#define symbol_put_addr(p) do { } while (0)

#endif 
int ref_module(struct module *a, struct module *b);


#define module_name(mod)			\
({						\
	struct module *__mod = (mod);		\
	__mod ? __mod->name : "kernel";		\
})


const char *module_address_lookup(unsigned long addr,
			    unsigned long *symbolsize,
			    unsigned long *offset,
			    char **modname,
			    char *namebuf);
int lookup_module_symbol_name(unsigned long addr, char *symname);
int lookup_module_symbol_attrs(unsigned long addr, unsigned long *size, unsigned long *offset, char *modname, char *name);

int register_module_notifier(struct notifier_block *nb);
int unregister_module_notifier(struct notifier_block *nb);

extern void print_modules(void);

static inline bool module_requested_async_probing(struct module *module)
{
	return module && module->async_probe_requested;
}

#ifdef CONFIG_LIVEPATCH
static inline bool is_livepatch_module(struct module *mod)
{
	return mod->klp;
}
#else 
static inline bool is_livepatch_module(struct module *mod)
{
	return false;
}
#endif 

#else 

static inline struct module *__module_address(unsigned long addr)
{
	return NULL;
}

static inline struct module *__module_text_address(unsigned long addr)
{
	return NULL;
}

static inline bool is_module_address(unsigned long addr)
{
	return false;
}

static inline bool is_module_percpu_address(unsigned long addr)
{
	return false;
}

static inline bool is_module_text_address(unsigned long addr)
{
	return false;
}


#define symbol_get(x) ({ extern typeof(x) x __attribute__((weak)); &(x); })
#define symbol_put(x) do { } while (0)
#define symbol_put_addr(x) do { } while (0)

static inline void __module_get(struct module *module)
{
}

static inline int try_module_get(struct module *module)
{
	return 1;
}

static inline void module_put(struct module *module)
{
}

#define module_name(mod) "kernel"


static inline const char *module_address_lookup(unsigned long addr,
					  unsigned long *symbolsize,
					  unsigned long *offset,
					  char **modname,
					  char *namebuf)
{
	return NULL;
}

static inline int lookup_module_symbol_name(unsigned long addr, char *symname)
{
	return -ERANGE;
}

static inline int lookup_module_symbol_attrs(unsigned long addr, unsigned long *size, unsigned long *offset, char *modname, char *name)
{
	return -ERANGE;
}

static inline int module_get_kallsym(unsigned int symnum, unsigned long *value,
					char *type, char *name,
					char *module_name, int *exported)
{
	return -ERANGE;
}

static inline unsigned long module_kallsyms_lookup_name(const char *name)
{
	return 0;
}

static inline int module_kallsyms_on_each_symbol(int (*fn)(void *, const char *,
							   struct module *,
							   unsigned long),
						 void *data)
{
	return 0;
}

static inline int register_module_notifier(struct notifier_block *nb)
{
	
	return 0;
}

static inline int unregister_module_notifier(struct notifier_block *nb)
{
	return 0;
}

#define module_put_and_exit(code) do_exit(code)

static inline void print_modules(void)
{
}

static inline bool module_requested_async_probing(struct module *module)
{
	return false;
}

#endif 

#ifdef CONFIG_SYSFS
extern struct kset *module_kset;
extern struct kobj_type module_ktype;
extern int module_sysfs_initialized;
#endif 

#define symbol_request(x) try_then_request_module(symbol_get(x), "symbol:" #x)



#define __MODULE_STRING(x) __stringify(x)

#ifdef CONFIG_DEBUG_SET_MODULE_RONX
extern void set_all_modules_text_rw(void);
extern void set_all_modules_text_ro(void);
extern void module_enable_ro(const struct module *mod, bool after_init);
extern void module_disable_ro(const struct module *mod);
#else
static inline void set_all_modules_text_rw(void) { }
static inline void set_all_modules_text_ro(void) { }
static inline void module_enable_ro(const struct module *mod, bool after_init) { }
static inline void module_disable_ro(const struct module *mod) { }
#endif

#ifdef CONFIG_GENERIC_BUG
void module_bug_finalize(const Elf_Ehdr *, const Elf_Shdr *,
			 struct module *);
void module_bug_cleanup(struct module *);

#else	

static inline void module_bug_finalize(const Elf_Ehdr *hdr,
					const Elf_Shdr *sechdrs,
					struct module *mod)
{
}
static inline void module_bug_cleanup(struct module *mod) {}
#endif	

#ifdef CONFIG_RETPOLINE
extern bool retpoline_module_ok(bool has_retpoline);
#else
static inline bool retpoline_module_ok(bool has_retpoline)
{
	return true;
}
#endif

#ifdef CONFIG_MODULE_SIG
static inline bool module_sig_ok(struct module *module)
{
	return module->sig_ok;
}
#else	
static inline bool module_sig_ok(struct module *module)
{
	return true;
}
#endif	

#ifdef CONFIG_BPF_JIT
const struct exception_table_entry *search_bpf_extables(unsigned long addr);
#else
static inline const struct exception_table_entry *
search_bpf_extables(unsigned long addr)
{
	return NULL;
}
#endif

#endif 
