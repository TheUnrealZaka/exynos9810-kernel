

#ifndef __CORESIGHT_REGS_H
#define __CORESIGHT_REGS_H

#include <linux/kernel.h>
#include <linux/types.h>

#define OSLOCK_MAGIC	(0xc5acce55)


#define CS_SJTAG_OFFSET	(0x8000)
#define SJTAG_STATUS	(0x4)
#define SJTAG_SOFT_LOCK	(1<<2)


#define DBGWFAR		(0x018)	
#define DBGVCR		(0x01c)	
#define DBGECR		(0x024)	
#define DBGDSCCR	(0x028)	
#define DBGDSMCR	(0x02c)	
#define DBGDTRRX	(0x080)	
#define DBGITR		(0x084)	
#define DBGDSCR		(0x088)	
#define DBGDTRTX	(0x08c)	
#define DBGDRCR		(0x090)	
#define DBGEACR		(0x094)	
#define DBGECCR		(0x098)	
#define DBGPCSRlo	(0x0a0)	
#define DBGCIDSR	(0x0a4)	
#define DBGVIDSR	(0x0a8) 
#define DBGPCSRhi	(0x0ac)	
#define DBGBXVR0	(0x250) 
#define DBGBXVR1	(0x254) 
#define DBGOSLAR	(0x300) 
#define DBGOSLSR	(0x304) 
#define DBGPRCR		(0x310) 
#define DBGPRSR		(0x314) 
#define DBGITOCTRL	(0xef8) 
#define DBGITISR	(0xefc) 
#define DBGITCTRL	(0xf00) 
#define DBGCLAIMSET	(0xfa0) 
#define DBGCLAIMCLR	(0xfa4) 
#define DBGLAR		(0xfb0) 
#define DBGLSR		(0xfb4) 
#define DBGAUTHSTATUS	(0xfb8) 
#define DBGDEVID2	(0xfc0) 
#define DBGDEVID1	(0xfc4)	
#define DBGDEVID0	(0xfc8) 
#define DBGDEVTYPE	(0xfcc) 

#define MIDR		(0xd00)	
#define ID_AA64DFR0_EL1 (0xd28)


#define DBGBVRn(n)	(0x400 + (n * 0x10)) 
#define DBGBCRn(n)	(0x408 + (n * 0x10))

#define DBGWVRn(n)	(0x800 + (n * 0x10)) 
#define DBGWCRn(n)	(0x808 + (n * 0x10))


#define DEBUG_ARCH_V8	(0x6)


#define ARMV8_PROCESSOR	(0xf << 16)
#define ARMV8_CORTEXA53	(0xd03)
#define ARMV8_CORTEXA57	(0xd07)


#define TMCRSZ		(0x004)
#define TMCSTS		(0x00c)
#define TMCRRD		(0x010)
#define TMCRRP		(0x014)
#define TMCRWP		(0x018)
#define TMCTGR		(0x01c)
#define TMCCTL		(0x020)
#define TMCRWD		(0x024)
#define TMCMODE		(0x028)
#define TMCLBUFLEVEL	(0x02c)
#define TMCCBUFLEVEL	(0x030)
#define TMCBUFWM	(0x034)
#define TMCRRPHI	(0x038)
#define TMCRWPHI	(0x03c)
#define TMCAXICTL	(0x110)
#define TMCDBALO	(0x118)
#define TMCDBAHI	(0x11c)
#define TMCFFSR		(0x300)
#define TMCFFCR		(0x304)
#define TMCPSCR		(0x308)


#define ITCTRL		(0xf00)
#define CLAIMSET	(0xfa0)
#define CLAIMCLR	(0xfa4)
#define LAR		(0xfb0)
#define LSR		(0xfb4)
#define AUTHSTATUS	(0xfb8)


#define FUNCTRL		(0x0)
#define FUNPRIORCTRL	(0x4)

#define DBG_OFFSET	(0x0)
#define CTI_OFFSET	(0x10000)
#define PMU_OFFSET	(0x20000)
#define ETM_OFFSET	(0x30000)
#define PMUPCSRlo	(0x200)
#define PMUPCSRhi	(0x204)


#define ETMCTLR		(0x004)
#define ETMPROCSELR	(0x008)
#define ETMSTATUS	(0x00c)
#define ETMCONFIG	(0x010)
#define ETMAUXCTLR	(0x018)
#define ETMEVENTCTL0R	(0x020)
#define ETMEVENTCTL1R	(0x024)
#define ETMSTALLCTLR	(0x02c)
#define ETMTSCTLR	(0x030)
#define ETMSYNCPR	(0x034)
#define ETMCCCCTLR	(0x038)
#define ETMBBCTLR	(0x03c)
#define ETMTRACEIDR	(0x040)
#define ETMQCTRLR	(0x044)
#define ETMVICTLR	(0x080)
#define ETMVIIECTLR	(0x084)
#define ETMVISSCTLR	(0x088)
#define ETMVIPCSSCTLR	(0x08c)
#define ETMVDCTLR	(0x0a0)
#define ETMVDSACCTLR	(0x0a4)
#define ETMVDARCCTLR	(0x0a8)
#define ETMSEQEVR(n)	(0x100 + (n * 4))
#define ETMSEQRSTEVR	(0x118)
#define ETMSEQSTR	(0x11c)
#define ETMEXTINSELR	(0x120)
#define ETMCNTRLDVR(n)	(0x140 + (n * 4))
#define ETMCNTCTLR(n)	(0x150 + (n * 4))
#define ETMCNTVR(n)	(0x160 + (n * 4))
#define ETMIDR8		(0x180)
#define ETMIDR9		(0x184)
#define ETMID10		(0x188)
#define ETMID11		(0x18c)
#define ETMID12		(0x190)
#define ETMID13		(0x194)
#define ETMID0		(0x1e0)
#define ETMID1		(0x1e4)
#define ETMID2		(0x1e8)
#define ETMID3		(0x1ec)
#define ETMID4		(0x1f0)
#define ETMID5		(0x1f4)
#define ETMID6		(0x1f8)
#define ETMID7		(0x1fc)
#define ETMRSCTLR(n)	(0x200 + (n * 4))
#define ETMSSCCR(n)	(0x280 + (n * 4))
#define ETMSSCSR(n)	(0x2a0 + (n * 4))
#define ETMSSPCICR(n)	(0x2c0 + (n * 4))
#define ETMOSLAR	(0x300)
#define ETMOSLSR	(0x304)
#define ETMPDCR		(0x310)
#define ETMPDSR		(0x314)
#define ETMACVR(n)	(0x400 + (n * 4))
#define ETMACAT(n) 	(0x480 + (n * 4))
#define ETMDVCVR(n)	(0x500 + (n * 4))
#define ETMDVCMR(n)	(0x580 + (n * 4))
#define ETMCIDCVR(n)	(0x600 + (n * 4))
#define ETMVMIDCVR(n)	(0x640 + (n * 4))
#define ETMCCIDCCTLR0	(0x680)
#define ETMCCIDCCTLR1	(0x684)
#define ETMVMIDCCTLR0	(0x688)
#define ETMVMIDCCTLR1	(0x68c)

#ifdef CONFIG_EXYNOS_CORESIGHT_ETM
extern void exynos_trace_stop(void);
#else
#define exynos_trace_stop()	do { } while(0)
#endif

#endif
