

#ifndef _DT_BINDINGS_CLOCK_SAMSUNG_S3C2412_CLOCK_H
#define _DT_BINDINGS_CLOCK_SAMSUNG_S3C2412_CLOCK_H






#define MPLL			2
#define UPLL			3
#define MDIVCLK			4
#define MSYSCLK			5
#define USYSCLK			6
#define HCLK			7
#define PCLK			8
#define ARMDIV			9
#define ARMCLK			10



#define SCLK_CAM		16
#define SCLK_UART		17
#define SCLK_I2S		18
#define SCLK_USBD		19
#define SCLK_USBH		20


#define PCLK_WDT		32
#define PCLK_SPI		33
#define PCLK_I2S		34
#define PCLK_I2C		35
#define PCLK_ADC		36
#define PCLK_RTC		37
#define PCLK_GPIO		38
#define PCLK_UART2		39
#define PCLK_UART1		40
#define PCLK_UART0		41
#define PCLK_SDI		42
#define PCLK_PWM		43
#define PCLK_USBD		44


#define HCLK_HALF		48
#define HCLK_X2			49
#define HCLK_SDRAM		50
#define HCLK_USBH		51
#define HCLK_LCD		52
#define HCLK_NAND		53
#define HCLK_DMA3		54
#define HCLK_DMA2		55
#define HCLK_DMA1		56
#define HCLK_DMA0		57


#define NR_CLKS			(HCLK_DMA0 + 1)

#endif 
