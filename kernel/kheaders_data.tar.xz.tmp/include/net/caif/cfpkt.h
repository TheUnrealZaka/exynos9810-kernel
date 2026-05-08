

#ifndef CFPKT_H_
#define CFPKT_H_
#include <net/caif/caif_layer.h>
#include <linux/types.h>
struct cfpkt;


struct cfpkt *cfpkt_create(u16 len);


void cfpkt_destroy(struct cfpkt *pkt);


int cfpkt_extr_head(struct cfpkt *pkt, void *data, u16 len);

static inline u8 cfpkt_extr_head_u8(struct cfpkt *pkt)
{
	u8 tmp;

	cfpkt_extr_head(pkt, &tmp, 1);

	return tmp;
}

static inline u16 cfpkt_extr_head_u16(struct cfpkt *pkt)
{
	__le16 tmp;

	cfpkt_extr_head(pkt, &tmp, 2);

	return le16_to_cpu(tmp);
}

static inline u32 cfpkt_extr_head_u32(struct cfpkt *pkt)
{
	__le32 tmp;

	cfpkt_extr_head(pkt, &tmp, 4);

	return le32_to_cpu(tmp);
}


int cfpkt_peek_head(struct cfpkt *pkt, void *data, u16 len);


int cfpkt_extr_trail(struct cfpkt *pkt, void *data, u16 len);


int cfpkt_add_head(struct cfpkt *pkt, const void *data, u16 len);


int cfpkt_add_trail(struct cfpkt *pkt, const void *data, u16 len);


int cfpkt_pad_trail(struct cfpkt *pkt, u16 len);


int cfpkt_addbdy(struct cfpkt *pkt, const u8 data);


int cfpkt_add_body(struct cfpkt *pkt, const void *data, u16 len);


bool cfpkt_more(struct cfpkt *pkt);


bool cfpkt_erroneous(struct cfpkt *pkt);


u16 cfpkt_getlen(struct cfpkt *pkt);


int cfpkt_setlen(struct cfpkt *pkt, u16 len);


struct cfpkt *cfpkt_append(struct cfpkt *dstpkt, struct cfpkt *addpkt,
		      u16 expectlen);


struct cfpkt *cfpkt_split(struct cfpkt *pkt, u16 pos);



int cfpkt_iterate(struct cfpkt *pkt,
		u16 (*iter_func)(u16 chks, void *buf, u16 len),
		u16 data);


struct cfpkt *cfpkt_fromnative(enum caif_direction dir, void *nativepkt);


void *cfpkt_tonative(struct cfpkt *pkt);


struct caif_payload_info *cfpkt_info(struct cfpkt *pkt);


void cfpkt_set_prio(struct cfpkt *pkt, int prio);

#endif				
