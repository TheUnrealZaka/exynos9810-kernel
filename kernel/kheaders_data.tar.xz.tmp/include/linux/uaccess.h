#ifndef __LINUX_UACCESS_H__
#define __LINUX_UACCESS_H__

#include <linux/sched.h>

#define uaccess_kernel() segment_eq(get_fs(), KERNEL_DS)

#include <asm/uaccess.h>

static __always_inline void pagefault_disabled_inc(void)
{
	current->pagefault_disabled++;
}

static __always_inline void pagefault_disabled_dec(void)
{
	current->pagefault_disabled--;
	WARN_ON(current->pagefault_disabled < 0);
}


static inline void pagefault_disable(void)
{
	pagefault_disabled_inc();
	
	barrier();
}

static inline void pagefault_enable(void)
{
	
	barrier();
	pagefault_disabled_dec();
}


#define pagefault_disabled() (current->pagefault_disabled != 0)


#define faulthandler_disabled() (pagefault_disabled() || in_atomic())

#ifndef ARCH_HAS_NOCACHE_UACCESS

static inline unsigned long __copy_from_user_inatomic_nocache(void *to,
				const void __user *from, unsigned long n)
{
	return __copy_from_user_inatomic(to, from, n);
}

static inline unsigned long __copy_from_user_nocache(void *to,
				const void __user *from, unsigned long n)
{
	return __copy_from_user(to, from, n);
}

#endif		


extern long probe_kernel_read(void *dst, const void *src, size_t size);
extern long __probe_kernel_read(void *dst, const void *src, size_t size);


extern long probe_user_read(void *dst, const void __user *src, size_t size);


extern long notrace probe_kernel_write(void *dst, const void *src, size_t size);
extern long notrace __probe_kernel_write(void *dst, const void *src, size_t size);


extern long notrace probe_user_write(void __user *dst, const void *src, size_t size);
extern long notrace __probe_user_write(void __user *dst, const void *src, size_t size);

extern long strncpy_from_unsafe(char *dst, const void *unsafe_addr, long count);
extern long strncpy_from_unsafe_user(char *dst, const void __user *unsafe_addr,
				     long count);
extern long strnlen_unsafe_user(const void __user *unsafe_addr, long count);


#define probe_kernel_address(addr, retval)		\
	probe_kernel_read(&retval, addr, sizeof(retval))

#ifndef user_access_begin
#define user_access_begin() do { } while (0)
#define user_access_end() do { } while (0)
#define unsafe_get_user(x, ptr, err) do { if (unlikely(__get_user(x, ptr))) goto err; } while (0)
#define unsafe_put_user(x, ptr, err) do { if (unlikely(__put_user(x, ptr))) goto err; } while (0)
#endif

#endif		
