#ifndef _LINUX_PIPE_FS_I_H
#define _LINUX_PIPE_FS_I_H

#define PIPE_DEF_BUFFERS	16

#define PIPE_BUF_FLAG_LRU	0x01	
#define PIPE_BUF_FLAG_ATOMIC	0x02	
#define PIPE_BUF_FLAG_GIFT	0x04	
#define PIPE_BUF_FLAG_PACKET	0x08	


struct pipe_buffer {
	struct page *page;
	unsigned int offset, len;
	const struct pipe_buf_operations *ops;
	unsigned int flags;
	unsigned long private;
};


struct pipe_inode_info {
	struct mutex mutex;
	wait_queue_head_t wait;
	unsigned int nrbufs, curbuf, buffers;
	unsigned int readers;
	unsigned int writers;
	unsigned int files;
	unsigned int waiting_writers;
	unsigned int r_counter;
	unsigned int w_counter;
	struct page *tmp_page;
	struct fasync_struct *fasync_readers;
	struct fasync_struct *fasync_writers;
	struct pipe_buffer *bufs;
	struct user_struct *user;
};


struct pipe_buf_operations {
	
	int can_merge;

	
	int (*confirm)(struct pipe_inode_info *, struct pipe_buffer *);

	
	void (*release)(struct pipe_inode_info *, struct pipe_buffer *);

	
	int (*steal)(struct pipe_inode_info *, struct pipe_buffer *);

	
	bool (*get)(struct pipe_inode_info *, struct pipe_buffer *);
};


static inline __must_check bool pipe_buf_get(struct pipe_inode_info *pipe,
				struct pipe_buffer *buf)
{
	return buf->ops->get(pipe, buf);
}


static inline void pipe_buf_release(struct pipe_inode_info *pipe,
				    struct pipe_buffer *buf)
{
	const struct pipe_buf_operations *ops = buf->ops;

	buf->ops = NULL;
	ops->release(pipe, buf);
}


static inline int pipe_buf_confirm(struct pipe_inode_info *pipe,
				   struct pipe_buffer *buf)
{
	return buf->ops->confirm(pipe, buf);
}


static inline int pipe_buf_steal(struct pipe_inode_info *pipe,
				 struct pipe_buffer *buf)
{
	return buf->ops->steal(pipe, buf);
}


#define PIPE_SIZE		PAGE_SIZE


void pipe_lock(struct pipe_inode_info *);
void pipe_unlock(struct pipe_inode_info *);
void pipe_double_lock(struct pipe_inode_info *, struct pipe_inode_info *);

extern unsigned int pipe_max_size, pipe_min_size;
extern unsigned long pipe_user_pages_hard;
extern unsigned long pipe_user_pages_soft;
int pipe_proc_fn(struct ctl_table *, int, void __user *, size_t *, loff_t *);


void pipe_wait(struct pipe_inode_info *pipe);

struct pipe_inode_info *alloc_pipe_info(void);
void free_pipe_info(struct pipe_inode_info *);


bool generic_pipe_buf_get(struct pipe_inode_info *, struct pipe_buffer *);
int generic_pipe_buf_confirm(struct pipe_inode_info *, struct pipe_buffer *);
int generic_pipe_buf_steal(struct pipe_inode_info *, struct pipe_buffer *);
void generic_pipe_buf_release(struct pipe_inode_info *, struct pipe_buffer *);
void pipe_buf_mark_unmergeable(struct pipe_buffer *buf);

extern const struct pipe_buf_operations nosteal_pipe_buf_ops;


long pipe_fcntl(struct file *, unsigned int, unsigned long arg);
struct pipe_inode_info *get_pipe_info(struct file *file);

int create_pipe_files(struct file **, int);

#endif
