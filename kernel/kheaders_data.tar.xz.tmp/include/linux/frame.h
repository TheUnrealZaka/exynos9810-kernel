#ifndef _LINUX_FRAME_H
#define _LINUX_FRAME_H

#ifdef CONFIG_STACK_VALIDATION

#define STACK_FRAME_NON_STANDARD(func) \
	static void __used __section(.discard.func_stack_frame_non_standard) \
		*__func_stack_frame_non_standard_##func = func

#else 

#define STACK_FRAME_NON_STANDARD(func)

#endif 

#endif 
