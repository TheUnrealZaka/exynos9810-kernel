

#ifndef __I2C_BFIN_TWI_H__
#define __I2C_BFIN_TWI_H__

#include <linux/types.h>
#include <linux/i2c.h>


struct bfin_twi_regs {
	u16 clkdiv;
	u16 dummy1;
	u16 control;
	u16 dummy2;
	u16 slave_ctl;
	u16 dummy3;
	u16 slave_stat;
	u16 dummy4;
	u16 slave_addr;
	u16 dummy5;
	u16 master_ctl;
	u16 dummy6;
	u16 master_stat;
	u16 dummy7;
	u16 master_addr;
	u16 dummy8;
	u16 int_stat;
	u16 dummy9;
	u16 int_mask;
	u16 dummy10;
	u16 fifo_ctl;
	u16 dummy11;
	u16 fifo_stat;
	u16 dummy12;
	u32 __pad[20];
	u16 xmt_data8;
	u16 dummy13;
	u16 xmt_data16;
	u16 dummy14;
	u16 rcv_data8;
	u16 dummy15;
	u16 rcv_data16;
	u16 dummy16;
};

struct bfin_twi_iface {
	int			irq;
	spinlock_t		lock;
	char			read_write;
	u8			command;
	u8			*transPtr;
	int			readNum;
	int			writeNum;
	int			cur_mode;
	int			manual_stop;
	int			result;
	struct i2c_adapter	adap;
	struct completion	complete;
	struct i2c_msg		*pmsg;
	int			msg_num;
	int			cur_msg;
	u16			saved_clkdiv;
	u16			saved_control;
	struct bfin_twi_regs __iomem *regs_base;
};



#define	CLKLOW(x)	((x) & 0xFF)	
#define CLKHI(y)	(((y)&0xFF)<<0x8) 


#define	PRESCALE	0x007F	
#define	TWI_ENA		0x0080	
#define	SCCB		0x0200	


#define	SEN		0x0001	
#define	SADD_LEN	0x0002	
#define	STDVAL		0x0004	
#define	NAK		0x0008	
#define	GEN		0x0010	


#define	SDIR		0x0001	
#define GCALL		0x0002	


#define	MEN		0x0001	
#define	MADD_LEN	0x0002	
#define	MDIR		0x0004	
#define	FAST		0x0008	
#define	STOP		0x0010	
#define	RSTART		0x0020	
#define	DCNT		0x3FC0	
#define	SDAOVR		0x4000	
#define	SCLOVR		0x8000	


#define	MPROG		0x0001	
#define	LOSTARB		0x0002	
#define	ANAK		0x0004	
#define	DNAK		0x0008	
#define	BUFRDERR	0x0010	
#define	BUFWRERR	0x0020	
#define	SDASEN		0x0040	
#define	SCLSEN		0x0080	
#define	BUSBUSY		0x0100	


#define	SINIT		0x0001	
#define	SCOMP		0x0002	
#define	SERR		0x0004	
#define	SOVF		0x0008	
#define	MCOMP		0x0010	
#define	MERR		0x0020	
#define	XMTSERV		0x0040	
#define	RCVSERV		0x0080	


#define	XMTFLUSH	0x0001	
#define	RCVFLUSH	0x0002	
#define	XMTINTLEN	0x0004	
#define	RCVINTLEN	0x0008	


#define	XMTSTAT		0x0003	
#define	XMT_EMPTY	0x0000	
#define	XMT_HALF	0x0001	
#define	XMT_FULL	0x0003	

#define	RCVSTAT		0x000C	
#define	RCV_EMPTY	0x0000	
#define	RCV_HALF	0x0004	
#define	RCV_FULL	0x000C	

#endif
