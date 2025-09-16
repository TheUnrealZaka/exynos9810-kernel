

#ifndef __GPIO_IR_RECV_H__
#define __GPIO_IR_RECV_H__

struct gpio_ir_recv_platform_data {
	int		gpio_nr;
	bool		active_low;
	u64		allowed_protos;
	const char	*map_name;
};

#endif 
