
#ifndef __PMEM_H__
#define __PMEM_H__

#include <linux/io.h>
#include <linux/uio.h>

#ifdef CONFIG_ARCH_HAS_PMEM_API
#define ARCH_MEMREMAP_PMEM MEMREMAP_WB
#include <asm/pmem.h>
#else
#define ARCH_MEMREMAP_PMEM MEMREMAP_WT

static inline void arch_memcpy_to_pmem(void *dst, const void *src, size_t n)
{
	BUG();
}

static inline int arch_memcpy_from_pmem(void *dst, const void *src, size_t n)
{
	BUG();
	return -EFAULT;
}

static inline size_t arch_copy_from_iter_pmem(void *addr, size_t bytes,
		struct iov_iter *i)
{
	BUG();
	return 0;
}

static inline void arch_clear_pmem(void *addr, size_t size)
{
	BUG();
}

static inline void arch_wb_cache_pmem(void *addr, size_t size)
{
	BUG();
}

static inline void arch_invalidate_pmem(void *addr, size_t size)
{
	BUG();
}
#endif

static inline bool arch_has_pmem_api(void)
{
	return IS_ENABLED(CONFIG_ARCH_HAS_PMEM_API);
}


static inline int memcpy_from_pmem(void *dst, void const *src, size_t size)
{
	if (arch_has_pmem_api())
		return arch_memcpy_from_pmem(dst, src, size);
	else
		memcpy(dst, src, size);
	return 0;
}


static inline void memcpy_to_pmem(void *dst, const void *src, size_t n)
{
	if (arch_has_pmem_api())
		arch_memcpy_to_pmem(dst, src, n);
	else
		memcpy(dst, src, n);
}


static inline size_t copy_from_iter_pmem(void *addr, size_t bytes,
		struct iov_iter *i)
{
	if (arch_has_pmem_api())
		return arch_copy_from_iter_pmem(addr, bytes, i);
	return copy_from_iter_nocache(addr, bytes, i);
}


static inline void clear_pmem(void *addr, size_t size)
{
	if (arch_has_pmem_api())
		arch_clear_pmem(addr, size);
	else
		memset(addr, 0, size);
}


static inline void invalidate_pmem(void *addr, size_t size)
{
	if (arch_has_pmem_api())
		arch_invalidate_pmem(addr, size);
}


static inline void wb_cache_pmem(void *addr, size_t size)
{
	if (arch_has_pmem_api())
		arch_wb_cache_pmem(addr, size);
}
#endif 
