
#ifndef _LINUX_FSCRYPT_H
#error "Incorrect include of linux/fscrypt_supp.h!"
#endif

#ifndef _LINUX_FSCRYPT_SUPP_H
#define _LINUX_FSCRYPT_SUPP_H

#include <linux/mm.h>
#include <linux/slab.h>


#define FS_CFLG_OWN_PAGES (1U << 1)


struct fscrypt_operations {
	unsigned int flags;
	const char *key_prefix;
	int (*get_context)(struct inode *, void *, size_t);
	int (*set_context)(struct inode *, const void *, size_t, void *);
#if defined(CONFIG_DDAR) || defined(CONFIG_FSCRYPT_SDP)
	int (*get_knox_context)(struct inode *, const char *, void *, size_t);
    int (*set_knox_context)(struct inode *, const char *, const void *, size_t, void *);
#endif
	bool (*dummy_context)(struct inode *);
	bool (*empty_dir)(struct inode *);
	unsigned int max_namelen;
};

struct fscrypt_ctx {
	union {
		struct {
			struct page *bounce_page;	
			struct page *control_page;	
		} w;
		struct {
			struct bio *bio;
			struct work_struct work;
		} r;
		struct list_head free_list;	
	};
	u8 flags;				
};

static inline bool fscrypt_has_encryption_key(const struct inode *inode)
{
	return (inode->i_crypt_info != NULL);
}

static inline bool fscrypt_dummy_context_enabled(struct inode *inode)
{
	return inode->i_sb->s_cop->dummy_context &&
		inode->i_sb->s_cop->dummy_context(inode);
}


extern void fscrypt_enqueue_decrypt_work(struct work_struct *);
extern struct fscrypt_ctx *fscrypt_get_ctx(const struct inode *, gfp_t);
extern void fscrypt_release_ctx(struct fscrypt_ctx *);
extern struct page *fscrypt_encrypt_page(const struct inode *, struct page *,
						unsigned int, unsigned int,
						u64, gfp_t);
extern int fscrypt_decrypt_page(const struct inode *, struct page *, unsigned int,
				unsigned int, u64);

static inline struct page *fscrypt_control_page(struct page *page)
{
	return ((struct fscrypt_ctx *)page_private(page))->w.control_page;
}

extern void fscrypt_restore_control_page(struct page *);


extern int fscrypt_ioctl_set_policy(struct file *, const void __user *);
extern int fscrypt_ioctl_get_policy(struct file *, void __user *);
extern int fscrypt_has_permitted_context(struct inode *, struct inode *);
extern int fscrypt_inherit_context(struct inode *, struct inode *,
					void *, bool);

extern int fscrypt_get_encryption_info(struct inode *);
extern void fscrypt_put_encryption_info(struct inode *);
#ifdef CONFIG_FSCRYPT_SDP
extern int fscrypt_get_encryption_key(struct inode *inode,
						struct fscrypt_key *key);
extern int fscrypt_get_encryption_kek(struct inode *inode,
						struct fscrypt_info *crypt_info,
						struct fscrypt_key *kek);
#endif


extern int fscrypt_setup_filename(struct inode *, const struct qstr *,
				int lookup, struct fscrypt_name *);

static inline void fscrypt_free_filename(struct fscrypt_name *fname)
{
	kfree(fname->crypto_buf.name);
}

extern int fscrypt_fname_alloc_buffer(const struct inode *, u32,
				struct fscrypt_str *);
extern void fscrypt_fname_free_buffer(struct fscrypt_str *);
extern int fscrypt_fname_disk_to_usr(struct inode *, u32, u32,
			const struct fscrypt_str *, struct fscrypt_str *);

#define FSCRYPT_FNAME_MAX_UNDIGESTED_SIZE	32


#define FSCRYPT_FNAME_DIGEST(name, len)	\
	((name) + round_down((len) - FS_CRYPTO_BLOCK_SIZE - 1, \
			     FS_CRYPTO_BLOCK_SIZE))

#define FSCRYPT_FNAME_DIGEST_SIZE	FS_CRYPTO_BLOCK_SIZE


struct fscrypt_digested_name {
	u32 hash;
	u32 minor_hash;
	u8 digest[FSCRYPT_FNAME_DIGEST_SIZE];
};


static inline bool fscrypt_match_name(const struct fscrypt_name *fname,
				      const u8 *de_name, u32 de_name_len)
{
	if (unlikely(!fname->disk_name.name)) {
		const struct fscrypt_digested_name *n =
			(const void *)fname->crypto_buf.name;
		if (WARN_ON_ONCE(fname->usr_fname->name[0] != '_'))
			return false;
		if (de_name_len <= FSCRYPT_FNAME_MAX_UNDIGESTED_SIZE)
			return false;
		return !memcmp(FSCRYPT_FNAME_DIGEST(de_name, de_name_len),
			       n->digest, FSCRYPT_FNAME_DIGEST_SIZE);
	}

	if (de_name_len != fname->disk_name.len)
		return false;
	return !memcmp(de_name, fname->disk_name.name, fname->disk_name.len);
}


extern void fscrypt_decrypt_bio(struct bio *);
extern void fscrypt_enqueue_decrypt_bio(struct fscrypt_ctx *ctx,
					struct bio *bio);
extern void fscrypt_pullback_bio_page(struct page **, bool);
extern int fscrypt_zeroout_range(const struct inode *, pgoff_t, sector_t,
				 unsigned int);
void fscrypt_set_bio(const struct inode *inode, struct bio *bio, u64 dun);
void *fscrypt_get_diskcipher(const struct inode *inode);
int fscrypt_disk_encrypted(const struct inode *inode);


extern int fscrypt_file_open(struct inode *inode, struct file *filp);
extern int __fscrypt_prepare_link(struct inode *inode, struct inode *dir);
extern int __fscrypt_prepare_rename(struct inode *old_dir,
				    struct dentry *old_dentry,
				    struct inode *new_dir,
				    struct dentry *new_dentry,
				    unsigned int flags);
extern int __fscrypt_prepare_lookup(struct inode *dir, struct dentry *dentry);
extern int __fscrypt_prepare_symlink(struct inode *dir, unsigned int len,
				     unsigned int max_len,
				     struct fscrypt_str *disk_link);
extern int __fscrypt_encrypt_symlink(struct inode *inode, const char *target,
				     unsigned int len,
				     struct fscrypt_str *disk_link);
extern const char *fscrypt_get_symlink(struct inode *inode, const void *caddr,
				       unsigned int max_size,
				       struct delayed_call *done);

#ifdef CONFIG_DDAR
extern int fscrypt_dd_decrypt_page(struct inode *inode, struct page *page);
extern int fscrypt_dd_encrypted(struct bio *bio);
extern int fscrypt_dd_encrypted_inode(const struct inode *inode);
extern int fscrypt_dd_is_traced_inode(const struct inode *inode);
extern void fscrypt_dd_trace_inode(const struct inode *inode);
extern long fscrypt_dd_get_ino(struct bio *bio);
extern long fscrypt_dd_ioctl(unsigned int cmd, unsigned long *arg, struct inode *inode);
extern int fscrypt_dd_submit_bio(struct inode *inode, struct bio *bio);
extern int fscrypt_dd_may_submit_bio(struct bio *bio);
extern struct inode *fscrypt_bio_get_inode(const struct bio *bio);
extern bool fscrypt_dd_can_merge_bio(struct bio *bio, struct address_space *mapping);
#endif

#endif	
