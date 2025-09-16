

#ifndef GPIO_DW_APB_H
#define GPIO_DW_APB_H

struct dwapb_port_property {
	struct fwnode_handle *fwnode;
	unsigned int	idx;
	unsigned int	ngpio;
	unsigned int	gpio_base;
	unsigned int	irq;
	bool		irq_shared;
};

struct dwapb_platform_data {
	struct dwapb_port_property *properties;
	unsigned int nports;
};

#endif
