#ifndef LINUX_IOMAP_H
#define LINUX_IOMAP_H 1

#include <linux/types.h>

struct fiemap_extent_info;
struct inode;
struct iov_iter;
struct kiocb;
struct vm_area_struct;
struct vm_fault;


#define IOMAP_HOLE	0x01	
#define IOMAP_DELALLOC	0x02	
#define IOMAP_MAPPED	0x03	
#define IOMAP_UNWRITTEN	0x04	


#define IOMAP_F_NEW	0x01	


#define IOMAP_F_MERGED	0x10	
#define IOMAP_F_SHARED	0x20	


#define IOMAP_NULL_BLOCK -1LL	

struct iomap {
	sector_t		blkno;	
	loff_t			offset;	
	u64			length;	
	u16			type;	
	u16			flags;	
	struct block_device	*bdev;	
};


#define IOMAP_WRITE		(1 << 0) 
#define IOMAP_ZERO		(1 << 1) 
#define IOMAP_REPORT		(1 << 2) 

struct iomap_ops {
	
	int (*iomap_begin)(struct inode *inode, loff_t pos, loff_t length,
			unsigned flags, struct iomap *iomap);

	
	int (*iomap_end)(struct inode *inode, loff_t pos, loff_t length,
			ssize_t written, unsigned flags, struct iomap *iomap);
};

ssize_t iomap_file_buffered_write(struct kiocb *iocb, struct iov_iter *from,
		struct iomap_ops *ops);
int iomap_file_dirty(struct inode *inode, loff_t pos, loff_t len,
		struct iomap_ops *ops);
int iomap_zero_range(struct inode *inode, loff_t pos, loff_t len,
		bool *did_zero, struct iomap_ops *ops);
int iomap_truncate_page(struct inode *inode, loff_t pos, bool *did_zero,
		struct iomap_ops *ops);
int iomap_page_mkwrite(struct vm_area_struct *vma, struct vm_fault *vmf,
		struct iomap_ops *ops);
int iomap_fiemap(struct inode *inode, struct fiemap_extent_info *fieinfo,
		loff_t start, loff_t len, struct iomap_ops *ops);

#endif 
