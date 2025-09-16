

#ifndef _DT_BINDINGS_CLOCK_SAMSUNG_S3C2410_CLOCK_H
#define _DT_BINDINGS_CLOCK_SAMSUNG_S3C2410_CLOCK_H






#define MPLL			2
#define UPLL			3
#define FCLK			4
#define HCLK			5
#define PCLK			6
#define UCLK			7
#define ARMCLK			8


#define PCLK_UART0		16
#define PCLK_UART1		17
#define PCLK_UART2		18
#define PCLK_I2C		19
#define PCLK_SDI		20
#define PCLK_SPI		21
#define PCLK_ADC		22
#define PCLK_AC97		23
#define PCLK_I2S		24
#define PCLK_PWM		25
#define PCLK_RTC		26
#define PCLK_GPIO		27



#define HCLK_LCD		32
#define HCLK_USBH		33
#define HCLK_USBD		34
#define HCLK_NAND		35
#define HCLK_CAM		36


#define CAMIF			40



#define NR_CLKS			(CAMIF + 1)

#endif 
