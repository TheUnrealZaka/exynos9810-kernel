#ifndef __LINUX_HYPEVISOR_H
#define __LINUX_HYPEVISOR_H



#ifdef CONFIG_HYPERVISOR_GUEST
#include <asm/hypervisor.h>
#else
static inline void hypervisor_pin_vcpu(int cpu)
{
}
#endif

#endif 
