

#ifndef _MEDIA_CEC_EDID_H
#define _MEDIA_CEC_EDID_H

#include <linux/types.h>

#define CEC_PHYS_ADDR_INVALID		0xffff
#define cec_phys_addr_exp(pa) \
	((pa) >> 12), ((pa) >> 8) & 0xf, ((pa) >> 4) & 0xf, (pa) & 0xf


u16 cec_get_edid_phys_addr(const u8 *edid, unsigned int size,
			   unsigned int *offset);


void cec_set_edid_phys_addr(u8 *edid, unsigned int size, u16 phys_addr);


u16 cec_phys_addr_for_input(u16 phys_addr, u8 input);


int cec_phys_addr_validate(u16 phys_addr, u16 *parent, u16 *port);

#endif 
