
#ifndef __CORDIC_H_
#define __CORDIC_H_

#include <linux/types.h>


struct cordic_iq {
	s32 i;
	s32 q;
};


struct cordic_iq cordic_calc_iq(s32 theta);

#endif 
