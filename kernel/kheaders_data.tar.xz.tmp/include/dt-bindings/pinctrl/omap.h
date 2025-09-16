

#ifndef _DT_BINDINGS_PINCTRL_OMAP_H
#define _DT_BINDINGS_PINCTRL_OMAP_H


#define MUX_MODE0	0
#define MUX_MODE1	1
#define MUX_MODE2	2
#define MUX_MODE3	3
#define MUX_MODE4	4
#define MUX_MODE5	5
#define MUX_MODE6	6
#define MUX_MODE7	7


#define PULL_ENA		(1 << 3)
#define PULL_UP			(1 << 4)
#define ALTELECTRICALSEL	(1 << 5)


#define INPUT_EN		(1 << 8)
#define OFF_EN			(1 << 9)
#define OFFOUT_EN		(1 << 10)
#define OFFOUT_VAL		(1 << 11)
#define OFF_PULL_EN		(1 << 12)
#define OFF_PULL_UP		(1 << 13)
#define WAKEUP_EN		(1 << 14)
#define WAKEUP_EVENT		(1 << 15)


#define PIN_OUTPUT		0
#define PIN_OUTPUT_PULLUP	(PIN_OUTPUT | PULL_ENA | PULL_UP)
#define PIN_OUTPUT_PULLDOWN	(PIN_OUTPUT | PULL_ENA)
#define PIN_INPUT		INPUT_EN
#define PIN_INPUT_PULLUP	(PULL_ENA | INPUT_EN | PULL_UP)
#define PIN_INPUT_PULLDOWN	(PULL_ENA | INPUT_EN)


#define PIN_OFF_NONE		0
#define PIN_OFF_OUTPUT_HIGH	(OFF_EN | OFFOUT_EN | OFFOUT_VAL)
#define PIN_OFF_OUTPUT_LOW	(OFF_EN | OFFOUT_EN)
#define PIN_OFF_INPUT_PULLUP	(OFF_EN | OFFOUT_EN | OFF_PULL_EN | OFF_PULL_UP)
#define PIN_OFF_INPUT_PULLDOWN	(OFF_EN | OFFOUT_EN | OFF_PULL_EN)
#define PIN_OFF_WAKEUPENABLE	WAKEUP_EN


#define OMAP_IOPAD_OFFSET(pa, offset)	(((pa) & 0xffff) - (offset))

#define OMAP2420_CORE_IOPAD(pa, val)	OMAP_IOPAD_OFFSET((pa), 0x0030) (val)
#define OMAP2430_CORE_IOPAD(pa, val)	OMAP_IOPAD_OFFSET((pa), 0x2030) (val)
#define OMAP3_CORE1_IOPAD(pa, val)	OMAP_IOPAD_OFFSET((pa), 0x2030) (val)
#define OMAP3430_CORE2_IOPAD(pa, val)	OMAP_IOPAD_OFFSET((pa), 0x25d8) (val)
#define OMAP3630_CORE2_IOPAD(pa, val)	OMAP_IOPAD_OFFSET((pa), 0x25a0) (val)
#define OMAP3_WKUP_IOPAD(pa, val)	OMAP_IOPAD_OFFSET((pa), 0x2a00) (val)
#define DM814X_IOPAD(pa, val)		OMAP_IOPAD_OFFSET((pa), 0x0800) (val)
#define DM816X_IOPAD(pa, val)		OMAP_IOPAD_OFFSET((pa), 0x0800) (val)
#define AM33XX_IOPAD(pa, val)		OMAP_IOPAD_OFFSET((pa), 0x0800) (val)


#define OMAP_PADCONF_OFFSET(offset, base_offset)	((offset) - (base_offset))

#define OMAP4_IOPAD(offset, val)	OMAP_PADCONF_OFFSET((offset), 0x0040) (val)
#define OMAP5_IOPAD(offset, val)	OMAP_PADCONF_OFFSET((offset), 0x0040) (val)


#define OMAP3_UART1_RX		0x152
#define OMAP3_UART2_RX		0x14a
#define OMAP3_UART3_RX		0x16e
#define OMAP4_UART2_RX		0xdc
#define OMAP4_UART3_RX		0x104
#define OMAP4_UART4_RX		0x11c

#endif

