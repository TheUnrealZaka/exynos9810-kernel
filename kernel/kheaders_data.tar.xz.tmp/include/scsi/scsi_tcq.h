#ifndef _SCSI_SCSI_TCQ_H
#define _SCSI_SCSI_TCQ_H

#include <linux/blkdev.h>
#include <scsi/scsi_cmnd.h>
#include <scsi/scsi_device.h>
#include <scsi/scsi_host.h>

#define SCSI_NO_TAG	(-1)    


#ifdef CONFIG_BLOCK

static inline struct scsi_cmnd *scsi_host_find_tag(struct Scsi_Host *shost,
		int tag)
{
	struct request *req = NULL;

	if (tag == SCSI_NO_TAG)
		return NULL;

	if (shost_use_blk_mq(shost)) {
		u16 hwq = blk_mq_unique_tag_to_hwq(tag);

		if (hwq < shost->tag_set.nr_hw_queues) {
			req = blk_mq_tag_to_rq(shost->tag_set.tags[hwq],
				blk_mq_unique_tag_to_tag(tag));
		}
	} else {
		req = blk_map_queue_find_tag(shost->bqt, tag);
	}

	if (!req)
		return NULL;
	return req->special;
}

#endif 
#endif 
