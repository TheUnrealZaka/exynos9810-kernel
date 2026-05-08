#ifndef _MC6821_H_
#define _MC6821_H_



#ifndef PIA_REG_PADWIDTH
#define PIA_REG_PADWIDTH 255
#endif

struct pia {
	union {
		volatile u_char pra;
		volatile u_char ddra;
	} ua;
	u_char pad1[PIA_REG_PADWIDTH];
	volatile u_char cra;
	u_char pad2[PIA_REG_PADWIDTH];
	union {
		volatile u_char prb;
		volatile u_char ddrb;
	} ub;
	u_char pad3[PIA_REG_PADWIDTH];
	volatile u_char crb;
	u_char pad4[PIA_REG_PADWIDTH];
};

#define ppra ua.pra
#define pddra ua.ddra
#define pprb ub.prb
#define pddrb ub.ddrb

#define PIA_C1_ENABLE_IRQ (1<<0)
#define PIA_C1_LOW_TO_HIGH (1<<1)
#define PIA_DDR (1<<2)
#define PIA_IRQ2 (1<<6)
#define PIA_IRQ1 (1<<7)

#endif
