

#ifndef _LINUX_DQL_H
#define _LINUX_DQL_H

#ifdef __KERNEL__

struct dql {
	
	unsigned int	num_queued;		
	unsigned int	adj_limit;		
	unsigned int	last_obj_cnt;		

	

	unsigned int	limit ____cacheline_aligned_in_smp; 
	unsigned int	num_completed;		

	unsigned int	prev_ovlimit;		
	unsigned int	prev_num_queued;	
	unsigned int	prev_last_obj_cnt;	

	unsigned int	lowest_slack;		
	unsigned long	slack_start_time;	

	
	unsigned int	max_limit;		
	unsigned int	min_limit;		
	unsigned int	slack_hold_time;	
};


#define DQL_MAX_OBJECT (UINT_MAX / 16)
#define DQL_MAX_LIMIT ((UINT_MAX / 2) - DQL_MAX_OBJECT)


static inline void dql_queued(struct dql *dql, unsigned int count)
{
	BUG_ON(count > DQL_MAX_OBJECT);

	dql->last_obj_cnt = count;

	
	barrier();

	dql->num_queued += count;
}


static inline int dql_avail(const struct dql *dql)
{
	return ACCESS_ONCE(dql->adj_limit) - ACCESS_ONCE(dql->num_queued);
}


void dql_completed(struct dql *dql, unsigned int count);


void dql_reset(struct dql *dql);


int dql_init(struct dql *dql, unsigned hold_time);

#endif 

#endif 
