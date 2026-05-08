

#ifndef SCSI_BSG_ISCSI_H
#define SCSI_BSG_ISCSI_H



#include <scsi/scsi.h>




#define ISCSI_DEFAULT_BSG_TIMEOUT      (10 * HZ)





#define ISCSI_BSG_CLS_MASK     0xF0000000      
#define ISCSI_BSG_HST_MASK     0x80000000      


#define ISCSI_BSG_HST_VENDOR           (ISCSI_BSG_HST_MASK | 0x000000FF)







struct iscsi_bsg_host_vendor {
	
	uint64_t vendor_id;

	
	uint32_t vendor_cmd[0];
};


struct iscsi_bsg_host_vendor_reply {
	
	uint32_t vendor_rsp[0];
};



struct iscsi_bsg_request {
	uint32_t msgcode;
	union {
		struct iscsi_bsg_host_vendor    h_vendor;
	} rqst_data;
} __attribute__((packed));



struct iscsi_bsg_reply {
	
	uint32_t result;

	
	uint32_t reply_payload_rcv_len;

	union {
		struct iscsi_bsg_host_vendor_reply      vendor_reply;
	} reply_data;
};


#endif 
