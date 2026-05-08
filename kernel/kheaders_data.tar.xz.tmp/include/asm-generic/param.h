#ifndef __ASM_GENERIC_PARAM_H
#define __ASM_GENERIC_PARAM_H

#include <uapi/asm-generic/param.h>

# undef HZ
# define HZ		CONFIG_HZ	
# define USER_HZ	100		
# define CLOCKS_PER_SEC	(USER_HZ)       
#endif 
