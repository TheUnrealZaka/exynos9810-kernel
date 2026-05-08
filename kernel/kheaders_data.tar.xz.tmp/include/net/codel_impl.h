#ifndef __NET_SCHED_CODEL_IMPL_H
#define __NET_SCHED_CODEL_IMPL_H





static void codel_params_init(struct codel_params *params)
{
	params->interval = MS2TIME(100);
	params->target = MS2TIME(5);
	params->ce_threshold = CODEL_DISABLED_THRESHOLD;
	params->ecn = false;
}

static void codel_vars_init(struct codel_vars *vars)
{
	memset(vars, 0, sizeof(*vars));
}

static void codel_stats_init(struct codel_stats *stats)
{
	stats->maxpacket = 0;
}


static void codel_Newton_step(struct codel_vars *vars)
{
	u32 invsqrt = ((u32)vars->rec_inv_sqrt) << REC_INV_SQRT_SHIFT;
	u32 invsqrt2 = ((u64)invsqrt * invsqrt) >> 32;
	u64 val = (3LL << 32) - ((u64)vars->count * invsqrt2);

	val >>= 2; 
	val = (val * invsqrt) >> (32 - 2 + 1);

	vars->rec_inv_sqrt = val >> REC_INV_SQRT_SHIFT;
}


static codel_time_t codel_control_law(codel_time_t t,
				      codel_time_t interval,
				      u32 rec_inv_sqrt)
{
	return t + reciprocal_scale(interval, rec_inv_sqrt << REC_INV_SQRT_SHIFT);
}

static bool codel_should_drop(const struct sk_buff *skb,
			      void *ctx,
			      struct codel_vars *vars,
			      struct codel_params *params,
			      struct codel_stats *stats,
			      codel_skb_len_t skb_len_func,
			      codel_skb_time_t skb_time_func,
			      u32 *backlog,
			      codel_time_t now)
{
	bool ok_to_drop;
	u32 skb_len;

	if (!skb) {
		vars->first_above_time = 0;
		return false;
	}

	skb_len = skb_len_func(skb);
	vars->ldelay = now - skb_time_func(skb);

	if (unlikely(skb_len > stats->maxpacket))
		stats->maxpacket = skb_len;

	if (codel_time_before(vars->ldelay, params->target) ||
	    *backlog <= params->mtu) {
		
		vars->first_above_time = 0;
		return false;
	}
	ok_to_drop = false;
	if (vars->first_above_time == 0) {
		
		vars->first_above_time = now + params->interval;
	} else if (codel_time_after(now, vars->first_above_time)) {
		ok_to_drop = true;
	}
	return ok_to_drop;
}

static struct sk_buff *codel_dequeue(void *ctx,
				     u32 *backlog,
				     struct codel_params *params,
				     struct codel_vars *vars,
				     struct codel_stats *stats,
				     codel_skb_len_t skb_len_func,
				     codel_skb_time_t skb_time_func,
				     codel_skb_drop_t drop_func,
				     codel_skb_dequeue_t dequeue_func)
{
	struct sk_buff *skb = dequeue_func(vars, ctx);
	codel_time_t now;
	bool drop;

	if (!skb) {
		vars->dropping = false;
		return skb;
	}
	now = codel_get_time();
	drop = codel_should_drop(skb, ctx, vars, params, stats,
				 skb_len_func, skb_time_func, backlog, now);
	if (vars->dropping) {
		if (!drop) {
			
			vars->dropping = false;
		} else if (codel_time_after_eq(now, vars->drop_next)) {
			
			while (vars->dropping &&
			       codel_time_after_eq(now, vars->drop_next)) {
				vars->count++; 
				codel_Newton_step(vars);
				if (params->ecn && INET_ECN_set_ce(skb)) {
					stats->ecn_mark++;
					vars->drop_next =
						codel_control_law(vars->drop_next,
								  params->interval,
								  vars->rec_inv_sqrt);
					goto end;
				}
				stats->drop_len += skb_len_func(skb);
				drop_func(skb, ctx);
				stats->drop_count++;
				skb = dequeue_func(vars, ctx);
				if (!codel_should_drop(skb, ctx,
						       vars, params, stats,
						       skb_len_func,
						       skb_time_func,
						       backlog, now)) {
					
					vars->dropping = false;
				} else {
					
					vars->drop_next =
						codel_control_law(vars->drop_next,
								  params->interval,
								  vars->rec_inv_sqrt);
				}
			}
		}
	} else if (drop) {
		u32 delta;

		if (params->ecn && INET_ECN_set_ce(skb)) {
			stats->ecn_mark++;
		} else {
			stats->drop_len += skb_len_func(skb);
			drop_func(skb, ctx);
			stats->drop_count++;

			skb = dequeue_func(vars, ctx);
			drop = codel_should_drop(skb, ctx, vars, params,
						 stats, skb_len_func,
						 skb_time_func, backlog, now);
		}
		vars->dropping = true;
		
		delta = vars->count - vars->lastcount;
		if (delta > 1 &&
		    codel_time_before(now - vars->drop_next,
				      16 * params->interval)) {
			vars->count = delta;
			
			codel_Newton_step(vars);
		} else {
			vars->count = 1;
			vars->rec_inv_sqrt = ~0U >> REC_INV_SQRT_SHIFT;
		}
		vars->lastcount = vars->count;
		vars->drop_next = codel_control_law(now, params->interval,
						    vars->rec_inv_sqrt);
	}
end:
	if (skb && codel_time_after(vars->ldelay, params->ce_threshold) &&
	    INET_ECN_set_ce(skb))
		stats->ce_mark++;
	return skb;
}

#endif
