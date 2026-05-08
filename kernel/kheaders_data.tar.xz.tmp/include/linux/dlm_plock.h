
#ifndef __DLM_PLOCK_DOT_H__
#define __DLM_PLOCK_DOT_H__

#include <uapi/linux/dlm_plock.h>

int dlm_posix_lock(dlm_lockspace_t *lockspace, u64 number, struct file *file,
		int cmd, struct file_lock *fl);
int dlm_posix_unlock(dlm_lockspace_t *lockspace, u64 number, struct file *file,
		struct file_lock *fl);
int dlm_posix_get(dlm_lockspace_t *lockspace, u64 number, struct file *file,
		struct file_lock *fl);
#endif
