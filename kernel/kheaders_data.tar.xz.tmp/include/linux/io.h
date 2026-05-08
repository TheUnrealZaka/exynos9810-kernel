

#ifndef _LINUX_IO_H
#define _LINUX_IO_H

#include <linux/types.h>
#include <linux/init.h>
#include <linux/bug.h>
#include <linux/err.h>
#include <asm/io.h>
#include <asm/page.h>

struct device;
struct resource;

__visible void __iowrite32_copy(void __iomem *to, const void *from, size_t count);
void __ioread32_copy(void *to, const void __iomem *from, size_t count);
void __iowrite64_copy(void __iomem *to, const void *from, size_t count);

#ifdef CONFIG_MMU
int ioremap_page_range(unsigned long addr, unsigned long end,
		       phys_addr_t phys_addr, pgprot_t prot);
#else
static inline int ioremap_page_range(unsigned long addr, unsigned long end,
				     phys_addr_t phys_addr, pgprot_t prot)
{
	return 0;
}
#endif

#ifdef CONFIG_HAVE_ARCH_HUGE_VMAP
void __init ioremap_huge_init(void);
int arch_ioremap_pud_supported(void);
int arch_ioremap_pmd_supported(void);
#else
static inline void ioremap_huge_init(void) { }
#endif


#ifdef CONFIG_HAS_IOPORT_MAP
void __iomem * devm_ioport_map(struct device *dev, unsigned long port,
			       unsigned int nr);
void devm_ioport_unmap(struct device *dev, void __iomem *addr);
#else
static inline void __iomem *devm_ioport_map(struct device *dev,
					     unsigned long port,
					     unsigned int nr)
{
	return NULL;
}

static inline void devm_ioport_unmap(struct device *dev, void __iomem *addr)
{
}
#endif

#define IOMEM_ERR_PTR(err) (__force void __iomem *)ERR_PTR(err)

void __iomem *devm_ioremap(struct device *dev, resource_size_t offset,
			   resource_size_t size);
void __iomem *devm_ioremap_nocache(struct device *dev, resource_size_t offset,
				   resource_size_t size);
void __iomem *devm_ioremap_wc(struct device *dev, resource_size_t offset,
				   resource_size_t size);
void devm_iounmap(struct device *dev, void __iomem *addr);
int check_signature(const volatile void __iomem *io_addr,
			const unsigned char *signature, int length);
void devm_ioremap_release(struct device *dev, void *res);

void *devm_memremap(struct device *dev, resource_size_t offset,
		size_t size, unsigned long flags);
void devm_memunmap(struct device *dev, void *addr);

void *__devm_memremap_pages(struct device *dev, struct resource *res);


#ifndef arch_has_dev_port
#define arch_has_dev_port()     (1)
#endif


#ifndef arch_phys_wc_add
static inline int __must_check arch_phys_wc_add(unsigned long base,
						unsigned long size)
{
	return 0;  
}

static inline void arch_phys_wc_del(int handle)
{
}

#define arch_phys_wc_add arch_phys_wc_add
#ifndef arch_phys_wc_index
static inline int arch_phys_wc_index(int handle)
{
	return -1;
}
#define arch_phys_wc_index arch_phys_wc_index
#endif
#endif

enum {
	
	MEMREMAP_WB = 1 << 0,
	MEMREMAP_WT = 1 << 1,
	MEMREMAP_WC = 1 << 2,
};

void *memremap(resource_size_t offset, size_t size, unsigned long flags);
void memunmap(void *addr);


#ifndef arch_io_reserve_memtype_wc
static inline int arch_io_reserve_memtype_wc(resource_size_t base,
					     resource_size_t size)
{
	return 0;
}

static inline void arch_io_free_memtype_wc(resource_size_t base,
					   resource_size_t size)
{
}
#endif

#endif 
