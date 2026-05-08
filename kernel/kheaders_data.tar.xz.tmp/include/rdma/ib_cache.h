

#ifndef _IB_CACHE_H
#define _IB_CACHE_H

#include <rdma/ib_verbs.h>


int ib_get_cached_gid(struct ib_device    *device,
		      u8                   port_num,
		      int                  index,
		      union ib_gid        *gid,
		      struct ib_gid_attr  *attr);


int ib_find_cached_gid(struct ib_device *device,
		       const union ib_gid *gid,
		       enum ib_gid_type gid_type,
		       struct net_device *ndev,
		       u8               *port_num,
		       u16              *index);


int ib_find_cached_gid_by_port(struct ib_device *device,
			       const union ib_gid *gid,
			       enum ib_gid_type gid_type,
			       u8               port_num,
			       struct net_device *ndev,
			       u16              *index);

int ib_find_gid_by_filter(struct ib_device *device,
			  const union ib_gid *gid,
			  u8 port_num,
			  bool (*filter)(const union ib_gid *gid,
					 const struct ib_gid_attr *,
					 void *),
			  void *context, u16 *index);

int ib_get_cached_pkey(struct ib_device    *device_handle,
		       u8                   port_num,
		       int                  index,
		       u16                 *pkey);


int ib_find_cached_pkey(struct ib_device    *device,
			u8                   port_num,
			u16                  pkey,
			u16                 *index);


int ib_find_exact_cached_pkey(struct ib_device    *device,
			      u8                   port_num,
			      u16                  pkey,
			      u16                 *index);


int ib_get_cached_lmc(struct ib_device *device,
		      u8                port_num,
		      u8                *lmc);

#endif 
