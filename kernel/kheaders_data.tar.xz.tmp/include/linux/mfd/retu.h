

#ifndef __LINUX_MFD_RETU_H
#define __LINUX_MFD_RETU_H

struct retu_dev;

int retu_read(struct retu_dev *, u8);
int retu_write(struct retu_dev *, u8, u16);


#define RETU_REG_WATCHDOG	0x17		
#define RETU_REG_CC1		0x0d		
#define RETU_REG_STATUS		0x16		


#define TAHVO_INT_VBUS		0		


#define TAHVO_STAT_VBUS		(1 << TAHVO_INT_VBUS)

#endif 
