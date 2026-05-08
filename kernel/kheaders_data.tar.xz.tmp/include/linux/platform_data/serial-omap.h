

#ifndef __OMAP_SERIAL_H__
#define __OMAP_SERIAL_H__

#include <linux/serial_core.h>
#include <linux/device.h>
#include <linux/pm_qos.h>

#define OMAP_SERIAL_DRIVER_NAME	"omap_uart"


#define OMAP_SERIAL_NAME	"ttyO"

struct omap_uart_port_info {
	bool			dma_enabled;	
	unsigned int		uartclk;	
	upf_t			flags;		
	unsigned int		dma_rx_buf_size;
	unsigned int		dma_rx_timeout;
	unsigned int		autosuspend_timeout;
	unsigned int		dma_rx_poll_rate;

	int (*get_context_loss_count)(struct device *);
	void (*enable_wakeup)(struct device *, bool);
};

#endif 
