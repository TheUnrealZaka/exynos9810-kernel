

#ifndef __LINUX_KVM_IRQFD_H
#define __LINUX_KVM_IRQFD_H

#include <linux/kvm_host.h>
#include <linux/poll.h>


struct kvm_kernel_irqfd_resampler {
	struct kvm *kvm;
	
	struct list_head list;
	struct kvm_irq_ack_notifier notifier;
	
	struct list_head link;
};

struct kvm_kernel_irqfd {
	
	struct kvm *kvm;
	wait_queue_t wait;
	
	struct kvm_kernel_irq_routing_entry irq_entry;
	seqcount_t irq_entry_sc;
	
	int gsi;
	struct work_struct inject;
	
	struct kvm_kernel_irqfd_resampler *resampler;
	
	struct eventfd_ctx *resamplefd;
	
	struct list_head resampler_link;
	
	struct eventfd_ctx *eventfd;
	struct list_head list;
	poll_table pt;
	struct work_struct shutdown;
	struct irq_bypass_consumer consumer;
	struct irq_bypass_producer *producer;
};

#endif 
