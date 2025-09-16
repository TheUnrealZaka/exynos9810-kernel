

#ifndef _FSCRYPTO_SDP_NAME_H
#define _FSCRYPTO_SDP_NAME_H

#ifdef CONFIG_FSCRYPT_SDP
int fscrypt_sdp_check_rename_pre(struct dentry *old_dentry);
void fscrypt_sdp_check_rename_post(struct inode *old_dir, struct dentry *old_dentry,
					struct inode *new_dir, struct dentry *new_dentry);
int fscrypt_sdp_check_rmdir(struct dentry *dentry);
//void fscrypt_sdp_check_chamber_event(struct inode *old_dir, struct dentry *old_dentry,
//					struct inode *new_dir, struct dentry *new_dentry);

#define FSCRYPT_EVT_RENAME_TO_CHAMBER      1
#define FSCRYPT_EVT_RENAME_OUT_OF_CHAMBER  2
#define FSCRYPT_IS_SENSITIVE_DENTRY(dentry) (dentry->d_inode->i_crypt_info->ci_sdp_info && (dentry->d_inode->i_crypt_info->ci_sdp_info->sdp_flags & SDP_DEK_IS_SENSITIVE))
#define FSCRYPT_IS_CHAMBER_DENTRY(dentry) (dentry->d_inode->i_crypt_info->ci_sdp_info && (dentry->d_inode->i_crypt_info->ci_sdp_info->sdp_flags & SDP_IS_CHAMBER_DIR))
#define FSCRYPT_IS_SENSITIVE_INODE(inode) (inode->i_crypt_info->ci_sdp_info && (inode->i_crypt_info->ci_sdp_info->sdp_flags & SDP_DEK_IS_SENSITIVE))
#define FSCRYPT_IS_CHAMBER_INODE(inode) (inode->i_crypt_info->ci_sdp_info && (inode->i_crypt_info->ci_sdp_info->sdp_flags & SDP_IS_CHAMBER_DIR))

#define FSCRYPT_STORAGE_TYPE_DATA_CE				-2
#define FSCRYPT_STORAGE_TYPE_MEDIA_CE				-3
#define FSCRYPT_STORAGE_TYPE_SYSTEM_CE				-4
#define FSCRYPT_STORAGE_TYPE_MISC_CE				-5
#define FSCRYPT_STORAGE_TYPE_DATA_DE				-6
#define FSCRYPT_STORAGE_TYPE_SYSTEM_DE				-7
#define FSCRYPT_STORAGE_TYPE_MISC_DE				-8
#define FSCRYPT_STORAGE_TYPE_SDP_ENC_USER			-9
#define FSCRYPT_STORAGE_TYPE_SDP_ENC_EMULATED		-10

#define FSCRYPT_SDP_NAME_FUNC_OK	0
#define FSCRYPT_SDP_NAME_FUNC_ERROR	-1
#define FSCRYPT_SDP_NAME_FUNC_FOUND_DATA_ROOT	1
#endif

#endif	
