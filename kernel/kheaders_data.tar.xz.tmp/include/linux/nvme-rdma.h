

#ifndef _LINUX_NVME_RDMA_H
#define _LINUX_NVME_RDMA_H

enum nvme_rdma_cm_fmt {
	NVME_RDMA_CM_FMT_1_0 = 0x0,
};

enum nvme_rdma_cm_status {
	NVME_RDMA_CM_INVALID_LEN	= 0x01,
	NVME_RDMA_CM_INVALID_RECFMT	= 0x02,
	NVME_RDMA_CM_INVALID_QID	= 0x03,
	NVME_RDMA_CM_INVALID_HSQSIZE	= 0x04,
	NVME_RDMA_CM_INVALID_HRQSIZE	= 0x05,
	NVME_RDMA_CM_NO_RSC		= 0x06,
	NVME_RDMA_CM_INVALID_IRD	= 0x07,
	NVME_RDMA_CM_INVALID_ORD	= 0x08,
};


struct nvme_rdma_cm_req {
	__le16		recfmt;
	__le16		qid;
	__le16		hrqsize;
	__le16		hsqsize;
	u8		rsvd[24];
};


struct nvme_rdma_cm_rep {
	__le16		recfmt;
	__le16		crqsize;
	u8		rsvd[28];
};


struct nvme_rdma_cm_rej {
	__le16		recfmt;
	__le16		sts;
};

#endif 
