

#ifndef __ARM64_KVM_MMU_H__
#define __ARM64_KVM_MMU_H__

#include <asm/page.h>
#include <asm/memory.h>
#include <asm/cpufeature.h>



#define HYP_PAGE_OFFSET_HIGH_MASK	((UL(1) << VA_BITS) - 1)
#define HYP_PAGE_OFFSET_LOW_MASK	((UL(1) << (VA_BITS - 1)) - 1)

#ifdef __ASSEMBLY__

#include <asm/alternative.h>
#include <asm/cpufeature.h>


.macro kern_hyp_va	reg
alternative_if_not ARM64_HAS_VIRT_HOST_EXTN
	and     \reg, \reg, #HYP_PAGE_OFFSET_HIGH_MASK
alternative_else_nop_endif
alternative_if ARM64_HYP_OFFSET_LOW
	and     \reg, \reg, #HYP_PAGE_OFFSET_LOW_MASK
alternative_else_nop_endif
.endm

#else

#include <asm/pgalloc.h>
#include <asm/cachetype.h>
#include <asm/cacheflush.h>
#include <asm/mmu_context.h>
#include <asm/pgtable.h>

static inline unsigned long __kern_hyp_va(unsigned long v)
{
	asm volatile(ALTERNATIVE("and %0, %0, %1",
				 "nop",
				 ARM64_HAS_VIRT_HOST_EXTN)
		     : "+r" (v)
		     : "i" (HYP_PAGE_OFFSET_HIGH_MASK));
	asm volatile(ALTERNATIVE("nop",
				 "and %0, %0, %1",
				 ARM64_HYP_OFFSET_LOW)
		     : "+r" (v)
		     : "i" (HYP_PAGE_OFFSET_LOW_MASK));
	return v;
}

#define kern_hyp_va(v) 	((typeof(v))(__kern_hyp_va((unsigned long)(v))))


#define hyp_symbol_addr(s)						\
	({								\
		typeof(s) *addr;					\
		asm("adrp	%0, %1\n"				\
		    "add	%0, %0, :lo12:%1\n"			\
		    : "=r" (addr) : "S" (&s));				\
		addr;							\
	})


#define KVM_PHYS_SHIFT	(40)
#define KVM_PHYS_SIZE	(1UL << KVM_PHYS_SHIFT)
#define KVM_PHYS_MASK	(KVM_PHYS_SIZE - 1UL)

#include <asm/stage2_pgtable.h>

int create_hyp_mappings(void *from, void *to, pgprot_t prot);
int create_hyp_io_mappings(void *from, void *to, phys_addr_t);
void free_hyp_pgds(void);

void stage2_unmap_vm(struct kvm *kvm);
int kvm_alloc_stage2_pgd(struct kvm *kvm);
void kvm_free_stage2_pgd(struct kvm *kvm);
int kvm_phys_addr_ioremap(struct kvm *kvm, phys_addr_t guest_ipa,
			  phys_addr_t pa, unsigned long size, bool writable);

int kvm_handle_guest_abort(struct kvm_vcpu *vcpu, struct kvm_run *run);

void kvm_mmu_free_memory_caches(struct kvm_vcpu *vcpu);

phys_addr_t kvm_mmu_get_httbr(void);
phys_addr_t kvm_get_idmap_vector(void);
phys_addr_t kvm_get_idmap_start(void);
int kvm_mmu_init(void);
void kvm_clear_hyp_idmap(void);

#define	kvm_set_pte(ptep, pte)		set_pte(ptep, pte)
#define	kvm_set_pmd(pmdp, pmd)		set_pmd(pmdp, pmd)

static inline pte_t kvm_s2pte_mkwrite(pte_t pte)
{
	pte_val(pte) |= PTE_S2_RDWR;
	return pte;
}

static inline pmd_t kvm_s2pmd_mkwrite(pmd_t pmd)
{
	pmd_val(pmd) |= PMD_S2_RDWR;
	return pmd;
}

static inline void kvm_set_s2pte_readonly(pte_t *pte)
{
	pteval_t pteval;
	unsigned long tmp;

	asm volatile("//	kvm_set_s2pte_readonly\n"
	"	prfm	pstl1strm, %2\n"
	"1:	ldxr	%0, %2\n"
	"	and	%0, %0, %3		// clear PTE_S2_RDWR\n"
	"	orr	%0, %0, %4		// set PTE_S2_RDONLY\n"
	"	stxr	%w1, %0, %2\n"
	"	cbnz	%w1, 1b\n"
	: "=&r" (pteval), "=&r" (tmp), "+Q" (pte_val(*pte))
	: "L" (~PTE_S2_RDWR), "L" (PTE_S2_RDONLY));
}

static inline bool kvm_s2pte_readonly(pte_t *pte)
{
	return (pte_val(*pte) & PTE_S2_RDWR) == PTE_S2_RDONLY;
}

static inline void kvm_set_s2pmd_readonly(pmd_t *pmd)
{
	kvm_set_s2pte_readonly((pte_t *)pmd);
}

static inline bool kvm_s2pmd_readonly(pmd_t *pmd)
{
	return kvm_s2pte_readonly((pte_t *)pmd);
}

static inline bool kvm_page_empty(void *ptr)
{
	struct page *ptr_page = virt_to_page(ptr);
	return page_count(ptr_page) == 1;
}

#define hyp_pte_table_empty(ptep) kvm_page_empty(ptep)

#ifdef __PAGETABLE_PMD_FOLDED
#define hyp_pmd_table_empty(pmdp) (0)
#else
#define hyp_pmd_table_empty(pmdp) kvm_page_empty(pmdp)
#endif

#ifdef __PAGETABLE_PUD_FOLDED
#define hyp_pud_table_empty(pudp) (0)
#else
#define hyp_pud_table_empty(pudp) kvm_page_empty(pudp)
#endif

struct kvm;

#define kvm_flush_dcache_to_poc(a,l)	__flush_dcache_area((a), (l))

static inline bool vcpu_has_cache_enabled(struct kvm_vcpu *vcpu)
{
	return (vcpu_sys_reg(vcpu, SCTLR_EL1) & 0b101) == 0b101;
}

static inline void __coherent_cache_guest_page(struct kvm_vcpu *vcpu,
					       kvm_pfn_t pfn,
					       unsigned long size,
					       bool ipa_uncached)
{
	void *va = page_address(pfn_to_page(pfn));

	kvm_flush_dcache_to_poc(va, size);

	if (!icache_is_aliasing()) {		
		flush_icache_range((unsigned long)va,
				   (unsigned long)va + size);
	} else if (!icache_is_aivivt()) {	
		
		__flush_icache_all();
	}
}

static inline void __kvm_flush_dcache_pte(pte_t pte)
{
	struct page *page = pte_page(pte);
	kvm_flush_dcache_to_poc(page_address(page), PAGE_SIZE);
}

static inline void __kvm_flush_dcache_pmd(pmd_t pmd)
{
	struct page *page = pmd_page(pmd);
	kvm_flush_dcache_to_poc(page_address(page), PMD_SIZE);
}

static inline void __kvm_flush_dcache_pud(pud_t pud)
{
	struct page *page = pud_page(pud);
	kvm_flush_dcache_to_poc(page_address(page), PUD_SIZE);
}

#define kvm_virt_to_phys(x)		__pa_symbol(x)

void kvm_set_way_flush(struct kvm_vcpu *vcpu);
void kvm_toggle_cache(struct kvm_vcpu *vcpu, bool was_enabled);

static inline bool __kvm_cpu_uses_extended_idmap(void)
{
	return __cpu_uses_extended_idmap();
}


static inline void __kvm_extend_hypmap(pgd_t *boot_hyp_pgd,
				       pgd_t *hyp_pgd,
				       pgd_t *merged_hyp_pgd,
				       unsigned long hyp_idmap_start)
{
	int idmap_idx;

	
	VM_BUG_ON(pgd_val(merged_hyp_pgd[0]));
	merged_hyp_pgd[0] = __pgd(__pa(hyp_pgd) | PMD_TYPE_TABLE);

	
	idmap_idx = hyp_idmap_start >> VA_BITS;
	VM_BUG_ON(pgd_val(merged_hyp_pgd[idmap_idx]));
	merged_hyp_pgd[idmap_idx] = __pgd(__pa(boot_hyp_pgd) | PMD_TYPE_TABLE);
}

static inline unsigned int kvm_get_vmid_bits(void)
{
	int reg = read_system_reg(SYS_ID_AA64MMFR1_EL1);

	return (cpuid_feature_extract_unsigned_field(reg, ID_AA64MMFR1_VMIDBITS_SHIFT) == 2) ? 16 : 8;
}


static inline int kvm_read_guest_lock(struct kvm *kvm,
				      gpa_t gpa, void *data, unsigned long len)
{
	int srcu_idx = srcu_read_lock(&kvm->srcu);
	int ret = kvm_read_guest(kvm, gpa, data, len);

	srcu_read_unlock(&kvm->srcu, srcu_idx);

	return ret;
}

#ifdef CONFIG_HARDEN_BRANCH_PREDICTOR
#include <asm/mmu.h>

static inline void *kvm_get_hyp_vector(void)
{
	struct bp_hardening_data *data = arm64_get_bp_hardening_data();
	void *vect = kvm_ksym_ref(__kvm_hyp_vector);

	if (data->template_start) {
		vect = __bp_harden_hyp_vecs_start +
		       data->hyp_vectors_slot * SZ_2K;

		if (!cpus_have_const_cap(ARM64_HAS_VIRT_HOST_EXTN))
			vect = lm_alias(vect);
	}

	return vect;
}

static inline int kvm_map_vectors(void)
{
	return create_hyp_mappings(kvm_ksym_ref(__bp_harden_hyp_vecs_start),
				   kvm_ksym_ref(__bp_harden_hyp_vecs_end),
				   PAGE_HYP_EXEC);
}

#else
static inline void *kvm_get_hyp_vector(void)
{
	return kvm_ksym_ref(__kvm_hyp_vector);
}

static inline int kvm_map_vectors(void)
{
	return 0;
}
#endif

#ifdef CONFIG_ARM64_SSBD
DECLARE_PER_CPU_READ_MOSTLY(u64, arm64_ssbd_callback_required);

static inline int hyp_map_aux_data(void)
{
	int cpu, err;

	for_each_possible_cpu(cpu) {
		u64 *ptr;

		ptr = per_cpu_ptr(&arm64_ssbd_callback_required, cpu);
		err = create_hyp_mappings(ptr, ptr + 1, PAGE_HYP);
		if (err)
			return err;
	}
	return 0;
}
#else
static inline int hyp_map_aux_data(void)
{
	return 0;
}
#endif

#endif 
#endif 
