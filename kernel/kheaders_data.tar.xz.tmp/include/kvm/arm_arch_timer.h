

#ifndef __ASM_ARM_KVM_ARCH_TIMER_H
#define __ASM_ARM_KVM_ARCH_TIMER_H

#include <linux/clocksource.h>
#include <linux/hrtimer.h>
#include <linux/workqueue.h>

struct arch_timer_kvm {
	
	cycle_t			cntvoff;
};

struct arch_timer_cpu {
	
	u32				cntv_ctl;	
	cycle_t				cntv_cval;	

	

	
	struct hrtimer			timer;

	
	struct work_struct		expired;

	
	bool				armed;

	
	struct kvm_irq_level		irq;

	
	bool				active_cleared_last;

	
	bool			enabled;
};

int kvm_timer_hyp_init(void);
int kvm_timer_enable(struct kvm_vcpu *vcpu);
void kvm_timer_init(struct kvm *kvm);
int kvm_timer_vcpu_reset(struct kvm_vcpu *vcpu,
			 const struct kvm_irq_level *irq);
void kvm_timer_vcpu_init(struct kvm_vcpu *vcpu);
void kvm_timer_flush_hwstate(struct kvm_vcpu *vcpu);
void kvm_timer_sync_hwstate(struct kvm_vcpu *vcpu);
void kvm_timer_vcpu_terminate(struct kvm_vcpu *vcpu);

u64 kvm_arm_timer_get_reg(struct kvm_vcpu *, u64 regid);
int kvm_arm_timer_set_reg(struct kvm_vcpu *, u64 regid, u64 value);

bool kvm_timer_should_fire(struct kvm_vcpu *vcpu);
void kvm_timer_schedule(struct kvm_vcpu *vcpu);
void kvm_timer_unschedule(struct kvm_vcpu *vcpu);

void kvm_timer_vcpu_put(struct kvm_vcpu *vcpu);

#endif
