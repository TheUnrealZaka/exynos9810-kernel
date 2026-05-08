

#ifndef _LINUX_BUFFER_HEAD_H
#define _LINUX_BUFFER_HEAD_H

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/linkage.h>
#include <linux/pagemap.h>
#include <linux/wait.h>
#include <linux/atomic.h>

#ifdef CONFIG_BLOCK

enum bh_state_bits {
	BH_Uptodate,	
	BH_Dirty,	
	BH_Lock,	
	BH_Req,		
	BH_Uptodate_Lock,

	BH_Mapped,	
	BH_New,		
	BH_Async_Read,	
	BH_Async_Write,	
	BH_Delay,	
	BH_Boundary,	
	BH_Write_EIO,	
	BH_Unwritten,	
	BH_Quiet,	
	BH_Meta,	
	BH_Prio,	
	BH_Sync_Flush,	
	BH_Defer_Completion, 
	BH_Bypass,	
#ifdef CONFIG_JOURNAL_DATA_TAG
	BH_Journal,     
#endif

	BH_PrivateStart,
};

#define MAX_BUF_PER_PAGE (PAGE_SIZE / 512)

struct page;
struct buffer_head;
struct address_space;
typedef void (bh_end_io_t)(struct buffer_head *bh, int uptodate);


struct buffer_head {
	unsigned long b_state;		
	struct buffer_head *b_this_page;
	struct page *b_page;		

	sector_t b_blocknr;		
	size_t b_size;			
	char *b_data;			

	struct block_device *b_bdev;
	bh_end_io_t *b_end_io;		
 	void *b_private;		
	struct list_head b_assoc_buffers; 
	struct address_space *b_assoc_map;	
	atomic_t b_count;		
};


#define BUFFER_FNS(bit, name)						\
static __always_inline void set_buffer_##name(struct buffer_head *bh)	\
{									\
	set_bit(BH_##bit, &(bh)->b_state);				\
}									\
static __always_inline void clear_buffer_##name(struct buffer_head *bh)	\
{									\
	clear_bit(BH_##bit, &(bh)->b_state);				\
}									\
static __always_inline int buffer_##name(const struct buffer_head *bh)	\
{									\
	return test_bit(BH_##bit, &(bh)->b_state);			\
}


#define TAS_BUFFER_FNS(bit, name)					\
static __always_inline int test_set_buffer_##name(struct buffer_head *bh) \
{									\
	return test_and_set_bit(BH_##bit, &(bh)->b_state);		\
}									\
static __always_inline int test_clear_buffer_##name(struct buffer_head *bh) \
{									\
	return test_and_clear_bit(BH_##bit, &(bh)->b_state);		\
}									\


BUFFER_FNS(Dirty, dirty)
TAS_BUFFER_FNS(Dirty, dirty)
BUFFER_FNS(Lock, locked)
BUFFER_FNS(Req, req)
TAS_BUFFER_FNS(Req, req)
BUFFER_FNS(Mapped, mapped)
BUFFER_FNS(New, new)
BUFFER_FNS(Async_Read, async_read)
BUFFER_FNS(Async_Write, async_write)
BUFFER_FNS(Delay, delay)
BUFFER_FNS(Boundary, boundary)
BUFFER_FNS(Write_EIO, write_io_error)
BUFFER_FNS(Unwritten, unwritten)
BUFFER_FNS(Meta, meta)
BUFFER_FNS(Prio, prio)
BUFFER_FNS(Sync_Flush, sync_flush)
BUFFER_FNS(Defer_Completion, defer_completion)
BUFFER_FNS(Bypass, bypass)
TAS_BUFFER_FNS(Bypass, bypass)
#ifdef CONFIG_JOURNAL_DATA_TAG
BUFFER_FNS(Journal, journal)
#endif

static __always_inline void set_buffer_uptodate(struct buffer_head *bh)
{
	
	if (test_bit(BH_Uptodate, &bh->b_state))
		return;

	
	smp_mb__before_atomic();
	set_bit(BH_Uptodate, &bh->b_state);
}

static __always_inline void clear_buffer_uptodate(struct buffer_head *bh)
{
	clear_bit(BH_Uptodate, &bh->b_state);
}

static __always_inline int buffer_uptodate(const struct buffer_head *bh)
{
	
	return (smp_load_acquire(&bh->b_state) & (1UL << BH_Uptodate)) != 0;
}

#define bh_offset(bh)		((unsigned long)(bh)->b_data & ~PAGE_MASK)


#define page_buffers(page)					\
	({							\
		BUG_ON(!PagePrivate(page));			\
		((struct buffer_head *)page_private(page));	\
	})
#define page_has_buffers(page)	PagePrivate(page)

void buffer_check_dirty_writeback(struct page *page,
				     bool *dirty, bool *writeback);



void mark_buffer_dirty(struct buffer_head *bh);
void mark_buffer_dirty_sync(struct buffer_head *bh);
void init_buffer(struct buffer_head *, bh_end_io_t *, void *);
void touch_buffer(struct buffer_head *bh);
void set_bh_page(struct buffer_head *bh,
		struct page *page, unsigned long offset);
int try_to_free_buffers(struct page *);
struct buffer_head *alloc_page_buffers(struct page *page, unsigned long size,
		int retry);
void create_empty_buffers(struct page *, unsigned long,
			unsigned long b_state);
void end_buffer_read_sync(struct buffer_head *bh, int uptodate);
void end_buffer_write_sync(struct buffer_head *bh, int uptodate);
void end_buffer_async_write(struct buffer_head *bh, int uptodate);


void mark_buffer_dirty_inode(struct buffer_head *bh, struct inode *inode);
void mark_buffer_dirty_inode_sync(struct buffer_head *bh, struct inode *inode);
int inode_has_buffers(struct inode *);
void invalidate_inode_buffers(struct inode *);
int remove_inode_buffers(struct inode *inode);
int sync_mapping_buffers(struct address_space *mapping);
void unmap_underlying_metadata(struct block_device *bdev, sector_t block);

void mark_buffer_async_write(struct buffer_head *bh);
void __wait_on_buffer(struct buffer_head *);
wait_queue_head_t *bh_waitq_head(struct buffer_head *bh);
struct buffer_head *__find_get_block(struct block_device *bdev, sector_t block,
			unsigned size);
struct buffer_head *__getblk_gfp(struct block_device *bdev, sector_t block,
				  unsigned size, gfp_t gfp);
void __brelse(struct buffer_head *);
void __bforget(struct buffer_head *);
void __breadahead(struct block_device *, sector_t block, unsigned int size);
struct buffer_head *__bread_gfp(struct block_device *,
				sector_t block, unsigned size, gfp_t gfp);
void invalidate_bh_lrus(void);
struct buffer_head *alloc_buffer_head(gfp_t gfp_flags);
void free_buffer_head(struct buffer_head * bh);
void unlock_buffer(struct buffer_head *bh);
void __lock_buffer(struct buffer_head *bh);
void ll_rw_block(int, int, int, struct buffer_head * bh[]);
int sync_dirty_buffer(struct buffer_head *bh);
int __sync_dirty_buffer(struct buffer_head *bh, int op_flags);
void write_dirty_buffer(struct buffer_head *bh, int op_flags);
int _submit_bh(int op, int op_flags, struct buffer_head *bh,
	       unsigned long bio_flags);
int submit_bh(int, int, struct buffer_head *);
void write_boundary_block(struct block_device *bdev,
			sector_t bblock, unsigned blocksize);
int bh_uptodate_or_lock(struct buffer_head *bh);
int bh_submit_read(struct buffer_head *bh);

extern int buffer_heads_over_limit;


void block_invalidatepage(struct page *page, unsigned int offset,
			  unsigned int length);
int block_write_full_page(struct page *page, get_block_t *get_block,
				struct writeback_control *wbc);
int __block_write_full_page(struct inode *inode, struct page *page,
			get_block_t *get_block, struct writeback_control *wbc,
			bh_end_io_t *handler);
int block_read_full_page(struct page*, get_block_t*);
int block_is_partially_uptodate(struct page *page, unsigned long from,
				unsigned long count);
int block_write_begin(struct address_space *mapping, loff_t pos, unsigned len,
		unsigned flags, struct page **pagep, get_block_t *get_block);
int __block_write_begin(struct page *page, loff_t pos, unsigned len,
		get_block_t *get_block);
int block_write_end(struct file *, struct address_space *,
				loff_t, unsigned, unsigned,
				struct page *, void *);
int generic_write_end(struct file *, struct address_space *,
				loff_t, unsigned, unsigned,
				struct page *, void *);
void page_zero_new_buffers(struct page *page, unsigned from, unsigned to);
void clean_page_buffers(struct page *page);
int cont_write_begin(struct file *, struct address_space *, loff_t,
			unsigned, unsigned, struct page **, void **,
			get_block_t *, loff_t *);
int generic_cont_expand_simple(struct inode *inode, loff_t size);
int block_commit_write(struct page *page, unsigned from, unsigned to);
int block_page_mkwrite(struct vm_area_struct *vma, struct vm_fault *vmf,
				get_block_t get_block);

static inline int block_page_mkwrite_return(int err)
{
	if (err == 0)
		return VM_FAULT_LOCKED;
	if (err == -EFAULT || err == -EAGAIN)
		return VM_FAULT_NOPAGE;
	if (err == -ENOMEM)
		return VM_FAULT_OOM;
	
	return VM_FAULT_SIGBUS;
}
sector_t generic_block_bmap(struct address_space *, sector_t, get_block_t *);
int block_truncate_page(struct address_space *, loff_t, get_block_t *);
int nobh_write_begin(struct address_space *, loff_t, unsigned, unsigned,
				struct page **, void **, get_block_t*);
int nobh_write_end(struct file *, struct address_space *,
				loff_t, unsigned, unsigned,
				struct page *, void *);
int nobh_truncate_page(struct address_space *, loff_t, get_block_t *);
int nobh_writepage(struct page *page, get_block_t *get_block,
                        struct writeback_control *wbc);

void buffer_init(void);



static inline void attach_page_buffers(struct page *page,
		struct buffer_head *head)
{
	get_page(page);
	SetPagePrivate(page);
	set_page_private(page, (unsigned long)head);
}

static inline void get_bh(struct buffer_head *bh)
{
        atomic_inc(&bh->b_count);
}

static inline void put_bh(struct buffer_head *bh)
{
        smp_mb__before_atomic();
        atomic_dec(&bh->b_count);
}

static inline void brelse(struct buffer_head *bh)
{
	if (bh)
		__brelse(bh);
}

static inline void bforget(struct buffer_head *bh)
{
	if (bh)
		__bforget(bh);
}

static inline struct buffer_head *
sb_bread(struct super_block *sb, sector_t block)
{
	return __bread_gfp(sb->s_bdev, block, sb->s_blocksize, __GFP_MOVABLE);
}

static inline struct buffer_head *
sb_bread_unmovable(struct super_block *sb, sector_t block)
{
	return __bread_gfp(sb->s_bdev, block, sb->s_blocksize, 0);
}

static inline void
sb_breadahead(struct super_block *sb, sector_t block)
{
	__breadahead(sb->s_bdev, block, sb->s_blocksize);
}

static inline struct buffer_head *
sb_getblk(struct super_block *sb, sector_t block)
{
	return __getblk_gfp(sb->s_bdev, block, sb->s_blocksize, __GFP_MOVABLE);
}


static inline struct buffer_head *
sb_getblk_gfp(struct super_block *sb, sector_t block, gfp_t gfp)
{
	return __getblk_gfp(sb->s_bdev, block, sb->s_blocksize, gfp);
}

static inline struct buffer_head *
sb_find_get_block(struct super_block *sb, sector_t block)
{
	return __find_get_block(sb->s_bdev, block, sb->s_blocksize);
}

static inline void
map_bh(struct buffer_head *bh, struct super_block *sb, sector_t block)
{
	set_buffer_mapped(bh);
	bh->b_bdev = sb->s_bdev;
	bh->b_blocknr = block;
	bh->b_size = sb->s_blocksize;
}

static inline void wait_on_buffer(struct buffer_head *bh)
{
	might_sleep();
	if (buffer_locked(bh))
		__wait_on_buffer(bh);
}

static inline int trylock_buffer(struct buffer_head *bh)
{
	return likely(!test_and_set_bit_lock(BH_Lock, &bh->b_state));
}

static inline void lock_buffer(struct buffer_head *bh)
{
	might_sleep();
	if (!trylock_buffer(bh))
		__lock_buffer(bh);
}

static inline struct buffer_head *getblk_unmovable(struct block_device *bdev,
						   sector_t block,
						   unsigned size)
{
	return __getblk_gfp(bdev, block, size, 0);
}

static inline struct buffer_head *__getblk(struct block_device *bdev,
					   sector_t block,
					   unsigned size)
{
	return __getblk_gfp(bdev, block, size, __GFP_MOVABLE);
}


static inline struct buffer_head *
__bread(struct block_device *bdev, sector_t block, unsigned size)
{
	return __bread_gfp(bdev, block, size, __GFP_MOVABLE);
}

extern int __set_page_dirty_buffers(struct page *page);

#else 

static inline void buffer_init(void) {}
static inline int try_to_free_buffers(struct page *page) { return 1; }
static inline int inode_has_buffers(struct inode *inode) { return 0; }
static inline void invalidate_inode_buffers(struct inode *inode) {}
static inline int remove_inode_buffers(struct inode *inode) { return 1; }
static inline int sync_mapping_buffers(struct address_space *mapping) { return 0; }

#endif 
#endif 
