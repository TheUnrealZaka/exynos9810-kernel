

#ifndef _LINUX_PERCPU_DEFS_H
#define _LINUX_PERCPU_DEFS_H

#ifdef CONFIG_SMP

#ifdef MODULE
#define PER_CPU_SHARED_ALIGNED_SECTION ""
#define PER_CPU_ALIGNED_SECTION ""
#else
#define PER_CPU_SHARED_ALIGNED_SECTION "..shared_aligned"
#define PER_CPU_ALIGNED_SECTION "..shared_aligned"
#endif
#define PER_CPU_FIRST_SECTION "..first"

#else

#define PER_CPU_SHARED_ALIGNED_SECTION ""
#define PER_CPU_ALIGNED_SECTION "..shared_aligned"
#define PER_CPU_FIRST_SECTION ""

#endif

#ifdef CONFIG_PAGE_TABLE_ISOLATION
#define USER_MAPPED_SECTION "..user_mapped"
#else
#define USER_MAPPED_SECTION ""
#endif


#define __PCPU_ATTRS(sec)						\
	__percpu __attribute__((section(PER_CPU_BASE_SECTION sec)))	\
	PER_CPU_ATTRIBUTES

#define __PCPU_DUMMY_ATTRS						\
	__attribute__((section(".discard"), unused))


#if defined(ARCH_NEEDS_WEAK_PER_CPU) || defined(CONFIG_DEBUG_FORCE_WEAK_PER_CPU)

#define DECLARE_PER_CPU_SECTION(type, name, sec)			\
	extern __PCPU_DUMMY_ATTRS char __pcpu_scope_##name;		\
	extern __PCPU_ATTRS(sec) __typeof__(type) name

#define DEFINE_PER_CPU_SECTION(type, name, sec)				\
	__PCPU_DUMMY_ATTRS char __pcpu_scope_##name;			\
	extern __PCPU_DUMMY_ATTRS char __pcpu_unique_##name;		\
	__PCPU_DUMMY_ATTRS char __pcpu_unique_##name;			\
	extern __PCPU_ATTRS(sec) __typeof__(type) name;			\
	__PCPU_ATTRS(sec) PER_CPU_DEF_ATTRIBUTES __weak			\
	__typeof__(type) name
#else

#define DECLARE_PER_CPU_SECTION(type, name, sec)			\
	extern __PCPU_ATTRS(sec) __typeof__(type) name

#define DEFINE_PER_CPU_SECTION(type, name, sec)				\
	__PCPU_ATTRS(sec) PER_CPU_DEF_ATTRIBUTES			\
	__typeof__(type) name
#endif


#define DECLARE_PER_CPU(type, name)					\
	DECLARE_PER_CPU_SECTION(type, name, "")

#define DEFINE_PER_CPU(type, name)					\
	DEFINE_PER_CPU_SECTION(type, name, "")

#define DECLARE_PER_CPU_USER_MAPPED(type, name)				\
	DECLARE_PER_CPU_SECTION(type, name, USER_MAPPED_SECTION)

#define DEFINE_PER_CPU_USER_MAPPED(type, name)				\
	DEFINE_PER_CPU_SECTION(type, name, USER_MAPPED_SECTION)


#define DECLARE_PER_CPU_FIRST(type, name)				\
	DECLARE_PER_CPU_SECTION(type, name, PER_CPU_FIRST_SECTION)

#define DEFINE_PER_CPU_FIRST(type, name)				\
	DEFINE_PER_CPU_SECTION(type, name, PER_CPU_FIRST_SECTION)


#define DECLARE_PER_CPU_SHARED_ALIGNED(type, name)			\
	DECLARE_PER_CPU_SECTION(type, name, PER_CPU_SHARED_ALIGNED_SECTION) \
	____cacheline_aligned_in_smp

#define DEFINE_PER_CPU_SHARED_ALIGNED(type, name)			\
	DEFINE_PER_CPU_SECTION(type, name, PER_CPU_SHARED_ALIGNED_SECTION) \
	____cacheline_aligned_in_smp

#define DECLARE_PER_CPU_SHARED_ALIGNED_USER_MAPPED(type, name)		\
	DECLARE_PER_CPU_SECTION(type, name, USER_MAPPED_SECTION PER_CPU_SHARED_ALIGNED_SECTION) \
	____cacheline_aligned_in_smp

#define DEFINE_PER_CPU_SHARED_ALIGNED_USER_MAPPED(type, name)		\
	DEFINE_PER_CPU_SECTION(type, name, USER_MAPPED_SECTION PER_CPU_SHARED_ALIGNED_SECTION) \
	____cacheline_aligned_in_smp

#define DECLARE_PER_CPU_ALIGNED(type, name)				\
	DECLARE_PER_CPU_SECTION(type, name, PER_CPU_ALIGNED_SECTION)	\
	____cacheline_aligned

#define DEFINE_PER_CPU_ALIGNED(type, name)				\
	DEFINE_PER_CPU_SECTION(type, name, PER_CPU_ALIGNED_SECTION)	\
	____cacheline_aligned


#define DECLARE_PER_CPU_PAGE_ALIGNED(type, name)			\
	DECLARE_PER_CPU_SECTION(type, name, "..page_aligned")		\
	__aligned(PAGE_SIZE)

#define DEFINE_PER_CPU_PAGE_ALIGNED(type, name)				\
	DEFINE_PER_CPU_SECTION(type, name, "..page_aligned")		\
	__aligned(PAGE_SIZE)

#define DECLARE_PER_CPU_PAGE_ALIGNED_USER_MAPPED(type, name)		\
	DECLARE_PER_CPU_SECTION(type, name, USER_MAPPED_SECTION"..page_aligned") \
	__aligned(PAGE_SIZE)

#define DEFINE_PER_CPU_PAGE_ALIGNED_USER_MAPPED(type, name)		\
	DEFINE_PER_CPU_SECTION(type, name, USER_MAPPED_SECTION"..page_aligned") \
	__aligned(PAGE_SIZE)


#define DECLARE_PER_CPU_READ_MOSTLY(type, name)				\
	DECLARE_PER_CPU_SECTION(type, name, "..read_mostly")

#define DEFINE_PER_CPU_READ_MOSTLY(type, name)				\
	DEFINE_PER_CPU_SECTION(type, name, "..read_mostly")


#ifndef __CHECKER__
#define EXPORT_PER_CPU_SYMBOL(var) EXPORT_SYMBOL(var)
#define EXPORT_PER_CPU_SYMBOL_GPL(var) EXPORT_SYMBOL_GPL(var)
#else
#define EXPORT_PER_CPU_SYMBOL(var)
#define EXPORT_PER_CPU_SYMBOL_GPL(var)
#endif


#ifndef __ASSEMBLY__


#define __verify_pcpu_ptr(ptr)						\
do {									\
	const void __percpu *__vpp_verify = (typeof((ptr) + 0))NULL;	\
	(void)__vpp_verify;						\
} while (0)

#ifdef CONFIG_SMP


#define SHIFT_PERCPU_PTR(__p, __offset)					\
	RELOC_HIDE((typeof(*(__p)) __kernel __force *)(__p), (__offset))

#define per_cpu_ptr(ptr, cpu)						\
({									\
	__verify_pcpu_ptr(ptr);						\
	SHIFT_PERCPU_PTR((ptr), per_cpu_offset((cpu)));			\
})

#define raw_cpu_ptr(ptr)						\
({									\
	__verify_pcpu_ptr(ptr);						\
	arch_raw_cpu_ptr(ptr);						\
})

#ifdef CONFIG_DEBUG_PREEMPT
#define this_cpu_ptr(ptr)						\
({									\
	__verify_pcpu_ptr(ptr);						\
	SHIFT_PERCPU_PTR(ptr, my_cpu_offset);				\
})
#else
#define this_cpu_ptr(ptr) raw_cpu_ptr(ptr)
#endif

#else	

#define VERIFY_PERCPU_PTR(__p)						\
({									\
	__verify_pcpu_ptr(__p);						\
	(typeof(*(__p)) __kernel __force *)(__p);			\
})

#define per_cpu_ptr(ptr, cpu)	({ (void)(cpu); VERIFY_PERCPU_PTR(ptr); })
#define raw_cpu_ptr(ptr)	per_cpu_ptr(ptr, 0)
#define this_cpu_ptr(ptr)	raw_cpu_ptr(ptr)

#endif	

#define per_cpu(var, cpu)	(*per_cpu_ptr(&(var), cpu))


#define get_cpu_var(var)						\
(*({									\
	preempt_disable();						\
	this_cpu_ptr(&var);						\
}))


#define put_cpu_var(var)						\
do {									\
	(void)&(var);							\
	preempt_enable();						\
} while (0)

#define get_cpu_ptr(var)						\
({									\
	preempt_disable();						\
	this_cpu_ptr(var);						\
})

#define put_cpu_ptr(var)						\
do {									\
	(void)(var);							\
	preempt_enable();						\
} while (0)



extern void __bad_size_call_parameter(void);

#ifdef CONFIG_DEBUG_PREEMPT
extern void __this_cpu_preempt_check(const char *op);
#else
static inline void __this_cpu_preempt_check(const char *op) { }
#endif

#define __pcpu_size_call_return(stem, variable)				\
({									\
	typeof(variable) pscr_ret__;					\
	__verify_pcpu_ptr(&(variable));					\
	switch(sizeof(variable)) {					\
	case 1: pscr_ret__ = stem##1(variable); break;			\
	case 2: pscr_ret__ = stem##2(variable); break;			\
	case 4: pscr_ret__ = stem##4(variable); break;			\
	case 8: pscr_ret__ = stem##8(variable); break;			\
	default:							\
		__bad_size_call_parameter(); break;			\
	}								\
	pscr_ret__;							\
})

#define __pcpu_size_call_return2(stem, variable, ...)			\
({									\
	typeof(variable) pscr2_ret__;					\
	__verify_pcpu_ptr(&(variable));					\
	switch(sizeof(variable)) {					\
	case 1: pscr2_ret__ = stem##1(variable, __VA_ARGS__); break;	\
	case 2: pscr2_ret__ = stem##2(variable, __VA_ARGS__); break;	\
	case 4: pscr2_ret__ = stem##4(variable, __VA_ARGS__); break;	\
	case 8: pscr2_ret__ = stem##8(variable, __VA_ARGS__); break;	\
	default:							\
		__bad_size_call_parameter(); break;			\
	}								\
	pscr2_ret__;							\
})


#define __pcpu_double_call_return_bool(stem, pcp1, pcp2, ...)		\
({									\
	bool pdcrb_ret__;						\
	__verify_pcpu_ptr(&(pcp1));					\
	BUILD_BUG_ON(sizeof(pcp1) != sizeof(pcp2));			\
	VM_BUG_ON((unsigned long)(&(pcp1)) % (2 * sizeof(pcp1)));	\
	VM_BUG_ON((unsigned long)(&(pcp2)) !=				\
		  (unsigned long)(&(pcp1)) + sizeof(pcp1));		\
	switch(sizeof(pcp1)) {						\
	case 1: pdcrb_ret__ = stem##1(pcp1, pcp2, __VA_ARGS__); break;	\
	case 2: pdcrb_ret__ = stem##2(pcp1, pcp2, __VA_ARGS__); break;	\
	case 4: pdcrb_ret__ = stem##4(pcp1, pcp2, __VA_ARGS__); break;	\
	case 8: pdcrb_ret__ = stem##8(pcp1, pcp2, __VA_ARGS__); break;	\
	default:							\
		__bad_size_call_parameter(); break;			\
	}								\
	pdcrb_ret__;							\
})

#define __pcpu_size_call(stem, variable, ...)				\
do {									\
	__verify_pcpu_ptr(&(variable));					\
	switch(sizeof(variable)) {					\
		case 1: stem##1(variable, __VA_ARGS__);break;		\
		case 2: stem##2(variable, __VA_ARGS__);break;		\
		case 4: stem##4(variable, __VA_ARGS__);break;		\
		case 8: stem##8(variable, __VA_ARGS__);break;		\
		default: 						\
			__bad_size_call_parameter();break;		\
	}								\
} while (0)




#define raw_cpu_read(pcp)		__pcpu_size_call_return(raw_cpu_read_, pcp)
#define raw_cpu_write(pcp, val)		__pcpu_size_call(raw_cpu_write_, pcp, val)
#define raw_cpu_add(pcp, val)		__pcpu_size_call(raw_cpu_add_, pcp, val)
#define raw_cpu_and(pcp, val)		__pcpu_size_call(raw_cpu_and_, pcp, val)
#define raw_cpu_or(pcp, val)		__pcpu_size_call(raw_cpu_or_, pcp, val)
#define raw_cpu_add_return(pcp, val)	__pcpu_size_call_return2(raw_cpu_add_return_, pcp, val)
#define raw_cpu_xchg(pcp, nval)		__pcpu_size_call_return2(raw_cpu_xchg_, pcp, nval)
#define raw_cpu_cmpxchg(pcp, oval, nval) \
	__pcpu_size_call_return2(raw_cpu_cmpxchg_, pcp, oval, nval)
#define raw_cpu_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2) \
	__pcpu_double_call_return_bool(raw_cpu_cmpxchg_double_, pcp1, pcp2, oval1, oval2, nval1, nval2)

#define raw_cpu_sub(pcp, val)		raw_cpu_add(pcp, -(val))
#define raw_cpu_inc(pcp)		raw_cpu_add(pcp, 1)
#define raw_cpu_dec(pcp)		raw_cpu_sub(pcp, 1)
#define raw_cpu_sub_return(pcp, val)	raw_cpu_add_return(pcp, -(typeof(pcp))(val))
#define raw_cpu_inc_return(pcp)		raw_cpu_add_return(pcp, 1)
#define raw_cpu_dec_return(pcp)		raw_cpu_add_return(pcp, -1)


#define __this_cpu_read(pcp)						\
({									\
	__this_cpu_preempt_check("read");				\
	raw_cpu_read(pcp);						\
})

#define __this_cpu_write(pcp, val)					\
({									\
	__this_cpu_preempt_check("write");				\
	raw_cpu_write(pcp, val);					\
})

#define __this_cpu_add(pcp, val)					\
({									\
	__this_cpu_preempt_check("add");				\
	raw_cpu_add(pcp, val);						\
})

#define __this_cpu_and(pcp, val)					\
({									\
	__this_cpu_preempt_check("and");				\
	raw_cpu_and(pcp, val);						\
})

#define __this_cpu_or(pcp, val)						\
({									\
	__this_cpu_preempt_check("or");					\
	raw_cpu_or(pcp, val);						\
})

#define __this_cpu_add_return(pcp, val)					\
({									\
	__this_cpu_preempt_check("add_return");				\
	raw_cpu_add_return(pcp, val);					\
})

#define __this_cpu_xchg(pcp, nval)					\
({									\
	__this_cpu_preempt_check("xchg");				\
	raw_cpu_xchg(pcp, nval);					\
})

#define __this_cpu_cmpxchg(pcp, oval, nval)				\
({									\
	__this_cpu_preempt_check("cmpxchg");				\
	raw_cpu_cmpxchg(pcp, oval, nval);				\
})

#define __this_cpu_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2) \
({	__this_cpu_preempt_check("cmpxchg_double");			\
	raw_cpu_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2);	\
})

#define __this_cpu_sub(pcp, val)	__this_cpu_add(pcp, -(typeof(pcp))(val))
#define __this_cpu_inc(pcp)		__this_cpu_add(pcp, 1)
#define __this_cpu_dec(pcp)		__this_cpu_sub(pcp, 1)
#define __this_cpu_sub_return(pcp, val)	__this_cpu_add_return(pcp, -(typeof(pcp))(val))
#define __this_cpu_inc_return(pcp)	__this_cpu_add_return(pcp, 1)
#define __this_cpu_dec_return(pcp)	__this_cpu_add_return(pcp, -1)


#define this_cpu_read(pcp)		__pcpu_size_call_return(this_cpu_read_, pcp)
#define this_cpu_write(pcp, val)	__pcpu_size_call(this_cpu_write_, pcp, val)
#define this_cpu_add(pcp, val)		__pcpu_size_call(this_cpu_add_, pcp, val)
#define this_cpu_and(pcp, val)		__pcpu_size_call(this_cpu_and_, pcp, val)
#define this_cpu_or(pcp, val)		__pcpu_size_call(this_cpu_or_, pcp, val)
#define this_cpu_add_return(pcp, val)	__pcpu_size_call_return2(this_cpu_add_return_, pcp, val)
#define this_cpu_xchg(pcp, nval)	__pcpu_size_call_return2(this_cpu_xchg_, pcp, nval)
#define this_cpu_cmpxchg(pcp, oval, nval) \
	__pcpu_size_call_return2(this_cpu_cmpxchg_, pcp, oval, nval)
#define this_cpu_cmpxchg_double(pcp1, pcp2, oval1, oval2, nval1, nval2) \
	__pcpu_double_call_return_bool(this_cpu_cmpxchg_double_, pcp1, pcp2, oval1, oval2, nval1, nval2)

#define this_cpu_sub(pcp, val)		this_cpu_add(pcp, -(typeof(pcp))(val))
#define this_cpu_inc(pcp)		this_cpu_add(pcp, 1)
#define this_cpu_dec(pcp)		this_cpu_sub(pcp, 1)
#define this_cpu_sub_return(pcp, val)	this_cpu_add_return(pcp, -(typeof(pcp))(val))
#define this_cpu_inc_return(pcp)	this_cpu_add_return(pcp, 1)
#define this_cpu_dec_return(pcp)	this_cpu_add_return(pcp, -1)

#endif 
#endif 
