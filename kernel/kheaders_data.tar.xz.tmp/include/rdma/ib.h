

#if !defined(_RDMA_IB_H)
#define _RDMA_IB_H

#include <linux/types.h>
#include <linux/sched.h>

struct ib_addr {
	union {
		__u8		uib_addr8[16];
		__be16		uib_addr16[8];
		__be32		uib_addr32[4];
		__be64		uib_addr64[2];
	} ib_u;
#define sib_addr8		ib_u.uib_addr8
#define sib_addr16		ib_u.uib_addr16
#define sib_addr32		ib_u.uib_addr32
#define sib_addr64		ib_u.uib_addr64
#define sib_raw			ib_u.uib_addr8
#define sib_subnet_prefix	ib_u.uib_addr64[0]
#define sib_interface_id	ib_u.uib_addr64[1]
};

static inline int ib_addr_any(const struct ib_addr *a)
{
	return ((a->sib_addr64[0] | a->sib_addr64[1]) == 0);
}

static inline int ib_addr_loopback(const struct ib_addr *a)
{
	return ((a->sib_addr32[0] | a->sib_addr32[1] |
		 a->sib_addr32[2] | (a->sib_addr32[3] ^ htonl(1))) == 0);
}

static inline void ib_addr_set(struct ib_addr *addr,
			       __be32 w1, __be32 w2, __be32 w3, __be32 w4)
{
	addr->sib_addr32[0] = w1;
	addr->sib_addr32[1] = w2;
	addr->sib_addr32[2] = w3;
	addr->sib_addr32[3] = w4;
}

static inline int ib_addr_cmp(const struct ib_addr *a1, const struct ib_addr *a2)
{
	return memcmp(a1, a2, sizeof(struct ib_addr));
}

struct sockaddr_ib {
	unsigned short int	sib_family;	
	__be16			sib_pkey;
	__be32			sib_flowinfo;
	struct ib_addr		sib_addr;
	__be64			sib_sid;
	__be64			sib_sid_mask;
	__u64			sib_scope_id;
};


static inline bool ib_safe_file_access(struct file *filp)
{
	return filp->f_cred == current_cred() && segment_eq(get_fs(), USER_DS);
}

#endif 
