

#ifndef __ASM_STACKPROTECTOR_H
#define __ASM_STACKPROTECTOR_H

#include <linux/random.h>
#include <linux/version.h>

extern unsigned long __stack_chk_guard;


static __always_inline void boot_init_stack_canary(void)
{
	unsigned long canary;

	
	get_random_bytes(&canary, sizeof(canary));
	canary ^= LINUX_VERSION_CODE;

	current->stack_canary = canary;
	__stack_chk_guard = current->stack_canary;
}

#endif	
