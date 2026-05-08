#ifndef LINUX_EXPORTFS_H
#define LINUX_EXPORTFS_H 1

#include <linux/types.h>

struct dentry;
struct iattr;
struct inode;
struct iomap;
struct super_block;
struct vfsmount;


#define MAX_HANDLE_SZ 128


enum fid_type {
	
	FILEID_ROOT = 0,

	
	FILEID_INO32_GEN = 1,

	
	FILEID_INO32_GEN_PARENT = 2,

	
	FILEID_BTRFS_WITHOUT_PARENT = 0x4d,

	
	FILEID_BTRFS_WITH_PARENT = 0x4e,

	
	FILEID_BTRFS_WITH_PARENT_ROOT = 0x4f,

	
	FILEID_UDF_WITHOUT_PARENT = 0x51,

	
	FILEID_UDF_WITH_PARENT = 0x52,

	
	FILEID_NILFS_WITHOUT_PARENT = 0x61,

	
	FILEID_NILFS_WITH_PARENT = 0x62,

	
	FILEID_FAT_WITHOUT_PARENT = 0x71,

	
	FILEID_FAT_WITH_PARENT = 0x72,

	
	FILEID_LUSTRE = 0x97,

	
	FILEID_INVALID = 0xff,
};

struct fid {
	union {
		struct {
			u32 ino;
			u32 gen;
			u32 parent_ino;
			u32 parent_gen;
		} i32;
 		struct {
 			u32 block;
 			u16 partref;
 			u16 parent_partref;
 			u32 generation;
 			u32 parent_block;
 			u32 parent_generation;
 		} udf;
		__u32 raw[0];
	};
};



struct export_operations {
	int (*encode_fh)(struct inode *inode, __u32 *fh, int *max_len,
			struct inode *parent);
	struct dentry * (*fh_to_dentry)(struct super_block *sb, struct fid *fid,
			int fh_len, int fh_type);
	struct dentry * (*fh_to_parent)(struct super_block *sb, struct fid *fid,
			int fh_len, int fh_type);
	int (*get_name)(struct dentry *parent, char *name,
			struct dentry *child);
	struct dentry * (*get_parent)(struct dentry *child);
	int (*commit_metadata)(struct inode *inode);

	int (*get_uuid)(struct super_block *sb, u8 *buf, u32 *len, u64 *offset);
	int (*map_blocks)(struct inode *inode, loff_t offset,
			  u64 len, struct iomap *iomap,
			  bool write, u32 *device_generation);
	int (*commit_blocks)(struct inode *inode, struct iomap *iomaps,
			     int nr_iomaps, struct iattr *iattr);
};

extern int exportfs_encode_inode_fh(struct inode *inode, struct fid *fid,
				    int *max_len, struct inode *parent);
extern int exportfs_encode_fh(struct dentry *dentry, struct fid *fid,
	int *max_len, int connectable);
extern struct dentry *exportfs_decode_fh(struct vfsmount *mnt, struct fid *fid,
	int fh_len, int fileid_type, int (*acceptable)(void *, struct dentry *),
	void *context);


extern struct dentry *generic_fh_to_dentry(struct super_block *sb,
	struct fid *fid, int fh_len, int fh_type,
	struct inode *(*get_inode) (struct super_block *sb, u64 ino, u32 gen));
extern struct dentry *generic_fh_to_parent(struct super_block *sb,
	struct fid *fid, int fh_len, int fh_type,
	struct inode *(*get_inode) (struct super_block *sb, u64 ino, u32 gen));

#endif 
