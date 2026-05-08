
#ifndef _LINUX_FSCRYPT_H
#define _LINUX_FSCRYPT_H

#include <linux/fs.h>

#define FS_CRYPTO_BLOCK_SIZE		16

struct fscrypt_ctx;

#ifndef __FS_HAS_ENCRYPTION
#define __FS_HAS_ENCRYPTION (IS_ENABLED(CONFIG_EXT4_FS_ENCRYPTION) ||	\
			IS_ENABLED(CONFIG_F2FS_FS_ENCRYPTION) ||	\
			IS_ENABLED(CONFIG_UBIFS_FS_ENCRYPTION))
#endif

struct fscrypt_info;

struct fscrypt_str {
	unsigned char *name;
	u32 len;
};

struct fscrypt_name {
	const struct qstr *usr_fname;
	struct fscrypt_str disk_name;
	u32 hash;
	u32 minor_hash;
	struct fscrypt_str crypto_buf;
};

#define FSTR_INIT(n, l)		{ .name = n, .len = l }
#define FSTR_TO_QSTR(f)		QSTR_INIT((f)->name, (f)->len)
#define fname_name(p)		((p)->disk_name.name)
#define fname_len(p)		((p)->disk_name.len)

#if defined(CONFIG_FSCRYPT_SDP) || defined(CONFIG_DDAR)
#define FSCRYPT_SET_CONTEXT_MAX_SIZE	32
#endif

#if __FS_HAS_ENCRYPTION
#include <linux/fscrypt_supp.h>
#else
#include <linux/fscrypt_notsupp.h>
#endif


static inline int fscrypt_require_key(struct inode *inode)
{
	if (IS_ENCRYPTED(inode)) {
		int err = fscrypt_get_encryption_info(inode);

		if (err)
			return err;
		if (!fscrypt_has_encryption_key(inode))
			return -ENOKEY;
	}
	return 0;
}


static inline int fscrypt_prepare_link(struct dentry *old_dentry,
				       struct inode *dir,
				       struct dentry *dentry)
{
	if (IS_ENCRYPTED(dir))
		return __fscrypt_prepare_link(d_inode(old_dentry), dir);
	return 0;
}


static inline int fscrypt_prepare_rename(struct inode *old_dir,
					 struct dentry *old_dentry,
					 struct inode *new_dir,
					 struct dentry *new_dentry,
					 unsigned int flags)
{
	if (IS_ENCRYPTED(old_dir) || IS_ENCRYPTED(new_dir))
		return __fscrypt_prepare_rename(old_dir, old_dentry,
						new_dir, new_dentry, flags);
	return 0;
}


static inline int fscrypt_prepare_lookup(struct inode *dir,
					 struct dentry *dentry,
					 unsigned int flags)
{
	if (IS_ENCRYPTED(dir))
		return __fscrypt_prepare_lookup(dir, dentry);
	return 0;
}


static inline int fscrypt_prepare_setattr(struct dentry *dentry,
					  struct iattr *attr)
{
	if (attr->ia_valid & ATTR_SIZE)
		return fscrypt_require_key(d_inode(dentry));
	return 0;
}


static inline int fscrypt_prepare_symlink(struct inode *dir,
					  const char *target,
					  unsigned int len,
					  unsigned int max_len,
					  struct fscrypt_str *disk_link)
{
	if (IS_ENCRYPTED(dir) || fscrypt_dummy_context_enabled(dir))
		return __fscrypt_prepare_symlink(dir, len, max_len, disk_link);

	disk_link->name = (unsigned char *)target;
	disk_link->len = len + 1;
	if (disk_link->len > max_len)
		return -ENAMETOOLONG;
	return 0;
}


static inline int fscrypt_encrypt_symlink(struct inode *inode,
					  const char *target,
					  unsigned int len,
					  struct fscrypt_str *disk_link)
{
	if (IS_ENCRYPTED(inode))
		return __fscrypt_encrypt_symlink(inode, target, len, disk_link);
	return 0;
}

#endif	
