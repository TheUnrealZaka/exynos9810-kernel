

#ifndef DLP_IOCTL_H_
#define DLP_IOCTL_H_

#include <linux/types.h>
#include <linux/ioctl.h>

#define DLP_DEV_PATH	"/dev/sdp_dlp"

#define __DLPIOC		0x78
#define MAX_EXT_LENGTH 1000

typedef struct _dlp_lock_set {
	int user_id;
} dlp_lock_set;

typedef struct _dlp_extension_set {
	int user_id;
	char extensions[MAX_EXT_LENGTH+1];
} dlp_extension_set;

#define DLP_LOCK_ENABLE		_IOW(__DLPIOC, 1, dlp_lock_set)
#define DLP_LOCK_DISABLE	_IOW(__DLPIOC, 2, dlp_lock_set)
#define DLP_EXTENSION_SET	_IOW(__DLPIOC, 3, dlp_extension_set)
#define DLP_FBE_SET			_IO(__DLPIOC, 4)

#endif 
