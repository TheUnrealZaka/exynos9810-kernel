#ifndef _LINUX_MMAN_H
#define _LINUX_MMAN_H

#include <linux/mm.h>
#include <linux/percpu_counter.h>

#include <linux/atomic.h>
#include <uapi/linux/mman.h>

extern int sysctl_overcommit_memory;
extern int sysctl_overcommit_ratio;
extern unsigned long sysctl_overcommit_kbytes;
extern struct percpu_counter vm_committed_as;

#ifdef CONFIG_SMP
extern s32 vm_committed_as_batch;
#else
#define vm_committed_as_batch 0
#endif

unsigned long vm_memory_committed(void);

static inline void vm_acct_memory(long pages)
{
	__percpu_counter_add(&vm_committed_as, pages, vm_committed_as_batch);
}

static inline void vm_unacct_memory(long pages)
{
	vm_acct_memory(-pages);
}



#ifndef arch_calc_vm_prot_bits
#define arch_calc_vm_prot_bits(prot, pkey) 0
#endif

#ifndef arch_vm_get_page_prot
#define arch_vm_get_page_prot(vm_flags) __pgprot(0)
#endif

#ifndef arch_validate_prot

static inline bool arch_validate_prot(unsigned long prot)
{
	return (prot & ~(PROT_READ | PROT_WRITE | PROT_EXEC | PROT_SEM)) == 0;
}
#define arch_validate_prot arch_validate_prot
#endif


#define _calc_vm_trans(x, bit1, bit2) \
  ((!(bit1) || !(bit2)) ? 0 : \
  ((bit1) <= (bit2) ? ((x) & (bit1)) * ((bit2) / (bit1)) \
   : ((x) & (bit1)) / ((bit1) / (bit2))))


static inline unsigned long
calc_vm_prot_bits(unsigned long prot, unsigned long pkey)
{
	return _calc_vm_trans(prot, PROT_READ,  VM_READ ) |
	       _calc_vm_trans(prot, PROT_WRITE, VM_WRITE) |
	       _calc_vm_trans(prot, PROT_EXEC,  VM_EXEC) |
	       arch_calc_vm_prot_bits(prot, pkey);
}


static inline unsigned long
calc_vm_flag_bits(unsigned long flags)
{
	return _calc_vm_trans(flags, MAP_GROWSDOWN,  VM_GROWSDOWN ) |
	       _calc_vm_trans(flags, MAP_DENYWRITE,  VM_DENYWRITE ) |
	       _calc_vm_trans(flags, MAP_LOCKED,     VM_LOCKED    );
}

unsigned long vm_commit_limit(void);
#endif 
