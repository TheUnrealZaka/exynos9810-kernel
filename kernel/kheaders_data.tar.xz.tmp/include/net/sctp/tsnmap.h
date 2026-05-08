
#include <net/sctp/constants.h>

#ifndef __sctp_tsnmap_h__
#define __sctp_tsnmap_h__


struct sctp_tsnmap {
	
	unsigned long *tsn_map;

	
	__u32 base_tsn;

	
	__u32 cumulative_tsn_ack_point;

	
	__u32 max_tsn_seen;

	
	__u16 len;

	
	__u16 pending_data;

	
	__u16 num_dup_tsns;
	__be32 dup_tsns[SCTP_MAX_DUP_TSNS];
};

struct sctp_tsnmap_iter {
	__u32 start;
};


struct sctp_tsnmap *sctp_tsnmap_init(struct sctp_tsnmap *, __u16 len,
				     __u32 initial_tsn, gfp_t gfp);

void sctp_tsnmap_free(struct sctp_tsnmap *map);


int sctp_tsnmap_check(const struct sctp_tsnmap *, __u32 tsn);


int sctp_tsnmap_mark(struct sctp_tsnmap *, __u32 tsn,
		     struct sctp_transport *trans);


void sctp_tsnmap_skip(struct sctp_tsnmap *map, __u32 tsn);


static inline __u32 sctp_tsnmap_get_ctsn(const struct sctp_tsnmap *map)
{
	return map->cumulative_tsn_ack_point;
}


static inline __u32 sctp_tsnmap_get_max_tsn_seen(const struct sctp_tsnmap *map)
{
	return map->max_tsn_seen;
}


static inline __u16 sctp_tsnmap_num_dups(struct sctp_tsnmap *map)
{
	return map->num_dup_tsns;
}


static inline __be32 *sctp_tsnmap_get_dups(struct sctp_tsnmap *map)
{
	map->num_dup_tsns = 0;
	return map->dup_tsns;
}


__u16 sctp_tsnmap_num_gabs(struct sctp_tsnmap *map,
			   struct sctp_gap_ack_block *gabs);


__u16 sctp_tsnmap_pending(struct sctp_tsnmap *map);


static inline int sctp_tsnmap_has_gap(const struct sctp_tsnmap *map)
{
	return map->cumulative_tsn_ack_point != map->max_tsn_seen;
}


static inline void sctp_tsnmap_mark_dup(struct sctp_tsnmap *map, __u32 tsn)
{
	if (map->num_dup_tsns < SCTP_MAX_DUP_TSNS)
		map->dup_tsns[map->num_dup_tsns++] = htonl(tsn);
}


void sctp_tsnmap_renege(struct sctp_tsnmap *, __u32 tsn);


int sctp_tsnmap_has_gap(const struct sctp_tsnmap *);

#endif 
