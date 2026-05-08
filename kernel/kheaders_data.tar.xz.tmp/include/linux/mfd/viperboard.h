

#ifndef __MFD_VIPERBOARD_H__
#define __MFD_VIPERBOARD_H__

#include <linux/types.h>
#include <linux/usb.h>

#define VPRBRD_EP_OUT               0x02
#define VPRBRD_EP_IN                0x86

#define VPRBRD_I2C_MSG_LEN          512 

#define VPRBRD_I2C_FREQ_6MHZ        1                        
#define VPRBRD_I2C_FREQ_3MHZ        2                        
#define VPRBRD_I2C_FREQ_1MHZ        3                        
#define VPRBRD_I2C_FREQ_FAST        4                        
#define VPRBRD_I2C_FREQ_400KHZ      VPRBRD_I2C_FREQ_FAST
#define VPRBRD_I2C_FREQ_200KHZ      5                        
#define VPRBRD_I2C_FREQ_STD         6                        
#define VPRBRD_I2C_FREQ_100KHZ      VPRBRD_I2C_FREQ_STD
#define VPRBRD_I2C_FREQ_10KHZ       7                        

#define VPRBRD_I2C_CMD_WRITE        0x00
#define VPRBRD_I2C_CMD_READ         0x01
#define VPRBRD_I2C_CMD_ADDR         0x02

#define VPRBRD_USB_TYPE_OUT	    0x40
#define VPRBRD_USB_TYPE_IN	    0xc0
#define VPRBRD_USB_TIMEOUT_MS       100
#define VPRBRD_USB_REQUEST_I2C_FREQ 0xe6
#define VPRBRD_USB_REQUEST_I2C      0xe9
#define VPRBRD_USB_REQUEST_MAJOR    0xea
#define VPRBRD_USB_REQUEST_MINOR    0xeb
#define VPRBRD_USB_REQUEST_ADC      0xec
#define VPRBRD_USB_REQUEST_GPIOA    0xed
#define VPRBRD_USB_REQUEST_GPIOB    0xdd

struct vprbrd_i2c_write_hdr {
	u8 cmd;
	u16 addr;
	u8 len1;
	u8 len2;
	u8 last;
	u8 chan;
	u16 spi;
} __packed;

struct vprbrd_i2c_read_hdr {
	u8 cmd;
	u16 addr;
	u8 len0;
	u8 len1;
	u8 len2;
	u8 len3;
	u8 len4;
	u8 len5;
	u16 tf1;                        
	u16 tf2;                        
} __packed;

struct vprbrd_i2c_status {
	u8 unknown[11];
	u8 status;
} __packed;

struct vprbrd_i2c_write_msg {
	struct vprbrd_i2c_write_hdr header;
	u8 data[VPRBRD_I2C_MSG_LEN
		- sizeof(struct vprbrd_i2c_write_hdr)];
} __packed;

struct vprbrd_i2c_read_msg {
	struct vprbrd_i2c_read_hdr header;
	u8 data[VPRBRD_I2C_MSG_LEN
		- sizeof(struct vprbrd_i2c_read_hdr)];
} __packed;

struct vprbrd_i2c_addr_msg {
	u8 cmd;
	u8 addr;
	u8 unknown1;
	u16 len;
	u8 unknown2;
	u8 unknown3;
} __packed;


struct vprbrd {
	struct usb_device *usb_dev; 
	struct mutex lock;
	u8 buf[sizeof(struct vprbrd_i2c_write_msg)];
	struct platform_device pdev;
};

#endif 
