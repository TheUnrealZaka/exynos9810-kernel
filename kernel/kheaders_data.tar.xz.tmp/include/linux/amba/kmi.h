
#ifndef ASM_ARM_HARDWARE_AMBA_KMI_H
#define ASM_ARM_HARDWARE_AMBA_KMI_H


#define KMICR		(KMI_BASE + 0x00)
#define KMICR_TYPE		(1 << 5)
#define KMICR_RXINTREN		(1 << 4)
#define KMICR_TXINTREN		(1 << 3)
#define KMICR_EN		(1 << 2)
#define KMICR_FD		(1 << 1)
#define KMICR_FC		(1 << 0)


#define KMISTAT		(KMI_BASE + 0x04)
#define KMISTAT_TXEMPTY		(1 << 6)
#define KMISTAT_TXBUSY		(1 << 5)
#define KMISTAT_RXFULL		(1 << 4)
#define KMISTAT_RXBUSY		(1 << 3)
#define KMISTAT_RXPARITY	(1 << 2)
#define KMISTAT_IC		(1 << 1)
#define KMISTAT_ID		(1 << 0)


#define KMIDATA		(KMI_BASE + 0x08)


#define KMICLKDIV	(KMI_BASE + 0x0c)


#define KMIIR		(KMI_BASE + 0x10)
#define KMIIR_TXINTR		(1 << 1)
#define KMIIR_RXINTR		(1 << 0)


#define KMI_SIZE	(0x100)

#endif
