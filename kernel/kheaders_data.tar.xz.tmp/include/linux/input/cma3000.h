

#ifndef _LINUX_CMA3000_H
#define _LINUX_CMA3000_H

#define CMAMODE_DEFAULT    0
#define CMAMODE_MEAS100    1
#define CMAMODE_MEAS400    2
#define CMAMODE_MEAS40     3
#define CMAMODE_MOTDET     4
#define CMAMODE_FF100      5
#define CMAMODE_FF400      6
#define CMAMODE_POFF       7

#define CMARANGE_2G   2000
#define CMARANGE_8G   8000



struct cma3000_platform_data {
	int fuzz_x;
	int fuzz_y;
	int fuzz_z;
	int g_range;
	uint8_t mode;
	uint8_t mdthr;
	uint8_t mdfftmr;
	uint8_t ffthr;
	unsigned long irqflags;
};

#endif
