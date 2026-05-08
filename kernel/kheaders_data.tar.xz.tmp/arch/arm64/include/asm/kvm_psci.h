

#ifndef __ARM64_KVM_PSCI_H__
#define __ARM64_KVM_PSCI_H__

#define KVM_ARM_PSCI_0_1	1
#define KVM_ARM_PSCI_0_2	2

int kvm_psci_version(struct kvm_vcpu *vcpu);
int kvm_psci_call(struct kvm_vcpu *vcpu);

#endif 
