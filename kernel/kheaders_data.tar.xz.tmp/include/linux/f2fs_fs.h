// SPDX-License-Identifier: GPL-2.0

#ifndef _LINUX_F2FS_FS_H
#define _LINUX_F2FS_FS_H

#include <linux/pagemap.h>
#include <linux/types.h>

#define F2FS_SUPER_OFFSET		1024	
#define F2FS_MIN_LOG_SECTOR_SIZE	9	
#define F2FS_MAX_LOG_SECTOR_SIZE	12	
#define F2FS_LOG_SECTORS_PER_BLOCK	3	
#define F2FS_BLKSIZE			4096	
#define F2FS_BLKSIZE_BITS		12	
#define F2FS_MAX_EXTENSION		64	
#define F2FS_EXTENSION_LEN		8	
#define F2FS_BLK_ALIGN(x)	(((x) + F2FS_BLKSIZE - 1) >> F2FS_BLKSIZE_BITS)

#define NULL_ADDR		((block_t)0)	
#define NEW_ADDR		((block_t)-1)	

#define F2FS_BYTES_TO_BLK(bytes)	((bytes) >> F2FS_BLKSIZE_BITS)
#define F2FS_BLK_TO_BYTES(blk)		((blk) << F2FS_BLKSIZE_BITS)


#define F2FS_RESERVED_NODE_NUM		3

#define F2FS_ROOT_INO(sbi)	((sbi)->root_ino_num)
#define F2FS_NODE_INO(sbi)	((sbi)->node_ino_num)
#define F2FS_META_INO(sbi)	((sbi)->meta_ino_num)

#define F2FS_MAX_QUOTAS		3

#define F2FS_IO_SIZE(sbi)	(1 << F2FS_OPTION(sbi).write_io_size_bits) 
#define F2FS_IO_SIZE_KB(sbi)	(1 << (F2FS_OPTION(sbi).write_io_size_bits + 2)) 
#define F2FS_IO_SIZE_BYTES(sbi)	(1 << (F2FS_OPTION(sbi).write_io_size_bits + 12)) 
#define F2FS_IO_SIZE_BITS(sbi)	(F2FS_OPTION(sbi).write_io_size_bits) 
#define F2FS_IO_SIZE_MASK(sbi)	(F2FS_IO_SIZE(sbi) - 1)


#define GFP_F2FS_ZERO		(GFP_NOFS | __GFP_ZERO)


#define MAX_ACTIVE_LOGS	16
#define MAX_ACTIVE_NODE_LOGS	8
#define MAX_ACTIVE_DATA_LOGS	8

#define VERSION_LEN	256
#define MAX_VOLUME_NAME		512
#define MAX_PATH_LEN		64
#define MAX_DEVICES		8


struct f2fs_device {
	__u8 path[MAX_PATH_LEN];
	__le32 total_segments;
} __packed;

struct f2fs_super_block {
	__le32 magic;			
	__le16 major_ver;		
	__le16 minor_ver;		
	__le32 log_sectorsize;		
	__le32 log_sectors_per_block;	
	__le32 log_blocksize;		
	__le32 log_blocks_per_seg;	
	__le32 segs_per_sec;		
	__le32 secs_per_zone;		
	__le32 checksum_offset;		
	__le64 block_count;		
	__le32 section_count;		
	__le32 segment_count;		
	__le32 segment_count_ckpt;	
	__le32 segment_count_sit;	
	__le32 segment_count_nat;	
	__le32 segment_count_ssa;	
	__le32 segment_count_main;	
	__le32 segment0_blkaddr;	
	__le32 cp_blkaddr;		
	__le32 sit_blkaddr;		
	__le32 nat_blkaddr;		
	__le32 ssa_blkaddr;		
	__le32 main_blkaddr;		
	__le32 root_ino;		
	__le32 node_ino;		
	__le32 meta_ino;		
	__u8 uuid[16];			
	__le16 volume_name[MAX_VOLUME_NAME];	
	__le32 extension_count;		
	__u8 extension_list[F2FS_MAX_EXTENSION][F2FS_EXTENSION_LEN];
	__le32 cp_payload;
	__u8 version[VERSION_LEN];	
	__u8 init_version[VERSION_LEN];	
	__le32 feature;			
	__u8 encryption_level;		
	__u8 encrypt_pw_salt[16];	
	struct f2fs_device devs[MAX_DEVICES];	
	__le32 qf_ino[F2FS_MAX_QUOTAS];	
	__u8 hot_ext_count;		
	__u8 reserved[246];		
	__u8 mount_opts[64];            
	__le32 crc;			
} __packed;


#define CP_DISABLED_QUICK_FLAG		0x00002000
#define CP_DISABLED_FLAG		0x00001000
#define CP_QUOTA_NEED_FSCK_FLAG		0x00000800
#define CP_LARGE_NAT_BITMAP_FLAG	0x00000400
#define CP_NOCRC_RECOVERY_FLAG	0x00000200
#define CP_TRIMMED_FLAG		0x00000100
#define CP_NAT_BITS_FLAG	0x00000080
#define CP_CRC_RECOVERY_FLAG	0x00000040
#define CP_FASTBOOT_FLAG	0x00000020
#define CP_FSCK_FLAG		0x00000010
#define CP_ERROR_FLAG		0x00000008
#define CP_COMPACT_SUM_FLAG	0x00000004
#define CP_ORPHAN_PRESENT_FLAG	0x00000002
#define CP_UMOUNT_FLAG		0x00000001

#define F2FS_CP_PACKS		2	

struct f2fs_checkpoint {
	__le64 checkpoint_ver;		
	__le64 user_block_count;	
	__le64 valid_block_count;	
	__le32 rsvd_segment_count;	
	__le32 overprov_segment_count;	
	__le32 free_segment_count;	

	
	__le32 cur_node_segno[MAX_ACTIVE_NODE_LOGS];
	__le16 cur_node_blkoff[MAX_ACTIVE_NODE_LOGS];
	
	__le32 cur_data_segno[MAX_ACTIVE_DATA_LOGS];
	__le16 cur_data_blkoff[MAX_ACTIVE_DATA_LOGS];
	__le32 ckpt_flags;		
	__le32 cp_pack_total_block_count;	
	__le32 cp_pack_start_sum;	
	__le32 valid_node_count;	
	__le32 valid_inode_count;	
	__le32 next_free_nid;		
	__le32 sit_ver_bitmap_bytesize;	
	__le32 nat_ver_bitmap_bytesize; 
	__le32 checksum_offset;		
	__le64 elapsed_time;		
	
	unsigned char alloc_type[MAX_ACTIVE_LOGS];

	
	unsigned char sit_nat_version_bitmap[1];
} __packed;


#define F2FS_ORPHANS_PER_BLOCK	1020

#define GET_ORPHAN_BLOCKS(n)	(((n) + F2FS_ORPHANS_PER_BLOCK - 1) / \
					F2FS_ORPHANS_PER_BLOCK)

struct f2fs_orphan_block {
	__le32 ino[F2FS_ORPHANS_PER_BLOCK];	
	__le32 reserved;	
	__le16 blk_addr;	
	__le16 blk_count;	
	__le32 entry_count;	
	__le32 check_sum;	
} __packed;


struct f2fs_extent {
	__le32 fofs;		
	__le32 blk;		
	__le32 len;		
} __packed;

#define F2FS_NAME_LEN		255

#define DEFAULT_INLINE_XATTR_ADDRS	50
#define DEF_ADDRS_PER_INODE	923	
#define CUR_ADDRS_PER_INODE(inode)	(DEF_ADDRS_PER_INODE - \
					get_extra_isize(inode))
#define DEF_NIDS_PER_INODE	5	
#define ADDRS_PER_INODE(inode)	addrs_per_inode(inode)
#define ADDRS_PER_BLOCK		1018	
#define NIDS_PER_BLOCK		1018	

#define ADDRS_PER_PAGE(page, inode)	\
	(IS_INODE(page) ? ADDRS_PER_INODE(inode) : ADDRS_PER_BLOCK)

#define	NODE_DIR1_BLOCK		(DEF_ADDRS_PER_INODE + 1)
#define	NODE_DIR2_BLOCK		(DEF_ADDRS_PER_INODE + 2)
#define	NODE_IND1_BLOCK		(DEF_ADDRS_PER_INODE + 3)
#define	NODE_IND2_BLOCK		(DEF_ADDRS_PER_INODE + 4)
#define	NODE_DIND_BLOCK		(DEF_ADDRS_PER_INODE + 5)

#define F2FS_INLINE_XATTR	0x01	
#define F2FS_INLINE_DATA	0x02	
#define F2FS_INLINE_DENTRY	0x04	
#define F2FS_DATA_EXIST		0x08	
#define F2FS_INLINE_DOTS	0x10	
#define F2FS_EXTRA_ATTR		0x20	
#define F2FS_PIN_FILE		0x40	

struct f2fs_inode {
	__le16 i_mode;			
	__u8 i_advise;			
	__u8 i_inline;			
	__le32 i_uid;			
	__le32 i_gid;			
	__le32 i_links;			
	__le64 i_size;			
	__le64 i_blocks;		
	__le64 i_atime;			
	__le64 i_ctime;			
	__le64 i_mtime;			
	__le32 i_atime_nsec;		
	__le32 i_ctime_nsec;		
	__le32 i_mtime_nsec;		
	__le32 i_generation;		
	union {
		__le32 i_current_depth;	
		__le16 i_gc_failures;	
	};
	__le32 i_xattr_nid;		
	__le32 i_flags;			
	__le32 i_pino;			
	__le32 i_namelen;		
	__u8 i_name[F2FS_NAME_LEN];	
	__u8 i_dir_level;		

	struct f2fs_extent i_ext;	

	union {
		struct {
			__le16 i_extra_isize;	
			__le16 i_inline_xattr_size;	
			__le32 i_projid;	
			__le32 i_inode_checksum;
			__le64 i_crtime;	
			__le32 i_crtime_nsec;	
			__le32 i_extra_end[0];	
		} __packed;
		__le32 i_addr[DEF_ADDRS_PER_INODE];	
	};
	__le32 i_nid[DEF_NIDS_PER_INODE];	
} __packed;

struct direct_node {
	__le32 addr[ADDRS_PER_BLOCK];	
} __packed;

struct indirect_node {
	__le32 nid[NIDS_PER_BLOCK];	
} __packed;

enum {
	COLD_BIT_SHIFT = 0,
	FSYNC_BIT_SHIFT,
	DENT_BIT_SHIFT,
	OFFSET_BIT_SHIFT
};

#define OFFSET_BIT_MASK		(0x07)	

struct node_footer {
	__le32 nid;		
	__le32 ino;		
	__le32 flag;		
	__le64 cp_ver;		
	__le32 next_blkaddr;	
} __packed;

struct f2fs_node {
	
	union {
		struct f2fs_inode i;
		struct direct_node dn;
		struct indirect_node in;
	};
	struct node_footer footer;
} __packed;


#define NAT_ENTRY_PER_BLOCK (PAGE_SIZE / sizeof(struct f2fs_nat_entry))

struct f2fs_nat_entry {
	__u8 version;		
	__le32 ino;		
	__le32 block_addr;	
} __packed;

struct f2fs_nat_block {
	struct f2fs_nat_entry entries[NAT_ENTRY_PER_BLOCK];
} __packed;


#define SIT_VBLOCK_MAP_SIZE 64
#define SIT_ENTRY_PER_BLOCK (PAGE_SIZE / sizeof(struct f2fs_sit_entry))


#define F2FS_MAX_SEGMENT       ((16 * 1024 * 1024) / 2)


#define SIT_VBLOCKS_SHIFT	10
#define SIT_VBLOCKS_MASK	((1 << SIT_VBLOCKS_SHIFT) - 1)
#define GET_SIT_VBLOCKS(raw_sit)				\
	(le16_to_cpu((raw_sit)->vblocks) & SIT_VBLOCKS_MASK)
#define GET_SIT_TYPE(raw_sit)					\
	((le16_to_cpu((raw_sit)->vblocks) & ~SIT_VBLOCKS_MASK)	\
	 >> SIT_VBLOCKS_SHIFT)

struct f2fs_sit_entry {
	__le16 vblocks;				
	__u8 valid_map[SIT_VBLOCK_MAP_SIZE];	
	__le64 mtime;				
} __packed;

struct f2fs_sit_block {
	struct f2fs_sit_entry entries[SIT_ENTRY_PER_BLOCK];
} __packed;


#define ENTRIES_IN_SUM		512
#define	SUMMARY_SIZE		(7)	
#define	SUM_FOOTER_SIZE		(5)	
#define SUM_ENTRY_SIZE		(SUMMARY_SIZE * ENTRIES_IN_SUM)


struct f2fs_summary {
	__le32 nid;		
	union {
		__u8 reserved[3];
		struct {
			__u8 version;		
			__le16 ofs_in_node;	
		} __packed;
	};
} __packed;


#define SUM_TYPE_NODE		(1)
#define SUM_TYPE_DATA		(0)

struct summary_footer {
	unsigned char entry_type;	
	__le32 check_sum;		
} __packed;

#define SUM_JOURNAL_SIZE	(F2FS_BLKSIZE - SUM_FOOTER_SIZE -\
				SUM_ENTRY_SIZE)
#define NAT_JOURNAL_ENTRIES	((SUM_JOURNAL_SIZE - 2) /\
				sizeof(struct nat_journal_entry))
#define NAT_JOURNAL_RESERVED	((SUM_JOURNAL_SIZE - 2) %\
				sizeof(struct nat_journal_entry))
#define SIT_JOURNAL_ENTRIES	((SUM_JOURNAL_SIZE - 2) /\
				sizeof(struct sit_journal_entry))
#define SIT_JOURNAL_RESERVED	((SUM_JOURNAL_SIZE - 2) %\
				sizeof(struct sit_journal_entry))


#define EXTRA_INFO_RESERVED	(SUM_JOURNAL_SIZE - 2 - 8)


enum {
	NAT_JOURNAL = 0,
	SIT_JOURNAL
};

struct nat_journal_entry {
	__le32 nid;
	struct f2fs_nat_entry ne;
} __packed;

struct nat_journal {
	struct nat_journal_entry entries[NAT_JOURNAL_ENTRIES];
	__u8 reserved[NAT_JOURNAL_RESERVED];
} __packed;

struct sit_journal_entry {
	__le32 segno;
	struct f2fs_sit_entry se;
} __packed;

struct sit_journal {
	struct sit_journal_entry entries[SIT_JOURNAL_ENTRIES];
	__u8 reserved[SIT_JOURNAL_RESERVED];
} __packed;

struct f2fs_extra_info {
	__le64 kbytes_written;
	__u8 reserved[EXTRA_INFO_RESERVED];
} __packed;

struct f2fs_journal {
	union {
		__le16 n_nats;
		__le16 n_sits;
	};
	
	union {
		struct nat_journal nat_j;
		struct sit_journal sit_j;
		struct f2fs_extra_info info;
	};
} __packed;


struct f2fs_summary_block {
	struct f2fs_summary entries[ENTRIES_IN_SUM];
	struct f2fs_journal journal;
	struct summary_footer footer;
} __packed;


#define F2FS_DOT_HASH		0
#define F2FS_DDOT_HASH		F2FS_DOT_HASH
#define F2FS_MAX_HASH		(~((0x3ULL) << 62))
#define F2FS_HASH_COL_BIT	((0x1ULL) << 63)

typedef __le32	f2fs_hash_t;


#define F2FS_SLOT_LEN		8
#define F2FS_SLOT_LEN_BITS	3

#define GET_DENTRY_SLOTS(x) (((x) + F2FS_SLOT_LEN - 1) >> F2FS_SLOT_LEN_BITS)


#define MAX_DIR_HASH_DEPTH	63


#define MAX_DIR_BUCKETS		(1 << ((MAX_DIR_HASH_DEPTH / 2) - 1))


#define NR_DENTRY_IN_BLOCK	214	
#define SIZE_OF_DIR_ENTRY	11	
#define SIZE_OF_DENTRY_BITMAP	((NR_DENTRY_IN_BLOCK + BITS_PER_BYTE - 1) / \
					BITS_PER_BYTE)
#define SIZE_OF_RESERVED	(PAGE_SIZE - ((SIZE_OF_DIR_ENTRY + \
				F2FS_SLOT_LEN) * \
				NR_DENTRY_IN_BLOCK + SIZE_OF_DENTRY_BITMAP))
#define MIN_INLINE_DENTRY_SIZE		40	


struct f2fs_dir_entry {
	__le32 hash_code;	
	__le32 ino;		
	__le16 name_len;	
	__u8 file_type;		
} __packed;


struct f2fs_dentry_block {
	
	__u8 dentry_bitmap[SIZE_OF_DENTRY_BITMAP];
	__u8 reserved[SIZE_OF_RESERVED];
	struct f2fs_dir_entry dentry[NR_DENTRY_IN_BLOCK];
	__u8 filename[NR_DENTRY_IN_BLOCK][F2FS_SLOT_LEN];
} __packed;


enum {
	F2FS_FT_UNKNOWN,
	F2FS_FT_REG_FILE,
	F2FS_FT_DIR,
	F2FS_FT_CHRDEV,
	F2FS_FT_BLKDEV,
	F2FS_FT_FIFO,
	F2FS_FT_SOCK,
	F2FS_FT_SYMLINK,
	F2FS_FT_MAX
};

#define S_SHIFT 12

#define	F2FS_DEF_PROJID		0	

#define	F2FS_SEC_EXTRA_FSCK_MAGIC	0xF5CE45EC
struct f2fs_sb_extra_flag_blk {
	__le32 need_fsck;
	__le32 spo_counter;
	__le64 fsck_read_bytes;
	__le64 fsck_written_bytes;
	__le64 fsck_elapsed_time;
	__le32 fsck_exit_code;
	__le32 valid_node_count;
	__le32 valid_inode_count;
	__u8   rsvd[4052];
} __packed;

#endif  
