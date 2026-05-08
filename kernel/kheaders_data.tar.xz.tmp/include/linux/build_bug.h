#ifndef _LINUX_BUILD_BUG_H
#define _LINUX_BUILD_BUG_H

#include <linux/compiler.h>

#ifdef __CHECKER__
#define __BUILD_BUG_ON_NOT_POWER_OF_2(n) (0)
#define BUILD_BUG_ON_NOT_POWER_OF_2(n) (0)
#define BUILD_BUG_ON_ZERO(e) (0)
#define BUILD_BUG_ON_NULL(e) ((void *)0)
#define BUILD_BUG_ON_INVALID(e) (0)
#define BUILD_BUG_ON_MSG(cond, msg) (0)
#define BUILD_BUG_ON(condition) (0)
#define BUILD_BUG() (0)
#else 


#define __BUILD_BUG_ON_NOT_POWER_OF_2(n)	\
	BUILD_BUG_ON(((n) & ((n) - 1)) != 0)
#define BUILD_BUG_ON_NOT_POWER_OF_2(n)			\
	BUILD_BUG_ON((n) == 0 || (((n) & ((n) - 1)) != 0))


#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:(-!!(e)); }))
#define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:(-!!(e)); }))


#define BUILD_BUG_ON_INVALID(e) ((void)(sizeof((__force long)(e))))


#define BUILD_BUG_ON_MSG(cond, msg) compiletime_assert(!(cond), msg)


#ifndef __OPTIMIZE__
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))
#else
#define BUILD_BUG_ON(condition) \
	BUILD_BUG_ON_MSG(condition, "BUILD_BUG_ON failed: " #condition)
#endif


#define BUILD_BUG() BUILD_BUG_ON_MSG(1, "BUILD_BUG failed")

#endif	

#endif	
