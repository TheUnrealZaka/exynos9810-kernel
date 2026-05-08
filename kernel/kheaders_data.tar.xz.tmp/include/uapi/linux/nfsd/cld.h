

#ifndef _NFSD_CLD_H
#define _NFSD_CLD_H


#define CLD_UPCALL_VERSION 1


#define NFS4_OPAQUE_LIMIT 1024

enum cld_command {
	Cld_Create,		
	Cld_Remove,		
	Cld_Check,		
	Cld_GraceDone,		
};


struct cld_name {
	uint16_t	cn_len;				
	unsigned char	cn_id[NFS4_OPAQUE_LIMIT];	
} __attribute__((packed));


struct cld_msg {
	uint8_t		cm_vers;		
	uint8_t		cm_cmd;			
	int16_t		cm_status;		
	uint32_t	cm_xid;			
	union {
		int64_t		cm_gracetime;	
		struct cld_name	cm_name;
	} __attribute__((packed)) cm_u;
} __attribute__((packed));

#endif 
