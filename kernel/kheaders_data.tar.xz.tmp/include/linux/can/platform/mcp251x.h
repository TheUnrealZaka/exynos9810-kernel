#ifndef _CAN_PLATFORM_MCP251X_H
#define _CAN_PLATFORM_MCP251X_H



#include <linux/spi/spi.h>



struct mcp251x_platform_data {
	unsigned long oscillator_frequency;
};

#endif 
