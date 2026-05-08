

#if !defined(RDMA_CM_IB_H)
#define RDMA_CM_IB_H

#include <rdma/rdma_cm.h>


int rdma_set_ib_paths(struct rdma_cm_id *id,
		      struct ib_sa_path_rec *path_rec, int num_paths);


#define RDMA_UDP_QKEY 0x01234567

#endif 
