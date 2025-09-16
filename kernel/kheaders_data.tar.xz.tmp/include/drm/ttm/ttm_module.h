


#ifndef _TTM_MODULE_H_
#define _TTM_MODULE_H_

#include <linux/kernel.h>
struct kobject;

#define TTM_PFX "[TTM] "
extern struct kobject *ttm_get_kobj(void);

#endif 
