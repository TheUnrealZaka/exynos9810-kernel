

#ifndef __CC2520_H
#define __CC2520_H

struct cc2520_platform_data {
	int fifo;
	int fifop;
	int cca;
	int sfd;
	int reset;
	int vreg;
};

#endif
