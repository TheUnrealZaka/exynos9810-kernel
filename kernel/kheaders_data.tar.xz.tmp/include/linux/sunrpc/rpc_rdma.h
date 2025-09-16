

#ifndef _LINUX_SUNRPC_RPC_RDMA_H
#define _LINUX_SUNRPC_RPC_RDMA_H

#include <linux/types.h>
#include <linux/bitops.h>

#define RPCRDMA_VERSION		1
#define rpcrdma_version		cpu_to_be32(RPCRDMA_VERSION)

enum {
	RPCRDMA_V1_DEF_INLINE_SIZE	= 1024,
};

struct rpcrdma_segment {
	__be32 rs_handle;	
	__be32 rs_length;	
	__be64 rs_offset;	
};


struct rpcrdma_read_chunk {
	__be32 rc_discrim;	
	__be32 rc_position;	
	struct rpcrdma_segment rc_target;
};


struct rpcrdma_write_chunk {
	struct rpcrdma_segment wc_target;
};


struct rpcrdma_write_array {
	__be32 wc_discrim;	
	__be32 wc_nchunks;	
	struct rpcrdma_write_chunk wc_array[0];
};

struct rpcrdma_msg {
	__be32 rm_xid;	
	__be32 rm_vers;	
	__be32 rm_credit;	
	__be32 rm_type;	
	union {

		struct {			
			__be32 rm_empty[3];	
		} rm_nochunks;

		struct {			
			__be32 rm_align;	
			__be32 rm_thresh;	
			__be32 rm_pempty[3];	
		} rm_padded;

		struct {
			__be32 rm_err;
			__be32 rm_vers_low;
			__be32 rm_vers_high;
		} rm_error;

		__be32 rm_chunks[0];	

	} rm_body;
};


#define RPCRDMA_HDRLEN_MIN	(sizeof(__be32) * 7)
#define RPCRDMA_HDRLEN_ERR	(sizeof(__be32) * 5)

enum rpcrdma_errcode {
	ERR_VERS = 1,
	ERR_CHUNK = 2
};

enum rpcrdma_proc {
	RDMA_MSG = 0,		
	RDMA_NOMSG = 1,		
	RDMA_MSGP = 2,		
	RDMA_DONE = 3,		
	RDMA_ERROR = 4		
};

#define rdma_msg	cpu_to_be32(RDMA_MSG)
#define rdma_nomsg	cpu_to_be32(RDMA_NOMSG)
#define rdma_msgp	cpu_to_be32(RDMA_MSGP)
#define rdma_done	cpu_to_be32(RDMA_DONE)
#define rdma_error	cpu_to_be32(RDMA_ERROR)


struct rpcrdma_connect_private {
	__be32			cp_magic;
	u8			cp_version;
	u8			cp_flags;
	u8			cp_send_size;
	u8			cp_recv_size;
} __packed;

#define rpcrdma_cmp_magic	__cpu_to_be32(0xf6ab0e18)

enum {
	RPCRDMA_CMP_VERSION		= 1,
	RPCRDMA_CMP_F_SND_W_INV_OK	= BIT(0),
};

static inline u8
rpcrdma_encode_buffer_size(unsigned int size)
{
	return (size >> 10) - 1;
}

static inline unsigned int
rpcrdma_decode_buffer_size(u8 val)
{
	return ((unsigned int)val + 1) << 10;
}

#endif				
