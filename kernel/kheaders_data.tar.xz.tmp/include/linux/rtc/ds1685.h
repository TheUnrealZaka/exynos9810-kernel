

#ifndef _LINUX_RTC_DS1685_H_
#define _LINUX_RTC_DS1685_H_

#include <linux/rtc.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>


struct ds1685_priv {
	struct rtc_device *dev;
	void __iomem *regs;
	u32 regstep;
	resource_size_t baseaddr;
	size_t size;
	spinlock_t lock;
	struct work_struct work;
	int irq_num;
	bool bcd_mode;
	bool no_irq;
	bool uie_unsupported;
	bool alloc_io_resources;
	u8 (*read)(struct ds1685_priv *, int);
	void (*write)(struct ds1685_priv *, int, u8);
	void (*prepare_poweroff)(void);
	void (*wake_alarm)(void);
	void (*post_ram_clear)(void);
};



struct ds1685_rtc_platform_data {
	const u32 regstep;
	const bool bcd_mode;
	const bool no_irq;
	const bool uie_unsupported;
	const bool alloc_io_resources;
	u8 (*plat_read)(struct ds1685_priv *, int);
	void (*plat_write)(struct ds1685_priv *, int, u8);
	void (*plat_prepare_poweroff)(void);
	void (*plat_wake_alarm)(void);
	void (*plat_post_ram_clear)(void);
};



#define RTC_SECS		0x00	
#define RTC_SECS_ALARM		0x01	
#define RTC_MINS		0x02	
#define RTC_MINS_ALARM		0x03	
#define RTC_HRS			0x04	
#define RTC_HRS_ALARM		0x05	
#define RTC_WDAY		0x06	
#define RTC_MDAY		0x07	
#define RTC_MONTH		0x08	
#define RTC_YEAR		0x09	
#define RTC_CENTURY		0x48	
#define RTC_MDAY_ALARM		0x49	



#define RTC_SECS_BCD_MASK	0x7f	
#define RTC_MINS_BCD_MASK	0x7f	
#define RTC_HRS_12_BCD_MASK	0x1f	
#define RTC_HRS_24_BCD_MASK	0x3f	
#define RTC_MDAY_BCD_MASK	0x3f	
#define RTC_MONTH_BCD_MASK	0x1f	
#define RTC_YEAR_BCD_MASK	0xff	


#define RTC_SECS_BIN_MASK	0x3f	
#define RTC_MINS_BIN_MASK	0x3f	
#define RTC_HRS_12_BIN_MASK	0x0f	
#define RTC_HRS_24_BIN_MASK	0x1f	
#define RTC_MDAY_BIN_MASK	0x1f	
#define RTC_MONTH_BIN_MASK	0x0f	
#define RTC_YEAR_BIN_MASK	0x7f	


#define RTC_WDAY_MASK		0x07	
#define RTC_CENTURY_MASK	0xff	
#define RTC_MDAY_ALARM_MASK	0xff	
#define RTC_HRS_AMPM_MASK	BIT(7)	




#define RTC_CTRL_A		0x0a	
#define RTC_CTRL_B		0x0b	
#define RTC_CTRL_C		0x0c	
#define RTC_CTRL_D		0x0d	
#define RTC_EXT_CTRL_4A		0x4a	
#define RTC_EXT_CTRL_4B		0x4b	



#define RTC_CTRL_A_UIP		BIT(7)	
#define RTC_CTRL_A_DV2		BIT(6)	
#define RTC_CTRL_A_DV1		BIT(5)	
#define RTC_CTRL_A_DV0		BIT(4)	
#define RTC_CTRL_A_RS2		BIT(2)	
#define RTC_CTRL_A_RS3		BIT(3)	
#define RTC_CTRL_A_RS1		BIT(1)	
#define RTC_CTRL_A_RS0		BIT(0)	
#define RTC_CTRL_A_RS_MASK	0x0f	


#define RTC_CTRL_B_SET		BIT(7)	
#define RTC_CTRL_B_PIE		BIT(6)	
#define RTC_CTRL_B_AIE		BIT(5)	
#define RTC_CTRL_B_UIE		BIT(4)	
#define RTC_CTRL_B_SQWE		BIT(3)	
#define RTC_CTRL_B_DM		BIT(2)	
#define RTC_CTRL_B_2412		BIT(1)	
#define RTC_CTRL_B_DSE		BIT(0)	
#define RTC_CTRL_B_PAU_MASK	0x70	



#define RTC_CTRL_C_IRQF		BIT(7)	
#define RTC_CTRL_C_PF		BIT(6)	
#define RTC_CTRL_C_AF		BIT(5)	
#define RTC_CTRL_C_UF		BIT(4)	
#define RTC_CTRL_C_PAU_MASK	0x70	



#define RTC_CTRL_D_VRT		BIT(7)	



#define RTC_CTRL_4A_VRT2	BIT(7)	
#define RTC_CTRL_4A_INCR	BIT(6)	
#define RTC_CTRL_4A_PAB		BIT(3)	
#define RTC_CTRL_4A_RF		BIT(2)	
#define RTC_CTRL_4A_WF		BIT(1)	
#define RTC_CTRL_4A_KF		BIT(0)	
#if !defined(CONFIG_RTC_DRV_DS1685) && !defined(CONFIG_RTC_DRV_DS1689)
#define RTC_CTRL_4A_BME		BIT(5)	
#endif
#define RTC_CTRL_4A_RWK_MASK	0x07	



#define RTC_CTRL_4B_ABE		BIT(7)	
#define RTC_CTRL_4B_E32K	BIT(6)	
#define RTC_CTRL_4B_CS		BIT(5)	
#define RTC_CTRL_4B_RCE		BIT(4)	
#define RTC_CTRL_4B_PRS		BIT(3)	
#define RTC_CTRL_4B_RIE		BIT(2)	
#define RTC_CTRL_4B_WIE		BIT(1)	
#define RTC_CTRL_4B_KSE		BIT(0)	
#define RTC_CTRL_4B_RWK_MASK	0x07	



#define RTC_BANK1_SSN_MODEL	0x40	
#define RTC_BANK1_SSN_BYTE_1	0x41	
#define RTC_BANK1_SSN_BYTE_2	0x42	
#define RTC_BANK1_SSN_BYTE_3	0x43	
#define RTC_BANK1_SSN_BYTE_4	0x44	
#define RTC_BANK1_SSN_BYTE_5	0x45	
#define RTC_BANK1_SSN_BYTE_6	0x46	
#define RTC_BANK1_SSN_CRC	0x47	
#define RTC_BANK1_RAM_DATA_PORT	0x53	



#if defined(CONFIG_RTC_DRV_DS1685)
#define RTC_BANK1_RAM_ADDR	0x50	
#elif defined(CONFIG_RTC_DRV_DS1689)
#define RTC_BANK1_VCC_CTR_LSB	0x54	
#define RTC_BANK1_VCC_CTR_MSB	0x57	
#define RTC_BANK1_VBAT_CTR_LSB	0x58	
#define RTC_BANK1_VBAT_CTR_MSB	0x5b	
#define RTC_BANK1_PWR_CTR_LSB	0x5c	
#define RTC_BANK1_PWR_CTR_MSB	0x5d	
#define RTC_BANK1_UNIQ_SN	0x60	
#else 
#define RTC_BANK1_RAM_ADDR_LSB	0x50	
#define RTC_BANK1_RAM_ADDR_MSB	0x51	
#define RTC_BANK1_WRITE_CTR	0x5e	
#endif



#define RTC_MODEL_DS1685	0x71	
#define RTC_MODEL_DS17285	0x72	
#define RTC_MODEL_DS1689	0x73	
#define RTC_MODEL_DS17485	0x74	
#define RTC_MODEL_DS17885	0x78	



					
#define RTC_SQW_8192HZ		0x03	
#define RTC_SQW_4096HZ		0x04	
#define RTC_SQW_2048HZ		0x05	
#define RTC_SQW_1024HZ		0x06	
#define RTC_SQW_512HZ		0x07	
#define RTC_SQW_256HZ		0x08	
#define RTC_SQW_128HZ		0x09	
#define RTC_SQW_64HZ		0x0a	
#define RTC_SQW_32HZ		0x0b	
#define RTC_SQW_16HZ		0x0c	
#define RTC_SQW_8HZ		0x0d	
#define RTC_SQW_4HZ		0x0e	
#define RTC_SQW_2HZ		0x0f	
#define RTC_SQW_0HZ		0x00	
#define RTC_SQW_32768HZ		32768	
#define RTC_MAX_USER_FREQ	8192



#define NVRAM_TIME_BASE		0x0e	
#define NVRAM_BANK0_BASE	0x40	
#define NVRAM_SZ_TIME		50
#define NVRAM_SZ_BANK0		64
#if defined(CONFIG_RTC_DRV_DS1685)
#  define NVRAM_SZ_EXTND	128
#elif defined(CONFIG_RTC_DRV_DS1689)
#  define NVRAM_SZ_EXTND	0
#elif defined(CONFIG_RTC_DRV_DS17285)
#  define NVRAM_SZ_EXTND	2048
#elif defined(CONFIG_RTC_DRV_DS17485)
#  define NVRAM_SZ_EXTND	4096
#elif defined(CONFIG_RTC_DRV_DS17885)
#  define NVRAM_SZ_EXTND	8192
#endif
#define NVRAM_TOTAL_SZ_BANK0	(NVRAM_SZ_TIME + NVRAM_SZ_BANK0)
#define NVRAM_TOTAL_SZ		(NVRAM_TOTAL_SZ_BANK0 + NVRAM_SZ_EXTND)



extern void __noreturn
ds1685_rtc_poweroff(struct platform_device *pdev);

#endif 
