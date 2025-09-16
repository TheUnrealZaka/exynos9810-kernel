#ifndef _LINUX_SEM_H
#define _LINUX_SEM_H

#include <linux/atomic.h>
#include <linux/rcupdate.h>
#include <linux/cache.h>
#include <uapi/linux/sem.h>

struct task_struct;


struct sem_array {
	struct kern_ipc_perm	____cacheline_aligned_in_smp
				sem_perm;	
	time_t			sem_ctime;	
	struct sem		*sem_base;	
	struct list_head	pending_alter;	
						
	struct list_head	pending_const;	
						
	struct list_head	list_id;	
	int			sem_nsems;	
	int			complex_count;	
	bool			complex_mode;	
};

#ifdef CONFIG_SYSVIPC

struct sysv_sem {
	struct sem_undo_list *undo_list;
};

extern int copy_semundo(unsigned long clone_flags, struct task_struct *tsk);
extern void exit_sem(struct task_struct *tsk);

#else

struct sysv_sem {
	
};

static inline int copy_semundo(unsigned long clone_flags, struct task_struct *tsk)
{
	return 0;
}

static inline void exit_sem(struct task_struct *tsk)
{
	return;
}
#endif

#endif 
