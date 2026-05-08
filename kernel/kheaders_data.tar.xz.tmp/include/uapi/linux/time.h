#ifndef _UAPI_LINUX_TIME_H
#define _UAPI_LINUX_TIME_H

#include <linux/types.h>


#ifndef _STRUCT_TIMESPEC
#define _STRUCT_TIMESPEC
struct timespec {
	__kernel_time_t	tv_sec;			
	long		tv_nsec;		
};
#endif

struct timeval {
	__kernel_time_t		tv_sec;		
	__kernel_suseconds_t	tv_usec;	
};

struct timezone {
	int	tz_minuteswest;	
	int	tz_dsttime;	
};



#define	ITIMER_REAL		0
#define	ITIMER_VIRTUAL		1
#define	ITIMER_PROF		2

struct itimerspec {
	struct timespec it_interval;	
	struct timespec it_value;	
};

struct itimerval {
	struct timeval it_interval;	
	struct timeval it_value;	
};

#ifndef __kernel_timespec
struct __kernel_timespec {
	__kernel_time64_t       tv_sec;                 
	long long               tv_nsec;                
};
#endif


#define CLOCK_REALTIME			0
#define CLOCK_MONOTONIC			1
#define CLOCK_PROCESS_CPUTIME_ID	2
#define CLOCK_THREAD_CPUTIME_ID		3
#define CLOCK_MONOTONIC_RAW		4
#define CLOCK_REALTIME_COARSE		5
#define CLOCK_MONOTONIC_COARSE		6
#define CLOCK_BOOTTIME			7
#define CLOCK_REALTIME_ALARM		8
#define CLOCK_BOOTTIME_ALARM		9
#define CLOCK_SGI_CYCLE			10	
#define CLOCK_TAI			11

#define MAX_CLOCKS			16
#define CLOCKS_MASK			(CLOCK_REALTIME | CLOCK_MONOTONIC)
#define CLOCKS_MONO			CLOCK_MONOTONIC


#define TIMER_ABSTIME			0x01

#endif 
