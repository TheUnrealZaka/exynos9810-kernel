
#ifndef _PPP_DEFS_H_
#define _PPP_DEFS_H_

#include <linux/crc-ccitt.h>
#include <uapi/linux/ppp_defs.h>

#define PPP_FCS(fcs, c) crc_ccitt_byte(fcs, c)
#endif 
