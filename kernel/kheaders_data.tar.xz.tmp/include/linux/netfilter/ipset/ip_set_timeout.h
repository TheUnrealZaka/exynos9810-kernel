#ifndef _IP_SET_TIMEOUT_H
#define _IP_SET_TIMEOUT_H



#ifdef __KERNEL__


#define IPSET_GC_TIME			(3 * 60)


#define IPSET_GC_PERIOD(timeout) \
	((timeout/3) ? min_t(u32, (timeout)/3, IPSET_GC_TIME) : 1)


#define IPSET_ELEM_PERMANENT	0


#define IPSET_NO_TIMEOUT	UINT_MAX

#define ip_set_adt_opt_timeout(opt, set)	\
((opt)->ext.timeout != IPSET_NO_TIMEOUT ? (opt)->ext.timeout : (set)->timeout)

static inline unsigned int
ip_set_timeout_uget(struct nlattr *tb)
{
	unsigned int timeout = ip_set_get_h32(tb);

	
	if (timeout > UINT_MAX/MSEC_PER_SEC)
		timeout = UINT_MAX/MSEC_PER_SEC;

	
	return timeout == IPSET_NO_TIMEOUT ? IPSET_NO_TIMEOUT - 1 : timeout;
}

static inline bool
ip_set_timeout_expired(unsigned long *t)
{
	return *t != IPSET_ELEM_PERMANENT && time_is_before_jiffies(*t);
}

static inline void
ip_set_timeout_set(unsigned long *timeout, u32 value)
{
	unsigned long t;

	if (!value) {
		*timeout = IPSET_ELEM_PERMANENT;
		return;
	}

	t = msecs_to_jiffies(value * MSEC_PER_SEC) + jiffies;
	if (t == IPSET_ELEM_PERMANENT)
		
		t--;
	*timeout = t;
}

static inline u32
ip_set_timeout_get(unsigned long *timeout)
{
	u32 t;

	if (*timeout == IPSET_ELEM_PERMANENT)
		return 0;

	t = jiffies_to_msecs(*timeout - jiffies)/MSEC_PER_SEC;
	
	return t == 0 ? 1 : t;
}

#endif	
#endif 
