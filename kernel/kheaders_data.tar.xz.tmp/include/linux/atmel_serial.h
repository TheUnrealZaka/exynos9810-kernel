

#ifndef ATMEL_SERIAL_H
#define ATMEL_SERIAL_H

#define ATMEL_US_CR		0x00	
#define	ATMEL_US_RSTRX		BIT(2)	
#define	ATMEL_US_RSTTX		BIT(3)	
#define	ATMEL_US_RXEN		BIT(4)	
#define	ATMEL_US_RXDIS		BIT(5)	
#define	ATMEL_US_TXEN		BIT(6)	
#define	ATMEL_US_TXDIS		BIT(7)	
#define	ATMEL_US_RSTSTA		BIT(8)	
#define	ATMEL_US_STTBRK		BIT(9)	
#define	ATMEL_US_STPBRK		BIT(10)	
#define	ATMEL_US_STTTO		BIT(11)	
#define	ATMEL_US_SENDA		BIT(12)	
#define	ATMEL_US_RSTIT		BIT(13)	
#define	ATMEL_US_RSTNACK	BIT(14)	
#define	ATMEL_US_RETTO		BIT(15)	
#define	ATMEL_US_DTREN		BIT(16)	
#define	ATMEL_US_DTRDIS		BIT(17)	
#define	ATMEL_US_RTSEN		BIT(18)	
#define	ATMEL_US_RTSDIS		BIT(19)	
#define	ATMEL_US_TXFCLR		BIT(24)	
#define	ATMEL_US_RXFCLR		BIT(25)	
#define	ATMEL_US_TXFLCLR	BIT(26)	
#define	ATMEL_US_FIFOEN		BIT(30)	
#define	ATMEL_US_FIFODIS	BIT(31)	

#define ATMEL_US_MR		0x04	
#define	ATMEL_US_USMODE		GENMASK(3, 0)	
#define		ATMEL_US_USMODE_NORMAL		0
#define		ATMEL_US_USMODE_RS485		1
#define		ATMEL_US_USMODE_HWHS		2
#define		ATMEL_US_USMODE_MODEM		3
#define		ATMEL_US_USMODE_ISO7816_T0	4
#define		ATMEL_US_USMODE_ISO7816_T1	6
#define		ATMEL_US_USMODE_IRDA		8
#define	ATMEL_US_USCLKS		GENMASK(5, 4)	
#define		ATMEL_US_USCLKS_MCK		(0 <<  4)
#define		ATMEL_US_USCLKS_MCK_DIV8	(1 <<  4)
#define		ATMEL_US_USCLKS_SCK		(3 <<  4)
#define	ATMEL_US_CHRL		GENMASK(7, 6)	
#define		ATMEL_US_CHRL_5			(0 <<  6)
#define		ATMEL_US_CHRL_6			(1 <<  6)
#define		ATMEL_US_CHRL_7			(2 <<  6)
#define		ATMEL_US_CHRL_8			(3 <<  6)
#define	ATMEL_US_SYNC		BIT(8)		
#define	ATMEL_US_PAR		GENMASK(11, 9)	
#define		ATMEL_US_PAR_EVEN		(0 <<  9)
#define		ATMEL_US_PAR_ODD		(1 <<  9)
#define		ATMEL_US_PAR_SPACE		(2 <<  9)
#define		ATMEL_US_PAR_MARK		(3 <<  9)
#define		ATMEL_US_PAR_NONE		(4 <<  9)
#define		ATMEL_US_PAR_MULTI_DROP		(6 <<  9)
#define	ATMEL_US_NBSTOP		GENMASK(13, 12)	
#define		ATMEL_US_NBSTOP_1		(0 << 12)
#define		ATMEL_US_NBSTOP_1_5		(1 << 12)
#define		ATMEL_US_NBSTOP_2		(2 << 12)
#define	ATMEL_US_CHMODE		GENMASK(15, 14)	
#define		ATMEL_US_CHMODE_NORMAL		(0 << 14)
#define		ATMEL_US_CHMODE_ECHO		(1 << 14)
#define		ATMEL_US_CHMODE_LOC_LOOP	(2 << 14)
#define		ATMEL_US_CHMODE_REM_LOOP	(3 << 14)
#define	ATMEL_US_MSBF		BIT(16)	
#define	ATMEL_US_MODE9		BIT(17)	
#define	ATMEL_US_CLKO		BIT(18)	
#define	ATMEL_US_OVER		BIT(19)	
#define	ATMEL_US_INACK		BIT(20)	
#define	ATMEL_US_DSNACK		BIT(21)	
#define	ATMEL_US_MAX_ITER	GENMASK(26, 24)	
#define	ATMEL_US_FILTER		BIT(28)	

#define ATMEL_US_IER		0x08	
#define	ATMEL_US_RXRDY		BIT(0)	
#define	ATMEL_US_TXRDY		BIT(1)	
#define	ATMEL_US_RXBRK		BIT(2)	
#define	ATMEL_US_ENDRX		BIT(3)	
#define	ATMEL_US_ENDTX		BIT(4)	
#define	ATMEL_US_OVRE		BIT(5)	
#define	ATMEL_US_FRAME		BIT(6)	
#define	ATMEL_US_PARE		BIT(7)	
#define	ATMEL_US_TIMEOUT	BIT(8)	
#define	ATMEL_US_TXEMPTY	BIT(9)	
#define	ATMEL_US_ITERATION	BIT(10)	
#define	ATMEL_US_TXBUFE		BIT(11)	
#define	ATMEL_US_RXBUFF		BIT(12)	
#define	ATMEL_US_NACK		BIT(13)	
#define	ATMEL_US_RIIC		BIT(16)	
#define	ATMEL_US_DSRIC		BIT(17)	
#define	ATMEL_US_DCDIC		BIT(18)	
#define	ATMEL_US_CTSIC		BIT(19)	
#define	ATMEL_US_RI		BIT(20)	
#define	ATMEL_US_DSR		BIT(21)	
#define	ATMEL_US_DCD		BIT(22)	
#define	ATMEL_US_CTS		BIT(23)	

#define ATMEL_US_IDR		0x0c	
#define ATMEL_US_IMR		0x10	
#define ATMEL_US_CSR		0x14	
#define ATMEL_US_RHR		0x18	
#define ATMEL_US_THR		0x1c	
#define	ATMEL_US_SYNH		BIT(15)	

#define ATMEL_US_BRGR		0x20	
#define	ATMEL_US_CD		GENMASK(15, 0)	
#define ATMEL_US_FP_OFFSET	16	
#define ATMEL_US_FP_MASK	0x7

#define ATMEL_US_RTOR		0x24	
#define ATMEL_UA_RTOR		0x28	
#define	ATMEL_US_TO		GENMASK(15, 0)	

#define ATMEL_US_TTGR		0x28	
#define	ATMEL_US_TG		GENMASK(7, 0)	

#define ATMEL_US_FIDI		0x40	
#define ATMEL_US_NER		0x44	
#define ATMEL_US_IF		0x4c	

#define ATMEL_US_CMPR		0x90	
#define ATMEL_US_FMR		0xa0	
#define	ATMEL_US_TXRDYM(data)	(((data) & 0x3) << 0)	
#define	ATMEL_US_RXRDYM(data)	(((data) & 0x3) << 4)	
#define		ATMEL_US_ONE_DATA	0x0
#define		ATMEL_US_TWO_DATA	0x1
#define		ATMEL_US_FOUR_DATA	0x2
#define	ATMEL_US_FRTSC		BIT(7)	
#define	ATMEL_US_TXFTHRES(thr)	(((thr) & 0x3f) << 8)	
#define	ATMEL_US_RXFTHRES(thr)	(((thr) & 0x3f) << 16)	
#define	ATMEL_US_RXFTHRES2(thr)	(((thr) & 0x3f) << 24)	

#define ATMEL_US_FLR		0xa4	
#define	ATMEL_US_TXFL(reg)	(((reg) >> 0) & 0x3f)	
#define	ATMEL_US_RXFL(reg)	(((reg) >> 16) & 0x3f)	

#define ATMEL_US_FIER		0xa8	
#define ATMEL_US_FIDR		0xac	
#define ATMEL_US_FIMR		0xb0	
#define ATMEL_US_FESR		0xb4	
#define	ATMEL_US_TXFEF		BIT(0)	
#define	ATMEL_US_TXFFF		BIT(1)	
#define	ATMEL_US_TXFTHF		BIT(2)	
#define	ATMEL_US_RXFEF		BIT(3)	
#define	ATMEL_US_RXFFF		BIT(4)	
#define	ATMEL_US_RXFTHF		BIT(5)	
#define	ATMEL_US_TXFPTEF	BIT(6)	
#define	ATMEL_US_RXFPTEF	BIT(7)	
#define	ATMEL_US_TXFLOCK	BIT(8)	
#define	ATMEL_US_RXFTHF2	BIT(9)	

#define ATMEL_US_NAME		0xf0	
#define ATMEL_US_VERSION	0xfc	

#endif
