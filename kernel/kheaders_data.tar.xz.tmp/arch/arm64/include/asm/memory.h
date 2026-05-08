
#ifndef __ASM_MEMORY_H
#define __ASM_MEMORY_H

#include <linux/compiler.h>
#include <linux/const.h>
#include <linux/types.h>
#include <asm/bug.h>
#include <asm/sizes.h>


#define UL(x) _AC(x, UL)


#define PCI_IO_SIZE		SZ_16M


#ifdef CONFIG_MEMCG
	#define STRUCT_PAGE_MAX_SHIFT	7
#else
#define STRUCT_PAGE_MAX_SHIFT	6
#endif


#define VMEMMAP_SIZE (UL(1) << (VA_BITS - PAGE_SHIFT - 1 + STRUCT_PAGE_MAX_SHIFT))


#define VA_BITS			(CONFIG_ARM64_VA_BITS)
#define VA_START		(UL(0xffffffffffffffff) - \
	(UL(1) << VA_BITS) + 1)
#define PAGE_OFFSET		(UL(0xffffffffffffffff) - \
	(UL(1) << (VA_BITS - 1)) + 1)
#define KIMAGE_VADDR		(MODULES_END)
#define MODULES_END		(MODULES_VADDR + MODULES_VSIZE)
#define MODULES_VADDR		(VA_START + KASAN_SHADOW_SIZE)
#define MODULES_VSIZE		(SZ_128M)
#define VMEMMAP_START		(PAGE_OFFSET - VMEMMAP_SIZE)
#define PCI_IO_END		(VMEMMAP_START - SZ_2M)
#define PCI_IO_START		(PCI_IO_END - PCI_IO_SIZE)
#define FIXADDR_TOP		(PCI_IO_START - SZ_2M)

#define KERNEL_START      _text
#define KERNEL_END        _end


#ifdef CONFIG_KASAN
#define KASAN_SHADOW_SIZE	(UL(1) << (VA_BITS - 3))
#else
#define KASAN_SHADOW_SIZE	(0)
#endif


#define __virt_to_phys(x) ({						\
	phys_addr_t __x = (phys_addr_t)(x);				\
	__x & BIT(VA_BITS - 1) ? (__x & ~PAGE_OFFSET) + PHYS_OFFSET :	\
				 (__x - kimage_voffset); })

#define __phys_to_virt(x)	((unsigned long)((x) - PHYS_OFFSET) | PAGE_OFFSET)
#define __phys_to_kimg(x)	((unsigned long)((x) + kimage_voffset))


#define page_to_phys(page)	(__pfn_to_phys(page_to_pfn(page)))
#define phys_to_page(phys)	(pfn_to_page(__phys_to_pfn(phys)))


#define MT_DEVICE_nGnRnE	0
#define MT_DEVICE_nGnRE		1
#define MT_DEVICE_GRE		2
#define MT_NORMAL_NC		3
#define MT_NORMAL		4
#define MT_NORMAL_WT		5


#define MT_S2_NORMAL		0xf
#define MT_S2_DEVICE_nGnRE	0x1

#ifdef CONFIG_ARM64_4K_PAGES
#define IOREMAP_MAX_ORDER	(PUD_SHIFT)
#else
#define IOREMAP_MAX_ORDER	(PMD_SHIFT)
#endif

#ifdef CONFIG_BLK_DEV_INITRD
#define __early_init_dt_declare_initrd(__start, __end)			\
	do {								\
		initrd_start = (__start);				\
		initrd_end = (__end);					\
	} while (0)
#endif

#ifndef __ASSEMBLY__

#include <linux/bitops.h>
#include <linux/mmdebug.h>

extern s64			memstart_addr;

#define PHYS_OFFSET		({ VM_BUG_ON(memstart_addr & 1); memstart_addr; })


extern u64			kimage_vaddr;


extern u64			kimage_voffset;

static inline unsigned long kaslr_offset(void)
{
	return kimage_vaddr - KIMAGE_VADDR;
}


#define MIN_MEMBLOCK_ADDR	0
#define MAX_MEMBLOCK_ADDR	U64_MAX


#define PHYS_PFN_OFFSET	(PHYS_OFFSET >> PAGE_SHIFT)


#define virt_to_phys virt_to_phys
static inline phys_addr_t virt_to_phys(const volatile void *x)
{
	return __virt_to_phys((unsigned long)(x));
}

#define phys_to_virt phys_to_virt
static inline void *phys_to_virt(phys_addr_t x)
{
	return (void *)(__phys_to_virt(x));
}


#define __pa(x)			__virt_to_phys((unsigned long)(x))
#define __va(x)			((void *)__phys_to_virt((phys_addr_t)(x)))
#define pfn_to_kaddr(pfn)	__va((pfn) << PAGE_SHIFT)
#define virt_to_pfn(x)      __phys_to_pfn(__virt_to_phys(x))
#define sym_to_pfn(x)	    __phys_to_pfn(__pa_symbol(x))


#define ARCH_PFN_OFFSET		((unsigned long)PHYS_PFN_OFFSET)

#ifndef CONFIG_SPARSEMEM_VMEMMAP
#define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)
#define _virt_addr_valid(kaddr)	pfn_valid(__pa(kaddr) >> PAGE_SHIFT)
#else
#define __virt_to_pgoff(kaddr)	(((u64)(kaddr) & ~PAGE_OFFSET) / PAGE_SIZE * sizeof(struct page))
#define __page_to_voff(kaddr)	(((u64)(kaddr) & ~VMEMMAP_START) * PAGE_SIZE / sizeof(struct page))

#define page_to_virt(page)	((void *)((__page_to_voff(page)) | PAGE_OFFSET))
#define virt_to_page(vaddr)	((struct page *)((__virt_to_pgoff(vaddr)) | VMEMMAP_START))

#define _virt_addr_valid(kaddr)	pfn_valid((((u64)(kaddr) & ~PAGE_OFFSET) \
					   + PHYS_OFFSET) >> PAGE_SHIFT)
#endif
#endif

#define _virt_addr_is_linear(kaddr)	(((u64)(kaddr)) >= PAGE_OFFSET)
#define virt_addr_valid(kaddr)		(_virt_addr_is_linear(kaddr) && \
					 _virt_addr_valid(kaddr))

#define ZONE_MOVABLE_SIZE_BYTES	0

#if defined(CONFIG_ZONE_MOVABLE) && defined(CONFIG_ZONE_MOVABLE_SIZE_MBYTES) \
					&& (CONFIG_ZONE_MOVABLE_SIZE_MBYTES > 0)
#undef ZONE_MOVABLE_SIZE_BYTES
#define ZONE_MOVABLE_SIZE_BYTES	((u32)(CONFIG_ZONE_MOVABLE_SIZE_MBYTES << 20))
#endif

#include <asm-generic/memory_model.h>

#endif
