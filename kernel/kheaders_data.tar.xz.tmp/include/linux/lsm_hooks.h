

#ifndef __LINUX_LSM_HOOKS_H
#define __LINUX_LSM_HOOKS_H

#include <linux/security.h>
#include <linux/init.h>
#include <linux/rculist.h>



union security_list_options {
	int (*binder_set_context_mgr)(const struct cred *mgr);
	int (*binder_transaction)(const struct cred *from,
					const struct cred *to);
	int (*binder_transfer_binder)(const struct cred *from,
					const struct cred *to);
	int (*binder_transfer_file)(const struct cred *from,
					const struct cred *to,
					struct file *file);

	int (*ptrace_access_check)(struct task_struct *child,
					unsigned int mode);
	int (*ptrace_traceme)(struct task_struct *parent);
	int (*capget)(struct task_struct *target, kernel_cap_t *effective,
			kernel_cap_t *inheritable, kernel_cap_t *permitted);
	int (*capset)(struct cred *new, const struct cred *old,
			const kernel_cap_t *effective,
			const kernel_cap_t *inheritable,
			const kernel_cap_t *permitted);
	int (*capable)(const struct cred *cred, struct user_namespace *ns,
			int cap, int audit);
	int (*quotactl)(int cmds, int type, int id, struct super_block *sb);
	int (*quota_on)(struct dentry *dentry);
	int (*syslog)(int type);
	int (*settime)(const struct timespec64 *ts, const struct timezone *tz);
	int (*vm_enough_memory)(struct mm_struct *mm, long pages);

	int (*bprm_set_creds)(struct linux_binprm *bprm);
	int (*bprm_check_security)(struct linux_binprm *bprm);
	int (*bprm_secureexec)(struct linux_binprm *bprm);
	void (*bprm_committing_creds)(struct linux_binprm *bprm);
	void (*bprm_committed_creds)(struct linux_binprm *bprm);

	int (*sb_alloc_security)(struct super_block *sb);
	void (*sb_free_security)(struct super_block *sb);
	int (*sb_copy_data)(char *orig, char *copy);
	int (*sb_remount)(struct super_block *sb, void *data);
	int (*sb_kern_mount)(struct super_block *sb, int flags, void *data);
	int (*sb_show_options)(struct seq_file *m, struct super_block *sb);
	int (*sb_statfs)(struct dentry *dentry);
	int (*sb_mount)(const char *dev_name, const struct path *path,
			const char *type, unsigned long flags, void *data);
	int (*sb_umount)(struct vfsmount *mnt, int flags);
	int (*sb_pivotroot)(const struct path *old_path, const struct path *new_path);
	int (*sb_set_mnt_opts)(struct super_block *sb,
				struct security_mnt_opts *opts,
				unsigned long kern_flags,
				unsigned long *set_kern_flags);
	int (*sb_clone_mnt_opts)(const struct super_block *oldsb,
					struct super_block *newsb);
	int (*sb_parse_opts_str)(char *options, struct security_mnt_opts *opts);
	int (*dentry_init_security)(struct dentry *dentry, int mode,
					const struct qstr *name, void **ctx,
					u32 *ctxlen);
	int (*dentry_create_files_as)(struct dentry *dentry, int mode,
					struct qstr *name,
					const struct cred *old,
					struct cred *new);


#ifdef CONFIG_SECURITY_PATH
	int (*path_unlink)(const struct path *dir, struct dentry *dentry);
	int (*path_mkdir)(const struct path *dir, struct dentry *dentry,
				umode_t mode);
	int (*path_rmdir)(const struct path *dir, struct dentry *dentry);
	int (*path_mknod)(const struct path *dir, struct dentry *dentry,
				umode_t mode, unsigned int dev);
	int (*path_truncate)(const struct path *path);
	int (*path_symlink)(const struct path *dir, struct dentry *dentry,
				const char *old_name);
	int (*path_link)(struct dentry *old_dentry, const struct path *new_dir,
				struct dentry *new_dentry);
	int (*path_rename)(const struct path *old_dir, struct dentry *old_dentry,
				const struct path *new_dir,
				struct dentry *new_dentry);
	int (*path_chmod)(const struct path *path, umode_t mode);
	int (*path_chown)(const struct path *path, kuid_t uid, kgid_t gid);
	int (*path_chroot)(const struct path *path);
#endif

	int (*inode_alloc_security)(struct inode *inode);
	void (*inode_free_security)(struct inode *inode);
	int (*inode_init_security)(struct inode *inode, struct inode *dir,
					const struct qstr *qstr,
					const char **name, void **value,
					size_t *len);
	int (*inode_create)(struct inode *dir, struct dentry *dentry,
				umode_t mode);
	int (*inode_link)(struct dentry *old_dentry, struct inode *dir,
				struct dentry *new_dentry);
	int (*inode_unlink)(struct inode *dir, struct dentry *dentry);
	int (*inode_symlink)(struct inode *dir, struct dentry *dentry,
				const char *old_name);
	int (*inode_mkdir)(struct inode *dir, struct dentry *dentry,
				umode_t mode);
	int (*inode_rmdir)(struct inode *dir, struct dentry *dentry);
	int (*inode_mknod)(struct inode *dir, struct dentry *dentry,
				umode_t mode, dev_t dev);
	int (*inode_rename)(struct inode *old_dir, struct dentry *old_dentry,
				struct inode *new_dir,
				struct dentry *new_dentry);
	int (*inode_readlink)(struct dentry *dentry);
	int (*inode_follow_link)(struct dentry *dentry, struct inode *inode,
				 bool rcu);
	int (*inode_permission)(struct inode *inode, int mask);
	int (*inode_setattr)(struct dentry *dentry, struct iattr *attr);
	int (*inode_getattr)(const struct path *path);
	int (*inode_setxattr)(struct dentry *dentry, const char *name,
				const void *value, size_t size, int flags);
	void (*inode_post_setxattr)(struct dentry *dentry, const char *name,
					const void *value, size_t size,
					int flags);
	int (*inode_getxattr)(struct dentry *dentry, const char *name);
	int (*inode_listxattr)(struct dentry *dentry);
	int (*inode_removexattr)(struct dentry *dentry, const char *name);
	int (*inode_need_killpriv)(struct dentry *dentry);
	int (*inode_killpriv)(struct dentry *dentry);
	int (*inode_getsecurity)(struct inode *inode, const char *name,
					void **buffer, bool alloc);
	int (*inode_setsecurity)(struct inode *inode, const char *name,
					const void *value, size_t size,
					int flags);
	int (*inode_listsecurity)(struct inode *inode, char *buffer,
					size_t buffer_size);
	void (*inode_getsecid)(struct inode *inode, u32 *secid);
	int (*inode_copy_up)(struct dentry *src, struct cred **new);
	int (*inode_copy_up_xattr)(const char *name);

	int (*file_permission)(struct file *file, int mask);
	int (*file_alloc_security)(struct file *file);
	void (*file_free_security)(struct file *file);
	int (*file_ioctl)(struct file *file, unsigned int cmd,
				unsigned long arg);
	int (*mmap_addr)(unsigned long addr);
	int (*mmap_file)(struct file *file, unsigned long reqprot,
				unsigned long prot, unsigned long flags);
	int (*file_mprotect)(struct vm_area_struct *vma, unsigned long reqprot,
				unsigned long prot);
	int (*file_lock)(struct file *file, unsigned int cmd);
	int (*file_fcntl)(struct file *file, unsigned int cmd,
				unsigned long arg);
	void (*file_set_fowner)(struct file *file);
	int (*file_send_sigiotask)(struct task_struct *tsk,
					struct fown_struct *fown, int sig);
	int (*file_receive)(struct file *file);
	int (*file_open)(struct file *file, const struct cred *cred);

	int (*task_create)(unsigned long clone_flags);
	void (*task_free)(struct task_struct *task);
	int (*cred_alloc_blank)(struct cred *cred, gfp_t gfp);
	void (*cred_free)(struct cred *cred);
	int (*cred_prepare)(struct cred *new, const struct cred *old,
				gfp_t gfp);
	void (*cred_transfer)(struct cred *new, const struct cred *old);
	int (*kernel_act_as)(struct cred *new, u32 secid);
	int (*kernel_create_files_as)(struct cred *new, struct inode *inode);
	int (*kernel_module_request)(char *kmod_name);
	int (*kernel_read_file)(struct file *file, enum kernel_read_file_id id);
	int (*kernel_post_read_file)(struct file *file, char *buf, loff_t size,
				     enum kernel_read_file_id id);
	int (*task_fix_setuid)(struct cred *new, const struct cred *old,
				int flags);
	int (*task_setpgid)(struct task_struct *p, pid_t pgid);
	int (*task_getpgid)(struct task_struct *p);
	int (*task_getsid)(struct task_struct *p);
	void (*task_getsecid)(struct task_struct *p, u32 *secid);
	int (*task_setnice)(struct task_struct *p, int nice);
	int (*task_setioprio)(struct task_struct *p, int ioprio);
	int (*task_getioprio)(struct task_struct *p);
	int (*task_setrlimit)(struct task_struct *p, unsigned int resource,
				struct rlimit *new_rlim);
	int (*task_setscheduler)(struct task_struct *p);
	int (*task_getscheduler)(struct task_struct *p);
	int (*task_movememory)(struct task_struct *p);
	int (*task_kill)(struct task_struct *p, struct siginfo *info,
				int sig, u32 secid);
	int (*task_prctl)(int option, unsigned long arg2, unsigned long arg3,
				unsigned long arg4, unsigned long arg5);
	void (*task_to_inode)(struct task_struct *p, struct inode *inode);

	int (*ipc_permission)(struct kern_ipc_perm *ipcp, short flag);
	void (*ipc_getsecid)(struct kern_ipc_perm *ipcp, u32 *secid);

	int (*msg_msg_alloc_security)(struct msg_msg *msg);
	void (*msg_msg_free_security)(struct msg_msg *msg);

	int (*msg_queue_alloc_security)(struct msg_queue *msq);
	void (*msg_queue_free_security)(struct msg_queue *msq);
	int (*msg_queue_associate)(struct msg_queue *msq, int msqflg);
	int (*msg_queue_msgctl)(struct msg_queue *msq, int cmd);
	int (*msg_queue_msgsnd)(struct msg_queue *msq, struct msg_msg *msg,
				int msqflg);
	int (*msg_queue_msgrcv)(struct msg_queue *msq, struct msg_msg *msg,
				struct task_struct *target, long type,
				int mode);

	int (*shm_alloc_security)(struct shmid_kernel *shp);
	void (*shm_free_security)(struct shmid_kernel *shp);
	int (*shm_associate)(struct shmid_kernel *shp, int shmflg);
	int (*shm_shmctl)(struct shmid_kernel *shp, int cmd);
	int (*shm_shmat)(struct shmid_kernel *shp, char __user *shmaddr,
				int shmflg);

	int (*sem_alloc_security)(struct sem_array *sma);
	void (*sem_free_security)(struct sem_array *sma);
	int (*sem_associate)(struct sem_array *sma, int semflg);
	int (*sem_semctl)(struct sem_array *sma, int cmd);
	int (*sem_semop)(struct sem_array *sma, struct sembuf *sops,
				unsigned nsops, int alter);

	int (*netlink_send)(struct sock *sk, struct sk_buff *skb);

	void (*d_instantiate)(struct dentry *dentry, struct inode *inode);

	int (*getprocattr)(struct task_struct *p, char *name, char **value);
	int (*setprocattr)(struct task_struct *p, char *name, void *value,
				size_t size);
	int (*ismaclabel)(const char *name);
	int (*secid_to_secctx)(u32 secid, char **secdata, u32 *seclen);
	int (*secctx_to_secid)(const char *secdata, u32 seclen, u32 *secid);
	void (*release_secctx)(char *secdata, u32 seclen);

	void (*inode_invalidate_secctx)(struct inode *inode);
	int (*inode_notifysecctx)(struct inode *inode, void *ctx, u32 ctxlen);
	int (*inode_setsecctx)(struct dentry *dentry, void *ctx, u32 ctxlen);
	int (*inode_getsecctx)(struct inode *inode, void **ctx, u32 *ctxlen);

#ifdef CONFIG_SECURITY_NETWORK
	int (*unix_stream_connect)(struct sock *sock, struct sock *other,
					struct sock *newsk);
	int (*unix_may_send)(struct socket *sock, struct socket *other);

	int (*socket_create)(int family, int type, int protocol, int kern);
	int (*socket_post_create)(struct socket *sock, int family, int type,
					int protocol, int kern);
	int (*socket_bind)(struct socket *sock, struct sockaddr *address,
				int addrlen);
	int (*socket_connect)(struct socket *sock, struct sockaddr *address,
				int addrlen);
	int (*socket_listen)(struct socket *sock, int backlog);
	int (*socket_accept)(struct socket *sock, struct socket *newsock);
	int (*socket_sendmsg)(struct socket *sock, struct msghdr *msg,
				int size);
	int (*socket_recvmsg)(struct socket *sock, struct msghdr *msg,
				int size, int flags);
	int (*socket_getsockname)(struct socket *sock);
	int (*socket_getpeername)(struct socket *sock);
	int (*socket_getsockopt)(struct socket *sock, int level, int optname);
	int (*socket_setsockopt)(struct socket *sock, int level, int optname);
	int (*socket_shutdown)(struct socket *sock, int how);
	int (*socket_sock_rcv_skb)(struct sock *sk, struct sk_buff *skb);
	int (*socket_getpeersec_stream)(struct socket *sock,
					char __user *optval,
					int __user *optlen, unsigned len);
	int (*socket_getpeersec_dgram)(struct socket *sock,
					struct sk_buff *skb, u32 *secid);
	int (*sk_alloc_security)(struct sock *sk, int family, gfp_t priority);
	void (*sk_free_security)(struct sock *sk);
	void (*sk_clone_security)(const struct sock *sk, struct sock *newsk);
	void (*sk_getsecid)(struct sock *sk, u32 *secid);
	void (*sock_graft)(struct sock *sk, struct socket *parent);
	int (*inet_conn_request)(struct sock *sk, struct sk_buff *skb,
					struct request_sock *req);
	void (*inet_csk_clone)(struct sock *newsk,
				const struct request_sock *req);
	void (*inet_conn_established)(struct sock *sk, struct sk_buff *skb);
	int (*secmark_relabel_packet)(u32 secid);
	void (*secmark_refcount_inc)(void);
	void (*secmark_refcount_dec)(void);
	void (*req_classify_flow)(const struct request_sock *req,
					struct flowi *fl);
	int (*tun_dev_alloc_security)(void **security);
	void (*tun_dev_free_security)(void *security);
	int (*tun_dev_create)(void);
	int (*tun_dev_attach_queue)(void *security);
	int (*tun_dev_attach)(struct sock *sk, void *security);
	int (*tun_dev_open)(void *security);
#endif	

#ifdef CONFIG_SECURITY_NETWORK_XFRM
	int (*xfrm_policy_alloc_security)(struct xfrm_sec_ctx **ctxp,
					  struct xfrm_user_sec_ctx *sec_ctx,
						gfp_t gfp);
	int (*xfrm_policy_clone_security)(struct xfrm_sec_ctx *old_ctx,
						struct xfrm_sec_ctx **new_ctx);
	void (*xfrm_policy_free_security)(struct xfrm_sec_ctx *ctx);
	int (*xfrm_policy_delete_security)(struct xfrm_sec_ctx *ctx);
	int (*xfrm_state_alloc)(struct xfrm_state *x,
				struct xfrm_user_sec_ctx *sec_ctx);
	int (*xfrm_state_alloc_acquire)(struct xfrm_state *x,
					struct xfrm_sec_ctx *polsec,
					u32 secid);
	void (*xfrm_state_free_security)(struct xfrm_state *x);
	int (*xfrm_state_delete_security)(struct xfrm_state *x);
	int (*xfrm_policy_lookup)(struct xfrm_sec_ctx *ctx, u32 fl_secid,
					u8 dir);
	int (*xfrm_state_pol_flow_match)(struct xfrm_state *x,
						struct xfrm_policy *xp,
						const struct flowi *fl);
	int (*xfrm_decode_session)(struct sk_buff *skb, u32 *secid, int ckall);
#endif	

	
#ifdef CONFIG_KEYS
	int (*key_alloc)(struct key *key, const struct cred *cred,
				unsigned long flags);
	void (*key_free)(struct key *key);
	int (*key_permission)(key_ref_t key_ref, const struct cred *cred,
				unsigned perm);
	int (*key_getsecurity)(struct key *key, char **_buffer);
#endif	

#ifdef CONFIG_AUDIT
	int (*audit_rule_init)(u32 field, u32 op, char *rulestr,
				void **lsmrule);
	int (*audit_rule_known)(struct audit_krule *krule);
	int (*audit_rule_match)(u32 secid, u32 field, u32 op, void *lsmrule,
				struct audit_context *actx);
	void (*audit_rule_free)(void *lsmrule);
#endif 

#ifdef CONFIG_BPF_SYSCALL
	int (*bpf)(int cmd, union bpf_attr *attr,
				 unsigned int size);
	int (*bpf_map)(struct bpf_map *map, fmode_t fmode);
	int (*bpf_prog)(struct bpf_prog *prog);
	int (*bpf_map_alloc_security)(struct bpf_map *map);
	void (*bpf_map_free_security)(struct bpf_map *map);
	int (*bpf_prog_alloc_security)(struct bpf_prog_aux *aux);
	void (*bpf_prog_free_security)(struct bpf_prog_aux *aux);
#endif 
#ifdef CONFIG_PERF_EVENTS
	int (*perf_event_open)(struct perf_event_attr *attr, int type);
	int (*perf_event_alloc)(struct perf_event *event);
	void (*perf_event_free)(struct perf_event *event);
	int (*perf_event_read)(struct perf_event *event);
	int (*perf_event_write)(struct perf_event *event);

#endif
};

struct security_hook_heads {
	struct list_head binder_set_context_mgr;
	struct list_head binder_transaction;
	struct list_head binder_transfer_binder;
	struct list_head binder_transfer_file;
	struct list_head ptrace_access_check;
	struct list_head ptrace_traceme;
	struct list_head capget;
	struct list_head capset;
	struct list_head capable;
	struct list_head quotactl;
	struct list_head quota_on;
	struct list_head syslog;
	struct list_head settime;
	struct list_head vm_enough_memory;
	struct list_head bprm_set_creds;
	struct list_head bprm_check_security;
	struct list_head bprm_secureexec;
	struct list_head bprm_committing_creds;
	struct list_head bprm_committed_creds;
	struct list_head sb_alloc_security;
	struct list_head sb_free_security;
	struct list_head sb_copy_data;
	struct list_head sb_remount;
	struct list_head sb_kern_mount;
	struct list_head sb_show_options;
	struct list_head sb_statfs;
	struct list_head sb_mount;
	struct list_head sb_umount;
	struct list_head sb_pivotroot;
	struct list_head sb_set_mnt_opts;
	struct list_head sb_clone_mnt_opts;
	struct list_head sb_parse_opts_str;
	struct list_head dentry_init_security;
	struct list_head dentry_create_files_as;
#ifdef CONFIG_SECURITY_PATH
	struct list_head path_unlink;
	struct list_head path_mkdir;
	struct list_head path_rmdir;
	struct list_head path_mknod;
	struct list_head path_truncate;
	struct list_head path_symlink;
	struct list_head path_link;
	struct list_head path_rename;
	struct list_head path_chmod;
	struct list_head path_chown;
	struct list_head path_chroot;
#endif
	struct list_head inode_alloc_security;
	struct list_head inode_free_security;
	struct list_head inode_init_security;
	struct list_head inode_create;
	struct list_head inode_link;
	struct list_head inode_unlink;
	struct list_head inode_symlink;
	struct list_head inode_mkdir;
	struct list_head inode_rmdir;
	struct list_head inode_mknod;
	struct list_head inode_rename;
	struct list_head inode_readlink;
	struct list_head inode_follow_link;
	struct list_head inode_permission;
	struct list_head inode_setattr;
	struct list_head inode_getattr;
	struct list_head inode_setxattr;
	struct list_head inode_post_setxattr;
	struct list_head inode_getxattr;
	struct list_head inode_listxattr;
	struct list_head inode_removexattr;
	struct list_head inode_need_killpriv;
	struct list_head inode_killpriv;
	struct list_head inode_getsecurity;
	struct list_head inode_setsecurity;
	struct list_head inode_listsecurity;
	struct list_head inode_getsecid;
	struct list_head inode_copy_up;
	struct list_head inode_copy_up_xattr;
	struct list_head file_permission;
	struct list_head file_alloc_security;
	struct list_head file_free_security;
	struct list_head file_ioctl;
	struct list_head mmap_addr;
	struct list_head mmap_file;
	struct list_head file_mprotect;
	struct list_head file_lock;
	struct list_head file_fcntl;
	struct list_head file_set_fowner;
	struct list_head file_send_sigiotask;
	struct list_head file_receive;
	struct list_head file_open;
	struct list_head task_create;
	struct list_head task_free;
	struct list_head cred_alloc_blank;
	struct list_head cred_free;
	struct list_head cred_prepare;
	struct list_head cred_transfer;
	struct list_head kernel_act_as;
	struct list_head kernel_create_files_as;
	struct list_head kernel_read_file;
	struct list_head kernel_post_read_file;
	struct list_head kernel_module_request;
	struct list_head task_fix_setuid;
	struct list_head task_setpgid;
	struct list_head task_getpgid;
	struct list_head task_getsid;
	struct list_head task_getsecid;
	struct list_head task_setnice;
	struct list_head task_setioprio;
	struct list_head task_getioprio;
	struct list_head task_setrlimit;
	struct list_head task_setscheduler;
	struct list_head task_getscheduler;
	struct list_head task_movememory;
	struct list_head task_kill;
	struct list_head task_prctl;
	struct list_head task_to_inode;
	struct list_head ipc_permission;
	struct list_head ipc_getsecid;
	struct list_head msg_msg_alloc_security;
	struct list_head msg_msg_free_security;
	struct list_head msg_queue_alloc_security;
	struct list_head msg_queue_free_security;
	struct list_head msg_queue_associate;
	struct list_head msg_queue_msgctl;
	struct list_head msg_queue_msgsnd;
	struct list_head msg_queue_msgrcv;
	struct list_head shm_alloc_security;
	struct list_head shm_free_security;
	struct list_head shm_associate;
	struct list_head shm_shmctl;
	struct list_head shm_shmat;
	struct list_head sem_alloc_security;
	struct list_head sem_free_security;
	struct list_head sem_associate;
	struct list_head sem_semctl;
	struct list_head sem_semop;
	struct list_head netlink_send;
	struct list_head d_instantiate;
	struct list_head getprocattr;
	struct list_head setprocattr;
	struct list_head ismaclabel;
	struct list_head secid_to_secctx;
	struct list_head secctx_to_secid;
	struct list_head release_secctx;
	struct list_head inode_invalidate_secctx;
	struct list_head inode_notifysecctx;
	struct list_head inode_setsecctx;
	struct list_head inode_getsecctx;
#ifdef CONFIG_SECURITY_NETWORK
	struct list_head unix_stream_connect;
	struct list_head unix_may_send;
	struct list_head socket_create;
	struct list_head socket_post_create;
	struct list_head socket_bind;
	struct list_head socket_connect;
	struct list_head socket_listen;
	struct list_head socket_accept;
	struct list_head socket_sendmsg;
	struct list_head socket_recvmsg;
	struct list_head socket_getsockname;
	struct list_head socket_getpeername;
	struct list_head socket_getsockopt;
	struct list_head socket_setsockopt;
	struct list_head socket_shutdown;
	struct list_head socket_sock_rcv_skb;
	struct list_head socket_getpeersec_stream;
	struct list_head socket_getpeersec_dgram;
	struct list_head sk_alloc_security;
	struct list_head sk_free_security;
	struct list_head sk_clone_security;
	struct list_head sk_getsecid;
	struct list_head sock_graft;
	struct list_head inet_conn_request;
	struct list_head inet_csk_clone;
	struct list_head inet_conn_established;
	struct list_head secmark_relabel_packet;
	struct list_head secmark_refcount_inc;
	struct list_head secmark_refcount_dec;
	struct list_head req_classify_flow;
	struct list_head tun_dev_alloc_security;
	struct list_head tun_dev_free_security;
	struct list_head tun_dev_create;
	struct list_head tun_dev_attach_queue;
	struct list_head tun_dev_attach;
	struct list_head tun_dev_open;
#endif	
#ifdef CONFIG_SECURITY_NETWORK_XFRM
	struct list_head xfrm_policy_alloc_security;
	struct list_head xfrm_policy_clone_security;
	struct list_head xfrm_policy_free_security;
	struct list_head xfrm_policy_delete_security;
	struct list_head xfrm_state_alloc;
	struct list_head xfrm_state_alloc_acquire;
	struct list_head xfrm_state_free_security;
	struct list_head xfrm_state_delete_security;
	struct list_head xfrm_policy_lookup;
	struct list_head xfrm_state_pol_flow_match;
	struct list_head xfrm_decode_session;
#endif	
#ifdef CONFIG_KEYS
	struct list_head key_alloc;
	struct list_head key_free;
	struct list_head key_permission;
	struct list_head key_getsecurity;
#endif	
#ifdef CONFIG_AUDIT
	struct list_head audit_rule_init;
	struct list_head audit_rule_known;
	struct list_head audit_rule_match;
	struct list_head audit_rule_free;
#endif 
#ifdef CONFIG_BPF_SYSCALL
	struct list_head bpf;
	struct list_head bpf_map;
	struct list_head bpf_prog;
	struct list_head bpf_map_alloc_security;
	struct list_head bpf_map_free_security;
	struct list_head bpf_prog_alloc_security;
	struct list_head bpf_prog_free_security;
#endif 
#ifdef CONFIG_PERF_EVENTS
	struct list_head perf_event_open;
	struct list_head perf_event_alloc;
	struct list_head perf_event_free;
	struct list_head perf_event_read;
	struct list_head perf_event_write;
#endif
};


struct security_hook_list {
	struct list_head		list;
	struct list_head		*head;
	union security_list_options	hook;
};


#define LSM_HOOK_INIT(HEAD, HOOK) \
	{ .head = &security_hook_heads.HEAD, .hook = { .HEAD = HOOK } }

extern struct security_hook_heads security_hook_heads;

static inline void security_add_hooks(struct security_hook_list *hooks,
				      int count)
{
	int i;

	for (i = 0; i < count; i++)
		list_add_tail_rcu(&hooks[i].list, hooks[i].head);
}

#ifdef CONFIG_SECURITY_SELINUX_DISABLE

static inline void security_delete_hooks(struct security_hook_list *hooks,
						int count)
{
	int i;

	for (i = 0; i < count; i++)
		list_del_rcu(&hooks[i].list);
}
#endif 

extern int __init security_module_enable(const char *module);
extern void __init capability_add_hooks(void);
#ifdef CONFIG_SECURITY_YAMA
extern void __init yama_add_hooks(void);
#else
static inline void __init yama_add_hooks(void) { }
#endif
#ifdef CONFIG_SECURITY_LOADPIN
void __init loadpin_add_hooks(void);
#else
static inline void loadpin_add_hooks(void) { };
#endif

#endif 
