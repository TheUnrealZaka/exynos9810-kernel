

#ifndef __XEN__PUBLIC_IO_SCSI_H__
#define __XEN__PUBLIC_IO_SCSI_H__

#include "ring.h"
#include "../grant_table.h"






#define VSCSIIF_ACT_SCSI_CDB		1


#define VSCSIIF_ACT_SCSI_ABORT		2


#define VSCSIIF_ACT_SCSI_RESET		3


#define VSCSIIF_ACT_SCSI_SG_PRESET	4


#define VSCSIIF_SG_TABLESIZE		26


#define VSCSIIF_MAX_COMMAND_SIZE	16
#define VSCSIIF_SENSE_BUFFERSIZE	96

struct scsiif_request_segment {
	grant_ref_t gref;
	uint16_t offset;
	uint16_t length;
};

#define VSCSIIF_SG_PER_PAGE (PAGE_SIZE / sizeof(struct scsiif_request_segment))


struct vscsiif_request {
	uint16_t rqid;		
	uint8_t act;		
	uint8_t cmd_len;	

	uint8_t cmnd[VSCSIIF_MAX_COMMAND_SIZE];	
	uint16_t timeout_per_command;	
	uint16_t channel, id, lun;	
	uint16_t ref_rqid;		
	uint8_t sc_data_direction;	
	uint8_t nr_segments;		

#define VSCSIIF_SG_GRANT	0x80

	struct scsiif_request_segment seg[VSCSIIF_SG_TABLESIZE];
	uint32_t reserved[3];
};


struct vscsiif_response {
	uint16_t rqid;		
	uint8_t padding;
	uint8_t sense_len;
	uint8_t sense_buffer[VSCSIIF_SENSE_BUFFERSIZE];
	int32_t rslt;
	uint32_t residual_len;	
	uint32_t reserved[36];
};

DEFINE_RING_TYPES(vscsiif, struct vscsiif_request, struct vscsiif_response);

#endif 
