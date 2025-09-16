#ifndef _LINUX_FS_H
#define _LINUX_FS_H

#include <linux/linkage.h>
#include <linux/wait.h>
#include <linux/kdev_t.h>
#include <linux/dcache.h>
#include <linux/path.h>
#include <linux/stat.h>
#include <linux/cache.h>
#include <linux/list.h>
#include <linux/list_lru.h>
#include <linux/llist.h>
#include <linux/radix-tree.h>
#include <linux/rbtree.h>
#include <linux/init.h>
#include <linux/pid.h>
#include <linux/bug.h>
#include <linux/mutex.h>
#include <linux/rwsem.h>
#include <linux/mm_types.h>
#include <linux/capability.h>
#include <linux/semaphore.h>
#include <linux/fcntl.h>
#include <linux/fiemap.h>
#include <linux/rculist_bl.h>
#include <linux/atomic.h>
#include <linux/shrinker.h>
#include <linux/migrate_mode.h>
#include <linux/uidgid.h>
#include <linux/lockdep.h>
#include <linux/percpu-rwsem.h>
#include <linux/blk_types.h>
#include <linux/workqueue.h>
#include <linux/percpu-rwsem.h>
#include <linux/delayed_call.h>

#include <asm/byteorder.h>
#include <uapi/linux/fs.h>

struct backing_dev_info;
struct bdi_writeback;
struct export_operations;
struct hd_geometry;
struct iovec;
struct kiocb;
struct kobject;
struct pipe_inode_info;
struct poll_table_struct;
struct kstatfs;
struct vm_area_struct;
struct vfsmount;
struct cred;
struct swap_info_struct;
struct seq_file;
struct workqueue_struct;
struct iov_iter;
struct fscrypt_info;
struct fscrypt_operations;

extern void __init inode_init(void);
extern void __init inode_init_early(void);
extern void __init files_init(void);
extern void __init files_maxfiles_init(void);

extern struct files_stat_struct files_stat;
extern unsigned long get_max_files(void);
extern unsigned int sysctl_nr_open;
extern struct inodes_stat_t inodes_stat;
extern int leases_enable, lease_break_time;
extern int sysctl_protected_symlinks;
extern int sysctl_protected_hardlinks;
extern int sysctl_protected_fifos;
extern int sysctl_protected_regular;

struct buffer_head;
typedef int (get_block_t)(struct inode *inode, sector_t iblock,
			struct buffer_head *bh_result, int create);
typedef int (dio_iodone_t)(struct kiocb *iocb, loff_t offset,
			ssize_t bytes, void *private);

#define MAY_EXEC		0x00000001
#define MAY_WRITE		0x00000002
#define MAY_READ		0x00000004
#define MAY_APPEND		0x00000008
#define MAY_ACCESS		0x00000010
#define MAY_OPEN		0x00000020
#define MAY_CHDIR		0x00000040

#define MAY_NOT_BLOCK		0x00000080




#define FMODE_READ		((__force fmode_t)0x1)

#define FMODE_WRITE		((__force fmode_t)0x2)

#define FMODE_LSEEK		((__force fmode_t)0x4)

#define FMODE_PREAD		((__force fmode_t)0x8)

#define FMODE_PWRITE		((__force fmode_t)0x10)

#define FMODE_EXEC		((__force fmode_t)0x20)

#define FMODE_NDELAY		((__force fmode_t)0x40)

#define FMODE_EXCL		((__force fmode_t)0x80)

#define FMODE_WRITE_IOCTL	((__force fmode_t)0x100)

#define FMODE_32BITHASH         ((__force fmode_t)0x200)

#define FMODE_64BITHASH         ((__force fmode_t)0x400)


#define FMODE_NOCMTIME		((__force fmode_t)0x800)


#define FMODE_RANDOM		((__force fmode_t)0x1000)


#define FMODE_UNSIGNED_OFFSET	((__force fmode_t)0x2000)


#define FMODE_PATH		((__force fmode_t)0x4000)


#define FMODE_ATOMIC_POS	((__force fmode_t)0x8000)

#define FMODE_WRITER		((__force fmode_t)0x10000)

#define FMODE_CAN_READ          ((__force fmode_t)0x20000)

#define FMODE_CAN_WRITE         ((__force fmode_t)0x40000)


#define FMODE_STREAM		((__force fmode_t)0x200000)


#define FMODE_NONOTIFY		((__force fmode_t)0x4000000)


#define FMODE_NOWAIT		((__force fmode_t)0x8000000)


#define CHECK_IOVEC_ONLY -1


#define RW_MASK			REQ_OP_WRITE

#define READ			REQ_OP_READ
#define WRITE			REQ_OP_WRITE

#define READ_SYNC		REQ_SYNC
#define WRITE_SYNC		(REQ_SYNC | REQ_NOIDLE)
#define WRITE_ODIRECT		REQ_SYNC
#define WRITE_FLUSH		(REQ_SYNC | REQ_NOIDLE | REQ_PREFLUSH)
#define WRITE_FUA		(REQ_SYNC | REQ_NOIDLE | REQ_FUA)
#define WRITE_FLUSH_FUA		(REQ_SYNC | REQ_NOIDLE | REQ_PREFLUSH | REQ_FUA)


#define ATTR_MODE	(1 << 0)
#define ATTR_UID	(1 << 1)
#define ATTR_GID	(1 << 2)
#define ATTR_SIZE	(1 << 3)
#define ATTR_ATIME	(1 << 4)
#define ATTR_MTIME	(1 << 5)
#define ATTR_CTIME	(1 << 6)
#define ATTR_ATIME_SET	(1 << 7)
#define ATTR_MTIME_SET	(1 << 8)
#define ATTR_FORCE	(1 << 9) 
#define ATTR_ATTR_FLAG	(1 << 10)
#define ATTR_KILL_SUID	(1 << 11)
#define ATTR_KILL_SGID	(1 << 12)
#define ATTR_FILE	(1 << 13)
#define ATTR_KILL_PRIV	(1 << 14)
#define ATTR_OPEN	(1 << 15) 
#define ATTR_TIMES_SET	(1 << 16)
#define ATTR_TOUCH	(1 << 17)


#define WHITEOUT_MODE 0
#define WHITEOUT_DEV 0


struct iattr {
	unsigned int	ia_valid;
	umode_t		ia_mode;
	kuid_t		ia_uid;
	kgid_t		ia_gid;
	loff_t		ia_size;
	struct timespec	ia_atime;
	struct timespec	ia_mtime;
	struct timespec	ia_ctime;

	
	struct file	*ia_file;
};


#include <linux/quota.h>


#define FILESYSTEM_MAX_STACK_DEPTH 2



enum positive_aop_returns {
	AOP_WRITEPAGE_ACTIVATE	= 0x80000,
	AOP_TRUNCATED_PAGE	= 0x80001,
};

#define AOP_FLAG_UNINTERRUPTIBLE	0x0001 
#define AOP_FLAG_CONT_EXPAND		0x0002 
#define AOP_FLAG_NOFS			0x0004 


struct page;
struct address_space;
struct writeback_control;


enum rw_hint {
	WRITE_LIFE_NOT_SET	= 0,
	WRITE_LIFE_NONE		= RWH_WRITE_LIFE_NONE,
	WRITE_LIFE_SHORT	= RWH_WRITE_LIFE_SHORT,
	WRITE_LIFE_MEDIUM	= RWH_WRITE_LIFE_MEDIUM,
	WRITE_LIFE_LONG		= RWH_WRITE_LIFE_LONG,
	WRITE_LIFE_EXTREME	= RWH_WRITE_LIFE_EXTREME,
};

#define IOCB_EVENTFD		(1 << 0)
#define IOCB_APPEND		(1 << 1)
#define IOCB_DIRECT		(1 << 2)
#define IOCB_HIPRI		(1 << 3)
#define IOCB_DSYNC		(1 << 4)
#define IOCB_SYNC		(1 << 5)
#define IOCB_WRITE		(1 << 6)
#define IOCB_NOWAIT		(1 << 7)

struct kiocb {
	struct file		*ki_filp;
	loff_t			ki_pos;
	void (*ki_complete)(struct kiocb *iocb, long ret, long ret2);
	void			*private;
	int			ki_flags;
	enum rw_hint		ki_hint;
};

static inline bool is_sync_kiocb(struct kiocb *kiocb)
{
	return kiocb->ki_complete == NULL;
}

static inline int iocb_flags(struct file *file);

static inline void init_sync_kiocb(struct kiocb *kiocb, struct file *filp)
{
	*kiocb = (struct kiocb) {
		.ki_filp = filp,
		.ki_flags = iocb_flags(filp),
	};
}


typedef struct {
	size_t written;
	size_t count;
	union {
		char __user *buf;
		void *data;
	} arg;
	int error;
} read_descriptor_t;

typedef int (*read_actor_t)(read_descriptor_t *, struct page *,
		unsigned long, unsigned long);

struct address_space_operations {
	int (*writepage)(struct page *page, struct writeback_control *wbc);
	int (*readpage)(struct file *, struct page *);

	
	int (*writepages)(struct address_space *, struct writeback_control *);

	
	int (*set_page_dirty)(struct page *page);

	int (*readpages)(struct file *filp, struct address_space *mapping,
			struct list_head *pages, unsigned nr_pages);

	int (*write_begin)(struct file *, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned flags,
				struct page **pagep, void **fsdata);
	int (*write_end)(struct file *, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned copied,
				struct page *page, void *fsdata);

	
	sector_t (*bmap)(struct address_space *, sector_t);
	void (*invalidatepage) (struct page *, unsigned int, unsigned int);
	int (*releasepage) (struct page *, gfp_t);
	void (*freepage)(struct page *);
	ssize_t (*direct_IO)(struct kiocb *, struct iov_iter *iter);
	
	int (*migratepage) (struct address_space *,
			struct page *, struct page *, enum migrate_mode);
	bool (*isolate_page)(struct page *, isolate_mode_t);
	void (*putback_page)(struct page *);
	int (*launder_page) (struct page *);
	int (*is_partially_uptodate) (struct page *, unsigned long,
					unsigned long);
	void (*is_dirty_writeback) (struct page *, bool *, bool *);
	int (*error_remove_page)(struct address_space *, struct page *);

	
	int (*swap_activate)(struct swap_info_struct *sis, struct file *file,
				sector_t *span);
	void (*swap_deactivate)(struct file *file);
};

extern const struct address_space_operations empty_aops;


int pagecache_write_begin(struct file *, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned flags,
				struct page **pagep, void **fsdata);

int pagecache_write_end(struct file *, struct address_space *mapping,
				loff_t pos, unsigned len, unsigned copied,
				struct page *page, void *fsdata);

struct address_space {
	struct inode		*host;		
	struct radix_tree_root	page_tree;	
	spinlock_t		tree_lock;	
	atomic_t		i_mmap_writable;
	struct rb_root		i_mmap;		
	struct rw_semaphore	i_mmap_rwsem;	
	
	unsigned long		nrpages;	
	
	unsigned long		nrexceptional;
	pgoff_t			writeback_index;
	const struct address_space_operations *a_ops;	
	unsigned long		flags;		
	spinlock_t		private_lock;	
	gfp_t			gfp_mask;	
	struct list_head	private_list;	
	void			*private_data;	
#if defined(CONFIG_SDP) && !defined(CONFIG_FSCRYPT_SDP)
	int userid;
#endif
} __attribute__((aligned(sizeof(long))));
	
struct request_queue;

struct block_device {
	dev_t			bd_dev;  
	int			bd_openers;
	struct inode *		bd_inode;	
	struct super_block *	bd_super;
	struct mutex		bd_mutex;	
	void *			bd_claiming;
	void *			bd_holder;
	int			bd_holders;
	bool			bd_write_holder;
#ifdef CONFIG_SYSFS
	struct list_head	bd_holder_disks;
#endif
	struct block_device *	bd_contains;
	unsigned		bd_block_size;
	struct hd_struct *	bd_part;
	
	unsigned		bd_part_count;
	int			bd_invalidated;
	struct gendisk *	bd_disk;
	struct request_queue *  bd_queue;
	struct list_head	bd_list;
	
	unsigned long		bd_private;

	
	int			bd_fsfreeze_count;
	
	struct mutex		bd_fsfreeze_mutex;
};


#define PAGECACHE_TAG_DIRTY	0
#define PAGECACHE_TAG_WRITEBACK	1
#define PAGECACHE_TAG_TOWRITE	2

int mapping_tagged(struct address_space *mapping, int tag);

static inline void i_mmap_lock_write(struct address_space *mapping)
{
	down_write(&mapping->i_mmap_rwsem);
}

static inline void i_mmap_unlock_write(struct address_space *mapping)
{
	up_write(&mapping->i_mmap_rwsem);
}

static inline void i_mmap_lock_read(struct address_space *mapping)
{
	down_read(&mapping->i_mmap_rwsem);
}

static inline void i_mmap_unlock_read(struct address_space *mapping)
{
	up_read(&mapping->i_mmap_rwsem);
}


static inline int mapping_mapped(struct address_space *mapping)
{
	return	!RB_EMPTY_ROOT(&mapping->i_mmap);
}


static inline int mapping_writably_mapped(struct address_space *mapping)
{
	return atomic_read(&mapping->i_mmap_writable) > 0;
}

static inline int mapping_map_writable(struct address_space *mapping)
{
	return atomic_inc_unless_negative(&mapping->i_mmap_writable) ?
		0 : -EPERM;
}

static inline void mapping_unmap_writable(struct address_space *mapping)
{
	atomic_dec(&mapping->i_mmap_writable);
}

static inline int mapping_deny_writable(struct address_space *mapping)
{
	return atomic_dec_unless_positive(&mapping->i_mmap_writable) ?
		0 : -EBUSY;
}

static inline void mapping_allow_writable(struct address_space *mapping)
{
	atomic_inc(&mapping->i_mmap_writable);
}


#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
#include <linux/seqlock.h>
#define __NEED_I_SIZE_ORDERED
#define i_size_ordered_init(inode) seqcount_init(&inode->i_size_seqcount)
#else
#define i_size_ordered_init(inode) do { } while (0)
#endif

struct posix_acl;
#define ACL_NOT_CACHED ((void *)(-1))
#define ACL_DONT_CACHE ((void *)(-3))

static inline struct posix_acl *
uncached_acl_sentinel(struct task_struct *task)
{
	return (void *)task + 1;
}

static inline bool
is_uncached_acl(struct posix_acl *acl)
{
	return (long)acl & 1;
}

#define IOP_FASTPERM	0x0001
#define IOP_LOOKUP	0x0002
#define IOP_NOFOLLOW	0x0004
#define IOP_XATTR	0x0008


struct inode {
	umode_t			i_mode;
	unsigned short		i_opflags;
	kuid_t			i_uid;
	kgid_t			i_gid;
	unsigned int		i_flags;

#ifdef CONFIG_FS_POSIX_ACL
	struct posix_acl	*i_acl;
	struct posix_acl	*i_default_acl;
#endif

	const struct inode_operations	*i_op;
	struct super_block	*i_sb;
	struct address_space	*i_mapping;

#ifdef CONFIG_SECURITY
	void			*i_security;
#endif

	
	unsigned long		i_ino;
	
	union {
		const unsigned int i_nlink;
		unsigned int __i_nlink;
	};
	dev_t			i_rdev;
	loff_t			i_size;
	struct timespec		i_atime;
	struct timespec		i_mtime;
	struct timespec		i_ctime;
	spinlock_t		i_lock;	
	unsigned short          i_bytes;
	unsigned int		i_blkbits;
	enum rw_hint		i_write_hint;
	blkcnt_t		i_blocks;

#ifdef __NEED_I_SIZE_ORDERED
	seqcount_t		i_size_seqcount;
#endif

	
	unsigned long		i_state;
	struct rw_semaphore	i_rwsem;

	unsigned long		dirtied_when;	
	unsigned long		dirtied_time_when;

	struct hlist_node	i_hash;
	struct list_head	i_io_list;	
#ifdef CONFIG_CGROUP_WRITEBACK
	struct bdi_writeback	*i_wb;		

	
	int			i_wb_frn_winner;
	u16			i_wb_frn_avg_time;
	u16			i_wb_frn_history;
#endif
	struct list_head	i_lru;		
	struct list_head	i_sb_list;
	struct list_head	i_wb_list;	
	union {
		struct hlist_head	i_dentry;
		struct rcu_head		i_rcu;
	};
	u64			i_version;
	atomic64_t		i_sequence; 
	atomic_t		i_count;
	atomic_t		i_dio_count;
	atomic_t		i_writecount;
#ifdef CONFIG_IMA
	atomic_t		i_readcount; 
#endif
	const struct file_operations	*i_fop;	
	struct file_lock_context	*i_flctx;
	struct address_space	i_data;
	struct list_head	i_devices;
	union {
		struct pipe_inode_info	*i_pipe;
		struct block_device	*i_bdev;
		struct cdev		*i_cdev;
		char			*i_link;
		unsigned		i_dir_seq;
	};

	__u32			i_generation;

#ifdef CONFIG_FSNOTIFY
	__u32			i_fsnotify_mask; 
	struct hlist_head	i_fsnotify_marks;
#endif

#if IS_ENABLED(CONFIG_FS_ENCRYPTION)
	struct fscrypt_info	*i_crypt_info;
#endif

	void			*i_private; 
};

static inline unsigned int i_blocksize(const struct inode *node)
{
	return (1 << node->i_blkbits);
}

static inline int inode_unhashed(struct inode *inode)
{
	return hlist_unhashed(&inode->i_hash);
}


enum inode_i_mutex_lock_class
{
	I_MUTEX_NORMAL,
	I_MUTEX_PARENT,
	I_MUTEX_CHILD,
	I_MUTEX_XATTR,
	I_MUTEX_NONDIR2,
	I_MUTEX_PARENT2,
};

static inline void inode_lock(struct inode *inode)
{
	down_write(&inode->i_rwsem);
}

static inline void inode_unlock(struct inode *inode)
{
	up_write(&inode->i_rwsem);
}

static inline void inode_lock_shared(struct inode *inode)
{
	down_read(&inode->i_rwsem);
}

static inline void inode_unlock_shared(struct inode *inode)
{
	up_read(&inode->i_rwsem);
}

static inline int inode_trylock(struct inode *inode)
{
	return down_write_trylock(&inode->i_rwsem);
}

static inline int inode_trylock_shared(struct inode *inode)
{
	return down_read_trylock(&inode->i_rwsem);
}

static inline int inode_is_locked(struct inode *inode)
{
	return rwsem_is_locked(&inode->i_rwsem);
}

static inline void inode_lock_nested(struct inode *inode, unsigned subclass)
{
	down_write_nested(&inode->i_rwsem, subclass);
}

void lock_two_nondirectories(struct inode *, struct inode*);
void unlock_two_nondirectories(struct inode *, struct inode*);


static inline loff_t i_size_read(const struct inode *inode)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	loff_t i_size;
	unsigned int seq;

	do {
		seq = read_seqcount_begin(&inode->i_size_seqcount);
		i_size = inode->i_size;
	} while (read_seqcount_retry(&inode->i_size_seqcount, seq));
	return i_size;
#elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPT)
	loff_t i_size;

	preempt_disable();
	i_size = inode->i_size;
	preempt_enable();
	return i_size;
#else
	return inode->i_size;
#endif
}


static inline void i_size_write(struct inode *inode, loff_t i_size)
{
#if BITS_PER_LONG==32 && defined(CONFIG_SMP)
	preempt_disable();
	write_seqcount_begin(&inode->i_size_seqcount);
	inode->i_size = i_size;
	write_seqcount_end(&inode->i_size_seqcount);
	preempt_enable();
#elif BITS_PER_LONG==32 && defined(CONFIG_PREEMPT)
	preempt_disable();
	inode->i_size = i_size;
	preempt_enable();
#else
	inode->i_size = i_size;
#endif
}

static inline unsigned iminor(const struct inode *inode)
{
	return MINOR(inode->i_rdev);
}

static inline unsigned imajor(const struct inode *inode)
{
	return MAJOR(inode->i_rdev);
}

extern struct block_device *I_BDEV(struct inode *inode);

struct fown_struct {
	rwlock_t lock;          
	struct pid *pid;	
	enum pid_type pid_type;	
	kuid_t uid, euid;	
	int signum;		
};


struct file_ra_state {
	pgoff_t start;			
	unsigned int size;		
	unsigned int async_size;	

	unsigned int ra_pages;		
	unsigned int mmap_miss;		
	loff_t prev_pos;		
};


static inline int ra_has_index(struct file_ra_state *ra, pgoff_t index)
{
	return (index >= ra->start &&
		index <  ra->start + ra->size);
}

struct file {
	union {
		struct llist_node	fu_llist;
		struct rcu_head 	fu_rcuhead;
	} f_u;
	struct path		f_path;
	struct inode		*f_inode;	
	const struct file_operations	*f_op;

	
	spinlock_t		f_lock;
	atomic_long_t		f_count;
	unsigned int 		f_flags;
	fmode_t			f_mode;
	struct mutex		f_pos_lock;
	loff_t			f_pos;
	struct fown_struct	f_owner;
	const struct cred	*f_cred;
	struct file_ra_state	f_ra;

	u64			f_version;
#ifdef CONFIG_SECURITY
	void			*f_security;
#endif
	
	void			*private_data;

#ifdef CONFIG_EPOLL
	
	struct list_head	f_ep_links;
	struct list_head	f_tfile_llink;
#endif 
	struct address_space	*f_mapping;
#if defined(CONFIG_FIVE_PA_FEATURE) || defined(CONFIG_PROCA)
	void *f_signature;
#endif
} __attribute__((aligned(4)));	

struct file_handle {
	__u32 handle_bytes;
	int handle_type;
	
	unsigned char f_handle[];
};

static inline struct file *get_file(struct file *f)
{
	atomic_long_inc(&f->f_count);
	return f;
}
#define get_file_rcu_many(x, cnt)	\
	atomic_long_add_unless(&(x)->f_count, (cnt), 0)
#define get_file_rcu(x) get_file_rcu_many((x), 1)
#define fput_atomic(x)	atomic_long_add_unless(&(x)->f_count, -1, 1)
#define file_count(x)	atomic_long_read(&(x)->f_count)

#define	MAX_NON_LFS	((1UL<<31) - 1)

 
#if BITS_PER_LONG==32
#define MAX_LFS_FILESIZE	((loff_t)ULONG_MAX << PAGE_SHIFT)
#elif BITS_PER_LONG==64
#define MAX_LFS_FILESIZE 	((loff_t)LLONG_MAX)
#endif

#define FL_POSIX	1
#define FL_FLOCK	2
#define FL_DELEG	4	
#define FL_ACCESS	8	
#define FL_EXISTS	16	
#define FL_LEASE	32	
#define FL_CLOSE	64	
#define FL_SLEEP	128	
#define FL_DOWNGRADE_PENDING	256 
#define FL_UNLOCK_PENDING	512 
#define FL_OFDLCK	1024	
#define FL_LAYOUT	2048	


#define FILE_LOCK_DEFERRED 1


typedef void *fl_owner_t;

struct file_lock;

struct file_lock_operations {
	void (*fl_copy_lock)(struct file_lock *, struct file_lock *);
	void (*fl_release_private)(struct file_lock *);
};

struct lock_manager_operations {
	int (*lm_compare_owner)(struct file_lock *, struct file_lock *);
	unsigned long (*lm_owner_key)(struct file_lock *);
	fl_owner_t (*lm_get_owner)(fl_owner_t);
	void (*lm_put_owner)(fl_owner_t);
	void (*lm_notify)(struct file_lock *);	
	int (*lm_grant)(struct file_lock *, int);
	bool (*lm_break)(struct file_lock *);
	int (*lm_change)(struct file_lock *, int, struct list_head *);
	void (*lm_setup)(struct file_lock *, void **);
};

struct lock_manager {
	struct list_head list;
	
	bool block_opens;
};

struct net;
void locks_start_grace(struct net *, struct lock_manager *);
void locks_end_grace(struct lock_manager *);
int locks_in_grace(struct net *);
int opens_in_grace(struct net *);


#include <linux/nfs_fs_i.h>


struct file_lock {
	struct file_lock *fl_next;	
	struct list_head fl_list;	
	struct hlist_node fl_link;	
	struct list_head fl_block;	
	fl_owner_t fl_owner;
	unsigned int fl_flags;
	unsigned char fl_type;
	unsigned int fl_pid;
	int fl_link_cpu;		
	struct pid *fl_nspid;
	wait_queue_head_t fl_wait;
	struct file *fl_file;
	loff_t fl_start;
	loff_t fl_end;

	struct fasync_struct *	fl_fasync; 
	
	unsigned long fl_break_time;
	unsigned long fl_downgrade_time;

	const struct file_lock_operations *fl_ops;	
	const struct lock_manager_operations *fl_lmops;	
	union {
		struct nfs_lock_info	nfs_fl;
		struct nfs4_lock_info	nfs4_fl;
		struct {
			struct list_head link;	
			int state;		
		} afs;
	} fl_u;
};

struct file_lock_context {
	spinlock_t		flc_lock;
	struct list_head	flc_flock;
	struct list_head	flc_posix;
	struct list_head	flc_lease;
};


#ifndef OFFSET_MAX
#define INT_LIMIT(x)	(~((x)1 << (sizeof(x)*8 - 1)))
#define OFFSET_MAX	INT_LIMIT(loff_t)
#define OFFT_OFFSET_MAX	INT_LIMIT(off_t)
#endif

extern void send_sigio(struct fown_struct *fown, int fd, int band);


static inline struct inode *locks_inode(const struct file *f)
{
	return f->f_path.dentry->d_inode;
}

#ifdef CONFIG_FILE_LOCKING
extern int fcntl_getlk(struct file *, unsigned int, struct flock __user *);
extern int fcntl_setlk(unsigned int, struct file *, unsigned int,
			struct flock __user *);

#if BITS_PER_LONG == 32
extern int fcntl_getlk64(struct file *, unsigned int, struct flock64 __user *);
extern int fcntl_setlk64(unsigned int, struct file *, unsigned int,
			struct flock64 __user *);
#endif

extern int fcntl_setlease(unsigned int fd, struct file *filp, long arg);
extern int fcntl_getlease(struct file *filp);


void locks_free_lock_context(struct inode *inode);
void locks_free_lock(struct file_lock *fl);
extern void locks_init_lock(struct file_lock *);
extern struct file_lock * locks_alloc_lock(void);
extern void locks_copy_lock(struct file_lock *, struct file_lock *);
extern void locks_copy_conflock(struct file_lock *, struct file_lock *);
extern void locks_remove_posix(struct file *, fl_owner_t);
extern void locks_remove_file(struct file *);
extern void locks_release_private(struct file_lock *);
extern void posix_test_lock(struct file *, struct file_lock *);
extern int posix_lock_file(struct file *, struct file_lock *, struct file_lock *);
extern int posix_unblock_lock(struct file_lock *);
extern int vfs_test_lock(struct file *, struct file_lock *);
extern int vfs_lock_file(struct file *, unsigned int, struct file_lock *, struct file_lock *);
extern int vfs_cancel_lock(struct file *filp, struct file_lock *fl);
extern int locks_lock_inode_wait(struct inode *inode, struct file_lock *fl);
extern int __break_lease(struct inode *inode, unsigned int flags, unsigned int type);
extern void lease_get_mtime(struct inode *, struct timespec *time);
extern int generic_setlease(struct file *, long, struct file_lock **, void **priv);
extern int vfs_setlease(struct file *, long, struct file_lock **, void **);
extern int lease_modify(struct file_lock *, int, struct list_head *);
struct files_struct;
extern void show_fd_locks(struct seq_file *f,
			 struct file *filp, struct files_struct *files);
#else 
static inline int fcntl_getlk(struct file *file, unsigned int cmd,
			      struct flock __user *user)
{
	return -EINVAL;
}

static inline int fcntl_setlk(unsigned int fd, struct file *file,
			      unsigned int cmd, struct flock __user *user)
{
	return -EACCES;
}

#if BITS_PER_LONG == 32
static inline int fcntl_getlk64(struct file *file, unsigned int cmd,
				struct flock64 __user *user)
{
	return -EINVAL;
}

static inline int fcntl_setlk64(unsigned int fd, struct file *file,
				unsigned int cmd, struct flock64 __user *user)
{
	return -EACCES;
}
#endif
static inline int fcntl_setlease(unsigned int fd, struct file *filp, long arg)
{
	return -EINVAL;
}

static inline int fcntl_getlease(struct file *filp)
{
	return F_UNLCK;
}

static inline void
locks_free_lock_context(struct inode *inode)
{
}

static inline void locks_init_lock(struct file_lock *fl)
{
	return;
}

static inline void locks_copy_conflock(struct file_lock *new, struct file_lock *fl)
{
	return;
}

static inline void locks_copy_lock(struct file_lock *new, struct file_lock *fl)
{
	return;
}

static inline void locks_remove_posix(struct file *filp, fl_owner_t owner)
{
	return;
}

static inline void locks_remove_file(struct file *filp)
{
	return;
}

static inline void posix_test_lock(struct file *filp, struct file_lock *fl)
{
	return;
}

static inline int posix_lock_file(struct file *filp, struct file_lock *fl,
				  struct file_lock *conflock)
{
	return -ENOLCK;
}

static inline int posix_unblock_lock(struct file_lock *waiter)
{
	return -ENOENT;
}

static inline int vfs_test_lock(struct file *filp, struct file_lock *fl)
{
	return 0;
}

static inline int vfs_lock_file(struct file *filp, unsigned int cmd,
				struct file_lock *fl, struct file_lock *conf)
{
	return -ENOLCK;
}

static inline int vfs_cancel_lock(struct file *filp, struct file_lock *fl)
{
	return 0;
}

static inline int locks_lock_inode_wait(struct inode *inode, struct file_lock *fl)
{
	return -ENOLCK;
}

static inline int __break_lease(struct inode *inode, unsigned int mode, unsigned int type)
{
	return 0;
}

static inline void lease_get_mtime(struct inode *inode, struct timespec *time)
{
	return;
}

static inline int generic_setlease(struct file *filp, long arg,
				    struct file_lock **flp, void **priv)
{
	return -EINVAL;
}

static inline int vfs_setlease(struct file *filp, long arg,
			       struct file_lock **lease, void **priv)
{
	return -EINVAL;
}

static inline int lease_modify(struct file_lock *fl, int arg,
			       struct list_head *dispose)
{
	return -EINVAL;
}

struct files_struct;
static inline void show_fd_locks(struct seq_file *f,
			struct file *filp, struct files_struct *files) {}
#endif 

static inline struct inode *file_inode(const struct file *f)
{
	return f->f_inode;
}

static inline struct dentry *file_dentry(const struct file *file)
{
	return d_real(file->f_path.dentry, file_inode(file), 0);
}

static inline int locks_lock_file_wait(struct file *filp, struct file_lock *fl)
{
	return locks_lock_inode_wait(locks_inode(filp), fl);
}

struct fasync_struct {
	spinlock_t		fa_lock;
	int			magic;
	int			fa_fd;
	struct fasync_struct	*fa_next; 
	struct file		*fa_file;
	struct rcu_head		fa_rcu;
};

#define FASYNC_MAGIC 0x4601


extern int fasync_helper(int, struct file *, int, struct fasync_struct **);
extern struct fasync_struct *fasync_insert_entry(int, struct file *, struct fasync_struct **, struct fasync_struct *);
extern int fasync_remove_entry(struct file *, struct fasync_struct **);
extern struct fasync_struct *fasync_alloc(void);
extern void fasync_free(struct fasync_struct *);


extern void kill_fasync(struct fasync_struct **, int, int);

extern void __f_setown(struct file *filp, struct pid *, enum pid_type, int force);
extern void f_setown(struct file *filp, unsigned long arg, int force);
extern void f_delown(struct file *filp);
extern pid_t f_getown(struct file *filp);
extern int send_sigurg(struct fown_struct *fown);

struct mm_struct;



#define MNT_FORCE	0x00000001	
#define MNT_DETACH	0x00000002	
#define MNT_EXPIRE	0x00000004	
#define UMOUNT_NOFOLLOW	0x00000008	
#define UMOUNT_UNUSED	0x80000000	


#define SB_I_CGROUPWB	0x00000001	
#define SB_I_NOEXEC	0x00000002	
#define SB_I_NODEV	0x00000004	
#define SB_I_MULTIROOT	0x00000008	


#define SB_I_USERNS_VISIBLE		0x00000010 


enum {
	SB_UNFROZEN = 0,		
	SB_FREEZE_WRITE	= 1,		
	SB_FREEZE_PAGEFAULT = 2,	
	SB_FREEZE_FS = 3,		
	SB_FREEZE_COMPLETE = 4,		
};

#define SB_FREEZE_LEVELS (SB_FREEZE_COMPLETE - 1)

struct sb_writers {
	int				frozen;		
	wait_queue_head_t		wait_unfrozen;	
	struct percpu_rw_semaphore	rw_sem[SB_FREEZE_LEVELS];
};

struct super_block {
	struct list_head	s_list;		
	dev_t			s_dev;		
	unsigned char		s_blocksize_bits;
	unsigned long		s_blocksize;
	loff_t			s_maxbytes;	
	struct file_system_type	*s_type;
	const struct super_operations	*s_op;
	const struct dquot_operations	*dq_op;
	const struct quotactl_ops	*s_qcop;
	const struct export_operations *s_export_op;
	unsigned long		s_flags;
	unsigned long		s_iflags;	
	unsigned long		s_magic;
	struct dentry		*s_root;
	struct rw_semaphore	s_umount;
	int			s_count;
	atomic_t		s_active;
#ifdef CONFIG_SECURITY
	void                    *s_security;
#endif
	const struct xattr_handler **s_xattr;

	const struct fscrypt_operations	*s_cop;

	struct hlist_bl_head	s_anon;		
	struct list_head	s_mounts;	
	struct block_device	*s_bdev;
	struct backing_dev_info *s_bdi;
	struct mtd_info		*s_mtd;
	struct hlist_node	s_instances;
	unsigned int		s_quota_types;	
	struct quota_info	s_dquot;	

	struct sb_writers	s_writers;

	char s_id[32];				
	u8 s_uuid[16];				

	void 			*s_fs_info;	
	unsigned int		s_max_links;
	fmode_t			s_mode;

	
	u32		   s_time_gran;

	
	struct mutex s_vfs_rename_mutex;	

	
	char *s_subtype;

	
	char __rcu *s_options;
	const struct dentry_operations *s_d_op; 

	
	int cleancache_poolid;

	struct shrinker s_shrink;	

	
	atomic_long_t s_remove_count;

	
	int s_readonly_remount;

	
	struct workqueue_struct *s_dio_done_wq;
	struct hlist_head s_pins;

	
	struct user_namespace *s_user_ns;

	
	struct list_lru		s_dentry_lru ____cacheline_aligned_in_smp;
	struct list_lru		s_inode_lru ____cacheline_aligned_in_smp;
	struct rcu_head		rcu;
	struct work_struct	destroy_work;

	struct mutex		s_sync_lock;	

	
	int s_stack_depth;

	
	spinlock_t		s_inode_list_lock ____cacheline_aligned_in_smp;
	struct list_head	s_inodes;	

	spinlock_t		s_inode_wblist_lock;
	struct list_head	s_inodes_wb;	
};


static inline uid_t i_uid_read(const struct inode *inode)
{
	return from_kuid(inode->i_sb->s_user_ns, inode->i_uid);
}

static inline gid_t i_gid_read(const struct inode *inode)
{
	return from_kgid(inode->i_sb->s_user_ns, inode->i_gid);
}

static inline void i_uid_write(struct inode *inode, uid_t uid)
{
	inode->i_uid = make_kuid(inode->i_sb->s_user_ns, uid);
}

static inline void i_gid_write(struct inode *inode, gid_t gid)
{
	inode->i_gid = make_kgid(inode->i_sb->s_user_ns, gid);
}

extern struct timespec current_fs_time(struct super_block *sb);
extern struct timespec current_time(struct inode *inode);



void __sb_end_write(struct super_block *sb, int level);
int __sb_start_write(struct super_block *sb, int level, bool wait);

#define __sb_writers_acquired(sb, lev)	\
	percpu_rwsem_acquire(&(sb)->s_writers.rw_sem[(lev)-1], 1, _THIS_IP_)
#define __sb_writers_release(sb, lev)	\
	percpu_rwsem_release(&(sb)->s_writers.rw_sem[(lev)-1], 1, _THIS_IP_)


static inline void sb_end_write(struct super_block *sb)
{
	__sb_end_write(sb, SB_FREEZE_WRITE);
}


static inline void sb_end_pagefault(struct super_block *sb)
{
	__sb_end_write(sb, SB_FREEZE_PAGEFAULT);
}


static inline void sb_end_intwrite(struct super_block *sb)
{
	__sb_end_write(sb, SB_FREEZE_FS);
}


static inline void sb_start_write(struct super_block *sb)
{
	__sb_start_write(sb, SB_FREEZE_WRITE, true);
}

static inline int sb_start_write_trylock(struct super_block *sb)
{
	return __sb_start_write(sb, SB_FREEZE_WRITE, false);
}


static inline void sb_start_pagefault(struct super_block *sb)
{
	__sb_start_write(sb, SB_FREEZE_PAGEFAULT, true);
}


static inline void sb_start_intwrite(struct super_block *sb)
{
	__sb_start_write(sb, SB_FREEZE_FS, true);
}


extern bool inode_owner_or_capable(const struct inode *inode);


extern int vfs_create(struct inode *, struct dentry *, umode_t, bool);
extern int vfs_create2(struct vfsmount *, struct inode *, struct dentry *, umode_t, bool);
extern int vfs_mkdir(struct inode *, struct dentry *, umode_t);
extern int vfs_mkdir2(struct vfsmount *, struct inode *, struct dentry *, umode_t);
extern int vfs_mknod(struct inode *, struct dentry *, umode_t, dev_t);
extern int vfs_mknod2(struct vfsmount *, struct inode *, struct dentry *, umode_t, dev_t);
extern int vfs_symlink(struct inode *, struct dentry *, const char *);
extern int vfs_symlink2(struct vfsmount *, struct inode *, struct dentry *, const char *);
extern int vfs_link(struct dentry *, struct inode *, struct dentry *, struct inode **);
extern int vfs_link2(struct vfsmount *, struct dentry *, struct inode *, struct dentry *, struct inode **);
extern int vfs_rmdir(struct inode *, struct dentry *);
extern int vfs_rmdir2(struct vfsmount *, struct inode *, struct dentry *);
extern int vfs_unlink(struct inode *, struct dentry *, struct inode **);
extern int vfs_unlink2(struct vfsmount *, struct inode *, struct dentry *, struct inode **);
extern int vfs_rename(struct inode *, struct dentry *, struct inode *, struct dentry *, struct inode **, unsigned int);
extern int vfs_rename2(struct vfsmount *, struct inode *, struct dentry *, struct inode *, struct dentry *, struct inode **, unsigned int);
extern int vfs_whiteout(struct inode *, struct dentry *);


extern void inode_init_owner(struct inode *inode, const struct inode *dir,
			umode_t mode);
extern bool may_open_dev(const struct path *path);

struct fiemap_extent_info {
	unsigned int fi_flags;		
	unsigned int fi_extents_mapped;	
	unsigned int fi_extents_max;	
	struct fiemap_extent __user *fi_extents_start; 
};
int fiemap_fill_next_extent(struct fiemap_extent_info *info, u64 logical,
			    u64 phys, u64 len, u32 flags);
int fiemap_check_flags(struct fiemap_extent_info *fieinfo, u32 fs_flags);
int fiemap_check_ranges(struct super_block *sb,
			       u64 start, u64 len, u64 *new_len);


#define DT_UNKNOWN	0
#define DT_FIFO		1
#define DT_CHR		2
#define DT_DIR		4
#define DT_BLK		6
#define DT_REG		8
#define DT_LNK		10
#define DT_SOCK		12
#define DT_WHT		14


struct dir_context;
typedef int (*filldir_t)(struct dir_context *, const char *, int, loff_t, u64,
			 unsigned);

struct dir_context {
	const filldir_t actor;
	loff_t pos;
};

struct block_device_operations;


#define HAVE_COMPAT_IOCTL 1
#define HAVE_UNLOCKED_IOCTL 1


#define NOMMU_MAP_COPY		0x00000001
#define NOMMU_MAP_DIRECT	0x00000008
#define NOMMU_MAP_READ		VM_MAYREAD
#define NOMMU_MAP_WRITE		VM_MAYWRITE
#define NOMMU_MAP_EXEC		VM_MAYEXEC

#define NOMMU_VMFLAGS \
	(NOMMU_MAP_READ | NOMMU_MAP_WRITE | NOMMU_MAP_EXEC)


struct iov_iter;

struct file_operations {
	struct module *owner;
	loff_t (*llseek) (struct file *, loff_t, int);
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
	int (*iterate) (struct file *, struct dir_context *);
	int (*iterate_shared) (struct file *, struct dir_context *);
	unsigned int (*poll) (struct file *, struct poll_table_struct *);
	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
	int (*mmap) (struct file *, struct vm_area_struct *);
	int (*open) (struct inode *, struct file *);
	int (*flush) (struct file *, fl_owner_t id);
	int (*release) (struct inode *, struct file *);
	int (*fsync) (struct file *, loff_t, loff_t, int datasync);
	int (*fasync) (int, struct file *, int);
	int (*lock) (struct file *, int, struct file_lock *);
	ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
	unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
	int (*check_flags)(int);
	int (*flock) (struct file *, int, struct file_lock *);
	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
	ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
	int (*setlease)(struct file *, long, struct file_lock **, void **);
	long (*fallocate)(struct file *file, int mode, loff_t offset,
			  loff_t len);
	void (*show_fdinfo)(struct seq_file *m, struct file *f);
#ifndef CONFIG_MMU
	unsigned (*mmap_capabilities)(struct file *);
#endif
	ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
			loff_t, size_t, unsigned int);
	int (*clone_file_range)(struct file *, loff_t, struct file *, loff_t,
			u64);
	ssize_t (*dedupe_file_range)(struct file *, u64, u64, struct file *,
			u64);
};

struct inode_operations {
	struct dentry * (*lookup) (struct inode *,struct dentry *, unsigned int);
	const char * (*get_link) (struct dentry *, struct inode *, struct delayed_call *);
	int (*permission) (struct inode *, int);
	int (*permission2) (struct vfsmount *, struct inode *, int);
	struct posix_acl * (*get_acl)(struct inode *, int);

	int (*readlink) (struct dentry *, char __user *,int);

	int (*create) (struct inode *,struct dentry *, umode_t, bool);
	int (*link) (struct dentry *,struct inode *,struct dentry *);
	int (*unlink) (struct inode *,struct dentry *);
	int (*symlink) (struct inode *,struct dentry *,const char *);
	int (*mkdir) (struct inode *,struct dentry *,umode_t);
	int (*rmdir) (struct inode *,struct dentry *);
	int (*mknod) (struct inode *,struct dentry *,umode_t,dev_t);
	int (*rename) (struct inode *, struct dentry *,
			struct inode *, struct dentry *, unsigned int);
	int (*setattr) (struct dentry *, struct iattr *);
	int (*setattr2) (struct vfsmount *, struct dentry *, struct iattr *);
	int (*getattr) (struct vfsmount *mnt, struct dentry *, struct kstat *);
	ssize_t (*listxattr) (struct dentry *, char *, size_t);
	int (*fiemap)(struct inode *, struct fiemap_extent_info *, u64 start,
		      u64 len);
	int (*update_time)(struct inode *, struct timespec *, int);
	int (*atomic_open)(struct inode *, struct dentry *,
			   struct file *, unsigned open_flag,
			   umode_t create_mode, int *opened);
	int (*tmpfile) (struct inode *, struct dentry *, umode_t);
	int (*set_acl)(struct inode *, struct posix_acl *, int);
} ____cacheline_aligned;

static inline ssize_t call_read_iter(struct file *file, struct kiocb *kio,
				     struct iov_iter *iter)
{
	return file->f_op->read_iter(kio, iter);
}

static inline ssize_t call_write_iter(struct file *file, struct kiocb *kio,
				      struct iov_iter *iter)
{
	return file->f_op->write_iter(kio, iter);
}

static inline int call_mmap(struct file *file, struct vm_area_struct *vma)
{
	return file->f_op->mmap(file, vma);
}

ssize_t rw_copy_check_uvector(int type, const struct iovec __user * uvector,
			      unsigned long nr_segs, unsigned long fast_segs,
			      struct iovec *fast_pointer,
			      struct iovec **ret_pointer);

extern ssize_t __vfs_read(struct file *, char __user *, size_t, loff_t *);
extern ssize_t __vfs_write(struct file *, const char __user *, size_t, loff_t *);
extern ssize_t vfs_read(struct file *, char __user *, size_t, loff_t *);
extern ssize_t vfs_write(struct file *, const char __user *, size_t, loff_t *);
extern ssize_t vfs_readv(struct file *, const struct iovec __user *,
		unsigned long, loff_t *, int);
extern ssize_t vfs_writev(struct file *, const struct iovec __user *,
		unsigned long, loff_t *, int);
extern ssize_t vfs_copy_file_range(struct file *, loff_t , struct file *,
				   loff_t, size_t, unsigned int);
extern int vfs_clone_file_range(struct file *file_in, loff_t pos_in,
		struct file *file_out, loff_t pos_out, u64 len);
extern int vfs_dedupe_file_range(struct file *file,
				 struct file_dedupe_range *same);

struct super_operations {
   	struct inode *(*alloc_inode)(struct super_block *sb);
	void (*destroy_inode)(struct inode *);

   	void (*dirty_inode) (struct inode *, int flags);
	int (*write_inode) (struct inode *, struct writeback_control *wbc);
	int (*drop_inode) (struct inode *);
	void (*evict_inode) (struct inode *);
	void (*put_super) (struct super_block *);
	int (*sync_fs)(struct super_block *sb, int wait);
	int (*freeze_super) (struct super_block *);
	int (*freeze_fs) (struct super_block *);
	int (*thaw_super) (struct super_block *);
	int (*unfreeze_fs) (struct super_block *);
	int (*statfs) (struct dentry *, struct kstatfs *);
	int (*remount_fs) (struct super_block *, int *, char *);
	int (*remount_fs2) (struct vfsmount *, struct super_block *, int *, char *);
	void *(*clone_mnt_data) (void *);
	void (*copy_mnt_data) (void *, void *);
	void (*umount_begin) (struct super_block *);
	void (*umount_end) (struct super_block *, int);

	int (*show_options)(struct seq_file *, struct dentry *);
	int (*show_options2)(struct vfsmount *,struct seq_file *, struct dentry *);
	int (*show_devname)(struct seq_file *, struct dentry *);
	int (*show_path)(struct seq_file *, struct dentry *);
	int (*show_stats)(struct seq_file *, struct dentry *);
#ifdef CONFIG_QUOTA
	ssize_t (*quota_read)(struct super_block *, int, char *, size_t, loff_t);
	ssize_t (*quota_write)(struct super_block *, int, const char *, size_t, loff_t);
	struct dquot **(*get_dquots)(struct inode *);
#endif
	int (*bdev_try_to_free_page)(struct super_block*, struct page*, gfp_t);
	long (*nr_cached_objects)(struct super_block *,
				  struct shrink_control *);
	long (*free_cached_objects)(struct super_block *,
				    struct shrink_control *);
};


#define S_SYNC		1	
#define S_NOATIME	2	
#define S_APPEND	4	
#define S_IMMUTABLE	8	
#define S_DEAD		16	
#define S_NOQUOTA	32	
#define S_DIRSYNC	64	
#define S_NOCMTIME	128	
#define S_SWAPFILE	256	
#define S_PRIVATE	512	
#define S_IMA		1024	
#define S_AUTOMOUNT	2048	
#define S_NOSEC		4096	
#ifdef CONFIG_FS_DAX
#define S_DAX		8192	
#else
#define S_DAX		0	
#endif
#define S_ENCRYPTED	16384	


#define __IS_FLG(inode, flg)	((inode)->i_sb->s_flags & (flg))

#define IS_RDONLY(inode)	((inode)->i_sb->s_flags & MS_RDONLY)
#define IS_SYNC(inode)		(__IS_FLG(inode, MS_SYNCHRONOUS) || \
					((inode)->i_flags & S_SYNC))
#define IS_DIRSYNC(inode)	(__IS_FLG(inode, MS_SYNCHRONOUS|MS_DIRSYNC) || \
					((inode)->i_flags & (S_SYNC|S_DIRSYNC)))
#define IS_MANDLOCK(inode)	__IS_FLG(inode, MS_MANDLOCK)
#define IS_NOATIME(inode)	__IS_FLG(inode, MS_RDONLY|MS_NOATIME)
#define IS_I_VERSION(inode)	__IS_FLG(inode, MS_I_VERSION)

#define IS_NOQUOTA(inode)	((inode)->i_flags & S_NOQUOTA)
#define IS_APPEND(inode)	((inode)->i_flags & S_APPEND)
#define IS_IMMUTABLE(inode)	((inode)->i_flags & S_IMMUTABLE)
#define IS_POSIXACL(inode)	__IS_FLG(inode, MS_POSIXACL)

#define IS_DEADDIR(inode)	((inode)->i_flags & S_DEAD)
#define IS_NOCMTIME(inode)	((inode)->i_flags & S_NOCMTIME)
#define IS_SWAPFILE(inode)	((inode)->i_flags & S_SWAPFILE)
#define IS_PRIVATE(inode)	((inode)->i_flags & S_PRIVATE)
#define IS_IMA(inode)		((inode)->i_flags & S_IMA)
#define IS_AUTOMOUNT(inode)	((inode)->i_flags & S_AUTOMOUNT)
#define IS_NOSEC(inode)		((inode)->i_flags & S_NOSEC)
#define IS_DAX(inode)		((inode)->i_flags & S_DAX)
#define IS_ENCRYPTED(inode)	((inode)->i_flags & S_ENCRYPTED)

#define IS_WHITEOUT(inode)	(S_ISCHR(inode->i_mode) && \
				 (inode)->i_rdev == WHITEOUT_DEV)

static inline bool HAS_UNMAPPED_ID(struct inode *inode)
{
	return !uid_valid(inode->i_uid) || !gid_valid(inode->i_gid);
}


#define I_DIRTY_SYNC		(1 << 0)
#define I_DIRTY_DATASYNC	(1 << 1)
#define I_DIRTY_PAGES		(1 << 2)
#define __I_NEW			3
#define I_NEW			(1 << __I_NEW)
#define I_WILL_FREE		(1 << 4)
#define I_FREEING		(1 << 5)
#define I_CLEAR			(1 << 6)
#define __I_SYNC		7
#define I_SYNC			(1 << __I_SYNC)
#define I_REFERENCED		(1 << 8)
#define __I_DIO_WAKEUP		9
#define I_DIO_WAKEUP		(1 << __I_DIO_WAKEUP)
#define I_LINKABLE		(1 << 10)
#define I_DIRTY_TIME		(1 << 11)
#define I_DIRTY_TIME_EXPIRED	(1 << 12)
#define I_WB_SWITCH		(1 << 13)
#define I_SYNC_QUEUED		(1 << 17)

#define I_DIRTY (I_DIRTY_SYNC | I_DIRTY_DATASYNC | I_DIRTY_PAGES)
#define I_DIRTY_ALL (I_DIRTY | I_DIRTY_TIME)

extern void __mark_inode_dirty(struct inode *, int);
static inline void mark_inode_dirty(struct inode *inode)
{
	__mark_inode_dirty(inode, I_DIRTY);
}

static inline void mark_inode_dirty_sync(struct inode *inode)
{
	__mark_inode_dirty(inode, I_DIRTY_SYNC);
}

extern void inc_nlink(struct inode *inode);
extern void drop_nlink(struct inode *inode);
extern void clear_nlink(struct inode *inode);
extern void set_nlink(struct inode *inode, unsigned int nlink);

static inline void inode_inc_link_count(struct inode *inode)
{
	inc_nlink(inode);
	mark_inode_dirty(inode);
}

static inline void inode_dec_link_count(struct inode *inode)
{
	drop_nlink(inode);
	mark_inode_dirty(inode);
}



static inline void inode_inc_iversion(struct inode *inode)
{
       spin_lock(&inode->i_lock);
       inode->i_version++;
       spin_unlock(&inode->i_lock);
}

enum file_time_flags {
	S_ATIME = 1,
	S_MTIME = 2,
	S_CTIME = 4,
	S_VERSION = 8,
};

extern void touch_atime(const struct path *);
static inline void file_accessed(struct file *file)
{
	if (!(file->f_flags & O_NOATIME))
		touch_atime(&file->f_path);
}

int sync_inode(struct inode *inode, struct writeback_control *wbc);
int sync_inode_metadata(struct inode *inode, int wait);

struct file_system_type {
	const char *name;
	int fs_flags;
#define FS_REQUIRES_DEV		1 
#define FS_BINARY_MOUNTDATA	2
#define FS_HAS_SUBTYPE		4
#define FS_USERNS_MOUNT		8	
#define FS_RENAME_DOES_D_MOVE	32768	
	struct dentry *(*mount) (struct file_system_type *, int,
		       const char *, void *);
	struct dentry *(*mount2) (struct vfsmount *, struct file_system_type *, int,
			       const char *, void *);
	void *(*alloc_mnt_data) (void);
	void (*kill_sb) (struct super_block *);
	struct module *owner;
	struct file_system_type * next;
	struct hlist_head fs_supers;

	struct lock_class_key s_lock_key;
	struct lock_class_key s_umount_key;
	struct lock_class_key s_vfs_rename_key;
	struct lock_class_key s_writers_key[SB_FREEZE_LEVELS];

	struct lock_class_key i_lock_key;
	struct lock_class_key i_mutex_key;
	struct lock_class_key i_mutex_dir_key;
};

#define MODULE_ALIAS_FS(NAME) MODULE_ALIAS("fs-" NAME)

extern struct dentry *mount_ns(struct file_system_type *fs_type,
	int flags, void *data, void *ns, struct user_namespace *user_ns,
	int (*fill_super)(struct super_block *, void *, int));
extern struct dentry *mount_bdev(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data,
	int (*fill_super)(struct super_block *, void *, int));
extern struct dentry *mount_single(struct file_system_type *fs_type,
	int flags, void *data,
	int (*fill_super)(struct super_block *, void *, int));
extern struct dentry *mount_nodev(struct file_system_type *fs_type,
	int flags, void *data,
	int (*fill_super)(struct super_block *, void *, int));
extern struct dentry *mount_subtree(struct vfsmount *mnt, const char *path);
void generic_shutdown_super(struct super_block *sb);
void kill_block_super(struct super_block *sb);
void kill_anon_super(struct super_block *sb);
void kill_litter_super(struct super_block *sb);
void deactivate_super(struct super_block *sb);
void deactivate_locked_super(struct super_block *sb);
int set_anon_super(struct super_block *s, void *data);
int get_anon_bdev(dev_t *);
void free_anon_bdev(dev_t);
struct super_block *sget_userns(struct file_system_type *type,
			int (*test)(struct super_block *,void *),
			int (*set)(struct super_block *,void *),
			int flags, struct user_namespace *user_ns,
			void *data);
struct super_block *sget(struct file_system_type *type,
			int (*test)(struct super_block *,void *),
			int (*set)(struct super_block *,void *),
			int flags, void *data);
extern struct dentry *mount_pseudo_xattr(struct file_system_type *, char *,
					 const struct super_operations *ops,
					 const struct xattr_handler **xattr,
					 const struct dentry_operations *dops,
					 unsigned long);

static inline struct dentry *
mount_pseudo(struct file_system_type *fs_type, char *name,
	     const struct super_operations *ops,
	     const struct dentry_operations *dops, unsigned long magic)
{
	return mount_pseudo_xattr(fs_type, name, ops, NULL, dops, magic);
}


#define fops_get(fops) \
	(((fops) && try_module_get((fops)->owner) ? (fops) : NULL))
#define fops_put(fops) \
	do { if (fops) module_put((fops)->owner); } while(0)

#define replace_fops(f, fops) \
	do {	\
		struct file *__file = (f); \
		fops_put(__file->f_op); \
		BUG_ON(!(__file->f_op = (fops))); \
	} while(0)

extern int register_filesystem(struct file_system_type *);
extern int unregister_filesystem(struct file_system_type *);
extern struct vfsmount *kern_mount_data(struct file_system_type *, void *data);
#define kern_mount(type) kern_mount_data(type, NULL)
extern void kern_unmount(struct vfsmount *mnt);
extern int may_umount_tree(struct vfsmount *);
extern int may_umount(struct vfsmount *);
extern long do_mount(const char *, const char __user *,
		     const char *, unsigned long, void *);
extern struct vfsmount *collect_mounts(struct path *);
extern void drop_collected_mounts(struct vfsmount *);
extern int iterate_mounts(int (*)(struct vfsmount *, void *), void *,
			  struct vfsmount *);
extern int vfs_statfs(struct path *, struct kstatfs *);
extern int user_statfs(const char __user *, struct kstatfs *);
extern int fd_statfs(int, struct kstatfs *);
extern int vfs_ustat(dev_t, struct kstatfs *);
extern int freeze_super(struct super_block *super);
extern int thaw_super(struct super_block *super);
extern bool our_mnt(struct vfsmount *mnt);

extern int current_umask(void);

extern void ihold(struct inode * inode);
extern void iput(struct inode *);
extern int generic_update_time(struct inode *, struct timespec *, int);


extern struct kobject *fs_kobj;

#define MAX_RW_COUNT (INT_MAX & PAGE_MASK)

#ifdef CONFIG_MANDATORY_FILE_LOCKING
extern int locks_mandatory_locked(struct file *);
extern int locks_mandatory_area(struct inode *, struct file *, loff_t, loff_t, unsigned char);



static inline int __mandatory_lock(struct inode *ino)
{
	return (ino->i_mode & (S_ISGID | S_IXGRP)) == S_ISGID;
}



static inline int mandatory_lock(struct inode *ino)
{
	return IS_MANDLOCK(ino) && __mandatory_lock(ino);
}

static inline int locks_verify_locked(struct file *file)
{
	if (mandatory_lock(locks_inode(file)))
		return locks_mandatory_locked(file);
	return 0;
}

static inline int locks_verify_truncate(struct inode *inode,
				    struct file *f,
				    loff_t size)
{
	if (!inode->i_flctx || !mandatory_lock(inode))
		return 0;

	if (size < inode->i_size) {
		return locks_mandatory_area(inode, f, size, inode->i_size - 1,
				F_WRLCK);
	} else {
		return locks_mandatory_area(inode, f, inode->i_size, size - 1,
				F_WRLCK);
	}
}

#else 

static inline int locks_mandatory_locked(struct file *file)
{
	return 0;
}

static inline int locks_mandatory_area(struct inode *inode, struct file *filp,
                                       loff_t start, loff_t end, unsigned char type)
{
	return 0;
}

static inline int __mandatory_lock(struct inode *inode)
{
	return 0;
}

static inline int mandatory_lock(struct inode *inode)
{
	return 0;
}

static inline int locks_verify_locked(struct file *file)
{
	return 0;
}

static inline int locks_verify_truncate(struct inode *inode, struct file *filp,
					size_t size)
{
	return 0;
}

#endif 


#ifdef CONFIG_FILE_LOCKING
static inline int break_lease(struct inode *inode, unsigned int mode)
{
	
	smp_mb();
	if (inode->i_flctx && !list_empty_careful(&inode->i_flctx->flc_lease))
		return __break_lease(inode, mode, FL_LEASE);
	return 0;
}

static inline int break_deleg(struct inode *inode, unsigned int mode)
{
	
	smp_mb();
	if (inode->i_flctx && !list_empty_careful(&inode->i_flctx->flc_lease))
		return __break_lease(inode, mode, FL_DELEG);
	return 0;
}

static inline int try_break_deleg(struct inode *inode, struct inode **delegated_inode)
{
	int ret;

	ret = break_deleg(inode, O_WRONLY|O_NONBLOCK);
	if (ret == -EWOULDBLOCK && delegated_inode) {
		*delegated_inode = inode;
		ihold(inode);
	}
	return ret;
}

static inline int break_deleg_wait(struct inode **delegated_inode)
{
	int ret;

	ret = break_deleg(*delegated_inode, O_WRONLY);
	iput(*delegated_inode);
	*delegated_inode = NULL;
	return ret;
}

static inline int break_layout(struct inode *inode, bool wait)
{
	smp_mb();
	if (inode->i_flctx && !list_empty_careful(&inode->i_flctx->flc_lease))
		return __break_lease(inode,
				wait ? O_WRONLY : O_WRONLY | O_NONBLOCK,
				FL_LAYOUT);
	return 0;
}

#else 
static inline int break_lease(struct inode *inode, unsigned int mode)
{
	return 0;
}

static inline int break_deleg(struct inode *inode, unsigned int mode)
{
	return 0;
}

static inline int try_break_deleg(struct inode *inode, struct inode **delegated_inode)
{
	return 0;
}

static inline int break_deleg_wait(struct inode **delegated_inode)
{
	BUG();
	return 0;
}

static inline int break_layout(struct inode *inode, bool wait)
{
	return 0;
}

#endif 


struct audit_names;
struct filename {
	const char		*name;	
	const __user char	*uptr;	
	struct audit_names	*aname;
	int			refcnt;
	const char		iname[];
};

extern long vfs_truncate(const struct path *, loff_t);
extern int do_truncate(struct dentry *, loff_t start, unsigned int time_attrs,
		       struct file *filp);
extern int do_truncate2(struct vfsmount *, struct dentry *, loff_t start,
			unsigned int time_attrs, struct file *filp);
extern int vfs_fallocate(struct file *file, int mode, loff_t offset,
			loff_t len);
extern long do_sys_open(int dfd, const char __user *filename, int flags,
			umode_t mode);
extern struct file *file_open_name(struct filename *, int, umode_t);
extern struct file *filp_open(const char *, int, umode_t);
extern struct file *file_open_root(struct dentry *, struct vfsmount *,
				   const char *, int, umode_t);
extern struct file * dentry_open(const struct path *, int, const struct cred *);
extern int filp_close(struct file *, fl_owner_t id);

extern struct filename *getname_flags(const char __user *, int, int *);
extern struct filename *getname(const char __user *);
extern struct filename *getname_kernel(const char *);
extern void putname(struct filename *name);

enum {
	FILE_CREATED = 1,
	FILE_OPENED = 2
};
extern int finish_open(struct file *file, struct dentry *dentry,
			int (*open)(struct inode *, struct file *),
			int *opened);
extern int finish_no_open(struct file *file, struct dentry *dentry);



extern int ioctl_preallocate(struct file *filp, void __user *argp);


extern void __init vfs_caches_init_early(void);
extern void __init vfs_caches_init(void);

extern struct kmem_cache *names_cachep;

#define __getname()		kmem_cache_alloc(names_cachep, GFP_KERNEL)
#define __putname(name)		kmem_cache_free(names_cachep, (void *)(name))

#ifdef CONFIG_BLOCK
extern int register_blkdev(unsigned int, const char *);
extern void unregister_blkdev(unsigned int, const char *);
extern struct block_device *bdget(dev_t);
extern struct block_device *bdgrab(struct block_device *bdev);
extern void bd_set_size(struct block_device *, loff_t size);
extern void bd_forget(struct inode *inode);
extern void bdput(struct block_device *);
extern void invalidate_bdev(struct block_device *);
extern void iterate_bdevs(void (*)(struct block_device *, void *), void *);
extern int sync_blockdev(struct block_device *bdev);
extern void kill_bdev(struct block_device *);
extern struct super_block *freeze_bdev(struct block_device *);
extern void emergency_thaw_all(void);
extern int thaw_bdev(struct block_device *bdev, struct super_block *sb);
extern int fsync_bdev(struct block_device *);

extern struct super_block *blockdev_superblock;

static inline bool sb_is_blkdev_sb(struct super_block *sb)
{
	return sb == blockdev_superblock;
}
#else
static inline void bd_forget(struct inode *inode) {}
static inline int sync_blockdev(struct block_device *bdev) { return 0; }
static inline void kill_bdev(struct block_device *bdev) {}
static inline void invalidate_bdev(struct block_device *bdev) {}

static inline struct super_block *freeze_bdev(struct block_device *sb)
{
	return NULL;
}

static inline int thaw_bdev(struct block_device *bdev, struct super_block *sb)
{
	return 0;
}

static inline void iterate_bdevs(void (*f)(struct block_device *, void *), void *arg)
{
}

static inline bool sb_is_blkdev_sb(struct super_block *sb)
{
	return false;
}
#endif
extern int sync_filesystem(struct super_block *);
extern const struct file_operations def_blk_fops;
extern const struct file_operations def_chr_fops;
#ifdef CONFIG_BLOCK
extern int ioctl_by_bdev(struct block_device *, unsigned, unsigned long);
extern int blkdev_ioctl(struct block_device *, fmode_t, unsigned, unsigned long);
extern long compat_blkdev_ioctl(struct file *, unsigned, unsigned long);
extern int blkdev_get(struct block_device *bdev, fmode_t mode, void *holder);
extern struct block_device *blkdev_get_by_path(const char *path, fmode_t mode,
					       void *holder);
extern struct block_device *blkdev_get_by_dev(dev_t dev, fmode_t mode,
					      void *holder);
extern void blkdev_put(struct block_device *bdev, fmode_t mode);
extern int __blkdev_reread_part(struct block_device *bdev);
extern int blkdev_reread_part(struct block_device *bdev);

#ifdef CONFIG_SYSFS
extern int bd_link_disk_holder(struct block_device *bdev, struct gendisk *disk);
extern void bd_unlink_disk_holder(struct block_device *bdev,
				  struct gendisk *disk);
#else
static inline int bd_link_disk_holder(struct block_device *bdev,
				      struct gendisk *disk)
{
	return 0;
}
static inline void bd_unlink_disk_holder(struct block_device *bdev,
					 struct gendisk *disk)
{
}
#endif
#endif


#define CHRDEV_MAJOR_HASH_SIZE	255

#define CHRDEV_MAJOR_DYN_END 234
extern int alloc_chrdev_region(dev_t *, unsigned, unsigned, const char *);
extern int register_chrdev_region(dev_t, unsigned, const char *);
extern int __register_chrdev(unsigned int major, unsigned int baseminor,
			     unsigned int count, const char *name,
			     const struct file_operations *fops);
extern void __unregister_chrdev(unsigned int major, unsigned int baseminor,
				unsigned int count, const char *name);
extern void unregister_chrdev_region(dev_t, unsigned);
extern void chrdev_show(struct seq_file *,off_t);

static inline int register_chrdev(unsigned int major, const char *name,
				  const struct file_operations *fops)
{
	return __register_chrdev(major, 0, 256, name, fops);
}

static inline void unregister_chrdev(unsigned int major, const char *name)
{
	__unregister_chrdev(major, 0, 256, name);
}


#define BDEVNAME_SIZE	32	
#define BDEVT_SIZE	10	

#ifdef CONFIG_BLOCK
#define BLKDEV_MAJOR_HASH_SIZE	255
extern const char *__bdevname(dev_t, char *buffer);
extern const char *bdevname(struct block_device *bdev, char *buffer);
extern struct block_device *lookup_bdev(const char *);
extern void blkdev_show(struct seq_file *,off_t);

#else
#define BLKDEV_MAJOR_HASH_SIZE	0
#endif

extern void init_special_inode(struct inode *, umode_t, dev_t);


extern void make_bad_inode(struct inode *);
extern bool is_bad_inode(struct inode *);

#ifdef CONFIG_BLOCK
static inline bool op_is_write(unsigned int op)
{
	return op == REQ_OP_READ ? false : true;
}


static inline int bio_data_dir(struct bio *bio)
{
	return op_is_write(bio_op(bio)) ? WRITE : READ;
}

extern void check_disk_size_change(struct gendisk *disk,
				   struct block_device *bdev);
extern int revalidate_disk(struct gendisk *);
extern int check_disk_change(struct block_device *);
extern int __invalidate_device(struct block_device *, bool);
extern int invalidate_partition(struct gendisk *, int);
#endif
unsigned long invalidate_mapping_pages(struct address_space *mapping,
					pgoff_t start, pgoff_t end);

static inline void invalidate_remote_inode(struct inode *inode)
{
	if (S_ISREG(inode->i_mode) || S_ISDIR(inode->i_mode) ||
	    S_ISLNK(inode->i_mode))
		invalidate_mapping_pages(inode->i_mapping, 0, -1);
}
extern int invalidate_inode_pages2(struct address_space *mapping);
extern int invalidate_inode_pages2_range(struct address_space *mapping,
					 pgoff_t start, pgoff_t end);
extern int write_inode_now(struct inode *, int);
extern int filemap_fdatawrite(struct address_space *);
extern int filemap_flush(struct address_space *);
extern int filemap_fdatawait(struct address_space *);
extern void filemap_fdatawait_keep_errors(struct address_space *);
extern int filemap_fdatawait_range(struct address_space *, loff_t lstart,
				   loff_t lend);
extern int filemap_write_and_wait(struct address_space *mapping);
extern int filemap_write_and_wait_range(struct address_space *mapping,
				        loff_t lstart, loff_t lend);
extern int __filemap_fdatawrite_range(struct address_space *mapping,
				loff_t start, loff_t end, int sync_mode);
extern int filemap_fdatawrite_range(struct address_space *mapping,
				loff_t start, loff_t end);
extern int filemap_check_errors(struct address_space *mapping);

extern int vfs_fsync_range(struct file *file, loff_t start, loff_t end,
			   int datasync);
extern int vfs_fsync(struct file *file, int datasync);


static inline ssize_t generic_write_sync(struct kiocb *iocb, ssize_t count)
{
	if (iocb->ki_flags & IOCB_DSYNC) {
		int ret = vfs_fsync_range(iocb->ki_filp,
				iocb->ki_pos - count, iocb->ki_pos - 1,
				(iocb->ki_flags & IOCB_SYNC) ? 0 : 1);
		if (ret)
			return ret;
	}

	return count;
}

extern void emergency_sync(void);
extern void emergency_remount(void);
extern int intr_sync(int *);
#ifdef CONFIG_BLOCK
extern sector_t bmap(struct inode *, sector_t);
#endif
extern int notify_change(struct dentry *, struct iattr *, struct inode **);
extern int notify_change2(struct vfsmount *, struct dentry *, struct iattr *, struct inode **);
extern int inode_permission(struct inode *, int);
extern int inode_permission2(struct vfsmount *, struct inode *, int);
extern int __inode_permission(struct inode *, int);
extern int __inode_permission2(struct vfsmount *, struct inode *, int);
extern int generic_permission(struct inode *, int);
extern int __check_sticky(struct inode *dir, struct inode *inode);

static inline bool execute_ok(struct inode *inode)
{
	return (inode->i_mode & S_IXUGO) || S_ISDIR(inode->i_mode);
}

static inline void file_start_write(struct file *file)
{
	if (!S_ISREG(file_inode(file)->i_mode))
		return;
	__sb_start_write(file_inode(file)->i_sb, SB_FREEZE_WRITE, true);
}

static inline bool file_start_write_trylock(struct file *file)
{
	if (!S_ISREG(file_inode(file)->i_mode))
		return true;
	return __sb_start_write(file_inode(file)->i_sb, SB_FREEZE_WRITE, false);
}

static inline void file_end_write(struct file *file)
{
	if (!S_ISREG(file_inode(file)->i_mode))
		return;
	__sb_end_write(file_inode(file)->i_sb, SB_FREEZE_WRITE);
}


static inline int get_write_access(struct inode *inode)
{
	return atomic_inc_unless_negative(&inode->i_writecount) ? 0 : -ETXTBSY;
}
static inline int deny_write_access(struct file *file)
{
	struct inode *inode = file_inode(file);
	return atomic_dec_unless_positive(&inode->i_writecount) ? 0 : -ETXTBSY;
}
static inline void put_write_access(struct inode * inode)
{
	atomic_dec(&inode->i_writecount);
}
static inline void allow_write_access(struct file *file)
{
	if (file)
		atomic_inc(&file_inode(file)->i_writecount);
}
static inline bool inode_is_open_for_write(const struct inode *inode)
{
	return atomic_read(&inode->i_writecount) > 0;
}

#ifdef CONFIG_IMA
static inline void i_readcount_dec(struct inode *inode)
{
	BUG_ON(!atomic_read(&inode->i_readcount));
	atomic_dec(&inode->i_readcount);
}
static inline void i_readcount_inc(struct inode *inode)
{
	atomic_inc(&inode->i_readcount);
}
#else
static inline void i_readcount_dec(struct inode *inode)
{
	return;
}
static inline void i_readcount_inc(struct inode *inode)
{
	return;
}
#endif
extern int do_pipe_flags(int *, int);

#define __kernel_read_file_id(id) \
	id(UNKNOWN, unknown)		\
	id(FIRMWARE, firmware)		\
	id(FIRMWARE_PREALLOC_BUFFER, firmware)	\
	id(MODULE, kernel-module)		\
	id(KEXEC_IMAGE, kexec-image)		\
	id(KEXEC_INITRAMFS, kexec-initramfs)	\
	id(POLICY, security-policy)		\
	id(MAX_ID, )

#define __fid_enumify(ENUM, dummy) READING_ ## ENUM,
#define __fid_stringify(dummy, str) #str,

enum kernel_read_file_id {
	__kernel_read_file_id(__fid_enumify)
};

static const char * const kernel_read_file_str[] = {
	__kernel_read_file_id(__fid_stringify)
};

static inline const char *kernel_read_file_id_str(enum kernel_read_file_id id)
{
	if (id < 0 || id >= READING_MAX_ID)
		return kernel_read_file_str[READING_UNKNOWN];

	return kernel_read_file_str[id];
}

extern int kernel_read(struct file *, loff_t, char *, unsigned long);
extern int kernel_read_file(struct file *, void **, loff_t *, loff_t,
			    enum kernel_read_file_id);
extern int kernel_read_file_from_path(char *, void **, loff_t *, loff_t,
				      enum kernel_read_file_id);
extern int kernel_read_file_from_fd(int, void **, loff_t *, loff_t,
				    enum kernel_read_file_id);
extern ssize_t kernel_write(struct file *, const char *, size_t, loff_t);
extern ssize_t __kernel_write(struct file *, const char *, size_t, loff_t *);
extern struct file * open_exec(const char *);
 

extern bool is_subdir(struct dentry *, struct dentry *);
extern bool path_is_under(struct path *, struct path *);

extern char *file_path(struct file *, char *, int);

#include <linux/err.h>


extern loff_t default_llseek(struct file *file, loff_t offset, int whence);

extern loff_t vfs_llseek(struct file *file, loff_t offset, int whence);

extern int inode_init_always(struct super_block *, struct inode *);
extern void inode_init_once(struct inode *);
extern void address_space_init_once(struct address_space *mapping);
extern struct inode * igrab(struct inode *);
extern ino_t iunique(struct super_block *, ino_t);
extern int inode_needs_sync(struct inode *inode);
extern int generic_delete_inode(struct inode *inode);
static inline int generic_drop_inode(struct inode *inode)
{
	return !inode->i_nlink || inode_unhashed(inode);
}

extern struct inode *ilookup5_nowait(struct super_block *sb,
		unsigned long hashval, int (*test)(struct inode *, void *),
		void *data);
extern struct inode *ilookup5(struct super_block *sb, unsigned long hashval,
		int (*test)(struct inode *, void *), void *data);
extern struct inode *ilookup(struct super_block *sb, unsigned long ino);

extern struct inode * iget5_locked(struct super_block *, unsigned long, int (*test)(struct inode *, void *), int (*set)(struct inode *, void *), void *);
extern struct inode * iget_locked(struct super_block *, unsigned long);
extern struct inode *find_inode_nowait(struct super_block *,
				       unsigned long,
				       int (*match)(struct inode *,
						    unsigned long, void *),
				       void *data);
extern int insert_inode_locked4(struct inode *, unsigned long, int (*test)(struct inode *, void *), void *);
extern int insert_inode_locked(struct inode *);
#ifdef CONFIG_DEBUG_LOCK_ALLOC
extern void lockdep_annotate_inode_mutex_key(struct inode *inode);
#else
static inline void lockdep_annotate_inode_mutex_key(struct inode *inode) { };
#endif
extern void unlock_new_inode(struct inode *);
extern unsigned int get_next_ino(void);
extern void evict_inodes(struct super_block *sb);

extern void __iget(struct inode * inode);
extern void iget_failed(struct inode *);
extern void clear_inode(struct inode *);
extern void __destroy_inode(struct inode *);
extern struct inode *new_inode_pseudo(struct super_block *sb);
extern struct inode *new_inode(struct super_block *sb);
extern void free_inode_nonrcu(struct inode *inode);
extern int should_remove_suid(struct dentry *);
extern int file_remove_privs(struct file *);

extern void __insert_inode_hash(struct inode *, unsigned long hashval);
static inline void insert_inode_hash(struct inode *inode)
{
	__insert_inode_hash(inode, inode->i_ino);
}

extern void __remove_inode_hash(struct inode *);
static inline void remove_inode_hash(struct inode *inode)
{
	if (!inode_unhashed(inode) && !hlist_fake(&inode->i_hash))
		__remove_inode_hash(inode);
}

extern void inode_sb_list_add(struct inode *inode);

#ifdef CONFIG_BLOCK
extern blk_qc_t submit_bio(struct bio *);
extern int bdev_read_only(struct block_device *);
#endif
extern int set_blocksize(struct block_device *, int);
extern int sb_set_blocksize(struct super_block *, int);
extern int sb_min_blocksize(struct super_block *, int);

extern int generic_file_mmap(struct file *, struct vm_area_struct *);
extern int generic_file_readonly_mmap(struct file *, struct vm_area_struct *);
extern ssize_t generic_write_checks(struct kiocb *, struct iov_iter *);
extern ssize_t generic_file_read_iter(struct kiocb *, struct iov_iter *);
extern ssize_t __generic_file_write_iter(struct kiocb *, struct iov_iter *);
extern ssize_t generic_file_write_iter(struct kiocb *, struct iov_iter *);
extern ssize_t generic_file_direct_write(struct kiocb *, struct iov_iter *);
extern ssize_t generic_perform_write(struct file *, struct iov_iter *, loff_t);

ssize_t vfs_iter_read(struct file *file, struct iov_iter *iter, loff_t *ppos,
		int flags);
ssize_t vfs_iter_write(struct file *file, struct iov_iter *iter, loff_t *ppos,
		int flags);


extern ssize_t blkdev_read_iter(struct kiocb *iocb, struct iov_iter *to);
extern ssize_t blkdev_write_iter(struct kiocb *iocb, struct iov_iter *from);
extern int blkdev_fsync(struct file *filp, loff_t start, loff_t end,
			int datasync);
extern void block_sync_page(struct page *page);


extern ssize_t generic_file_splice_read(struct file *, loff_t *,
		struct pipe_inode_info *, size_t, unsigned int);
extern ssize_t iter_file_splice_write(struct pipe_inode_info *,
		struct file *, loff_t *, size_t, unsigned int);
extern ssize_t generic_splice_sendpage(struct pipe_inode_info *pipe,
		struct file *out, loff_t *, size_t len, unsigned int flags);
extern long do_splice_direct(struct file *in, loff_t *ppos, struct file *out,
		loff_t *opos, size_t len, unsigned int flags);


extern void
file_ra_state_init(struct file_ra_state *ra, struct address_space *mapping);
extern loff_t noop_llseek(struct file *file, loff_t offset, int whence);
extern loff_t no_llseek(struct file *file, loff_t offset, int whence);
extern loff_t vfs_setpos(struct file *file, loff_t offset, loff_t maxsize);
extern loff_t generic_file_llseek(struct file *file, loff_t offset, int whence);
extern loff_t generic_file_llseek_size(struct file *file, loff_t offset,
		int whence, loff_t maxsize, loff_t eof);
extern loff_t fixed_size_llseek(struct file *file, loff_t offset,
		int whence, loff_t size);
extern loff_t no_seek_end_llseek_size(struct file *, loff_t, int, loff_t);
extern loff_t no_seek_end_llseek(struct file *, loff_t, int);
extern int generic_file_open(struct inode * inode, struct file * filp);
extern int nonseekable_open(struct inode * inode, struct file * filp);
extern int stream_open(struct inode * inode, struct file * filp);

#ifdef CONFIG_BLOCK
typedef void (dio_submit_t)(struct bio *bio, struct inode *inode,
			    loff_t file_offset);

enum {
	
	DIO_LOCKING	= 0x01,

	
	DIO_SKIP_HOLES	= 0x02,

	
	DIO_ASYNC_EXTEND = 0x04,

	
	DIO_SKIP_DIO_COUNT = 0x08,
};

void dio_end_io(struct bio *bio, int error);
#ifdef CONFIG_DDAR
struct inode *dio_bio_get_inode(struct bio *bio);
#endif

ssize_t __blockdev_direct_IO(struct kiocb *iocb, struct inode *inode,
			     struct block_device *bdev, struct iov_iter *iter,
			     get_block_t get_block,
			     dio_iodone_t end_io, dio_submit_t submit_io,
			     int flags);

static inline ssize_t blockdev_direct_IO(struct kiocb *iocb,
					 struct inode *inode,
					 struct iov_iter *iter,
					 get_block_t get_block)
{
	return __blockdev_direct_IO(iocb, inode, inode->i_sb->s_bdev, iter,
			get_block, NULL, NULL, DIO_LOCKING | DIO_SKIP_HOLES);
}
#endif

void inode_dio_wait(struct inode *inode);


static inline void inode_dio_begin(struct inode *inode)
{
	atomic_inc(&inode->i_dio_count);
}


static inline void inode_dio_end(struct inode *inode)
{
	if (atomic_dec_and_test(&inode->i_dio_count))
		wake_up_bit(&inode->i_state, __I_DIO_WAKEUP);
}

extern void inode_set_flags(struct inode *inode, unsigned int flags,
			    unsigned int mask);

extern const struct file_operations generic_ro_fops;

#define special_file(m) (S_ISCHR(m)||S_ISBLK(m)||S_ISFIFO(m)||S_ISSOCK(m))

extern int readlink_copy(char __user *, int, const char *);
extern int page_readlink(struct dentry *, char __user *, int);
extern const char *page_get_link(struct dentry *, struct inode *,
				 struct delayed_call *);
extern void page_put_link(void *);
extern int __page_symlink(struct inode *inode, const char *symname, int len,
		int nofs);
extern int page_symlink(struct inode *inode, const char *symname, int len);
extern const struct inode_operations page_symlink_inode_operations;
extern void kfree_link(void *);
extern int generic_readlink(struct dentry *, char __user *, int);
extern void generic_fillattr(struct inode *, struct kstat *);
int vfs_getattr_nosec(struct path *path, struct kstat *stat);
extern int vfs_getattr(struct path *, struct kstat *);
void __inode_add_bytes(struct inode *inode, loff_t bytes);
void inode_add_bytes(struct inode *inode, loff_t bytes);
void __inode_sub_bytes(struct inode *inode, loff_t bytes);
void inode_sub_bytes(struct inode *inode, loff_t bytes);
loff_t inode_get_bytes(struct inode *inode);
void inode_set_bytes(struct inode *inode, loff_t bytes);
const char *simple_get_link(struct dentry *, struct inode *,
			    struct delayed_call *);
extern const struct inode_operations simple_symlink_inode_operations;

extern int iterate_dir(struct file *, struct dir_context *);

extern int vfs_stat(const char __user *, struct kstat *);
extern int vfs_lstat(const char __user *, struct kstat *);
extern int vfs_fstat(unsigned int, struct kstat *);
extern int vfs_fstatat(int , const char __user *, struct kstat *, int);
extern const char *vfs_get_link(struct dentry *, struct delayed_call *);

extern int __generic_block_fiemap(struct inode *inode,
				  struct fiemap_extent_info *fieinfo,
				  loff_t start, loff_t len,
				  get_block_t *get_block);
extern int generic_block_fiemap(struct inode *inode,
				struct fiemap_extent_info *fieinfo, u64 start,
				u64 len, get_block_t *get_block);

extern void get_filesystem(struct file_system_type *fs);
extern void put_filesystem(struct file_system_type *fs);
extern struct file_system_type *get_fs_type(const char *name);
extern struct super_block *get_super(struct block_device *);
extern struct super_block *get_super_thawed(struct block_device *);
extern struct super_block *get_active_super(struct block_device *bdev);
extern void drop_super(struct super_block *sb);
extern void iterate_supers(void (*)(struct super_block *, void *), void *);
extern void iterate_supers_type(struct file_system_type *,
			        void (*)(struct super_block *, void *), void *);

extern int dcache_dir_open(struct inode *, struct file *);
extern int dcache_dir_close(struct inode *, struct file *);
extern loff_t dcache_dir_lseek(struct file *, loff_t, int);
extern int dcache_readdir(struct file *, struct dir_context *);
extern int simple_setattr(struct dentry *, struct iattr *);
extern int simple_getattr(struct vfsmount *, struct dentry *, struct kstat *);
extern int simple_statfs(struct dentry *, struct kstatfs *);
extern int simple_open(struct inode *inode, struct file *file);
extern int simple_link(struct dentry *, struct inode *, struct dentry *);
extern int simple_unlink(struct inode *, struct dentry *);
extern int simple_rmdir(struct inode *, struct dentry *);
extern int simple_rename(struct inode *, struct dentry *,
			 struct inode *, struct dentry *, unsigned int);
extern int noop_fsync(struct file *, loff_t, loff_t, int);
extern int simple_empty(struct dentry *);
extern int simple_readpage(struct file *file, struct page *page);
extern int simple_write_begin(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned flags,
			struct page **pagep, void **fsdata);
extern int simple_write_end(struct file *file, struct address_space *mapping,
			loff_t pos, unsigned len, unsigned copied,
			struct page *page, void *fsdata);
extern int always_delete_dentry(const struct dentry *);
extern struct inode *alloc_anon_inode(struct super_block *);
extern int simple_nosetlease(struct file *, long, struct file_lock **, void **);
extern const struct dentry_operations simple_dentry_operations;

extern struct dentry *simple_lookup(struct inode *, struct dentry *, unsigned int flags);
extern ssize_t generic_read_dir(struct file *, char __user *, size_t, loff_t *);
extern const struct file_operations simple_dir_operations;
extern const struct inode_operations simple_dir_inode_operations;
extern void make_empty_dir_inode(struct inode *inode);
extern bool is_empty_dir_inode(struct inode *inode);
struct tree_descr { char *name; const struct file_operations *ops; int mode; };
struct dentry *d_alloc_name(struct dentry *, const char *);
extern int simple_fill_super(struct super_block *, unsigned long, struct tree_descr *);
extern int simple_pin_fs(struct file_system_type *, struct vfsmount **mount, int *count);
extern void simple_release_fs(struct vfsmount **mount, int *count);

extern ssize_t simple_read_from_buffer(void __user *to, size_t count,
			loff_t *ppos, const void *from, size_t available);
extern ssize_t simple_write_to_buffer(void *to, size_t available, loff_t *ppos,
		const void __user *from, size_t count);

extern int __generic_file_fsync(struct file *, loff_t, loff_t, int);
extern int generic_file_fsync(struct file *, loff_t, loff_t, int);

extern int generic_check_addressable(unsigned, u64);

#ifdef CONFIG_MIGRATION
extern int buffer_migrate_page(struct address_space *,
				struct page *, struct page *,
				enum migrate_mode);
#else
#define buffer_migrate_page NULL
#endif

extern int setattr_prepare(struct dentry *, struct iattr *);
extern int inode_newsize_ok(const struct inode *, loff_t offset);
extern void setattr_copy(struct inode *inode, const struct iattr *attr);

extern int file_update_time(struct file *file);

extern int generic_show_options(struct seq_file *m, struct dentry *root);
extern void save_mount_options(struct super_block *sb, char *options);
extern void replace_mount_options(struct super_block *sb, char *options);

static inline bool io_is_direct(struct file *filp)
{
	return (filp->f_flags & O_DIRECT) || IS_DAX(filp->f_mapping->host);
}

static inline bool vma_is_dax(struct vm_area_struct *vma)
{
	return vma->vm_file && IS_DAX(vma->vm_file->f_mapping->host);
}

static inline bool vma_is_fsdax(struct vm_area_struct *vma)
{
	struct inode *inode;

	if (!vma->vm_file)
		return false;
	if (!vma_is_dax(vma))
		return false;
	inode = file_inode(vma->vm_file);
	if (S_ISCHR(inode->i_mode))
		return false; 
	return true;
}

static inline int iocb_flags(struct file *file)
{
	int res = 0;
	if (file->f_flags & O_APPEND)
		res |= IOCB_APPEND;
	if (io_is_direct(file))
		res |= IOCB_DIRECT;
	if ((file->f_flags & O_DSYNC) || IS_SYNC(file->f_mapping->host))
		res |= IOCB_DSYNC;
	if (file->f_flags & __O_SYNC)
		res |= IOCB_SYNC;
	return res;
}

static inline int iocb_to_rw_flags(int ifl, int iocb_mask)
{
	return ifl & iocb_mask;
}

static inline ino_t parent_ino(struct dentry *dentry)
{
	ino_t res;

	
	spin_lock(&dentry->d_lock);
	res = dentry->d_parent->d_inode->i_ino;
	spin_unlock(&dentry->d_lock);
	return res;
}




struct simple_transaction_argresp {
	ssize_t size;
	char data[0];
};

#define SIMPLE_TRANSACTION_LIMIT (PAGE_SIZE - sizeof(struct simple_transaction_argresp))

char *simple_transaction_get(struct file *file, const char __user *buf,
				size_t size);
ssize_t simple_transaction_read(struct file *file, char __user *buf,
				size_t size, loff_t *pos);
int simple_transaction_release(struct inode *inode, struct file *file);

void simple_transaction_set(struct file *file, size_t n);


#define DEFINE_SIMPLE_ATTRIBUTE_XSIGNED(__fops, __get, __set, __fmt, __is_signed)	\
static int __fops ## _open(struct inode *inode, struct file *file)	\
{									\
	__simple_attr_check_format(__fmt, 0ull);			\
	return simple_attr_open(inode, file, __get, __set, __fmt);	\
}									\
static const struct file_operations __fops = {				\
	.owner	 = THIS_MODULE,						\
	.open	 = __fops ## _open,					\
	.release = simple_attr_release,					\
	.read	 = simple_attr_read,					\
	.write	 = (__is_signed) ? simple_attr_write_signed : simple_attr_write,	\
	.llseek	 = generic_file_llseek,					\
}

#define DEFINE_SIMPLE_ATTRIBUTE(__fops, __get, __set, __fmt)		\
	DEFINE_SIMPLE_ATTRIBUTE_XSIGNED(__fops, __get, __set, __fmt, false)

#define DEFINE_SIMPLE_ATTRIBUTE_SIGNED(__fops, __get, __set, __fmt)	\
	DEFINE_SIMPLE_ATTRIBUTE_XSIGNED(__fops, __get, __set, __fmt, true)

static inline __printf(1, 2)
void __simple_attr_check_format(const char *fmt, ...)
{
	
}

int simple_attr_open(struct inode *inode, struct file *file,
		     int (*get)(void *, u64 *), int (*set)(void *, u64),
		     const char *fmt);
int simple_attr_release(struct inode *inode, struct file *file);
ssize_t simple_attr_read(struct file *file, char __user *buf,
			 size_t len, loff_t *ppos);
ssize_t simple_attr_write(struct file *file, const char __user *buf,
			  size_t len, loff_t *ppos);
ssize_t simple_attr_write_signed(struct file *file, const char __user *buf,
				 size_t len, loff_t *ppos);

struct ctl_table;
int proc_nr_files(struct ctl_table *table, int write,
		  void __user *buffer, size_t *lenp, loff_t *ppos);
int proc_nr_dentry(struct ctl_table *table, int write,
		  void __user *buffer, size_t *lenp, loff_t *ppos);
int proc_nr_inodes(struct ctl_table *table, int write,
		   void __user *buffer, size_t *lenp, loff_t *ppos);
int __init get_filesystem_list(char *buf);

#define __FMODE_EXEC		((__force int) FMODE_EXEC)
#define __FMODE_NONOTIFY	((__force int) FMODE_NONOTIFY)

#define ACC_MODE(x) ("\004\002\006\006"[(x)&O_ACCMODE])
#define OPEN_FMODE(flag) ((__force fmode_t)(((flag + 1) & O_ACCMODE) | \
					    (flag & __FMODE_NONOTIFY)))

static inline bool is_sxid(umode_t mode)
{
	return (mode & S_ISUID) || ((mode & S_ISGID) && (mode & S_IXGRP));
}

static inline int check_sticky(struct inode *dir, struct inode *inode)
{
	if (!(dir->i_mode & S_ISVTX))
		return 0;

	return __check_sticky(dir, inode);
}

static inline void inode_has_no_xattr(struct inode *inode)
{
	if (!is_sxid(inode->i_mode) && (inode->i_sb->s_flags & MS_NOSEC))
		inode->i_flags |= S_NOSEC;
}

static inline bool is_root_inode(struct inode *inode)
{
	return inode == inode->i_sb->s_root->d_inode;
}

static inline bool dir_emit(struct dir_context *ctx,
			    const char *name, int namelen,
			    u64 ino, unsigned type)
{
	return ctx->actor(ctx, name, namelen, ctx->pos, ino, type) == 0;
}
static inline bool dir_emit_dot(struct file *file, struct dir_context *ctx)
{
	return ctx->actor(ctx, ".", 1, ctx->pos,
			  file->f_path.dentry->d_inode->i_ino, DT_DIR) == 0;
}
static inline bool dir_emit_dotdot(struct file *file, struct dir_context *ctx)
{
	return ctx->actor(ctx, "..", 2, ctx->pos,
			  parent_ino(file->f_path.dentry), DT_DIR) == 0;
}
static inline bool dir_emit_dots(struct file *file, struct dir_context *ctx)
{
	if (ctx->pos == 0) {
		if (!dir_emit_dot(file, ctx))
			return false;
		ctx->pos = 1;
	}
	if (ctx->pos == 1) {
		if (!dir_emit_dotdot(file, ctx))
			return false;
		ctx->pos = 2;
	}
	return true;
}
static inline bool dir_relax(struct inode *inode)
{
	inode_unlock(inode);
	inode_lock(inode);
	return !IS_DEADDIR(inode);
}

static inline bool dir_relax_shared(struct inode *inode)
{
	inode_unlock_shared(inode);
	inode_lock_shared(inode);
	return !IS_DEADDIR(inode);
}

extern bool path_noexec(const struct path *path);
extern void inode_nohighmem(struct inode *inode);

#define AID_USE_ROOT_RESERVED	KGIDT_INIT(5678)

#endif 
