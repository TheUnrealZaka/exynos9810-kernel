

#ifndef _DT_BINDINGS_PINCTRL_HISI_H
#define _DT_BINDINGS_PINCTRL_HISI_H


#define MUX_M0		0
#define MUX_M1		1
#define MUX_M2		2
#define MUX_M3		3
#define MUX_M4		4
#define MUX_M5		5
#define MUX_M6		6
#define MUX_M7		7


#define PULL_MASK	(3)
#define PULL_DIS	(0)
#define PULL_UP		(1 << 0)
#define PULL_DOWN	(1 << 1)


#define DRIVE_MASK	(7 << 4)
#define DRIVE1_02MA	(0 << 4)
#define DRIVE1_04MA	(1 << 4)
#define DRIVE1_08MA	(2 << 4)
#define DRIVE1_10MA	(3 << 4)
#define DRIVE2_02MA	(0 << 4)
#define DRIVE2_04MA	(1 << 4)
#define DRIVE2_08MA	(2 << 4)
#define DRIVE2_10MA	(3 << 4)
#define DRIVE3_04MA	(0 << 4)
#define DRIVE3_08MA	(1 << 4)
#define DRIVE3_12MA	(2 << 4)
#define DRIVE3_16MA	(3 << 4)
#define DRIVE3_20MA	(4 << 4)
#define DRIVE3_24MA	(5 << 4)
#define DRIVE3_32MA	(6 << 4)
#define DRIVE3_40MA	(7 << 4)
#define DRIVE4_02MA	(0 << 4)
#define DRIVE4_04MA	(2 << 4)
#define DRIVE4_08MA	(4 << 4)
#define DRIVE4_10MA	(6 << 4)

#endif
