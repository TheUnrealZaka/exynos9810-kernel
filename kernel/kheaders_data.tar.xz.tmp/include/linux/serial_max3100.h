


#ifndef _LINUX_SERIAL_MAX3100_H
#define _LINUX_SERIAL_MAX3100_H 1



struct plat_max3100 {
	int loopback;
	int crystal;
	void (*max3100_hw_suspend) (int suspend);
	int poll_time;
};

#endif
