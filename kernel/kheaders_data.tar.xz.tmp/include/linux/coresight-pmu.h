

#ifndef _LINUX_CORESIGHT_PMU_H
#define _LINUX_CORESIGHT_PMU_H

#define CORESIGHT_ETM_PMU_NAME "cs_etm"
#define CORESIGHT_ETM_PMU_SEED  0x10


#define ETM_OPT_CYCACC  12
#define ETM_OPT_TS      28

static inline int coresight_get_trace_id(int cpu)
{
	
	return (CORESIGHT_ETM_PMU_SEED + (cpu * 2));
}

#endif
