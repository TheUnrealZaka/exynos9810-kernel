

#ifndef _LINUX_MFD_SYSCON_ATMEL_ST_H
#define _LINUX_MFD_SYSCON_ATMEL_ST_H

#include <linux/bitops.h>

#define AT91_ST_CR	0x00	
#define		AT91_ST_WDRST	BIT(0)	

#define AT91_ST_PIMR	0x04	
#define		AT91_ST_PIV	0xffff	

#define AT91_ST_WDMR	0x08	
#define		AT91_ST_WDV	0xffff	
#define		AT91_ST_RSTEN	BIT(16)	
#define		AT91_ST_EXTEN	BIT(17)	

#define AT91_ST_RTMR	0x0c	
#define		AT91_ST_RTPRES	0xffff	

#define AT91_ST_SR	0x10	
#define		AT91_ST_PITS	BIT(0)	
#define		AT91_ST_WDOVF	BIT(1)	
#define		AT91_ST_RTTINC	BIT(2)	
#define		AT91_ST_ALMS	BIT(3)	

#define AT91_ST_IER	0x14	
#define AT91_ST_IDR	0x18	
#define AT91_ST_IMR	0x1c	

#define AT91_ST_RTAR	0x20	
#define		AT91_ST_ALMV	0xfffff	

#define AT91_ST_CRTR	0x24	
#define		AT91_ST_CRTV	0xfffff	

#endif 
