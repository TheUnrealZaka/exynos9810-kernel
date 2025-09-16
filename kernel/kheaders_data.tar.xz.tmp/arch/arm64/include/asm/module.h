
#ifndef __ASM_MODULE_H
#define __ASM_MODULE_H

#include <asm-generic/module.h>
#include <asm/memory.h>

#define MODULE_ARCH_VERMAGIC	"aarch64"

#ifdef CONFIG_ARM64_MODULE_PLTS
struct mod_arch_specific {
	struct elf64_shdr	*plt;
	int			plt_num_entries;
	int			plt_max_entries;
};
#endif

u64 module_emit_plt_entry(struct module *mod, const Elf64_Rela *rela,
			  Elf64_Sym *sym);

#if defined(CONFIG_RANDOMIZE_BASE) || defined(CONFIG_RELOCATABLE_KERNEL)
#ifdef CONFIG_MODVERSIONS
#define ARCH_RELOCATES_KCRCTAB
#define reloc_start 		(kimage_vaddr - KIMAGE_VADDR)
#endif
#endif
#if defined(CONFIG_RANDOMIZE_BASE)
extern u64 module_alloc_base;
#else
#define module_alloc_base	((u64)_etext - MODULES_VSIZE)
#endif

#endif 
