#ifndef _LINUX_GLOB_H
#define _LINUX_GLOB_H

#include <linux/types.h>	
#include <linux/compiler.h>	

bool __pure glob_match(char const *pat, char const *str);

#endif	
