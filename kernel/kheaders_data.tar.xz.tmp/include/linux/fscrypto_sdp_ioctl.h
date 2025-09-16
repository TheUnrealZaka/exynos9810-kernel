

#ifndef _FSCRYPTO_SDP_IOCTL_H
#define _FSCRYPTO_SDP_IOCTL_H

#ifdef CONFIG_FSCRYPT_SDP

struct engine_id_container {
	int engine_id;
};

typedef struct engine_id_container dek_arg_set_sdp_policy_t;
typedef struct engine_id_container dek_arg_set_sensitive_t;
typedef struct engine_id_container dek_arg_set_protected_t;
typedef struct engine_id_container dek_arg_add_chamber_t;

struct dek_arg_sdp_info {
	int engine_id;
	int sdp_enabled;
	int is_sensitive;
	int is_chamber;
	unsigned int type;
};

#define FS_IOC_GET_SDP_INFO     _IOR('l', 0x11, struct dek_arg_sdp_info)
#define FS_IOC_SET_SDP_POLICY   _IOW('l', 0x12, dek_arg_set_sdp_policy_t)
#define FS_IOC_SET_SENSITIVE    _IOW('l', 0x15, dek_arg_set_sensitive_t)
#define FS_IOC_SET_PROTECTED    _IOW('l', 0x16, dek_arg_set_protected_t)
#define FS_IOC_ADD_CHAMBER      _IOW('l', 0x17, dek_arg_add_chamber_t)
#define FS_IOC_REMOVE_CHAMBER   _IOW('l', 0x18, __u32)

int fscrypt_sdp_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#endif

#endif	
