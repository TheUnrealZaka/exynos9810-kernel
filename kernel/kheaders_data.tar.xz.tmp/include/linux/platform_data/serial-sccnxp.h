

#ifndef _PLATFORM_DATA_SERIAL_SCCNXP_H_
#define _PLATFORM_DATA_SERIAL_SCCNXP_H_

#define SCCNXP_MAX_UARTS	2


#define LINE_OP0		1
#define LINE_OP1		2
#define LINE_OP2		3
#define LINE_OP3		4
#define LINE_OP4		5
#define LINE_OP5		6
#define LINE_OP6		7
#define LINE_OP7		8


#define LINE_IP0		9
#define LINE_IP1		10
#define LINE_IP2		11
#define LINE_IP3		12
#define LINE_IP4		13
#define LINE_IP5		14
#define LINE_IP6		15


#define DTR_OP			0	
#define RTS_OP			4	
#define DSR_IP			8	
#define CTS_IP			12	
#define DCD_IP			16	
#define RNG_IP			20	

#define DIR_OP			24	


#define MCTRL_SIG(sig, line)	((line) << (sig))




struct sccnxp_pdata {
	
	const u8		reg_shift;
	
	const u32		mctrl_cfg[SCCNXP_MAX_UARTS];
	
	const unsigned int	poll_time_us;
};

#endif
