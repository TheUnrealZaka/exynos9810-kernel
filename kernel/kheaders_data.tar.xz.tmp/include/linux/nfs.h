
#ifndef _LINUX_NFS_H
#define _LINUX_NFS_H

#include <linux/sunrpc/msg_prot.h>
#include <linux/string.h>
#include <uapi/linux/nfs.h>


#define NFS_MAXFHSIZE		128
struct nfs_fh {
	unsigned short		size;
	unsigned char		data[NFS_MAXFHSIZE];
};


static inline int nfs_compare_fh(const struct nfs_fh *a, const struct nfs_fh *b)
{
	return a->size != b->size || memcmp(a->data, b->data, a->size) != 0;
}

static inline void nfs_copy_fh(struct nfs_fh *target, const struct nfs_fh *source)
{
	target->size = source->size;
	memcpy(target->data, source->data, source->size);
}



#define NFS_OFFSET_MAX		((__s64)((~(__u64)0) >> 1))


enum nfs3_stable_how {
	NFS_UNSTABLE = 0,
	NFS_DATA_SYNC = 1,
	NFS_FILE_SYNC = 2,

	
	NFS_INVALID_STABLE_HOW = -1
};
#endif 
