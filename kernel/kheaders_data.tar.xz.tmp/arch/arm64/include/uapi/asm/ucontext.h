
#ifndef _UAPI__ASM_UCONTEXT_H
#define _UAPI__ASM_UCONTEXT_H

#include <linux/types.h>

struct ucontext {
	unsigned long	  uc_flags;
	struct ucontext	 *uc_link;
	stack_t		  uc_stack;
	sigset_t	  uc_sigmask;
	
	__u8		  __unused[1024 / 8 - sizeof(sigset_t)];
	
	struct sigcontext uc_mcontext;
};

#endif 
