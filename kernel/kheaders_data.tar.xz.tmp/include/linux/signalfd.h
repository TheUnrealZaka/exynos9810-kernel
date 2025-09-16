
#ifndef _LINUX_SIGNALFD_H
#define _LINUX_SIGNALFD_H

#include <uapi/linux/signalfd.h>


#ifdef CONFIG_SIGNALFD


static inline void signalfd_notify(struct task_struct *tsk, int sig)
{
	if (unlikely(waitqueue_active(&tsk->sighand->signalfd_wqh)))
		wake_up(&tsk->sighand->signalfd_wqh);
}

extern void signalfd_cleanup(struct sighand_struct *sighand);

#else 

static inline void signalfd_notify(struct task_struct *tsk, int sig) { }

static inline void signalfd_cleanup(struct sighand_struct *sighand) { }

#endif 

#endif 
