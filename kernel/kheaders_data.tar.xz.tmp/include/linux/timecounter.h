
#ifndef _LINUX_TIMECOUNTER_H
#define _LINUX_TIMECOUNTER_H

#include <linux/types.h>


#define CYCLECOUNTER_MASK(bits) (cycle_t)((bits) < 64 ? ((1ULL<<(bits))-1) : -1)


struct cyclecounter {
	cycle_t (*read)(const struct cyclecounter *cc);
	cycle_t mask;
	u32 mult;
	u32 shift;
};


struct timecounter {
	const struct cyclecounter *cc;
	cycle_t cycle_last;
	u64 nsec;
	u64 mask;
	u64 frac;
};


static inline u64 cyclecounter_cyc2ns(const struct cyclecounter *cc,
				      cycle_t cycles, u64 mask, u64 *frac)
{
	u64 ns = (u64) cycles;

	ns = (ns * cc->mult) + *frac;
	*frac = ns & mask;
	return ns >> cc->shift;
}


static inline void timecounter_adjtime(struct timecounter *tc, s64 delta)
{
	tc->nsec += delta;
}


extern void timecounter_init(struct timecounter *tc,
			     const struct cyclecounter *cc,
			     u64 start_tstamp);


extern u64 timecounter_read(struct timecounter *tc);


extern u64 timecounter_cyc2time(struct timecounter *tc,
				cycle_t cycle_tstamp);

#endif
