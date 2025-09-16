



#ifndef EDMA_H_
#define EDMA_H_

enum dma_event_q {
	EVENTQ_0 = 0,
	EVENTQ_1 = 1,
	EVENTQ_2 = 2,
	EVENTQ_3 = 3,
	EVENTQ_DEFAULT = -1
};

#define EDMA_CTLR_CHAN(ctlr, chan)	(((ctlr) << 16) | (chan))
#define EDMA_CTLR(i)			((i) >> 16)
#define EDMA_CHAN_SLOT(i)		((i) & 0xffff)

#define EDMA_FILTER_PARAM(ctlr, chan)	((int[]) { EDMA_CTLR_CHAN(ctlr, chan) })

struct edma_rsv_info {

	const s16	(*rsv_chans)[2];
	const s16	(*rsv_slots)[2];
};

struct dma_slave_map;


struct edma_soc_info {
	
	enum dma_event_q	default_queue;

	
	struct edma_rsv_info	*rsv;

	
	s32			*memcpy_channels;

	s8	(*queue_priority_mapping)[2];
	const s16	(*xbar_chans)[2];

	const struct dma_slave_map *slave_map;
	int slavecnt;
};

#endif
