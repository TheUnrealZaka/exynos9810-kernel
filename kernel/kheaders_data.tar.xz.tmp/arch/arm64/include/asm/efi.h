#ifndef _ASM_EFI_H
#define _ASM_EFI_H

#include <asm/cpufeature.h>
#include <asm/io.h>
#include <asm/mmu_context.h>
#include <asm/neon.h>
#include <asm/ptrace.h>
#include <asm/tlbflush.h>

#ifdef CONFIG_EFI
extern void efi_init(void);
#else
#define efi_init()
#endif

int efi_create_mapping(struct mm_struct *mm, efi_memory_desc_t *md);
int efi_set_mapping_permissions(struct mm_struct *mm, efi_memory_desc_t *md);

#define arch_efi_call_virt_setup()					\
({									\
	kernel_neon_begin();						\
	efi_virtmap_load();						\
})

#define arch_efi_call_virt(p, f, args...)				\
({									\
	efi_##f##_t *__f;						\
	__f = p->f;							\
	__f(args);							\
})

#define arch_efi_call_virt_teardown()					\
({									\
	efi_virtmap_unload();						\
	kernel_neon_end();						\
})

#define ARCH_EFI_IRQ_FLAGS_MASK (PSR_D_BIT | PSR_A_BIT | PSR_I_BIT | PSR_F_BIT)




#define EFI_FDT_ALIGN	SZ_2M   
#define MAX_FDT_OFFSET	SZ_512M

#define efi_call_early(f, ...)		sys_table_arg->boottime->f(__VA_ARGS__)
#define __efi_call_early(f, ...)	f(__VA_ARGS__)
#define efi_is_64bit()			(true)

#define alloc_screen_info(x...)		&screen_info

static inline void free_screen_info(efi_system_table_t *sys_table_arg,
				    struct screen_info *si)
{
}


extern struct screen_info screen_info __attribute__((__visibility__("hidden")));

static inline void efifb_setup_from_dmi(struct screen_info *si, const char *opt)
{
}

#define EFI_ALLOC_ALIGN		SZ_64K



static inline void efi_set_pgd(struct mm_struct *mm)
{
	__switch_mm(mm);

	if (system_uses_ttbr0_pan()) {
		if (mm != current->active_mm) {
			
			update_saved_ttbr0(current, mm);
			uaccess_ttbr0_enable();
			post_ttbr_update_workaround();
		} else {
			
			uaccess_ttbr0_disable();
			update_saved_ttbr0(current, current->active_mm);
		}
	}
}

void efi_virtmap_load(void);
void efi_virtmap_unload(void);

#endif 
