
#ifndef IW_CM_H
#define IW_CM_H

#include <linux/in.h>
#include <rdma/ib_cm.h>

struct iw_cm_id;

enum iw_cm_event_type {
	IW_CM_EVENT_CONNECT_REQUEST = 1, 
	IW_CM_EVENT_CONNECT_REPLY,	 
	IW_CM_EVENT_ESTABLISHED,	 
	IW_CM_EVENT_DISCONNECT,		 
	IW_CM_EVENT_CLOSE		 
};

struct iw_cm_event {
	enum iw_cm_event_type event;
	int			 status;
	struct sockaddr_storage local_addr;
	struct sockaddr_storage remote_addr;
	void *private_data;
	void *provider_data;
	u8 private_data_len;
	u8 ord;
	u8 ird;
};


typedef int (*iw_cm_handler)(struct iw_cm_id *cm_id,
			     struct iw_cm_event *event);


typedef int (*iw_event_handler)(struct iw_cm_id *cm_id,
				 struct iw_cm_event *event);

struct iw_cm_id {
	iw_cm_handler		cm_handler;      
	void		        *context;	 
	struct ib_device	*device;
	struct sockaddr_storage local_addr;      
	struct sockaddr_storage	remote_addr;
	struct sockaddr_storage m_local_addr;	 
	struct sockaddr_storage	m_remote_addr;	 
	void			*provider_data;	 
	iw_event_handler        event_handler;   
	
	void (*add_ref)(struct iw_cm_id *);
	void (*rem_ref)(struct iw_cm_id *);
	u8  tos;
	bool mapped;
};

struct iw_cm_conn_param {
	const void *private_data;
	u16 private_data_len;
	u32 ord;
	u32 ird;
	u32 qpn;
};

struct iw_cm_verbs {
	void		(*add_ref)(struct ib_qp *qp);

	void		(*rem_ref)(struct ib_qp *qp);

	struct ib_qp *	(*get_qp)(struct ib_device *device,
				  int qpn);

	int		(*connect)(struct iw_cm_id *cm_id,
				   struct iw_cm_conn_param *conn_param);

	int		(*accept)(struct iw_cm_id *cm_id,
				  struct iw_cm_conn_param *conn_param);

	int		(*reject)(struct iw_cm_id *cm_id,
				  const void *pdata, u8 pdata_len);

	int		(*create_listen)(struct iw_cm_id *cm_id,
					 int backlog);

	int		(*destroy_listen)(struct iw_cm_id *cm_id);
	char		ifname[IFNAMSIZ];
};


struct iw_cm_id *iw_create_cm_id(struct ib_device *device,
				 iw_cm_handler cm_handler, void *context);


void iw_destroy_cm_id(struct iw_cm_id *cm_id);


void iw_cm_unbind_qp(struct iw_cm_id *cm_id, struct ib_qp *qp);


struct ib_qp *iw_cm_get_qp(struct ib_device *device, int qpn);


int iw_cm_listen(struct iw_cm_id *cm_id, int backlog);


int iw_cm_accept(struct iw_cm_id *cm_id, struct iw_cm_conn_param *iw_param);


int iw_cm_reject(struct iw_cm_id *cm_id, const void *private_data,
		 u8 private_data_len);


int iw_cm_connect(struct iw_cm_id *cm_id, struct iw_cm_conn_param *iw_param);


int iw_cm_disconnect(struct iw_cm_id *cm_id, int abrupt);


int iw_cm_init_qp_attr(struct iw_cm_id *cm_id, struct ib_qp_attr *qp_attr,
		       int *qp_attr_mask);

#endif 
