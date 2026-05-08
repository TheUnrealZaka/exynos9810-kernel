

#ifndef __LINUX_USB_GADGET_MSM72K_UDC_H__
#define __LINUX_USB_GADGET_MSM72K_UDC_H__


#define USB2_PHY_SEL         0xfd4ab000

#define USB_AHBBURST         (MSM_USB_BASE + 0x0090)
#define USB_AHBMODE          (MSM_USB_BASE + 0x0098)
#define USB_GENCONFIG_2      (MSM_USB_BASE + 0x00a0)
#define ULPI_TX_PKT_EN_CLR_FIX	BIT(19)

#define USB_CAPLENGTH        (MSM_USB_BASE + 0x0100) 

#define USB_USBCMD           (MSM_USB_BASE + 0x0140)
#define USB_PORTSC           (MSM_USB_BASE + 0x0184)
#define USB_OTGSC            (MSM_USB_BASE + 0x01A4)
#define USB_USBMODE          (MSM_USB_BASE + 0x01A8)
#define USB_PHY_CTRL         (MSM_USB_BASE + 0x0240)
#define USB_PHY_CTRL2        (MSM_USB_BASE + 0x0278)

#define GENCONFIG_2_SESS_VLD_CTRL_EN	BIT(7)
#define USBCMD_SESS_VLD_CTRL		BIT(25)

#define USBCMD_RESET   2
#define USB_USBINTR          (MSM_USB_BASE + 0x0148)

#define PORTSC_PHCD            (1 << 23) 
#define PORTSC_PTS_MASK        (3 << 30)
#define PORTSC_PTS_ULPI        (2 << 30)
#define PORTSC_PTS_SERIAL      (3 << 30)

#define USB_ULPI_VIEWPORT    (MSM_USB_BASE + 0x0170)
#define ULPI_RUN              (1 << 30)
#define ULPI_WRITE            (1 << 29)
#define ULPI_READ             (0 << 29)
#define ULPI_ADDR(n)          (((n) & 255) << 16)
#define ULPI_DATA(n)          ((n) & 255)
#define ULPI_DATA_READ(n)     (((n) >> 8) & 255)


#define ULPI_PWR_CLK_MNG_REG	0x88
#define OTG_COMP_DISABLE	BIT(0)

#define ULPI_MISC_A			0x96
#define ULPI_MISC_A_VBUSVLDEXTSEL	BIT(1)
#define ULPI_MISC_A_VBUSVLDEXT		BIT(0)

#define ASYNC_INTR_CTRL         (1 << 29) 
#define ULPI_STP_CTRL           (1 << 30) 
#define PHY_RETEN               (1 << 1) 
#define PHY_POR_ASSERT		(1 << 0) 


#define OTGSC_INTSTS_MASK	(0x7f << 16)
#define OTGSC_ID		(1 << 8)
#define OTGSC_BSV		(1 << 11)
#define OTGSC_IDIS		(1 << 16)
#define OTGSC_BSVIS		(1 << 19)
#define OTGSC_IDIE		(1 << 24)
#define OTGSC_BSVIE		(1 << 27)

#endif 
