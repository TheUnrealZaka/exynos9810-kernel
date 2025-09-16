

#ifndef __KVM_TYPES_H__
#define __KVM_TYPES_H__

struct kvm;
struct kvm_async_pf;
struct kvm_device_ops;
struct kvm_interrupt;
struct kvm_irq_routing_table;
struct kvm_memory_slot;
struct kvm_one_reg;
struct kvm_run;
struct kvm_userspace_memory_region;
struct kvm_vcpu;
struct kvm_vcpu_init;
struct kvm_memslots;

enum kvm_mr_change;

#include <asm/types.h>



typedef unsigned long  gva_t;
typedef u64            gpa_t;
typedef u64            gfn_t;

typedef unsigned long  hva_t;
typedef u64            hpa_t;
typedef u64            hfn_t;

typedef hfn_t kvm_pfn_t;

struct gfn_to_hva_cache {
	u64 generation;
	gpa_t gpa;
	unsigned long hva;
	unsigned long len;
	struct kvm_memory_slot *memslot;
};

#endif 
