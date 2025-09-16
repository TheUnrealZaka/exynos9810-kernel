

#ifndef _LINUX_DEVPTS_FS_H
#define _LINUX_DEVPTS_FS_H

#include <linux/errno.h>

#ifdef CONFIG_UNIX98_PTYS

struct pts_fs_info;

struct pts_fs_info *devpts_acquire(struct file *);
void devpts_release(struct pts_fs_info *);

int devpts_new_index(struct pts_fs_info *);
void devpts_kill_index(struct pts_fs_info *, int);


struct dentry *devpts_pty_new(struct pts_fs_info *, int, void *);

void *devpts_get_priv(struct dentry *);

void devpts_pty_kill(struct dentry *);

#endif


#endif 
