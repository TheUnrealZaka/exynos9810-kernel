#ifndef __UAPI_CORESIGHT_STM_H_
#define __UAPI_CORESIGHT_STM_H_

#include <linux/const.h>

#define STM_FLAG_TIMESTAMPED   _BITUL(3)
#define STM_FLAG_GUARANTEED    _BITUL(7)


enum {
	STM_OPTION_GUARANTEED = 0,
	STM_OPTION_INVARIANT,
};

#endif
