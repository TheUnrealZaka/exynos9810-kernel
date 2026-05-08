#ifndef _LINUX_SCHED_LOADAVG_H
#define _LINUX_SCHED_LOADAVG_H


extern unsigned long avenrun[];		
extern void get_avenrun(unsigned long *loads, unsigned long offset, int shift);

#define FSHIFT		11		
#define FIXED_1		(1<<FSHIFT)	
#define LOAD_FREQ	(5*HZ+1)	
#define EXP_1		1884		
#define EXP_5		2014		
#define EXP_15		2037		


static inline unsigned long
calc_load(unsigned long load, unsigned long exp, unsigned long active)
{
	unsigned long newload;

	newload = load * exp + active * (FIXED_1 - exp);
	if (active >= load)
		newload += FIXED_1-1;

	return newload / FIXED_1;
}

extern unsigned long calc_load_n(unsigned long load, unsigned long exp,
				 unsigned long active, unsigned int n);

#define LOAD_INT(x) ((x) >> FSHIFT)
#define LOAD_FRAC(x) LOAD_INT(((x) & (FIXED_1-1)) * 100)

extern void calc_global_load(unsigned long ticks);

#endif 
