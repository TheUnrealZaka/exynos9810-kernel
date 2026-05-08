#ifndef _LINUX_LGUEST_LAUNCHER
#define _LINUX_LGUEST_LAUNCHER

#include <linux/types.h>




enum lguest_req
{
	LHREQ_INITIALIZE, 
	LHREQ_GETDMA, 
	LHREQ_IRQ, 
	LHREQ_BREAK, 
	LHREQ_EVENTFD, 
	LHREQ_GETREG, 
	LHREQ_SETREG, 
	LHREQ_TRAP, 
};


struct lguest_pending {
	__u8 trap;
	__u8 insn[7];
	__u32 addr;
};
#endif 
