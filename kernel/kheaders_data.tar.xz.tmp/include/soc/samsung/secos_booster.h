


#ifndef __LINUX_SECOS_BOOST_H__
#define __LINUX_SECOS_BOOST_H__


enum secos_boost_policy {
	MAX_PERFORMANCE,
	MID_PERFORMANCE,
	MIN_PERFORMANCE,
	STB_PERFORMANCE,
	PERFORMANCE_MAX_CNT,
};

int secos_booster_start(enum secos_boost_policy policy);
int secos_booster_stop(void);

#endif
